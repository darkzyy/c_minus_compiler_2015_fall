#include<string.h>
#include<stdio.h>
#include<assert.h>
#include"dag.h"
#include"debug.h"
#include"intercode.h"
#include"new_tools.h"

#define NODE_AMOUNT 256

static dagnode pool[NODE_AMOUNT];

int current_nodeno = 1;
int last_killno = 1;
ListHead opti_code_head;

static int max(int x,int y){
    return x>y?x:y;
}

int opcmp(operand* x,operand* y){
    if(x->kind!=y->kind){
        return 1;
    }
    else if(x->kind == OP_VAR || x->kind == OP_ADDR){
        return (strcmp(x->var_str,y->var_str));
    }
    else if(x->kind == OP_INT){
        return x->val_int != y->val_int;
    }
    else{
        return !(((x->val_float-y->val_float)>(-0.000001)) &&
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
    nd->res = op;
    nd->ic = NULL;
    Log3("leaf node line:%d ",current_nodeno-1);
    return current_nodeno-1;
}

int find_make(operand* op){
    if(op->kind == OP_VAR || op->kind == OP_ADDR){
        tmpvar_ht_node* nd = find_tmpvar(op->var_str);
        assert(nd);
        if(nd->dag_node_no == 0){
            nd->dag_node_no = make_leaf(op);
        }
        else{
            Log3("found : %d",nd->dag_node_no);
        }
        return nd->dag_node_no;
    }
    else if(op->kind == OP_INT){
        int i;
        for(i=1;i<current_nodeno;i++){
            if(pool[i].type == -3 && pool[i].res->val_int == op->val_int){
                break;
            }
        }
        if(i==current_nodeno){
            Log3("make");
            return make_leaf(op);
        }
        else{
            Log3("found : %d",i);
            return i;
        }
    }
    else{
        int i;
        for(i=1;i<current_nodeno;i++){
            if(pool[i].type == -2 && pool[i].res->val_float == op->val_float){
                break;
            }
        }
        if(i==current_nodeno){
            Log3("make");
            return make_leaf(op);
        }
        else{
            Log3("found : %d",i);
            return i;
        }
    }
}

int get_update_time(operand* op){
    if(op->kind == OP_INT || op->kind == OP_FLOAT){
        Log3();
        return find_make(op);
    }
    else{
        Log3();
        return find_tmpvar(op->var_str)->update_no;
    }
}

int ic_match(int start,int end,int op1_no,int op2_no,intercode* ic){
    int i;
    for(i=start+1;i<end;i++){
        dagnode* nd = &pool[i];
        if(nd->status!=VALID || nd->type != ic->kind){
            Log3();
            continue;
        }
        if(nd->lch == op1_no && nd->rch == op2_no){
            Log3();
            break;
        }
        if(nd->lch == op2_no && nd->rch == op1_no){
            Log3();
            break;
        }
    }
    if(i>=end){
        Log3("not found");
        return 0;
    }
    else{
        Log3("found");
        return i;
    }
}

void kill_refer(){
    int i = last_killno;
    for(;i<current_nodeno;i++){
        dagnode* nd = &pool[i];
        operand* op1 = pool[nd->lch].res;
        operand* op2 = pool[nd->lch].res;
        if(nd->ic && nd->ic->use_addr == 0 && ((op1 && op1->kind == OP_ADDR) || 
                        (op2 && op2->kind == OP_ADDR))){
            Log3("Last kill = %d,i = %d",last_killno,i);
            nd->status = KILLED;
        }
    }
    last_killno = current_nodeno;
}

#define COMMON_ACTION {\
    nd->type = ic->kind;\
    nd->status = VALID;\
    nd->ic = ic;\
}

dagnode* get_current_node(){
    pool[current_nodeno++].status = VALID;
    return &pool[current_nodeno-1];
}

extern void print_intercode(intercode* ic);

void handle_ic(intercode* ic){
    //print_intercode(ic);
    switch(ic->kind)
    {
        case ICN_CALL:
            {
                tmpvar_ht_node* res_ht_nd = find_tmpvar(ic->res->var_str);
                dagnode* nd = get_current_node();
                nd->ic = ic;
                nd->res = ic->res;
                nd->type = ic->kind;
                res_ht_nd->dag_node_no = current_nodeno-1;
                res_ht_nd->update_no = current_nodeno-1; //record in the temp var table

                if(ic->use_addr == 0 && ic->res->kind == OP_ADDR){
                    kill_refer();
                }
                break;
            }
        case ICN_ASSIGN:
            {
                int op1_no = find_make(ic->op1);//get op1 no
                dagnode* nd = get_current_node();//get node for current ic
                nd->ic = ic;
                nd->type = ic->kind;
                nd->res = ic->res;

                tmpvar_ht_node* res_ht_nd = find_tmpvar(ic->res->var_str);
                res_ht_nd->dag_node_no = op1_no;
                res_ht_nd->update_no = current_nodeno-1; //record in the temp var table

                dagnode* op_nd = &pool[op1_no];
                Log3("ic->op1 kind:%d",ic->op1->kind);
                Log3("op1_no = %d",op1_no);
                Log3("loaded res addr:%p",op_nd->res);
                ic->op1 = op_nd->res; //modify current ic
                Log3("ic->op1 kind:%d",ic->op1->kind);
                if(ic->use_addr == 0 && ic->res->kind == OP_ADDR){
                    kill_refer();
                }
                break;
            }
        case ICN_PLUS:
            {
            }
        case ICN_MINUS:
            {
            }
        case ICN_MUL:
            {
            }
        case ICN_DIV:
            {
                int op1_no = find_make(ic->op1);
                int op2_no = find_make(ic->op2);
                int op1_upd_no = get_update_time(ic->op1);
                int op2_upd_no = get_update_time(ic->op2);
                Log3("update time, op1 : %d ,op2 : %d",op1_upd_no,op2_upd_no);
                tmpvar_ht_node* res_ht_nd = find_tmpvar(ic->res->var_str);
                int start = max(op1_upd_no,op2_upd_no);
                int end = current_nodeno;
                int match_no = ic_match(start,end,op1_no,op2_no,ic);
                int ic_type = ic->kind;
                if(match_no == 0){ //a new node
                    ic->op1 = pool[op1_no].res;
                    ic->op2 = pool[op2_no].res;
                    if(ic->op1->kind == OP_INT && ic->op2->kind == OP_INT){
                        operand* tmpres ;
                        int tmpres_no;
                        switch(ic->kind){
                            case ICN_PLUS:
                                {
                                    Log3();
                                    tmpres = make_int(ic->op1->val_int+ic->op2->val_int);
                                    tmpres_no = find_make(tmpres);
                                    break;
                                }
                            case ICN_MINUS:
                                {
                                    Log3();
                                    tmpres = make_int(ic->op1->val_int-ic->op2->val_int);
                                    tmpres_no = find_make(tmpres);
                                    break;
                                }
                            case ICN_MUL:
                                {
                                    Log3();
                                    tmpres = make_int(ic->op1->val_int*ic->op2->val_int);
                                    tmpres_no = find_make(tmpres);
                                    break;
                                }
                            case ICN_DIV:
                                {
                                    Log3();
                                    tmpres = make_int(ic->op1->val_int/ic->op2->val_int);
                                    tmpres_no = find_make(tmpres);
                                    break;
                                }
                            default:
                                {}
                        }
                        ic->kind = ICN_ASSIGN;
                        ic->op1 = tmpres;
                        tmpres_no = tmpres_no+1;//!!!!!!!!!!!!!
                    }
                    if(ic->op1->kind == OP_FLOAT && ic->op2->kind == OP_FLOAT){
                        operand* tmpres ;
                        float tmpres_no;
                        switch(ic->kind){
                            case ICN_PLUS:
                                {
                                    tmpres = make_float(ic->op1->val_float+ic->op2->val_float);
                                    tmpres_no = find_make(tmpres);
                                    break;
                                }
                            case ICN_MINUS:
                                {
                                    tmpres = make_float(ic->op1->val_float-ic->op2->val_float);
                                    tmpres_no = find_make(tmpres);
                                    break;
                                }
                            case ICN_MUL:
                                {
                                    tmpres = make_float(ic->op1->val_float*ic->op2->val_float);
                                    tmpres_no = find_make(tmpres);
                                    break;
                                }
                            case ICN_DIV:
                                {
                                    tmpres = make_float(ic->op1->val_float/ic->op2->val_float);
                                    tmpres_no = find_make(tmpres);
                                    break;
                                }
                            default:
                                {}
                        }
                        ic->kind = ICN_ASSIGN;
                        ic->op1 = tmpres;
                        tmpres_no = tmpres_no+1;
                    }
                    dagnode* nd = get_current_node();
                    res_ht_nd->dag_node_no = current_nodeno-1;
                    res_ht_nd->update_no = current_nodeno-1;
                    nd->ic = ic;
                    nd->lch = op1_no; //lch and rch record the src
                    nd->rch = op2_no;
                    nd->type = ic_type;
                    nd->res = ic->res;
                    Log3("stored res addr:%p",nd->res);
                    Log3("generated arith: No %d, lch:%d,rch%d",current_nodeno-1,nd->lch,nd->rch);
                }
                else{
                    Log3("matched No : %d",match_no);
                    res_ht_nd->dag_node_no = match_no;
                    res_ht_nd->update_no = match_no;
                    dagnode* nd = get_current_node();
                    nd->type = ic->kind;
                    ic->kind = ICN_ASSIGN;
                    ic->op1 = pool[match_no].res;
                    nd->res = ic->res;
                    Log3("convert arith to assign");
                }

                if(ic->use_addr == 0 && ic->res->kind == OP_ADDR){
                    kill_refer();
                }
                break;
            }
        case ICN_READ:
            {
                tmpvar_ht_node* res_ht_nd = find_tmpvar(ic->res->var_str);
                dagnode* nd = get_current_node();
                nd->ic = ic;
                nd->res = ic->res;
                nd->type = ic->kind;
                res_ht_nd->dag_node_no = current_nodeno-1;
                res_ht_nd->update_no = current_nodeno-1; //record in the temp var table

                if(ic->use_addr == 0 && ic->res->kind == OP_ADDR){
                    kill_refer();
                }
                break;
            }
        default:
            {
                dagnode* nd = get_current_node();
                nd->type = ic->kind;
                nd->ic = ic;
            }
    }
}

void handle_cb(code_block* cb){
    intercode* ic = cb->start;
    init_nodepoll();
    tmpvar_node_no_init();
    current_nodeno = 1;
    last_killno = 1;
    for(;ic!=cb->end;ic = list_entry(ic->list.next,intercode,list)){
        handle_ic(ic);
    }
}

void dag_opti(){
    ListHead* p;
    list_foreach(p,&block_head){
        handle_cb(list_entry(p,code_block,list));
    }
}






