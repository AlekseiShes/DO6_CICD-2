#include <stdlib.h>

#include "../common/common.h"

typedef struct S_getopt_long_Params_Colletion golparcol;

/* flags for cat: +beEnstTv*/
typedef struct {
  int s; /* Suppress repeated empty output lines. */
  int v; /* Show nonprinting ie use ^ and M- notation, except for LFD & TAB. */
  int E; /* Display $ at end of each line. */
  int e; /* -vE */
  int T; /* Display TAB characters as ^I. */
  int t; /* -vT */
  int b; /* Number nonempty output lines. */
  int n; /* Number all output lines. */
} flags;

/* 0 - ok, 1 -error */
int input(int argc, char **argv, flags *flag, int *currentFile) {
  golparcol par = {.opt_index = -1, .read = Init_getopt_long_Params_Cat()};
  int opt, status = 0;
  while ((opt = getopt_long(argc, argv, par.read->opt, par.read->long_opt,
                            &par.opt_index)) != -1) {
    (*flag).b += (opt == 'b');
    (*flag).E += ((opt == 'e') + (opt == 'E'));
    (*flag).v += ((opt == 'e') + (opt == 't') + (opt == 'v'));
    (*flag).n += (opt == 'n');
    (*flag).s += (opt == 's');
    (*flag).T += ((opt == 't') + (opt == 'T'));
    if (opt == '?') {
      fprintf(stderr, "usage: %s [%s] [file ...]\n", argv[0], par.read->opt);
      status = 1;
    }
  }
  *currentFile = optind;
  return status;
}

void strcat_all(char **dst, size_t *idst, char *app, size_t iapp) {
  char *out = (char *)realloc(*dst, sizeof(char) * (*idst + iapp));
  for (size_t i = 0; i < iapp; i++) out[*idst + i] = app[i];
  *idst += iapp;
  *dst = out;
}

void fprintf_length(char *buffer, int length, int flaglastline) {
  for (int i = 0; i < length; i++) printf("%c", buffer[i]);
  if (!flaglastline) printf("\n");
}

void output(FILE *fp, flags flag) {
  if (fp != NULL) {
    char *stroka = NULL;
    size_t length = 0;
    int lastLineBlank = 0, flaglastline = 0, lineNumber = 1;
    while ((length = fgets_up2NL(&stroka, fp)) > 0) {
      flaglastline = (stroka[length - 1] != NWL); /* preparing */
      stroka[--length] = '\0';
      if (flag.s) { /* Suppress  */
        int currentLineBlank = (length <= 1) ? 1 : 0;
        if (lastLineBlank && currentLineBlank) continue;
        lastLineBlank = currentLineBlank;
      }
      if (flag.v || flag.T) { /* showing non-printing char and TABs */
        char *temp = NULL, newC;
        size_t lengT = 0;
        for (size_t i = 0; i < length; i++) {
          if (((0 <= stroka[i]) && (stroka[i] <= 31) && (stroka[i] != TAB) &&
               flag.v) ||
              (flag.T && (stroka[i] == TAB))) {
            strcat_all(&temp, &lengT, "^", 1);
            newC = stroka[i] + 64;
            strcat_all(&temp, &lengT, &newC, 1);
          } else if (flag.v && (stroka[i] == DEL)) {
            strcat_all(&temp, &lengT, "^?", 2);
          } else {
            strcat_all(&temp, &lengT, stroka + i * sizeof(char), 1);
          }
        }
        str_free_end_null(&stroka);
        length = 0;
        strcat_all(&stroka, &length, temp, lengT);
        str_free_end_null(&temp);
      }
      if (((flag.b || flag.n) && (length >= 1)) ||
          (flag.n && !flag.b && (length == 0))) { /* numering */
        char *temp = NULL;
        size_t lengT = 0;
        strcat_all(&temp, &lengT, stroka, length);
        str_free_end_null(&stroka);
        length = 0;
        char prefix[8];
        sprintf(prefix, "%*d\t", 6, lineNumber++);
        strcat_all(&stroka, &length, prefix, 7);
        strcat_all(&stroka, &length, temp, lengT);
        str_free_end_null(&temp);
      }
      /* Show EL end line*/
      if (flag.E && !flaglastline) strcat_all(&stroka, &length, "$", 1);
      fprintf_length(stroka, length, flaglastline); /* printing */
    }
  }
}

int main(int argc, char **argv) {
  flags flag = {0, 0, 0, 0, 0, 0, 0, 0};
  int currentFile = 0, IsError = input(argc, argv, &flag, &currentFile);
  FILE *fp;
  while ((currentFile < argc) && !IsError) {
    if ((IsError = ((fp = fopen(argv[currentFile], "r")) == NULL))) {
      fprintf(stderr, "%s: %s: No such file or directory\n", argv[0],
              argv[currentFile]);
    } else {
      output(fp, flag);
      fclose(fp);
    }
    currentFile++;
  }
  return 0;
}
