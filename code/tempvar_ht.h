#ifndef __TEMPVAR_HT_H__
#define __TEMPVAR_HT_H__

#include"uthash.h"
#include"list.h"
#include"intercode.h"

struct code_block_node{
    code_block* cb;
    struct code_block_node* next;
};
typedef struct code_block_node code_block_node;

struct tmpvar_ht_node{
    char* varstr;
    code_block_node* modified_block;
    code_block_node* used_block;
    UT_hash_handle hh;
    int dag_node_no;
    int update_no;
};
typedef struct tmpvar_ht_node tmpvar_ht_node;

extern tmpvar_ht_node* tmpvar_ht;

void add_tmpvar(char* varstr);
tmpvar_ht_node* find_tmpvar(char* varstr);



#endif
