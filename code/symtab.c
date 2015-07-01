#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include"symtab.h"
#include"debug.h"
#include"uthash.h"

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

/*
int main(){
    add_sym("zyyyyy",1);
    add_sym("zyyyyyxx",2);
    symbol* s = find_sym("zyyyyy");
    if(s==NULL){
        printf("cannot find zyyyyy\n");
    }
    else{
        printf("dim is %d\n",s->dim);
    }
    s = find_sym("zyyyyyxx");
    if(s==NULL){
        printf("cannot find zyyyyyxx\n");
    }
    else{
        printf("dim is %d\n",s->dim);
    }
    del_sym("zyyyyy");
    s = find_sym("zyyyyy");
    if(s==NULL){
        printf("cannot find zyyyyy\n");
    }
    else{
        printf("dim is %d\n",s->dim);
    }
    return 0;
}
*/
