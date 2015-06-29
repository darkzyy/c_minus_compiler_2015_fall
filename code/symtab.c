#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"symtab.h"

symbol* func_tab;
symbol* struct_tab;
symbol* var_tab;
symbol* field_tab;

const int size = 0x3fff;

unsigned int hash_pjw(char* name){
    unsigned int val = 0,high;
    for(;*name;name++){
        val = (val<<2)+*name;
        high = val&(~size);
        if(high != 0){
            val = (val^(high>>12))&size;
        }
    }
    return val;
}

void hash_init(){
    func_tab = malloc(size*sizeof(symbol));
    struct_tab = malloc(size*sizeof(symbol));
    var_tab = malloc(size*sizeof(symbol));
    field_tab = malloc(size*sizeof(symbol));
    int i;
    for(i=0;i<size;i++){
        func_tab[i].next = NULL;
        struct_tab[i].next = NULL;
        var_tab[i].next = NULL;
        field_tab[i].next = NULL;
        func_tab[i].valid = 0;
        struct_tab[i].valid = 0;
        var_tab[i].valid = 0;
        field_tab[i].valid = 0;
    }
}

symbol* find_sym(symbol** ht,char* id_name){
    unsigned int idx = hash_pjw(id_name);
    symbol* sp = *ht;
    if(sp[idx].valid == 1 && strcmp(sp[idx].id_name,id_name)==0){
        return &(sp[idx]);
    }
    while(sp->next!=NULL){
        sp = sp->next;
        if(sp[idx].valid == 1 && strcmp(sp[idx].id_name,id_name)==0){
            return &(sp[idx]);
        }
    }
    return NULL;
}

void add_sym_node(symbol** ht,symbol* new_node){
    if(find_sym(ht,new_node->id_name)!=NULL){
        return ;
    }
    else{
        unsigned int idx = hash_pjw(new_node->id_name);
        symbol* sp = *ht+idx;
        if(sp->valid == 0){
            memcpy(sp,new_node,sizeof(symbol));
            sp->valid = 1;
        }
        while(sp->next!=NULL){
            sp = sp->next;
            if(sp->valid == 0){
                memcpy(sp,new_node,sizeof(symbol));
                sp->valid = 1;
            }
        }
        sp->next = new_node;
        new_node->valid = 1;
        return;
    }
}

void check_func(){
    int i;
    for(i=0;i<size;i++){
        symbol* s = &func_tab[i];
        if(s->def_ed==0 && s->dec_ed==1){
            printf("Error type 18 at Line %d: Undefined function \"%s\".\n",s->line,s->id_name);
        }
    }
}
