#ifndef __VAR_DESC_H__
#define __VAR_DESC_H__

#include<stdio.h>
#include"uthash.h"

struct reg_desc;

struct var_desc{
    char* var_str;
    UT_hash_handle hh;
    int offset;
    int in_reg[32];
    int is_in_mem;
};
typedef struct var_desc var_desc;

extern var_desc* vd_head;

void add_var(char* str);
var_desc* find_var(char* str);
void inc_reg_contain_var(char* str,int regno);
void dec_reg_contain_var(char* str,int regno);
void print_var();


#endif
