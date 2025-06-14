#include "assembler.h"
short ERROR =0;
void pre_assembler(FILE *fp,FILE *as)
{
    macro *arr;
    int num_macro=0,flag=0,i,last=0;// last: 1 if last thing in the line was macro that we prined, 0 otherwise
    char line[LINE_LENGTH],*chp,name[LINE_LENGTH]; 
    long line_num=1;

    
    if(!(arr = (macro *)malloc(sizeof(macro))))
    {
        perror("Not enough memory");
        exit(2);
    }
    while (fgets(line,LINE_LENGTH,fp))
    {
        if((chp=strstr(line,"mcr"))) // start reading macro
        {
            sscanf(chp+3,"%s[^'\n']",name);
            if(!(arr[num_macro].name = (char*)malloc(strlen(name)+1))) // name 
            {
                perror("Not enough memory");
                exit(2);
            }
            strcpy(arr[num_macro].name,name);
            if(!check_macro_name(arr[num_macro])) // if not valid name
            {
                printf("Error line %ld: name of macro not valid.\n",line_num);
                free(arr[num_macro].name);
            }
            else
            {
                flag = read_macro(&arr[num_macro],fp);
                num_macro++;
                if(!(arr = (macro *)realloc(arr,(1+num_macro)*sizeof(macro))))
                {
                    perror("Not enough memory");
                    exit(2);
                }
            }
                
        }
        else // print line
        {   
            int i,len=0;
            if(!last && line_num>1)
             putc('\n',as);
            while (sscanf(line+len,"%s",name)==1 && len<strlen(line))
            {
                i=maccro_index(arr,num_macro,name);
                if(i!=-1)
                {
                    fprintf(as,"%s",arr[i].data);
                    len += 1+ strlen(arr[i].name);
                    last =1;
                }
                else
                {
                    fprintf(as,"%s ",name);
                    len += 1+ strlen(name)+strspn(line+len," \t\n");
                    last=0;
                }
            }
        }
        line_num++;
    }
    if(flag)
    fputs(arr[num_macro-1].data,as); // if we got mcr but did not have endmcr at the end
    for ( i =0; i <num_macro; i++)
        {
           free(arr[i].data);
           free(arr[i].name);
        }
        free(arr);
}

int check_macro_name(macro mc) // return 1 if macro name is valid , 0 otherwise
{
    char *op[]={"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop"};
    int i;
    for ( i = 0; i < 16; i++)
    {
        if(!strcmp(mc.name,op[i]))
        {
            ERROR =1;
            return 0;
        }
    }
    for ( i = 0; i < 8; i++)
    {
        if(strlen(mc.name)==2 && mc.name[0]=='r'&&mc.name[1]==(i+'0'))
        {
            ERROR =1;
            return 0;
        }
    }
    return 1;
}

int read_macro(macro *m,FILE *fp) // reteurn 0 if we got to "endmcr" , 1 otherwise
{
    short flag_first= 1;
    char line[LINE_LENGTH+1];
     while (fgets(line,LINE_LENGTH,fp))
     {
       if(strstr(line,"endmcr")) // if got to end of macro
       {
            return 0;
       }
       else
       {
           if(!(m->data = (char*)realloc(flag_first?NULL:m->data,flag_first?strlen(line)+1:strlen(line)+strlen(m->data))))
           {
                perror("Not enough memory");
                exit(2);
           }
           if(flag_first)
               strcpy(m->data,line);
             else
                strcat(m->data,line);
           flag_first = 0;
       }
     }
     return 1;
}

int maccro_index(macro arr[],int size,char *word) // return the index if word if it is macro name , -1 otherwise
{
    int i;
    for ( i = 0; i < size; i++)
    {
       if(!strcmp(word,arr[i].name))
            return i;
    }
    return -1;
}

