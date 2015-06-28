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
        int val_int;
        float val_float;
    };
    void* addr;
    UT_hash_handle hh;
};


#endif
