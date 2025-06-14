
#include "first_pass.h"
 
ERROR_FIRST_PASS * first_pass(FILE *am )
{
    char line[LINE_LENGTH];
    short num; // in am
    opcode op;
    if(!(error_arr=(ERROR_FIRST_PASS*)malloc(sizeof(ERROR_FIRST_PASS))))
    {
        perror("DF");
        exit(1);
    }
    error_arr->arr_err=NULL;
    error_arr->size=0;
    if(!(Data_Table_=(data_table_*)malloc(sizeof(data_table_))))
    {
        exit(1);
    }
    Data_Table_->data_table=NULL;
    Data_Table_->size=0;
    reset_has();
    while (fgets(line,LINE_LENGTH,am))
    {
        if(!is_comment_or_empty(line)) // if the line is not comment or empty
        {
            if(strstr(line,STRING)!=NULL)
            {
                first_pass_label_string(line);
            }
            else if (strstr(line,DATA)!=NULL)
            {
                first_pass_label_data(line);
            }
            else if (strstr(line,EXTERN)!=NULL)
            {
                first_pass_label_extern(line);
            }
            else if((op = what_opcode_in_line(line))!= ERROR_opcode) // if there is opcode
            {  
                num = line_to_add_instruction(line,op);
                if(num!=-1)
                    I_line+= num+1;
            }
            
        } 
        cuur_line++;
    }
   update_lable_line();
    return error_arr;
}



/////******** other help function  *********////////////

int is_comment_or_empty(char line[LINE_LENGTH]) // returh 1 if the line is comment or empty , 0 otherwise
{
    if(line && line[0]==';') // this line is a comment
        return 1;
    return strlen(line)==strspn(line," \t\n"); // if line onclude only white space strlen(ch)=0 
}

void first_pass_label_string(char line[LINE_LENGTH]) // get line that have ".string" and insert the label/data , if there error put it in file
{
    lable *la=NULL;;
    data *da = NULL;
    char ch,name[LABLE_SIZE];
    if(sscanf(line,"%[^:]:%c",name,&ch)==2) // it is in moled  label: .string..
    {
        
        switch (check_label_name(name,ch))
        {
        case -1: // the label name is save word
            if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            error_arr->arr_err[error_arr->size].en=LABEL_CANT_BE_SAVED_WORD;
            error_arr->size++;
            return;
        case 0: // if the length is illegal or no space after the label
            if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            if(ch !=' ')// after the lable was space
                error_arr->arr_err[error_arr->size].en=EXPECTED_SPACE_AFTER_LABEL;
            else
                error_arr->arr_err[error_arr->size].en=LABEL_TOO_LONG_MISSING;
            error_arr->size++;
            return;
        case -2:
            if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            error_arr->arr_err[error_arr->size].en=LABEL_INVALID_FIRST_CHAR;
            error_arr->size++;
            return;
        case -3:
            if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            error_arr->arr_err[error_arr->size].en=LABEL_ONLY_ALPHANUMERIC_DIGIT;
            error_arr->size++;
            return;
        default: // if the name was leegal
            if(!(la= (lable*)malloc(sizeof(lable))))
            {
                perror("Not enough memory.");
                exit(1);
            }
            if(!(la->name=(char*)malloc(strlen(name)+1)))
            {
                perror("Not enough memory.");
                exit(1);
            }
            la->ext=no;
            la->line=D_line;
            la->is_d=yes;
            strcpy(la->name,name);
        }
    }
    else // no label -> moled  .string...
    {
        if( strncmp(line+strspn(line," \t"),STRING,strlen(STRING)) )// there is at least not white-space char before ".string"
        {
            if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            error_arr->arr_err[error_arr->size].en=EXTRA_STRING_B4_STRING;
            error_arr->size++;
            return;
        }
    }
    da=getData_string(strstr(line,STRING)+strlen(STRING));
    if(da != NULL) // if we succefuly scaned the data(the string)
    {
        Data_Table_->size++;
        Data_Table_->data_table = (data**)realloc(Data_Table_->data_table,(sizeof(data*))*Data_Table_->size*Data_Table_->size);// add one pointer in the data table
        Data_Table_->data_table[Data_Table_->size-1]=da;
    }
    if(la) // if we scaned succefuly label
    {
        if(find_labe(la->name)) // the label already exist
        {
            if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            error_arr->arr_err[error_arr->size].en=LABEL_ALREADY_EXISTS;
            error_arr->size++;
            free(la->name);
            free(la);
            return;
        }
        insert_to_hash(la);
    }
    
}

