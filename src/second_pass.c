#include "second_pass.h"
FILE *object_file;
void secound_pass(FILE *am,char *file_name)
{ 
    char line[LINE_LENGTH],out_put_name[strlen(file_name)+4];
    FILE *entry_file,*extern_file;
    short next_line_with_error=-1,i=0,num_entry=0; // -1 if no there is no other line with error
    RAM *ram;
    bool flag_error;
    ERROR_ENUM err_curr_line;
    entry_label=NULL;
    if(!error_arr->arr_err) // no error in the first
    { 
        flag_error = no;
        if(!(ram=(RAM*)malloc(sizeof(RAM))))
        {
            perror("Not enough memory.\n");
            exit(1);
        }
        if(!(ram->word_ram=(Word*)malloc(I_line*sizeof(Word)))) // number of rows in the RAM =I_line
        {
            perror("Not enough memory.\n");
            exit(1);
        }
        ram->number_of_word_enterd=0;
    }
    else
    { 
        flag_error = yes;
        next_line_with_error = (error_arr->arr_err)[i].line;
    }
    if(!(arr_ext=(Extern_array*)malloc(sizeof(Extern_array))))
    {
        perror("Not enough memory.\n");
        exit(1);
    }
    
    arr_ext->size = 0;
    arr_ext->arr_extern = NULL;
    rewind(am);
    cuur_line =1;
    while (fgets(line,LINE_LENGTH,am))
    {
        if(!is_comment_or_empty(line)) // !strstr(line,STRING)&&!strstr(line,DATA)&&!strstr(line,EXTERN)
        { 
            if(cuur_line ==next_line_with_error) // if we found error in this line in the first_pass
            {
                print_error(cuur_line,error_arr->arr_err[i].en);
                if(i+1<error_arr->size)
                {
                    next_line_with_error = (error_arr->arr_err)[++i].line;
                }
            }
            else if(strstr(line,ENTRY)) // .extern
            {
                err_curr_line = secound_pass_entry(line);
                if(err_curr_line!=NO_ERROR)
                {
                    print_error(cuur_line,err_curr_line);
                    flag_error = yes;
                }
                else
                    num_entry++;
            }
            else if(!strstr(line,STRING)&&!strstr(line,DATA)&&!strstr(line,EXTERN)) // we scaned the line in the first_pass
            {
                opcode op ;
                ERROR_ENUM er;
                op =what_opcode_in_line(line);
                switch (op)
                {
                case MOV: case ADD: case SUB: case CMP:
                    if((er = secound_pass_mov_add_sub(line,op,ram))!= NO_ERROR)
                    {
                        print_error(cuur_line,er);
                         flag_error = yes;
                    }
                    break;
                case NOT: case CLR: case INC: case DEC: case RED: case PRN:
                    if((er = secound_pass_not_clr_inc_dec_red(line,op,ram))!= NO_ERROR)
                    {
                        print_error(cuur_line,er);
                         flag_error = yes;
                    }
                    break;
                case JMP: case BNE: case JSR: 
                    if((er = secound_pass_jmp_bne_jsr(line,op,ram))!= NO_ERROR)
                    {
                        print_error(cuur_line,er);
                         flag_error = yes;
                    }
                    break;
                case STOP: case RTS:
                    if((er = secound_pass_stop_rts(line,op,ram))!= NO_ERROR)
                    {
                        print_error(cuur_line,er);
                        flag_error = yes;
                    }
                    break;
                case LEA:
                    if((er = secound_pass_lea(line,op,ram))!= NO_ERROR)
                    {
                        print_error(cuur_line,er);
                        flag_error = yes;
                    }
                    break;
                case ERROR_opcode:
                    flag_error = yes;
                    print_error(cuur_line,COMMAND_NOT_FOUND);
                
                }
            }
        }
        cuur_line++;
    }
    if(flag_error == no)
    {
        strcpy(out_put_name,file_name);
        strcat(out_put_name,".ob");
        if(!(object_file=fopen(out_put_name,"wb")))
        {
            perror("File not open");
            exit(1);
        }
        fprintf(object_file," %d\t%d\n",I_line-100,D_line); 
        for ( i = 0; i < ram->number_of_word_enterd; i++) // print instruction
        {
            print_word(ram->word_ram[i]);
        }
        ///   printing Data:
        for ( i = 0; i < Data_Table_->size; i++) // print data
        {
            print_Data_to_RAM(*(Data_Table_->data_table[i]));
        }
        fclose(object_file);
         ///////   printing Extern:
         if( arr_ext->size)
         {
            out_put_name[strlen(file_name)]= '\0';
            strcat(out_put_name,".ext");
            if(!(extern_file=fopen(out_put_name,"w")))
            {
                perror("File not open");
                exit(1);
            }
            for ( i = 0; i < arr_ext->size; i++)
            {
               fprintf(extern_file,"%s\t%d\n",arr_ext->arr_extern[i].la->name,arr_ext->arr_extern[i].line);
            }
            fclose(extern_file);
         }
        //////printing Entry:
        if(num_entry)
        {
            out_put_name[strlen(file_name)]= '\0';
            strcat(out_put_name,".ent");
            if(!(entry_file=fopen(out_put_name,"w")))
            {
                perror("File not open");
                exit(1);
            }
            for ( i = 0; i < num_entry ; i++)
            {
                fprintf(entry_file,"%s\t%d\n",entry_label[i]->name,entry_label[i]->line);
            }
            fclose(entry_file);
        }
    }
    /// free 
    free_error();
    if(ram)
        free_RAM(ram);
    free_entery();
    free_extern();
    free_data();
}

