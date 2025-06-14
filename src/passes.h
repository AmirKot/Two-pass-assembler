#ifndef _HEADER_PASS
#define _HEADER_PASS
#include "libary.h"
#include "hash.h"
#define START_INSTRCTION 100
typedef enum 
{
	MOV,CMP,ADD,SUB,NOT,CLR,LEA,INC,DEC,JMP,BNE,RED,PRN,JSR,RTS,STOP,ERROR_opcode=-1
}opcode; 

typedef struct data_table_
{
   data **data_table;
   long size;
}data_table_;

lable **entry_label;
extern short int I_line;
extern short int D_line;
extern short int cuur_line;
extern data_table_ *Data_Table_;

typedef struct ERROR_LINE
{
   short line;
   ERROR_ENUM en;
}ERROR_LINE;
 
typedef struct ERROR_FIRST_PASS
{
   ERROR_LINE *arr_err;
   short size;
}ERROR_FIRST_PASS;

ERROR_FIRST_PASS *error_arr;
void free_error();
int is_comment_or_empty(char line[LINE_LENGTH]);
int check_label_name(char *label_name,char char_after_label);
void first_pass_label_string(char line[LINE_LENGTH]);
void first_pass_label_data(char line[LINE_LENGTH]);
int my_atoi(char *str,int*num);
data * getData_string(char *data_string);
data * getData_int(char *data_string);
void first_pass_label_extern(char line[LINE_LENGTH]);
opcode what_opcode_in_line(char line[LINE_LENGTH]);
char * what_opcode_string(opcode op);
char * if_label_before_opcode(char line[LINE_LENGTH]);
int if_no_extra_string_brfore_opcode(char line[LINE_LENGTH],opcode op);
int if_first_operand_register(char line[LINE_LENGTH],char *op_string) ;
int if_second_operand_register(char line[LINE_LENGTH],char *op_string);
int if_register(char *reg);
int line_to_add_instruction(char line[LINE_LENGTH],opcode op);
int is_in_range(int num);
void print_error(int line,ERROR_ENUM err);
void free_has_map();
/////
void insert_entry_label(lable * la);


#endif