void first_pass_label_data(char line[LINE_LENGTH]) // get line that have ".data" and insert the label/data , if there error put it in file
{
    lable *la=NULL;;
    data *da = NULL;
    char ch,name[LABLE_SIZE];
    if(sscanf(line,"%[^:]:%c",name,&ch)==2) // it is in moled  label: .data..
    {
        switch (check_label_name(name,ch))
        {
        case -1: // the label name is save word
            if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            error_arr->arr_err[error_arr->size].en=LABEL_CANT_BE_SAVED_WORD;
            error_arr->size++;
            return;
        case 0: // if the length is illegal or no space after the label
            if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            if(ch !=' ')// after the lable was space
                error_arr->arr_err[error_arr->size].en=EXPECTED_SPACE_AFTER_LABEL;
            else
                error_arr->arr_err[error_arr->size].en=LABEL_TOO_LONG_MISSING;
            error_arr->size++;
            return;
        case -2:
            if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            error_arr->arr_err[error_arr->size].en=LABEL_INVALID_FIRST_CHAR;
            error_arr->size++;
            return;
        case -3:
            if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            error_arr->arr_err[error_arr->size].en=LABEL_ONLY_ALPHANUMERIC_DIGIT;
            error_arr->size++;
            return;
        default: // if the name was leegal
               if(!(la= (lable*)malloc(sizeof(lable))))
               {
                perror("Not enough memory.");
                exit(1);
               }
               if(!(la->name=(char*)malloc(strlen(name)+1)))
               {
                perror("Not enough memory.");
                exit(1);
               }
               la->ext=no;
               la->is_d=yes;
               la->line=D_line;
               strcpy(la->name,name);
        }
    }
    else // no label -> moled  .data...
    {
       
        if( strncmp(line+strspn(line," \t"),DATA,strlen(DATA)) )// there is at least not white-space char before ".data"
        {
            if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            error_arr->arr_err[error_arr->size].en=EXTRA_STRING_B4_DATA;
            error_arr->size++;
            return;
        }
    }
    if(strlen(strstr(line,DATA)+strlen(DATA))== strspn(strstr(line,DATA)+strlen(DATA)," \t\n")) // only ".data" in line
    {
        if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            error_arr->arr_err[error_arr->size].en=DATA_EXPECTED_NUM;
            error_arr->size++;
            return;
    }
    da=getData_int(strstr(line,DATA)+strlen(DATA));
    if(da != NULL) // if we succefuly scaned the data(the string)
    {
        Data_Table_->size++;
        Data_Table_->data_table = (data**)realloc(Data_Table_->data_table,(sizeof(data*))*Data_Table_->size);// add one pointer in the data table
        Data_Table_->data_table[Data_Table_->size-1]=da;
    }
    if(la) // if we scaned succefuly label
    {
        if(find_labe(la->name)) // the label already exist
        {
            if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            error_arr->arr_err[error_arr->size].en=LABEL_ALREADY_EXISTS;
            error_arr->size++;
            free(la->name);
            free(la);
            return;
        }
        insert_to_hash(la);
    }
}

