#include<string.h>
#include<assert.h>
#include"dag.h"

#define NODE_AMOUNT 256

static dagnode pool[NODE_AMOUNT];

int current_nodeno = 1;

int opcmp(operand* x,operand* y){
    if(x->kind!=y->kind){
        return 0;
    }
    else if(x->kind == OP_VAR || x->kind == OP_ADDR){
        return (strcmp(x->var_str,y->var_str)==0);
    }
    else if(x->kind == OP_INT){
        return x->val_int == y->val_int;
    }
    else{
        return (((x->val_float-y->val_float)>(-0.000001)) &&
                    (( x->val_float-y->val_float)<0.000001));
    }
}

void init_nodepoll(){
    memset(pool,0,NODE_AMOUNT*sizeof(dagnode));
};

int make_leaf(operand* op){
    dagnode* nd = &pool[current_nodeno++];
    if(op->kind == OP_INT){
        nd->type = -3;
    }
    else if(op->kind == OP_FLOAT){
        nd->type = -2;
    }
    else{
        nd->type = -1;
    }
    nd->lch = 0;
    nd->rch = 0;
    nd->status = VALID;
    nd->varlist[nd->varamt++] = op;
    nd->ic = NULL;
    return current_nodeno-1;
}

int find_make(operand* op){
    if(op->kind == OP_VAR || op->kind == OP_ADDR){
        tmpvar_ht_node* nd = find_tmpvar(op->var_str);
        assert(nd);
        if(nd->dag_node_no == 0){
            nd->dag_node_no = make_leaf(op);
        }
        return nd->dag_node_no;
    }
    else if(op->kind == OP_INT){
        int i;
        for(i=1;i<current_nodeno;i++){
            if(pool[i].type == -3 && pool[i].varlist[0]->val_int == op->val_int){
                break;
            }
        }
        if(i==current_nodeno){
            return make_leaf(op);
        }
        else{
            return i;
        }
    }
    else{
        int i;
        for(i=1;i<current_nodeno;i++){
            if(pool[i].type == -3 && pool[i].varlist[0]->val_int == op->val_int){
                break;
            }
        }
        if(i==current_nodeno){
            return make_leaf(op);
        }
        else{
            return i;
        }
    }
}


#define COMMON_ACTION {\
    nd->type = ic->kind;\
    nd->status = VALID;\
    nd->ic = ic;\
}

void handle_ic(intercode* ic){
    switch(ic->kind)
    {
        case ICN_CALL:
            {
                make_leaf(ic->res);
                //record as a new var;
                break;
            }
        case ICN_ASSIGN:
            {
                int op1_no = find_make(ic->op1);
                tmpvar_ht_node* res_ht_nd = find_tmpvar(ic->res->var_str);
                res_ht_nd->dag_node_no = current_nodeno;
                dagnode* nd = &pool[current_nodeno];
                COMMON_ACTION;
                nd->lch = op1_no;
                nd->varlist[nd->varamt++] = ic->res;
            }
        default:
        {
            dagnode* nd = &pool[current_nodeno];
            COMMON_ACTION;
        }
    }
}






