#include<string.h>
#include"intercode.h"
#include"tempvar_ht.h"
#include"list.h"

intercode* pp;
#define pp list_entry(p,intercode,list)

void find_goto_next(){
    ListHead* p = NULL;
    list_foreach(p,&code_head){
        if(pp->kind == ICN_GOTO && list_entry(p->next,intercode,list)->kind == ICN_LABEL){
            if(strcmp(pp->label,list_entry(p->next,intercode,list)->label)==0){
                label_node* nd = find_label(pp->label);
                nd->refer_times--;
                list_del(p);
            }
        }
    }
}

void del_useless_label(){
    ListHead* p = NULL;
    list_foreach(p,&code_head){
        if(pp->kind ==ICN_LABEL){
            label_node* nd = find_label(pp->label);
            if(nd && nd->refer_times <= 0){
                list_del(p);
            }
        }
    }
}






#undef pp