void first_pass_label_extern(char line[LINE_LENGTH])// get line that have ".extern" and insert label to the hash , if there error put it in file
{
    char ch,label_name[LABLE_SIZE];
    lable *la;
    if(sscanf(line,"%[^:]:%c",label_name,&ch)==2) // it is in moled  label: .extern..
    {
        switch (check_label_name(label_name,ch))
        {
        case -1: // the label name is save word
            if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            error_arr->arr_err[error_arr->size].en=LABEL_CANT_BE_SAVED_WORD;
            error_arr->size++;
            return;
        case 0: // if the length is illegal or no space after the label
            if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            if(ch !=' ')// after the lable was space
                error_arr->arr_err[error_arr->size].en=EXPECTED_SPACE_AFTER_LABEL;
            else
                error_arr->arr_err[error_arr->size].en=LABEL_TOO_LONG_MISSING;
            error_arr->size++;
            return;
        case -2:
            if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            error_arr->arr_err[error_arr->size].en=LABEL_INVALID_FIRST_CHAR;
            error_arr->size++;
            return;
        case -3:
            if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            error_arr->arr_err[error_arr->size].en=LABEL_ONLY_ALPHANUMERIC_DIGIT;
            error_arr->size++;
            return;
        default: // if the name was leegal
               if(!(la= (lable*)malloc(sizeof(lable))))
               {
                perror("Not enough memory.");
                exit(1);
               }
               if(!(la->name=(char*)malloc(strlen(label_name)+1)))
               {
                perror("Not enough memory.");
                exit(1);
               }
               la->ext=no;
               la->is_d=no;
               la->line=I_line;
               strcpy(la->name,label_name);
        }
    }
    else // no label -> moled  .extern...
    {
       
        if( strncmp(line+strspn(line," \t"),EXTERN,strlen(EXTERN)) )// there is at least not white-space char before ".extern"
        {
            if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            error_arr->arr_err[error_arr->size].en=EXTRA_STRING_B4_EXTERN;
            error_arr->size++;
            return;
        }
    }
    insert_extern_label(strstr(line,EXTERN)+strlen(EXTERN));
}

void insert_extern_label(char *line)
{
    char label_name[LABLE_SIZE],*str;
    lable *la;
    sscanf(line,"%s",label_name);
    if(check_label_name(label_name,' ')!=1) //chek label name
    {
        if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            if(!strlen(label_name)) // no label just white-space after.extern
                error_arr->arr_err[error_arr->size].en=EXTERN_NO_LABEL;
            else
                error_arr->arr_err[error_arr->size].en=EXTERN_INVALID_LABEL;
            error_arr->size++;
        return;
    }
    str=strstr(line,label_name)+strlen(label_name);
    if(strspn(str," \t\n")!=strlen(str))// if after the label name there is string other than white-space
    {
        if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
        error_arr->arr_err[error_arr->size].line=cuur_line;
        error_arr->arr_err[error_arr->size].en=EXTERN_TOO_MANY_OPERANDS;
        error_arr->size++;
        return;
    }
    if(!(la=find_labe(label_name))) // label dosen't exiset
    {
        if(!(la=(lable*)malloc(sizeof(lable))))
        {
            perror("Not enough memory.");
            exit(1);
        }
        if(!(la->name=(char*)malloc(strlen(label_name)+1)))
        {
            perror("Not enough memory.");
            exit(1);
        }
        la->ext=yes;
        la->is_d=no;
        strcpy(la->name,label_name);
        insert_to_hash(la);
    }
    else if( la->ext == no) // label exist and not extern
    {
        if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
        error_arr->arr_err[error_arr->size].line=cuur_line;
        error_arr->arr_err[error_arr->size].en=LABEL_CANT_BE_EXTERN;
        error_arr->size++;
        return;
    }
}

