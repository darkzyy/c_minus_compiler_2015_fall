#include <stdlib.h>
#include <string.h>
#include "uthash.h"
#include "var_desc.h"

var_desc* vd_head = NULL;

void add_var(char* str){
    var_desc* vd;
    HASH_FIND_STR(vd_head,str,vd);
    if(vd==NULL){
        vd = (var_desc*) malloc(sizeof(var_desc));
        vd->var_str = str;
        memset(vd->in_reg,0,32*sizeof(int));
        vd->offset = -1;
        HASH_ADD_KEYPTR(hh,vd_head,vd->var_str,strlen(vd->var_str),vd);
    }
}

var_desc* find_var(char* str){
    var_desc* vd;
    HASH_FIND_STR(vd_head,str,vd);
    return vd;
}

void inc_reg_contain_var(char* str,int regno){
    var_desc* vd;
    HASH_FIND_STR(vd_head,str,vd);
    if(vd){
        vd->in_reg[regno] = 1;
    }
}

void dec_reg_contain_var(char* str,int regno){
    var_desc* vd;
    HASH_FIND_STR(vd_head,str,vd);
    if(vd){
        vd->in_reg[regno] = 0;
    }
}
