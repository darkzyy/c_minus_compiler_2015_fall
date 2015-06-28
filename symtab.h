#ifndef __SYMTAB_H__
#define __SYMTAB_H__

#include"uthash.h"
#include"cmmtypes.h"

typedef struct symbol_ symbol;

struct symbol_{
    char* id_name;
    int id_type;//func struct variable ?
    Type* val_type;//int float user_struct
    int dim;
    int argamt;//arguement amount
    union{
        struct{
            unsigned int is_struct : 1;
            unsigned int is_func : 1;
            unsigned int is_var : 1;
            unsigned int is_domain : 1;
            unsigned int def_ed : 1;
            unsigned int dec_ed : 1;
            unsigned int init_ed : 1;
        };
        unsigned int property;
    };
    union{
        int val_int;
        float val_float;
    };
    void* addr;
    UT_hash_handle hh;
};

void add_sym_name(char* id_name);
void add_sym_node(symbol* new_node);
symbol* find_sym(char* id_name);
symbol* del_sym(char* id_name);
void print_symtab();


#endif