data * getData_string(char *data_string) // retrun the data structure if data(.string) have been scaend llegaly ,otherwise NULL and input the error in erfile
{
    data *da;
    char *first_quot,*last_quot;
    int length_data=0;
    if(!(first_quot=strchr(data_string,'\"'))) // if no quote in after .string
    {
       if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
        error_arr->arr_err[error_arr->size].line=cuur_line;
        error_arr->arr_err[error_arr->size].en=(strlen(data_string)==strspn(data_string," \t\n"))?STRING_TOO_MISSING_OPERANDS:STRING_OPERAND_NOT_VALID;
        error_arr->size++;
        return NULL;
    }
    last_quot=strrchr(data_string,'\"');
    length_data = last_quot-first_quot ; // this will be the length of the charcter bettwen " , " +1
    if(strspn(last_quot+1," \t\n") !=strlen(last_quot+1)|| first_quot == last_quot) // the rest of the line (at the end)contain not only a white spcae
    {
        if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
        error_arr->arr_err[error_arr->size].line=cuur_line;
        error_arr->arr_err[error_arr->size].en=STRING_OPERAND_NOT_VALID;
        error_arr->size++;
        return NULL;
    }

    if(!( da=(data*)malloc(sizeof(data))))
    {
        perror("Not enough memory.");
        exit(1);
    }
    if(!( da->arr=(char*)malloc(length_data)))
    {
        perror("Not enough memory.");
        exit(1);
    }
    strcpy((char*)(da->arr),first_quot+1); // get the string copyed
    ((char*)(da->arr))[length_data-1]='\0';
    da->type =s;
    da->size = length_data;
    D_line+=length_data; // add the the number of data word(rows) the new data take
    return da;
}

data * getData_int(char *data_string)// retrun the data structure if data(.data) have been scaend llegaly ,otherwise NULL and input the error in erfile
{
    char *tok;
    int num,*arr=NULL,size_of_array=0;
    data *da;
    if(strstr(data_string,",,")) // if ",," in the line withe no white-space
    {
        if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
        error_arr->arr_err[error_arr->size].line=cuur_line;
        error_arr->arr_err[error_arr->size].en=DATA_DOUBLE_COMMAS_ROW;
        error_arr->size++;
        return NULL;
    }
    if(data_string[strspn(data_string," \t")]==',') // start with comma
    {
        if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
        error_arr->arr_err[error_arr->size].line=cuur_line;
        error_arr->arr_err[error_arr->size].en=DATA_START_COMMA;
        error_arr->size++;
        return NULL;
    }
    tok = strtok(data_string,",");
    while (tok)
    {
        if(strspn(tok," \t")==strlen(tok)) // there is double comma in the data withe whitse-space between , example: ",   , "-> tok="   "
        {
            if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            error_arr->arr_err[error_arr->size].en=DATA_UNEXPECTED_COMMA;
            error_arr->size++;
            return NULL;
        }
        if(strstr(tok,".")) // if the number is flat
        {
            if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            error_arr->arr_err[error_arr->size].en=DATA_UNEXPECTED_DECIMAL_POINT;
            error_arr->size++;
            return NULL;
        }
        if(!my_atoi(tok,&num)) // if tok not an integer
        {
            if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            error_arr->arr_err[error_arr->size].en=DATA_EXPECTED_NUM;
            error_arr->size++;
            return NULL;
        }
        else if(!is_in_range(num)) // if it is in 14-bit
        {
            if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            error_arr->arr_err[error_arr->size].en=DATA_OUT_RANGE;
            error_arr->size++;
            return NULL;
        }
        else // add the number to the array
        {
            if(!(arr=(int*)realloc(arr,(1+size_of_array)*sizeof(int))))
            {
                perror("Not enough memory..\n");
                exit(1);
            }
            arr[size_of_array++]=num;
        }
        tok = strtok(NULL,",");
    }
    if(arr == NULL) // thier is no data after ".data"
    {
        if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
        error_arr->arr_err[error_arr->size].line=cuur_line;
        error_arr->arr_err[error_arr->size].en=DATA_OUT_RANGE;
        error_arr->size++;
        return NULL;
    }
    if(!(da=(data*)malloc(sizeof(data))))
    {
        perror("Not enough memory.\n");
        exit(1);
    }
    da->arr = (void*)arr;
    da->type= d;
    da->size = size_of_array;
    D_line += size_of_array;
    return da;
}

