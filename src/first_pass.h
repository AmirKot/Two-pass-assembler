#ifndef _HEADER_FIRST_PASS
#define _HEADER_FIRST_PASS
#include "passes.h"
ERROR_FIRST_PASS * first_pass(FILE *am);
void insert_extern_label(char *line);
void update_lable_line();
#endif
