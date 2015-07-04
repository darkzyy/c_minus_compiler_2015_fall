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
    UT_hash_handle hh;
    int dag_node_no;
    int update_no;
    int is_alive;
};
typedef struct tmpvar_ht_node tmpvar_ht_node;

extern tmpvar_ht_node* tmpvar_ht;

void add_tmpvar(char* varstr);
tmpvar_ht_node* find_tmpvar(char* varstr);
void tmpvar_node_no_init();
void tmpvar_kill_ret();

/********labels*************/
struct label_node{
    char* label;
    UT_hash_handle hh;
    int refer_times;
};

typedef struct label_node label_node;

extern label_node* label_ht;

void add_label_refer(char* str);
void add_label(char* str);
label_node* find_label(char* str);

#endif
