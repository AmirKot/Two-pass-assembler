#include "hash.h"

///////************ HASH FUNCTIONS *****////////
unsigned short hash_key(char *name) 
{
    unsigned short key=0;
    int i;
    for ( i = 0; i < strlen(name); i++)
    {
       key+= name[i]*(i+1); // the ascii number * (index+1)
       key%=HASH_SIZE;
    }
    return key%HASH_SIZE; // key in range[0,HASH_SIZE-1]
}

void reset_has() // rest all hash map to null
{
    int i=0;
    for ( i = 0; i < HASH_SIZE; i++)
    {
        label_map[i]=NULL;
    }
}

lable * find_labe(char*name) //return a pointer to the label field name equal to name(parameter)
{
    lable *tmp=NULL;
    unsigned short key = hash_key(name);
    tmp = label_map[key];
    while (tmp != NULL) 
    {
       if(!strcmp(name,tmp->name)) // we found the label
          return tmp;
        tmp = tmp->next;
    }
    return tmp;
}

void insert_to_hash(lable *la) // we assume the parameter la is not NULL
{
    unsigned short key= hash_key(la->name);
    la->next=label_map[key];
    label_map[key]=la; // we add it to the start of the ket , if there is label there we link it to the end of this 
}
