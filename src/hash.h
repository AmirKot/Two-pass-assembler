#ifndef _HEADER_HASH
#define _HEADER_HASH
#include "libary.h"
#define LABLE_SIZE 31 // with '\0'
#define DATA ".data"
#define STRING ".string"
#define EXTERN ".extern"
#define ENTRY ".entry" 
////
#define HASH_SIZE 20
typedef enum{s,d} data__type; //  d=.data , ext=.extern
typedef struct data 
{
   void* arr;
   data__type type; 
   short int size;
}data;
typedef struct lable
{
    char *name; // the name of the label
    bool ext; // if the label is extern
    bool is_d; // if we defined data(.data or .string) in the line that the label was delcared 
    short line;
    struct lable *next;
}lable;

lable *label_map[HASH_SIZE];  // lable- hash-map: the key depend on the field name in  the label

unsigned short hash_key(char *name);
void reset_has(void);
lable * find_labe(char*name);
void insert_to_hash(lable *la);




#endif
