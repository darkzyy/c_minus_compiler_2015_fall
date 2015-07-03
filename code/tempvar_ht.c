#include"tempvar_ht.h"

tmpvar_ht_node* tmpvar_ht = NULL;

void add_tmpvar(char* varstr){
    tmpvar_ht_node* nd;
    HASH_FIND_STR(tmpvar_ht,varstr,nd);
    if(nd==NULL){
        nd = (tmpvar_ht_node*)malloc(sizeof(tmpvar_ht_node));
        nd->varstr = varstr;
        nd->dag_node_no = 0;
        HASH_ADD_KEYPTR(hh,tmpvar_ht,nd->varstr,strlen(nd->varstr),nd);
    }
}

tmpvar_ht_node* find_tmpvar(char* varstr){
    tmpvar_ht_node* nd;
    HASH_FIND_STR(tmpvar_ht,varstr,nd);
    return nd;
}

label_node* label_ht = NULL;

label_node* find_label(char* str){
    label_node* nd;
    HASH_FIND_STR(label_ht,str,nd);
    return nd;
}

void add_label(char* str){
    label_node* nd;
    HASH_FIND_STR(label_ht,str,nd);
    if(nd==NULL){
        nd = (label_node*)malloc(sizeof(label_node));
        nd->label = str;
        nd->refer_times = 0;
        HASH_ADD_KEYPTR(hh,label_ht,nd->label,strlen(nd->label),nd);
    }
}

void add_label_refer(char* str){
    label_node* nd;
    HASH_FIND_STR(label_ht,str,nd);
    if(nd==NULL){
        nd = (label_node*)malloc(sizeof(label_node));
        nd->label = str;
        nd->refer_times = 1;
        HASH_ADD_KEYPTR(hh,label_ht,nd->label,strlen(nd->label),nd);
    }
    else{
        nd->refer_times++;
    }
}
