//This file is used for semantic analyzation
#include<string.h>
#include"multi_tree.h"
#include"symtab.h"
#include"cmmtypes.h"
#include"syntax.tab.h"
#include"semantic.h"
#include"debug.h"

//#define __ARRAY_WIDTH_CMP__

void init_basic_type(){
    type_int = malloc(sizeof(Type));
    type_int->kind = basic;
    type_int->basic = INT; 
    type_float = malloc(sizeof(Type));
    type_float->kind = basic;
    type_float->basic = FLOAT; 
    type_error = malloc(sizeof(Type));
    type_error->kind = semantic_error;
}

int type_cmp(Type* tx,Type* ty){
    if(tx==NULL||ty==NULL){
        if(tx==NULL&&ty==NULL){
            return 0;
        }
        else{
            return 1;
        }
    }
    if(tx->kind != ty->kind){
        Log("not one kind");
        return 1;
    }
    if(tx->kind == basic || tx->kind == structure){
        Log("one kind");
        return tx != ty;
    }
    else if(tx->kind == semantic_error){
        Log("error kind");
        return 1;
    }
    else{// array
#ifdef __ARRAY_WIDTH_CMP__
        if(tx->array.size != ty->array.size){
            return 1;
        }
        else{
            return type_cmp(tx->array.elem,ty->array.elem);
        }
#else
        return type_cmp(tx->array.elem,ty->array.elem);
#endif
    }
}
