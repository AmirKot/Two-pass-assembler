#ifndef _HEADER_ASEM
#define _HEADER_ASEM
#include "libary.h"
typedef struct macro
{
    char *name;
    char *data;
}macro;

int check_macro_name(macro);
int read_macro(macro *m,FILE *fp);
int maccro_index(macro arr[],int size,char *word);
void pre_assembler(FILE *fp,FILE *as);


#endif
