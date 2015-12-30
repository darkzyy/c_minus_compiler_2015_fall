#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "uthash.h"
#include "var_desc.h"
#include "asmprint.h"
#include "debug.h"

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
    //Log4("str %s",str);
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

void print_var(){
    var_desc* vd;
    for(vd = vd_head;vd!=NULL;vd = vd->hh.next){
        printf("%s :",vd->var_str);
        int i = 0;
        for(i=3;i<=25;i++){
            if(vd->in_reg[i]){
                printf(" %s",reg_str[i]);
            }
        }
        if(vd->is_in_mem){
            printf(" mem");
        }
        printf("\n");
    }
}

void del_var(char* var_str){
    var_desc* vd;
    HASH_FIND_STR(vd_head,var_str,vd);
    if(vd){
        HASH_DEL(vd_head,vd);
        free(vd);
    }
}