int my_atoi(char *str,int*num)// retrun 1 the string is int num assign it in what ans poin to, otherwise return 0
{
    int sign=1,len_of_digit; // lenth of the number (how many digit)
    int before,after; // white-space or +/- before  the digit , white-space after the digit
    before=strspn(str," \t");
    if(*(str+before)=='+')
    {
        before++;
    }
    if(*(str+before)=='-')
    {
        sign =-1;
        before++;
    }
    len_of_digit=strspn(str+before,"0123456789");
    after =strspn(str+before+len_of_digit," \t\n"); 
    if(strlen(str)!=before+len_of_digit+after) //if the string not in molde  :(optional white_space)digit(optional white_space)
        return 0;
    *num =sign*atoi(str+before);
    return 1;
    
} 

int check_label_name(char *label_name,char char_after_label)// if the label_name legal retrun 1 ,-1 if it is assmbler saved word, 0 otherwise , -2
{
    if(strlen(label_name)+1<=LABLE_SIZE && strcmp(label_name,"") && char_after_label == ' ') // if 0<label_name<=LABLE_SIZE and after it there is a space ,-3
    {
        char *word_in_asembler[]={"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop",ENTRY,EXTERN,STRING,DATA};
        int i;
        for ( i = 0; i < 21; i++) // check if the label name is one of the 20 
        {
            if(!strcmp(label_name,word_in_asembler[i])) // found one
                return -1;
        }
        for ( i = 0; i < 8; i++) // check registers
           if(strlen(label_name)==2 &&label_name[0]=='r'&& label_name[1]=='0'+i){
                return -1;}
        if(!isalpha(label_name[0]))
                return -2;
        for ( i = 1; i < strlen(label_name); i++)
        {
            if(!isalnum(label_name[i]))
                return -3;
        }
        
        return 1;
    }
    return 0;
}

opcode what_opcode_in_line(char line[LINE_LENGTH])// rerturn the opcode acoording to enum , if the line have no opcode return ERROR_opcode
{
    char *str;
    if((str=strstr(line,"mov"))&&isspace(str[strlen("mov")]))
        return MOV;
    if((str=strstr(line,"cmp"))&&isspace(str[strlen("cmp")]))
        return CMP;
    if((str=strstr(line,"add"))&&isspace(str[strlen("add")]))
        return ADD;
    if((str=strstr(line,"sub"))&&isspace(str[strlen("sub")]))
        return SUB;
    if((str=strstr(line,"not"))&&isspace(str[strlen("not")]))
        return NOT;
    if((str=strstr(line,"clr"))&&isspace(str[strlen("clr")]))
        return CLR;
    if((str=strstr(line,"lea"))&&isspace(str[strlen("lea")]))
        return LEA;
    if((str=strstr(line,"inc"))&&isspace(str[strlen("inc")]))
        return INC;
    if((str=strstr(line,"dec"))&&isspace(str[strlen("dec")]))
        return DEC;
    if((str=strstr(line,"jmp"))&&isspace(str[strlen("jmp")]))
        return JMP;
    if((str=strstr(line,"bne"))&&isspace(str[strlen("bne")]))
        return BNE;
    if((str=strstr(line,"red"))&&isspace(str[strlen("red")]))
        return RED;
    if((str=strstr(line,"prn"))&&isspace(str[strlen("prn")]))
        return PRN;
    if((str=strstr(line,"jsr"))&&isspace(str[strlen("jsr")]))
        return JSR;
    if((str=strstr(line,"rts"))&&isspace(str[strlen("rts")]))
        return RTS;
    if((str=strstr(line,"stop"))&&isspace(str[strlen("stop")]))
        return STOP;
    return ERROR_opcode;
}

