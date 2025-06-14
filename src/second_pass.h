#ifndef _HEADER_SECOND_PASS
#define _HEADER_SECOND_PASS
#include "passes.h"
#define ONE "/"
#define ZERO "."
typedef enum {Number,Register,Label}operand_type;
typedef enum{Instant,Direct,Jumping,Direct_Register}addressing_methods;
typedef enum{r0,r1,r2,r3,r4,r5,r6,r7}Register_value;
typedef struct Extern_label
{
   short line;
   lable  *la;
}Extern_label;

typedef struct Word
{
   char word_arr[2];// word_arr[1]the righ 8 bit , word_arr[0] the  left 6 bit
}Word;

typedef struct RAM
{
   Word * word_ram;
   short int number_of_word_enterd;
}RAM;

typedef struct Extern_array
{
   Extern_label *arr_extern;
   short int size;
}Extern_array;

lable **entry_label;
Extern_array *arr_ext;

ERROR_ENUM secound_pass_entry(char line[LINE_LENGTH]);
operand_type what_type_is_operand(char *operan);
void insert_entry_label(lable * la);
addressing_methods what_addressing_methods_operans(operand_type oper1);
void insert_command_to_word(Word *word,addressing_methods oper1,addressing_methods oper2,opcode op);
void insert_label_to_word(Word *word,lable *la,short the_label_in);
void complement_2_word(Word *word);
void insert_number_to_word(Word *word,int num) ;
void insert_Register_to_word(Word *word,Register_value first,Register_value second) ;
Register_value what_Register_operans(char *oper);
ERROR_ENUM secound_pass_mov_add_sub(char line[LINE_LENGTH],opcode op,RAM *ram);
ERROR_ENUM secound_pass_not_clr_inc_dec_red(char line[LINE_LENGTH],opcode op,RAM *ram);
ERROR_ENUM check_if_the_operand_llegal(char *operan,int *num,operand_type oper_type);
ERROR_ENUM secound_pass_jmp_bne_jsr(char line[LINE_LENGTH],opcode op,RAM *ram);
ERROR_ENUM secound_pass_stop_rts(char line[LINE_LENGTH],opcode op,RAM *ram);
void print_word(Word w);
void insert_paramter_addressing_to_word(Word *word,addressing_methods oper1,addressing_methods oper2);
void secound_pass(FILE *am,char *file_name);
void print_Data_to_RAM(data da);
void insert_number_to_word_data(Word *word,int num);
void free_RAM(RAM *ram);
void free_entery();
void free_extern();
ERROR_ENUM secound_pass_lea(char line[LINE_LENGTH],opcode op,RAM *ram);
void free_data();

#endif
