#include <getopt.h>
#include <stdio.h>

#ifndef SRC_COMMON_COMMON_H_
#define SRC_COMMON_COMMON_H_
#define NWL '\n'
#define TAB '\t'
#define DEL 127

typedef struct {
  char *opt;
  struct option long_opt[];
} getopt_long_ConstParams_Colletion;

typedef getopt_long_ConstParams_Colletion *InitPtr;

typedef struct S_getopt_long_Params_Colletion {
  const getopt_long_ConstParams_Colletion *read;
  int opt_index;
} getopt_long_Params_Colletion;

InitPtr Init_getopt_long_Params_Cat();
InitPtr Init_getopt_long_Params_Grep();
size_t fgets_up2NL(char **stroka, FILE *fp);
void str_free_end_null(char **str);

#endif  // SRC_COMMON_COMMON_H_
