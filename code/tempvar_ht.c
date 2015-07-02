#include"tempvar_ht.h"

tmpvar_ht_node* tmpvar_ht = NULL;

void add_tmpvar(char* varstr){
    tmpvar_ht_node* nd;
    HASH_FIND_STR(tmpvar_ht,varstr,nd);
    if(nd==NULL){
        nd = (tmpvar_ht_node*)malloc(sizeof(tmpvar_ht_node));
        nd->varstr = varstr;
        HASH_ADD_KEYPTR(hh,tmpvar_ht,nd->varstr,strlen(nd->varstr),nd);
    }
}

tmpvar_ht_node* find_tmpvar(char* varstr){
    tmpvar_ht_node* nd;
    HASH_FIND_STR(tmpvar_ht,varstr,nd);
    return nd;
}


