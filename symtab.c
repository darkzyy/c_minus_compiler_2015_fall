#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include"symtab.h"

symbol* ht = NULL;

void add_sym_name( char* id_name,int dim){
    symbol* s;
    HASH_FIND_STR( ht,id_name,s);
    if(s==NULL){
        s=(symbol*)malloc(sizeof(symbol));
        s->id_name = id_name;
        s->dim = dim;
        HASH_ADD_KEYPTR(hh,ht,s->id_name,strlen(s->id_name),s);
    }
}

void add_sym_node(symbol* new_node){
    symbol* s;
    HASH_FIND_STR( ht,new_node->id_name,s);
    if(s==NULL){
        s=(symbol*)malloc(sizeof(symbol));
        s->id_name = id_name;
        s->dim = dim;
        HASH_ADD_KEYPTR(hh,ht,new_node->id_name,strlen(new_node->id_name),new_node);
    }
}

symbol* find_sym(char* id_name){
    symbol* s;
    HASH_FIND_STR(ht,id_name,s);
    return s;
}
symbol* del_sym(char* id_name){
    symbol* s = find_sym(id_name);
    if(s!=NULL){
        HASH_DEL(ht,s);
    }
    return s;
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