char * what_opcode_string(opcode op)//get opcode return the apropriate string , if op=ERROR_opcode rertun NULL
{
    switch (op)
    {
    case MOV:
        return "mov";
    case CMP:
        return  "cmp";
    case ADD:
        return "add";
    case SUB:
        return "sub";
    case NOT:
        return "not";
    case CLR:
        return "clr";
    case LEA:
        return "lea";
    case INC:
        return "inc";
    case DEC:
        return "dec";
    case JMP:
        return "jmp";
    case BNE:
        return "bne";
    case RED:
        return "red";
    case PRN:
        return "prn";
    case JSR:
        return "jsr";
    case RTS:
        return "rts";
    case STOP:
        return "stop";
    default:
        return NULL;
    }
}

char * if_label_before_opcode(char line[LINE_LENGTH]) // rertun the label name if the line of opcode havel label declar, otherwise NULL
{
    char ch,name[LABLE_SIZE],*return_name;
    int num;
    if(sscanf(line,"%[^:]:%c",name,&ch)==2) // it is in moled  label: opcode parameters..
    {
        if( (num=check_label_name(name,ch)) !=1)
        {
            if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            if(num ==-1)
                error_arr->arr_err[error_arr->size].en=LABEL_CANT_BE_SAVED_WORD;
            else if(num ==0 && ch ==' ') // after the lable was space
                error_arr->arr_err[error_arr->size].en=EXPECTED_SPACE_AFTER_LABEL;
            else if(num ==0 && ch !=' ') // after the lable was no space , label to long/short
                error_arr->arr_err[error_arr->size].en=LABEL_TOO_LONG_MISSING;
            else if(num == -2)// num = -2
                error_arr->arr_err[error_arr->size].en=LABEL_INVALID_FIRST_CHAR;
            else // num= -3
                error_arr->arr_err[error_arr->size].en=LABEL_ONLY_ALPHANUMERIC_DIGIT;
            error_arr->size++;
            return NULL;
        }
        if(!(return_name=(char*)malloc(strlen(name)+1)))
        {
            perror("Not enough memory.\n");
            exit(1);
        }
        strcpy(return_name,name);
        return return_name; // it was leegal name
    }
    return NULL;
}

int line_to_add_instruction(char line[LINE_LENGTH],opcode op)//retrun how many added words the line proudose, -1 if error in line
{
    char *label_name;
    lable *la;
    label_name = if_label_before_opcode(line);
    if (label_name) //if there is leggal label
    {
        if(!if_no_extra_string_brfore_opcode(line+strlen(label_name)+1,op))// there is a string(not white-space)between label and opcode , +1 for ':'
        {
            if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            error_arr->arr_err[error_arr->size].en=EXTRA_STRING_OPCODE;
            error_arr->size++;
            return -1;
        }
         // insert the label
        if(find_labe(label_name)) // labbel exist
        {
            if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
            error_arr->arr_err[error_arr->size].line=cuur_line;
            error_arr->arr_err[error_arr->size].en=LABEL_ALREADY_EXISTS;
            error_arr->size++;
            return -1;
        }
        if(!(la=(lable*)malloc(sizeof(lable)))) // add new label
        {
            perror("Not enough memory.\n");
            exit(1);
        }
        la->name=label_name;
        la->ext=no;
        la->line=I_line;
        la->is_d=no;
        insert_to_hash(la);
    }
    else if (!if_no_extra_string_brfore_opcode(line,op)) // there is a string , but no label name
    {
       if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
        error_arr->arr_err[error_arr->size].line=cuur_line;
        error_arr->arr_err[error_arr->size].en=EXTRA_STRING_OPCODE;
        error_arr->size++;
        return -1;
    }
    switch (op) //,,,,,,,,,JMP,,,,,,,ERROR_opcode=-1
    {
    case MOV: case CMP: case ADD: case SUB: case LEA: // if the 2 operands register rertun 1 else 2
        if ( if_first_operand_register(line,what_opcode_string(op)) && if_second_operand_register(line,what_opcode_string(op)) )
            return 1;
        else 
            return 2;
        break;
    case PRN: case RED: case DEC: case NOT: case CLR: case INC:
            return 1;
        break;;
    case RTS: case STOP:
            return 0;
        break;;
    case JSR: case BNE: case JMP:
            if(strstr(strstr(line,what_opcode_string(op))+3,"(")  )// if have 2 paramter
            { 
                char par1[LABLE_SIZE],par2[LABLE_SIZE];
                sscanf(strstr(line,"(")+1,"%[^,],%[^)])",par1,par2); 
                if(if_register(par1) && if_register(par2))
                    return 2;
                else // not 2 register
                    return 3;
            }
            else 
                return 1;
    default: // not command / datat line
        if(!(error_arr->arr_err= ((ERROR_LINE*)realloc(error_arr->arr_err,(sizeof(ERROR_LINE))*(error_arr->size+1))) ))
            {
                perror("Not enough memory.\n");
                exit(1);
            }
        error_arr->arr_err[error_arr->size].line=cuur_line;
        error_arr->arr_err[error_arr->size].en=COMMAND_NOT_FOUND;
        error_arr->size++;
        return -1;
    }
}

