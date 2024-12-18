#include <getopt.h>
#include <regex.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#define bufs_size 2048
#define modeREAD 3
#include <stdlib.h>

#include "../common/common.h"
#include "s21_grep.h"

/* 0 - ok, 1 - error */
int insertinfirst(mylist *list, char *word) {
  int IsPossible = (list->cnt < ARRSIZE);
  if (IsPossible) {
    list->cnt++;
    for (int i = list->cnt - 1; i > 0; i--) list->arr[i] = list->arr[i - 1];
    list->arr[0] = word;
  }
  return !IsPossible;
}

/* 0 - ok, 1 - error */
int add(mylist *list, char *word) {
  int IsPossible = (list->cnt < ARRSIZE);
  if (IsPossible) list->arr[list->cnt++] = word;
  return !IsPossible;
}

/* 0 - ok, 1 - error */
int ini_unknownpar(ss_and_fns *data, char *unknownpar) {
  int status = 0;
  if (unknownpar != NULL) {
    if ((*data).samples.cnt || (*data).file_samp.cnt)
      status = insertinfirst(&((*data).file2find), unknownpar);
    else
      status = insertinfirst(&((*data).samples), unknownpar);
  }
  return status;
}

/* 1 - ok; 0 - error */
int input(int argc, char **argv, flags *flag, ss_and_fns *data) {
  golparcol p = {.opt_index = -1,
                 .read = Init_getopt_long_Params_Grep()}; /* p - params*/
  int rez = 0, status = 1;
  char *unknownpar = NULL; /*for first unknown parametr*/
  while (((rez = getopt_long(argc, argv, p.read->opt, p.read->long_opt,
                             &p.opt_index)) != -1) &&
         status) {
    if (rez == 'i') (*flag).i = 1;
    if (rez == 'v') (*flag).v = 1;
    if (rez == 'c') (*flag).c = 1;
    if (rez == 'l') (*flag).l = 1;
    if (rez == 'n') (*flag).n = 1;
    if (rez == 'h') (*flag).h = 1;
    if (rez == 's') (*flag).s = 1;
    if (rez == 'o') (*flag).o = 1;
    if (rez == 1) {
      if (((*data).samples.cnt == 0) && ((*data).file_samp.cnt == 0) &&
          (unknownpar == NULL))
        unknownpar = optarg;
      else
        status = !add(&((*data).file2find), optarg);
    } else if (rez == 'e') {
      status = !add(&((*data).samples), optarg);
    } else if (rez == 'f') {
      if ((status = !access(optarg, modeREAD)))
        status = !add(&((*data).file_samp), optarg);
      else
        fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], optarg);
    } else if (rez == '?') {
      fprintf(stderr, "usage: %s [%s] template [file_names]\n", argv[0],
              p.read->opt);
      status = 0;
    }
  }
  if (argc < 2)
    fprintf(stderr, "usage: %s [%s] template [file_name]\n", argv[0],
            p.read->opt);
  return (status * !ini_unknownpar(data, unknownpar));
}
/* 1 - matched, 0 - no matched */
int isfind(mystroka *s2search, char **word, flags *flag, char *filename,
           int printfilename, int numline) {
  regex_t regex;
  int matched = 0, regim = flag->i ? REG_ICASE : REG_EXTENDED;
  regcomp(&regex, *word, regim);
  regmatch_t pmatch[1];
  int success = regexec(&regex, (*s2search).stroka, 1, pmatch, 0);
  if (strchr((*s2search).stroka, '\n') == NULL)
    strcat((*s2search).stroka, "\n");
  (*s2search).cnt = strlen((*s2search).stroka);
  matched = (success == 0 && !flag->v) || (success == REG_NOMATCH && flag->v);
  if (matched && !flag->h && printfilename && !flag->c && !flag->l)
    printf("%s:", filename);
  if (matched && !flag->l && !flag->c && flag->n) printf("%d:", numline);
  if (matched && !flag->l && !flag->c && !flag->o)
    printf("%s", (*s2search).stroka);
  if (matched && flag->o && !flag->c) {
    if (!flag->v) {
      for (int j = pmatch[0].rm_so;
           (j < pmatch[0].rm_eo) && (j >= 0) && (j < (*s2search).cnt); j++)
        printf("%c", ((*s2search).stroka)[j]);
      printf("\n");
    } else {
      printf("%s", (*s2search).stroka);
    }
  }
  regfree(&regex);
  return matched;
}

void Compare_with_File(flags *flag, ss_and_fns *data, int *matchedyet,
                       int *matchedcount, int *printfilename, int *numline,
                       mystroka *s2search, char *file2findarr) {
  for (int j = 0; (j < (*data).file_samp.cnt) && (!(*matchedyet)); j++) {
    FILE *f_s = fopen((*data).file_samp.arr[j], "r");
    if (f_s != NULL) {
      mystroka patern = {0, malloc(sizeof(char) * bufs_size)};
      while (fgets((patern.stroka), bufs_size, f_s) != NULL) {
        *matchedyet = isfind(s2search, &(patern.stroka), flag, file2findarr,
                             *printfilename, *numline);
        if (*matchedyet) (*matchedcount)++;  // added if
      }
      fclose(f_s);
      free(patern.stroka);
    }
  }
}

void postoperation(flags *flag, char *file2findarr, int *matchedcount,
                   int *printfilename) {
  if (flag->c) {
    if (flag->l) {
      *matchedcount = 1;
      printf("%s:", file2findarr);
    }
    if (!flag->h && *printfilename) printf("%s:", file2findarr);
    printf("%d\n", *matchedcount);
  }
  if (flag->l && *matchedcount > 0) printf("%s\n", file2findarr);
}
/* 0 - ok; 1 - error */
int search_and_print(flags *flag, ss_and_fns *data) {
  int status = 0, printfilename = (data->file2find.cnt > 1) && !flag->h;
  for (int i = 0; (i < (*data).file2find.cnt); i++) {
    FILE *f2s = fopen((*data).file2find.arr[i], "r");
    if (f2s != NULL) {
      int numline = 1, matchedcount = 0;
      mystroka s2search = {0, malloc(sizeof(char) * bufs_size)};
      while (fgets((s2search.stroka), bufs_size, f2s) != NULL) {
        int matchedyet = 0;
        /* From files with samples */
        Compare_with_File(flag, data, &matchedyet, &matchedcount,
                          &printfilename, &numline, &s2search,
                          (*data).file2find.arr[i]);
        /* From command line */
        for (int k = 0; (k < (*data).samples.cnt) && (!matchedyet); k++) {
          matchedyet = isfind(&s2search, &((*data).samples.arr[k]), flag,
                              (*data).file2find.arr[i], printfilename, numline);
          if (matchedyet) matchedcount++;  // added if
        }
        numline++;
      }
      postoperation(flag, (*data).file2find.arr[i], &matchedcount,
                    &printfilename);
      fclose(f2s);
      free(s2search.stroka);
    } else {
      if (!flag->s)
        fprintf(stdout, "s21_grep: %s: No such file or directory\n",
                (*data).file2find.arr[i]);
    }
  }
  return status;
}

/* 0 - ok; 1 - error*/
int main(int argc, char *argv[]) {
  flags flag = {0, 0, 0, 0, 0, 0, 0, 0};
  ss_and_fns data = {{0, {NULL}}, {0, {NULL}}, {0, {NULL}}};
  int status = 0;
  if (!(status = !input(argc, argv, &flag, &data)))
    status = search_and_print(&flag, &data);
  return status;
}
