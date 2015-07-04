#include<string.h>
#include"intercode.h"
#include"tempvar_ht.h"
#include"list.h"
#include"debug.h"

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

static void cond_false(intercode* ic){
    assert(ic->kind == ICN_IF);
    if(strcmp(ic->relop,"==")==0){
        ic->relop = "!=";
    }
    else if(strcmp(ic->relop,"!=")==0){
        ic->relop = "==";
    }
    else if(strcmp(ic->relop,">=")==0){
        ic->relop = "<";
    }
    else if(strcmp(ic->relop,"<=")==0){
        ic->relop = ">";
    }
    else if(strcmp(ic->relop,">")==0){
        ic->relop = "<=";
    }
    else if(strcmp(ic->relop,"<")==0){
        ic->relop = ">=";
    }
    else{
        assert(0);
    }
}

void opti_if(){
    ListHead* p = NULL;
    list_foreach(p,&code_head){
        if(pp->kind == ICN_GOTO && list_entry(p->prev,intercode,list)->kind == ICN_IF){
            if(p->next && (list_entry(p->next,intercode,list)->kind == ICN_LABEL)){
                intercode* ic_if = list_entry(p->prev,intercode,list);
                intercode* ic_goto = pp;
                intercode* ic_label = list_entry(p->next,intercode,list);
                if(strcmp(ic_if->label,ic_label->label)==0){
                    label_node* nd = find_label(ic_if->label);
                    nd->refer_times--;
                    cond_false(ic_if);
                    ic_if->label = ic_goto->label;
                    list_del(p);
                    Log3();
                }
            }
        }
    }
}

static void redirect_label(char* old_label,char* new_label){
    ListHead* p = NULL;
    label_node* nd = find_label(new_label);
    list_foreach(p,&code_head){
        if(pp->kind == ICN_GOTO || pp->kind == ICN_IF){
            if(strcmp(pp->label,old_label)==0){
                pp->label = new_label;
                nd->refer_times++;
            }
        }
    }
}

void find_cont_label(){
    ListHead* p = NULL;
    list_foreach(p,&code_head){
        intercode* pn = list_entry(p->next,intercode,list);
        if(pp->kind == ICN_LABEL && (pn->kind == ICN_LABEL || pn->kind == ICN_GOTO)){
            list_del(p);
            redirect_label(pp->label,pn->label);
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

void peep_opti(){
    find_goto_next();
    opti_if();
    del_useless_label();
    find_cont_label();
    find_goto_next();
    opti_if();
    del_useless_label();
    find_cont_label();
}

#undef pp