int if_no_extra_string_brfore_opcode(char line[LINE_LENGTH],opcode op)// retrun 1 if therer is no string(only white-space) before opcode, otherwise 0
{
    char *op_in_line;
    op_in_line =strstr(line,what_opcode_string(op));
    return (op_in_line-line) == strspn(line," \t"); // if the white_space at first equal to the number of char berore opcode
}

int if_first_operand_register(char line[LINE_LENGTH],char *op_string) //retrun 1 if the operand is register,(opcode=op_string),0 otherwise
{
    char reg[LINE_LENGTH-3];
    sscanf(strstr(line,op_string)+strlen(op_string)+1,"%[^,]",reg);
    if(strlen(reg)>1 &&reg[0]=='r' && reg[1]<'8' && reg[1]>='0')
    {
        return (2+(int)strspn(reg+2," \t")) == strlen(reg); // if only white-space after the register
    }
    return 0;
}

int if_second_operand_register(char line[LINE_LENGTH],char *op_string)// retrun 1 if the second operand is register,(opcode=op_string) ,0 otherwise
{
    char reg[LINE_LENGTH-3],*ch;
    sscanf(strstr( strstr(line,op_string),",")+1,"%s",reg); // scane after the comma after the op_string
    if(strlen(reg)>1 &&reg[0]=='r' && reg[1]<'8' && reg[1]>='0')
    {
        ch = strstr(strstr( strstr(line,op_string),",") , reg) +2; //pointer to the char after the  register
        return strlen(ch)== strspn(ch," \t\n") &&  strlen(ch); // if only white-space after the register
    }
    return 0;
}

int if_register(char *reg)//retrun 1 if register,0 otherwise
{
    return (strlen(reg)== 2 +strspn(reg+2," \t\n") )&& reg[0]=='r' && reg[1]<'8' && reg[1]>='0';
}

int is_in_range(int num) // if the number out if range 13-bit retuen 0 , otherwise 1
{
    if(num >=0)
        return num<= ((int)pow(2,NUMBER_BIT-1-2)-1);
    else
         return (-1*num)<=((int)pow(2,NUMBER_BIT-2)-1);
}

void free_has_map()
{
    int i;
    lable *la;//*pre;
    for ( i = 0; i < HASH_SIZE; i++)
    {
        la=label_map[i];
        while (la)
        {
            free(la->name);
            label_map[i] = la->next;
            free(la);
        }
    }    
}

void update_lable_line()
{
    int i;
    lable *la;
    for ( i = 0; i < HASH_SIZE; i++)
    {
        la = label_map[i];
        while (la)
        {
            if(la->is_d==yes)
                la->line += I_line;
            la = la->next;
        }
        
    }
}