ERROR_ENUM secound_pass_entry(char line[LINE_LENGTH])
{
    char ch,label_name[LINE_LENGTH],name_entry[LABLE_SIZE],*str;
    lable *la;
    if(sscanf(line,"%[^:]:%c",label_name,&ch)==2) // it is in moled  label: .entry..
    {
        switch (check_label_name(label_name,ch))
        {
        case -1: // the label name is save word
            return LABEL_CANT_BE_SAVED_WORD;
        case 0: // if the length is illegal or no space after the label
            if(ch !=' ')// after the lable was space
                return EXPECTED_SPACE_AFTER_LABEL;
            else
                return LABEL_TOO_LONG_MISSING;
        case -2:
            return LABEL_INVALID_FIRST_CHAR;
        case -3:
            return LABEL_ONLY_ALPHANUMERIC_DIGIT;
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
    else // no label -> moled  .entry...
    {
       
        if( strncmp(line+strspn(line," \t"),ENTRY,strlen(ENTRY)) )// there is at least not white-space char before ".extern"
        {
            return STRING_B4_ENTRY;
        }
    }
    str = strstr(line,ENTRY)+strlen(ENTRY);
    sscanf(str,"%s",name_entry);
   if(strspn(str," \t\n")==strlen(str)) // no label was found after .entry
        return ENTRY_NO_LABEL;
    if(!(la=find_labe(name_entry))) // no label in this name
        return LABEL_NAME_DOSENT_EXISTS_ENTRY;
    str = strstr(line,name_entry)+strlen(name_entry);
    if(strspn(str," \t\n")!= strlen(str)) // if after the label name there is string(with no white-space)
        return ENTRY_TOO_MANY_OPERANDS;
    insert_entry_label(la);
    return NO_ERROR;
}


void insert_entry_label(lable * la)
{
    /// entry_label is upword 
    lable *tmp,*t;
    static int num_entry=0;
    int i,j;
    if(!entry_label ) // the first entry label
    {
        if(!(entry_label=(lable**)malloc(sizeof(lable*))))
        {
            perror("Not enough memory.\n");
            exit(1);
        }
        entry_label[0]= la;
        num_entry++;
        return;
    }
    if(!(entry_label=(lable**)realloc(entry_label,(num_entry+1)*sizeof(lable*))))
    {
        perror("Not enough memory.\n");
        exit(1);
    }
    for ( i = 0; i < num_entry; i++) // we add tha label to the array to the apropriate place
    {
        if(entry_label[i]->line > la->line) // the label la must be before  
        {
           tmp = entry_label[i];
           entry_label[i] =la;
           break;
        }
    }
    for ( j = i+1; j < num_entry+1; j++)
    {
        t= entry_label[j];
        entry_label[j]=tmp;
        tmp = t;
    }
    num_entry++;// we added on more place it will be the label with the max (line feld)
}

ERROR_ENUM secound_pass_mov_add_sub(char line[LINE_LENGTH],opcode op,RAM *ram)
{
    char operand1[LABLE_SIZE],operand2[LABLE_SIZE]; // might be register or label ,operand2 might be number
    char *str;
    int num1,num2;
    operand_type oper1,oper2;
    ERROR_ENUM er;
    str = strstr(line,what_opcode_string(op)) +strlen(what_opcode_string(op)); // point after the opcode
    if( !strstr(str,",")) // no comma after the opcode
        return EXPECTED_COMMA_BETWEEN_OPERANDS;
    sscanf(str,"%[^,]",operand1);
    sscanf(str+strspn(operand1," \t"),"%[^,]",operand1); // delete white-space
    str = strstr(str,operand1)+strlen(operand1); // point after the first operand
    if(strcspn(str," \t,")) /// the operand have more than 30 char-> cant be label
        return COMMAND_INVALID_OPERANDS_METHODS;
    str = strstr(str,",")+1;  
    sscanf(str,"%s",operand2);
    sscanf(str+strspn(operand2," \t"),"%s",operand2); // delete white-space 
    str = strstr(str,operand2)+strlen(operand2); // point after the first operand
    if(strcspn(str," \t\n")) /// the operand have more than 30 char-> cant be labe
        return COMMAND_INVALID_OPERANDS_METHODS; 
    if(strspn(str," \t\n")!=strlen(str)) //after the second another operand
        return COMMAND_INVALID_NUMBER_OF_OPERANDS;
    oper1 = what_type_is_operand(operand1);
    oper2 = what_type_is_operand(operand2);
    if((er= check_if_the_operand_llegal(operand1,&num1,oper1))!=NO_ERROR)
        return er;
    if((er= check_if_the_operand_llegal(operand2,&num2,oper2))!=NO_ERROR)
        return er;
    if(oper2 == Number && op != CMP) // only cmp can have oper1 to be a number
        return OPERAND_CANT_B_NUMBER;

       /// now the line is leegal -> insert to the RAM
    insert_command_to_word(&(ram->word_ram[ram->number_of_word_enterd]),what_addressing_methods_operans(oper1),what_addressing_methods_operans(oper2),op);
    if(oper1==Register && oper2==Register)
    { 
        insert_Register_to_word(&(ram->word_ram[ram->number_of_word_enterd+1]), what_Register_operans(operand1),what_Register_operans(operand2)) ;
        ram->number_of_word_enterd+=2;
     return NO_ERROR;
    }
    switch (oper1) // inster firts operand
    {
    case Number:
        insert_number_to_word(&(ram->word_ram[ram->number_of_word_enterd+1]),num1);
        break;
    case Label:
        insert_label_to_word(&(ram->word_ram[ram->number_of_word_enterd+1]),find_labe(operand2),ram->number_of_word_enterd+1);
        break;;
    default: // if not the above ->Register
     insert_Register_to_word(&(ram->word_ram[ram->number_of_word_enterd+1]),what_Register_operans(operand1),r0); // r0 = 0
    }
    switch (oper2) // inster second operand
    {
    case Number:
        insert_number_to_word(&(ram->word_ram[ram->number_of_word_enterd+2]),num2);
        break;
    case Label: 
        insert_label_to_word(&(ram->word_ram[ram->number_of_word_enterd+2]),find_labe(operand2),ram->number_of_word_enterd+2);
        break;;
    default: // if not the above ->Register
     insert_Register_to_word(&(ram->word_ram[ram->number_of_word_enterd+2]),r0,what_Register_operans(operand2)); // r0 = 0
    }
    ram->number_of_word_enterd+=3;
    return NO_ERROR;
}

ERROR_ENUM secound_pass_not_clr_inc_dec_red(char line[LINE_LENGTH],opcode op,RAM *ram)
{
    char operand1[LINE_LENGTH], *str;
    lable *l1=NULL;
    int num;
    ERROR_ENUM er;
    operand_type oper1;
    str = strstr(line,what_opcode_string(op)) +strlen(what_opcode_string(op)); // point after the opcode, we cheked before it 
    sscanf(str,"%s",operand1);
    str = strstr(str,operand1)+strlen(operand1); // point after the first operand
    if(strspn(operand1," \t")==strlen(operand1)) // no paramter
        return DIRECTIVE_NO_PARAMS;
    if(strstr(str,","))
     return COMMAND_INVALID_NUMBER_OF_OPERANDS;
    if(strspn(str," \t\n")!=strlen(str)) /// the operand have more than 30 char-> cant be label
        return COMMAND_INVALID_OPERANDS_METHODS;
    oper1 = what_type_is_operand(operand1);
    if(oper1== Number && op!= PRN ) // only prn can have oper1 to be a number
    {
        return OPERAND_CANT_B_NUMBER;
    }
    else if(oper1== Label)
    {
        if(!(l1=find_labe(operand1)))
            return LABEL_OPERANDS_DOSENT_EXISTS;
    }
    if((er= check_if_the_operand_llegal(operand1,&num,oper1))!=NO_ERROR)
        return er;
    /// now the line is leegal -> insert to the RAM
    insert_command_to_word(&(ram->word_ram[ram->number_of_word_enterd]),0,what_addressing_methods_operans(oper1),op);
    if(oper1==Register)
        insert_Register_to_word(&(ram->word_ram[ram->number_of_word_enterd+1]),0,what_Register_operans(operand1));
    else if (oper1 == Label)
        insert_label_to_word(&(ram->word_ram[ram->number_of_word_enterd+1]),l1,ram->number_of_word_enterd+1);
    else
        insert_number_to_word(&(ram->word_ram[ram->number_of_word_enterd+1]),num);
    ram->number_of_word_enterd+=2;
    return NO_ERROR;
}

ERROR_ENUM secound_pass_jmp_bne_jsr(char line[LINE_LENGTH],opcode op,RAM *ram)
{
    char par2[LABLE_SIZE],par1[LABLE_SIZE],label_operand[LABLE_SIZE],*str;
    lable *la;
    str = strstr(line,what_opcode_string(op)) +strlen(what_opcode_string(op)); // point after the opcode, we cheked before it 
    str+= strspn(str," \t");
    if(strstr(line,"("))
        sscanf(str,"%[^(]",label_operand); //////////////////////////////////////////////////////////////////////
    else
        sscanf(str,"%s",label_operand);
    str = strstr(str,label_operand)+strlen(label_operand); // point after the first operand
    /**   Chek it all every thing llegal before paramters  **/
    if(!strlen(label_operand)) // no operand
        return DIRECTIVE_NO_PARAMS;
    if(what_type_is_operand(label_operand)!= Label)
        return EXPECTED_LABEL_AFTER_OPCODE;
    if(!(la=find_labe(label_operand)) )
        return LABEL_OPERANDS_DOSENT_EXISTS;
    if(strstr(str,"(")) // expect paramters  
    {
        ERROR_ENUM er;
        operand_type par1_type,par2_type;
        int num1,num2;
        if(str[0]!='(')
            return JMP_EXPECTED_PARENTHESIS;
        if(!strstr(str,",") )
         return COMMAND_INVALID_NUMBER_OF_PARAMTERS;
        sscanf(str+1,"%[^,]",par1); // scan first paramter
        //// check first paramter
        if( strstr(par1," ") || strstr(par1,"\t")) // have white-space
            return CANT_BE_WHITE_SPACE_IN_PARAMTERS;
        str= strstr(str,",")+1; // point to the start of the second parampter
        if(!strstr(str,")"))
            return JMP_EXPECTED_PARENTHESIS;
        sscanf(str,"%[^)]",par2); // scan second paramter
        //// check second paramter 
        if( strstr(par2," ") || strstr(par2,"\t")) // have white-space
            return CANT_BE_WHITE_SPACE_IN_PARAMTERS;
        if(strspn(strstr(str,")")," \t\n")) // if there is none white-space string after the ')' of the second paramter
            return JMP_EXPECTED_PARENTHESIS;
        par1_type =what_type_is_operand(par1);
        par2_type =what_type_is_operand(par2);
        if((er=check_if_the_operand_llegal(par1,&num1,par1_type))!= NO_ERROR) 
            return er; 
        if((er=check_if_the_operand_llegal(par2,&num2,par2_type))!= NO_ERROR) 
            return er;
        ////// all the parater and label are legal -> insert
        insert_command_to_word(&(ram->word_ram[ram->number_of_word_enterd]),0,Jumping,op);
        insert_paramter_addressing_to_word(&(ram->word_ram[ram->number_of_word_enterd]),what_addressing_methods_operans(par1_type),what_addressing_methods_operans(par2_type));
        insert_label_to_word(&(ram->word_ram[ram->number_of_word_enterd+1]),la,ram->number_of_word_enterd+1);
        if(par1_type==Register && par2_type==Register) // insert in the same RAM word
        {
            insert_Register_to_word(&(ram->word_ram[ram->number_of_word_enterd+2]), what_Register_operans(par1),what_Register_operans(par2)) ;
            ram->number_of_word_enterd+=3;
            return NO_ERROR;
        }
        switch(par1_type) // inster firts paramter
        {
            case Number:
                insert_number_to_word(&(ram->word_ram[ram->number_of_word_enterd+2]),num1);
                break;
            case Label:
                insert_label_to_word(&(ram->word_ram[ram->number_of_word_enterd+2]),find_labe(par1),ram->number_of_word_enterd+2);
                break;;
            default: // if not the above ->Register
                insert_Register_to_word(&(ram->word_ram[ram->number_of_word_enterd+2]),what_Register_operans(par1),r0); // r0 = 0
        }
        switch(par2_type) // inster firts paramter
        {
            case Number:
                insert_number_to_word(&(ram->word_ram[ram->number_of_word_enterd+3]),num2);
                break;
            case Label:
                insert_label_to_word(&(ram->word_ram[ram->number_of_word_enterd+3]),find_labe(par2),ram->number_of_word_enterd+3);
                break;;
            default: // if not the above ->Register
                insert_Register_to_word(&(ram->word_ram[ram->number_of_word_enterd+3]),0,what_Register_operans(par2)); // r0 = 0
        }
        ram->number_of_word_enterd += 4;
        return NO_ERROR;
        
    }
    {
        if(strcspn(str," \t\n")) // extra string (not white-space ) after the lable
            return EXTRA_STRING_AFTER_OPERAND;
        insert_command_to_word(&(ram->word_ram[ram->number_of_word_enterd]),0,Direct,op);
        insert_label_to_word(&(ram->word_ram[ram->number_of_word_enterd+1]),la,ram->number_of_word_enterd+1);
        ram->number_of_word_enterd +=2;
        return NO_ERROR;
    }
}

ERROR_ENUM secound_pass_stop_rts(char line[LINE_LENGTH],opcode op,RAM *ram)
{
    char *str;
    if(op == STOP)
    {
        str= strstr(line,"stop")+strlen("stop"); // we checked before(stop) in the first pass
        if(strcspn(str," \t\n")) // there is string after that have no white-space
            return EXTRA_STRING_AFTER_COMMAND;
        insert_command_to_word(&(ram->word_ram[ram->number_of_word_enterd]),0,0,STOP);
        ram->number_of_word_enterd++;
        return NO_ERROR;
    } 
    /* this mean that the op= RTS*/
    str= strstr(line,"rts")+strlen("rts"); // we checked before(stop) in the first pass
    if(strcspn(str," \t\n")) // there is string after that have no white-space
        return EXTRA_STRING_AFTER_COMMAND;
    insert_command_to_word(&(ram->word_ram[ram->number_of_word_enterd]),0,0,RTS);
    ram->number_of_word_enterd++;
    return NO_ERROR;
}

operand_type what_type_is_operand(char *operan) // retyrn the type or operand , if not numric or rigester -> label
{
    if(operan[0]=='#')
        return Number;
    if(if_register(operan))
        return Register;
    return Label;
}

void insert_command_to_word(Word *word,addressing_methods oper1,addressing_methods oper2,opcode op)// insert the word in the RAM to the appropriate opcode and operands
{
    unsigned char mask;
    word->word_arr[0] = word->word_arr[1] = 0; 
    mask=op;
    word->word_arr[0] = op >> 2; // put the last 2 bit of op in bit 0,1 (start from 0)
    word->word_arr[1] |=  mask << 6 ; // put the first 2 bit of op in bit 6,7 (start from 0)
    word->word_arr[1] |= oper1 << 4; // the addressing_methods
    word->word_arr[1] |= oper2 << 2; // addressing_methods
}

void insert_paramter_addressing_to_word(Word *word,addressing_methods oper1,addressing_methods oper2)
{
    word->word_arr[0] |= oper1 << 4; 
    word->word_arr[0] |= oper2 << 2; 
}


addressing_methods what_addressing_methods_operans(operand_type oper1)// retuen the addressing_method it won't be for jumping opcode
{
    if(oper1 == Number)
        return Instant;
    if(oper1 == Label)
        return Direct;
    return Direct_Register;
}

void insert_label_to_word(Word *word,lable *la,short the_label_in) // insert the word in the RAM to the appropriate label line
{
    unsigned short mask=1;
    int i;
    word->word_arr[0]=word->word_arr[1]=0;
    if(la->ext==yes) // if the label is extern
    {
        word->word_arr[1]=1; // 00000000000001
        if(!(arr_ext->arr_extern=(Extern_label*)realloc(arr_ext->arr_extern,(sizeof(Extern_label)*(arr_ext->size+1)))))
        {
            perror("Not enough memory.\n");
            exit(1);
        }
        arr_ext->arr_extern[arr_ext->size].la = la;
        arr_ext->arr_extern[arr_ext->size].line = the_label_in+ START_INSTRCTION;
        arr_ext->size++;
        return;
    } 
    word->word_arr[1]=2; // 00000000000010  , start
    for ( i = 0; i < CHAR_SIZE_BIT-2; i++)// we start from thierd bit from the right
    {
        word->word_arr[1] |=  (mask&la->line) <<2; // insert the bit to the appropriat word_arr, if word_arr[1]we move number by CHAR_SIZE_BIT
        mask <<= 1;
    }
    for ( i = 0; i < CHAR_SIZE_BIT-2; i++)// we start from thierd bit from the right
    {
        word->word_arr[0] |= (mask&la->line) >>(CHAR_SIZE_BIT-2); // insert the bit to the appropriat word_arr, if word_arr[1]we move number by CHAR_SIZE_BIT
        mask <<= 1;
    }
}

void insert_number_to_word(Word *word,int num) // insert the word in the RAM to the appropriate numer in 2 complet 
{
    unsigned short mask=1;
    int i;
    bool negative = num<0?yes:no;
    if(negative==yes)
     num *= -1;
    word->word_arr[0]=word->word_arr[1]=0;
    for ( i = 0; i < CHAR_SIZE_BIT-2; i++)// we start from thierd bit from the right
    {
        word->word_arr[1] |=  (mask&num) <<2; // insert the bit to the appropriat word_arr, if word_arr[1]we move number by CHAR_SIZE_BIT
        mask <<= 1;
    }
    for ( i = 0; i < CHAR_SIZE_BIT-2; i++)// we start from thierd bit from the right
    {
        word->word_arr[0] |= (mask&num) >>CHAR_SIZE_BIT; // insert the bit to the appropriat word_arr, if word_arr[1]we move number by CHAR_SIZE_BIT
        mask <<= 1;
    }
    if(negative == yes)
    complement_2_word(word);
}

void complement_2_word(Word *word) // insert the 2 complement to the the number( bit[2,14],bit[0,1]=0)
{
    unsigned char mask=~0;
    word->word_arr[0] = ~word->word_arr[0];
    word->word_arr[1] = ~word->word_arr[1];
    if((word->word_arr[1]|mask )== word->word_arr[1])// if tall the bit is 1 -> we add 1 we will add 1 to word->word_arr[0]
    {
        word->word_arr[0]++;
        word->word_arr[1]=0;
    }
    else
    {
        mask = 4; // 00000100
        word->word_arr[1] += mask; 
        mask =3; // 00000011
        word->word_arr[1] &= ~mask ;// reset bit 0,1
    }
}

void insert_Register_to_word(Word *word,Register_value first,Register_value second) //insert the word in the RAM to the appropriate Register
{
    word->word_arr[0]=word->word_arr[1]=0;
    word->word_arr[0] = first;
    word->word_arr[1] = (second<<2);
    
    
}

Register_value what_Register_operans(char *oper) // get legal Register_value in string return the appropriate Register_value
{
    if(strstr(oper,"r0"))
        return r0;
    if(strstr(oper,"r1"))
        return r1;
    if(strstr(oper,"r2"))
        return r2;
    if(strstr(oper,"r3"))
        return r3;
    if(strstr(oper,"r4"))
        return r4;
    if(strstr(oper,"r5"))
        return r5;
    if(strstr(oper,"r6"))
        return r6;
    return r7; // because its  legal it will be r7 if not one before
    
}

ERROR_ENUM check_if_the_operand_llegal(char *operan,int *num,operand_type oper_type)// retrun if there is error, if not retrun no_erroe, if the operand_type number -> inser it to the int pointer
{
   if(oper_type==Number)
    {
        if(!my_atoi(operan+1,num))
        {
            if(strstr(operan,"."))
                return OPERANDS_UNEXPECTED_DECIMAL_POINT;
            return OPERANDS_INVALID_NUM;
        }
        if(!is_in_range(*num))
           return OPERANDS_NUMBER_OUT_RANGE;
    } 
    else if(oper_type == Label)
    {
        if(find_labe(operan)==NULL)
            return LABEL_OPERANDS_DOSENT_EXISTS;
    }
    return NO_ERROR;
}

void print_word(Word w)
{
    unsigned char mask = 1;
    int i;
    for ( mask=mask<<5,i = 0; i < 6; i++,mask>>=1)
    {
        if(mask&w.word_arr[0])
            fprintf(object_file,ONE);
        else
            fprintf(object_file,ZERO);
    }
    mask = 1;
    for ( mask=mask<<7,i = 0; i < 8; i++,mask>>=1)
    {
        if(mask&w.word_arr[1])
            fprintf(object_file,ONE);
        else
            fprintf(object_file,ZERO);
    }
    fprintf(object_file,"\n");
}

void print_Data_to_RAM(data da)
{
    Word word;
    int len,i;
    len = da.size;
    if(da.type == d) // .data
    {
        for ( i = 0; i < len; i++)
        {
            insert_number_to_word_data(&word,((int*)da.arr)[i]);
            print_word(word);
        }
    }
    else // .string
    {
        for ( i = 0; i < len; i++)
        {
            insert_number_to_word_data(&word,((char*)da.arr)[i]);
            print_word(word);
        }
    }
    
}

void insert_number_to_word_data(Word *word,int num) 
{
    unsigned short mask=1;
    int i;
    bool negative = num<0?yes:no;
    if(negative==yes)
        num *= -1;
    word->word_arr[0]=word->word_arr[1]=0;
    for ( i = 0; i < CHAR_SIZE_BIT; i++)// we start from thierd bit from the right
    {
        word->word_arr[1] |=  (mask&num); // insert the bit to the appropriat word_arr, if word_arr[1]we move number by CHAR_SIZE_BIT
        mask <<= 1;
    }
    for ( i = 0; i < CHAR_SIZE_BIT-2; i++)// we start from thierd bit from the right
    {
        word->word_arr[0] |= (mask&num) >>CHAR_SIZE_BIT; // insert the bit to the appropriat word_arr, if word_arr[1]we move number by CHAR_SIZE_BIT
        mask <<= 1;
    }
    mask = ~0;
    if(negative == yes)
    {
        word->word_arr[0] = ~word->word_arr[0];
        word->word_arr[1] = ~word->word_arr[1];
        if((word->word_arr[1]|mask )== word->word_arr[1])// if tall the bit is 1 -> we add 1 we will add 1 to word->word_arr[0]
        {
            word->word_arr[0]++;
            word->word_arr[1]=0;
        }
        else
        {
            word->word_arr[1]++;
        }
    }
}

void free_error()
{
    free(error_arr->arr_err);
    free(error_arr);
}

void free_RAM(RAM *ram)
{
    free(ram->word_ram);
    free(ram);
}

void free_extern()
{
    free(arr_ext->arr_extern);
    free(arr_ext);
}

void free_entery()
{
    free(entry_label);
}

void free_data()
{
    int i;
    for ( i = 0; i < Data_Table_->size; i++)
    {
        free(Data_Table_->data_table[i]);
    }
    free(Data_Table_->data_table);
    free(Data_Table_);
}


ERROR_ENUM secound_pass_lea(char line[LINE_LENGTH],opcode op,RAM *ram)
{
    char operand1[LABLE_SIZE],operand2[LABLE_SIZE]; // might be register or label ,operand2 might be number
    char *str;
    int num1,num2;
    operand_type oper1,oper2;
    ERROR_ENUM er;
    str = strstr(line,what_opcode_string(op)) +strlen(what_opcode_string(op)); // point after the opcode
    if( !strstr(str,",")) // no comma after the opcode
        return EXPECTED_COMMA_BETWEEN_OPERANDS;
    sscanf(str,"%[^,]",operand1);
    sscanf(str+strspn(operand1," \t"),"%[^,]",operand1); // delete white-space
    str = strstr(str,operand1)+strlen(operand1); // point after the first operand
    if(strcspn(str," \t,")) /// the operand have more than 30 char-> cant be label
        return COMMAND_INVALID_OPERANDS_METHODS;
    str = strstr(str,",")+1;  
    sscanf(str,"%s",operand2);
    sscanf(str+strspn(operand2," \t"),"%s",operand2); // delete white-space 
    str = strstr(str,operand2)+strlen(operand2); // point after the first operand
    if(strcspn(str," \t\n")) /// the operand have more than 30 char-> cant be labe
        return COMMAND_INVALID_OPERANDS_METHODS; 
    if(strspn(str," \t\n")!=strlen(str)) //after the second another operand
        return COMMAND_INVALID_NUMBER_OF_OPERANDS;
    oper1 = what_type_is_operand(operand1);
    oper2 = what_type_is_operand(operand2);
    if((er= check_if_the_operand_llegal(operand1,&num1,oper1))!=NO_ERROR)
        return er;
    if((er= check_if_the_operand_llegal(operand2,&num2,oper2))!=NO_ERROR)
        return er;
    if(oper1 != Label || oper2== Number )
        return COMMAND_INVALID_OPERANDS_METHODS;
       /// now the line is leegal -> insert to the RAM
    insert_command_to_word(&(ram->word_ram[ram->number_of_word_enterd]),what_addressing_methods_operans(oper1),what_addressing_methods_operans(oper2),op);
    
    insert_label_to_word(&(ram->word_ram[ram->number_of_word_enterd+1]),find_labe(operand2),ram->number_of_word_enterd+1);
    if(oper2 == Label)
        insert_label_to_word(&(ram->word_ram[ram->number_of_word_enterd+2]),find_labe(operand2),ram->number_of_word_enterd+2);
    else
        insert_Register_to_word(&(ram->word_ram[ram->number_of_word_enterd+2]),r0,what_Register_operans(operand2)); // r0 = 0
    ram->number_of_word_enterd+=3;
    return NO_ERROR;
}
