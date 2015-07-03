#ifndef __GEN_CODE_TOOLS_H__
#define __GEN_CODE_TOOLS_H__

#include"list.h"
#include"intercode.h"
#include"multi_tree.h"
#include"tempvar_ht.h"

static int in_deflist;

void op01_init(){
    zero = malloc(sizeof(operand));
    zero->kind = OP_INT;
    zero->val_int = 0;
    one = malloc(sizeof(operand));
    one->kind = OP_INT;
    one->val_int = 1;
}
static int* get_int_addr(MTnode* root){
    assert(root);
    if(root->type==INT){
        return &(root->valt);
    }
    else{
        return get_int_addr(ch(0));
    }
}
static float* get_float_addr(MTnode* root){
    assert(root);
    if(root->type==FLOAT){
        return &(root->valf);
    }
    else{
        return get_float_addr(ch(0));
    }
}
static int get_int_val(MTnode* root){
    assert(root);
    if(root->type == INT){
        return root->valt;
    }
    else{
        return get_int_val(ch(0));
    }
}
static float get_float_val(MTnode* root){
    assert(root);
    if(root->type == FLOAT){
        return root->valf;
    }
    else{
        return get_float_val(ch(0));
    }
}
static char* get_var_no(){
    char* v = malloc(6);
    if(in_deflist){
        v[0] = 'v';
    }
    else{
        v[0] = 't';
    }
    sprintf(v+1,"%04d",current_var_no);
    add_tmpvar(v);
    current_var_no += 1;
    return v;
}
static inline char* get_new_label(){
    char* label = malloc(10);
    strncpy(label,"label",5);
    sprintf(label+5,"%04d",current_label_no);
    current_label_no += 1;
    Log3("LABEL applied : %s",label);
    return label;
}

static void gen_assign(operand* left,operand* right,int right_kind,void* right_val){
    /*if left and right are given , this func will not change its content*/
    if(left==NULL){
        left = malloc(sizeof(operand));
        left->kind = OP_VAR;
        left->var_str = get_var_no();
    }
    if(right==NULL){
        assert(right_val);
        right = malloc(sizeof(operand));
        right->kind = right_kind;
        if(right_kind == OP_INT){
            right->val_int = *((int*) right_val);
        }
        else if(right_kind == OP_FLOAT){
            right->val_float = *((float*) right_val);
        }
        else{
            right->var_str = get_var_no();
        }
    }
    intercode* ic = malloc(sizeof(intercode));
    ic->kind = ICN_ASSIGN;
    ic->res = left;
    ic->op1 = right;
    list_add_before(&code_head,&(ic->list));
    Log2("------intercode addr : %p",ic);
}

#define gen_label(s,suffix) {\
    intercode* ic = malloc(sizeof(intercode));\
    ic->kind = ICN_##suffix;\
    ic->label = s;\
    list_add_before(&code_head,&(ic->list));\
    Log3("label : %s",s);\
}

#define gen_single_var(op,suffix) {\
    intercode* ic = malloc(sizeof(intercode));\
    ic->kind = ICN_##suffix;\
    ic->res = op;\
    list_add_before(&code_head,&(ic->list));\
}

static void gen_if(operand* op1,operand* op2,char* relop,char* label){
    intercode* ic = malloc(sizeof(intercode));
    ic->kind = ICN_IF;
    ic->op1 = op1;
    ic->op2 = op2;
    ic->relop = relop;
    ic->label = label;
    list_add_before(&code_head,&(ic->list));
}

static void gen_call(operand* op,char* func){
    intercode* ic = malloc(sizeof(intercode));
    ic->kind = ICN_CALL;
    ic->res = op;
    ic->label = func;
    list_add_before(&code_head,&(ic->list));
}

extern void gen(MTnode* root);

static void translate_cond(MTnode* root,char* label_true,char* label_false){
    Log2("--translate condition");
    switch(root->type)
    {
        case Exp2:{
                      char* label = get_new_label();
                      translate_cond(ch(0),label,label_false);
                      gen_label(label,LABEL);
                      translate_cond(ch(2),label_true,label_false);
                      break;
                  }
        case Exp3:{
                      char* label = get_new_label();
                      translate_cond(ch(0),label_true,label);
                      gen_label(label,LABEL);
                      translate_cond(ch(2),label_true,label_false);
                      break;
                  }
        case Exp4:{
                      gen(ch(0));
                      gen(ch(2));
                      gen_if(ch(0)->op,ch(2)->op,ch(1)->str,label_true);
                      Log3("GOTO:");
                      gen_label(label_false,GOTO);
                      break;
                  }
        case Exp11:{
                       translate_cond(ch(1),label_false,label_true);
                       break;
                   }
        default:{
                    gen(root);
                    gen_if(root->op,zero,"!=",label_true);
                    gen_label(label_false,GOTO);
                    break;
                }
    }
}
#define bool_translate {\
    char* label1 = get_new_label();\
    char* label2 = get_new_label();\
    root->op->kind = OP_VAR;\
    root->op->var_str = get_var_no();\
    gen_assign(root->op,zero,0,NULL);\
    translate_cond(root,label1,label2);\
    gen_label(label1,LABEL);\
    gen_assign(root->op,one,0,NULL);\
    gen_label(label2,LABEL);\
}

static operand* make_op(int kind,void* val){
    operand* op = malloc(sizeof(operand));
    op->kind = kind;
    if(kind == OP_INT){
        op->val_int = *(int*)val;
    }
    else if(kind == OP_FLOAT){
        op->val_float = *(float*)val;
    }
    else if(kind == OP_VAR){
        if(val==NULL){
            op->var_str = get_var_no();
        }
        else{
            op->var_str = (char*)val;
        }
    }
    return op;
}


#endif
