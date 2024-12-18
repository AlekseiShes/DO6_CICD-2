#include "common.h"

#include <stdlib.h>

// const
InitPtr Init_getopt_long_Params_Cat() {
  static getopt_long_ConstParams_Colletion out = {
      "+beEnstTv?",
      {{"number-nonblank", no_argument, NULL, 'b'},
       {"visible-character-end-line", no_argument, NULL, 'e'},
       {"end-line", no_argument, NULL, 'E'},
       {"number", no_argument, NULL, 'n'},
       {"squeeze-blank", no_argument, NULL, 's'},
       {"visible-character-tab", no_argument, NULL, 't'},
       {"tab", no_argument, NULL, 'T'},
       {"visible-character", no_argument, NULL, 'v'},
       {NULL, 0, NULL, 0}}};
  return &out;
}

InitPtr Init_getopt_long_Params_Grep() {
  static getopt_long_ConstParams_Colletion out = {
      "-e:ivclnf:hso?",
      {{"regexp", required_argument, NULL, 'e'},
       {"ignore-case", no_argument, NULL, 'i'},
       {"invert-match", no_argument, NULL, 'v'},
       {"count", no_argument, NULL, 'c'},
       {"files-with-matches", no_argument, NULL, 'l'},
       {"line-number", no_argument, NULL, 'n'},
       {"file", required_argument, NULL, 'f'},
       {"no-filename", no_argument, NULL, 'h'},
       {"no-messages", no_argument, NULL, 's'},
       {"only-matching", no_argument, NULL, 'o'},
       {NULL, 0, NULL, 0}}};
  return &out;
}

/* analogy of fgets from <string.h> */
size_t fgets_up2NL(char **stroka, FILE *fp) {
  if (*stroka != NULL) str_free_end_null(stroka);
  size_t len = 0;
  char c = 0;
  char *p[2] = {NULL, NULL};
  while (!(c == EOF || c == NWL)) {
    p[(len + 1) % 2] = (char *)realloc(p[len % 2], (len + 1) * sizeof(char));
    p[len % 2] = NULL;
    *(p[(len + 1) % 2] + len) = (c = getc((fp == NULL ? stdin : fp)));
    len++;
  }
  *stroka = (p[0] == NULL ? p[1] : p[0]);
  if ((len == 1) && (*stroka[0] < 0)) {
    len = 0;
    if (*stroka != NULL) str_free_end_null(stroka);
  }
  return len;
}

void str_free_end_null(char **str) {
  if (*str != NULL) {
    free(*str);
    *str = NULL;
  }
}