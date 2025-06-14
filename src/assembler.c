#include "assembler.h"
#include "first_pass.h"
#include "second_pass.h"
short int I_line=START_INSTRCTION;
short int D_line=0;
short int cuur_line=1;
data_table_ *Data_Table_;

int main(int argc,char ** argv)
{
    FILE *fp,*am;
    int i;
    for ( i = 1; i < argc; i++)
    {
        I_line=START_INSTRCTION;
        D_line=0;
        cuur_line=1;
        char filename[strlen(argv[i]+4)];
        strncpy(filename,argv[i],strlen(argv[i]));
        strcat(filename,".as");
        printf("prosicing %s:\n",argv[i]);
        if(!(fp=fopen(filename,"r")))
        {
            perror("File not open");
            exit(1);
        }
        filename[strlen(argv[i])]='\0';
        strcat(filename,".am");
        if(!(am=fopen(filename,"w")))
        {
            perror("File not open");
            exit(1);
        }
        pre_assembler(fp,am);
        fclose(am);
        fclose(fp);
        if(!(am=fopen(filename,"r")))
        {
            perror("File not open");
            exit(1);
        }
        filename[strlen(argv[i])]='\0';
        first_pass(am);
        secound_pass(am,filename);
        fclose(fp);
    }
    return 0;
}
