#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include"symtab.h"
#include"debug.h"
#include"uthash.h"
#include"semantic.h"

symbol* func_tab = NULL;
symbol* struct_tab = NULL;
symbol* var_tab = NULL;
symbol* field_tab = NULL;

void add_sym_name(symbol** ht, char* id_name){
    symbol* s;
    HASH_FIND_STR( *ht,id_name,s);
    if(s==NULL){
        s=(symbol*)malloc(sizeof(symbol));
        s->id_name = id_name;
        HASH_ADD_KEYPTR(hh,*ht,s->id_name,strlen(s->id_name),s);
    }
}

void add_sym_node(symbol** ht,symbol* new_node){
    symbol* s;
    HASH_FIND_STR( *ht,new_node->id_name,s);
    if(s==NULL){
        HASH_ADD_KEYPTR(hh,*ht,new_node->id_name,strlen(new_node->id_name),new_node);
    }
}

symbol* find_sym(symbol** ht,char* id_name){
    symbol* s;
    HASH_FIND_STR(*ht,id_name,s);
    return s;
}

symbol* del_sym(symbol** ht,char* id_name){
    symbol* s = find_sym(ht,id_name);
    if(s!=NULL){
        HASH_DEL(*ht,s);
    }
    return s;
}

void print_symtab(symbol* ht){
    symbol* s;
    for(s=ht;s!=NULL;s=s->hh.next){
        printf("ID: %s\n",s->id_name);
    }
}

void check_func(){
    symbol* s;
    for(s=func_tab;s!=NULL;s=s->hh.next){
        if(s->def_ed==0 && s->dec_ed==1){
            printf("Error type 18 at Line %d: Undefined function \"%s\".\n",s->line,s->id_name);
        }
    }
}

void init_rw(){
    symbol* r = malloc(sizeof(symbol));
    r->id_name = "read";
    r->argamt = 0;
    r->val_type = type_int;
    add_sym_node(&func_tab,r);
    ArgList* wal = malloc(sizeof(ArgList));
    wal->type = type_int;
    wal->next = NULL;
    symbol* w = malloc(sizeof(symbol));
    w->id_name = "write";
    w->argamt = 1;
    w->val_type = type_int;
    w->func_arg = wal;
    add_sym_node(&func_tab,w);
}

