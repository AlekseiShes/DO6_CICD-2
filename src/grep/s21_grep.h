#include "../common/common.h"

#ifndef SRC_S21_GREP_H_
#define SRC_S21_GREP_H_
#define ARRSIZE 1024

// https://www.gnu.org/software/grep/manual/grep.pdf

typedef struct S_getopt_long_Params_Colletion golparcol;

typedef struct {
  // int e; /* --regexp=patterns ex: word or 'desired string' or "the word"*/
  int i; /* --ignore-case ex: 'S' is equal 's' */
  int v; /* --invert-match ex: select non-matching lines */
  int c; /* --count ex: print a count of matching lines for each input file */
  int l; /* --files-with-matches ex: print name of each input patern-files */
  int n; /* --line-number ex: Prefix each line of output with the 1-based */
  // int f; /* --file=filename ex: /patch/filename.c */
  int h; /* --no-filename Suppress the prefixing of file names on output */
  int s; /* --no-messages Suppress error msg about nonexist, unread files */
  int o; /* --only-matching Print only matched nonEmptParts of matching lines*/
} flags;

typedef struct mylist {
  int cnt;            /* count */
  char *arr[ARRSIZE]; /* static size = ARRSIZE */
} mylist;

typedef struct mystroka {
  int cnt;      /* count */
  char *stroka; /* dinamic size */
} mystroka;

typedef struct {    /* samples_and_filenames */
  mylist file2find, /* files_to_search.txt */
      file_samp,    /* -f file_with_samples.txt*/
      samples;      /* -e 'samles'*/
} ss_and_fns;

#endif  // SRC_S21_GREP_H_