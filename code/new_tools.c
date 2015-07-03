#include"intercode.h"

operand* make_new_op_addr(char* var_str){
    operand* op = malloc(sizeof(operand));
    op->var_str = var_str;
    op->kind = OP_ADDR;
    return op;
}

operand* make_new_op_var(char* var_str){
    operand* op = malloc(sizeof(operand));
    op->var_str = var_str;
    op->kind = OP_VAR;
    return op;
}

operand* make_int(int val){
    operand* op = malloc(sizeof(operand));
    op->val_int = val;
    op->kind = OP_INT;
    return op;
}

operand* make_float(float val){
    operand* op = malloc(sizeof(operand));
    op->val_float = val;
    op->kind = OP_FLOAT;
    return op;
}

operand* gen_assign_addr(operand* res,operand* op1){
    intercode* ic = malloc(sizeof(intercode));
    ic->kind = ICN_ASSIGN;
    ic->res = res;
    ic->op1 = op1;
    ic->use_addr = 1;
    list_add_before(&code_head,&(ic->list));
    return res;
}

operand* gen_assign_var(operand* res,operand* op1){
    intercode* ic = malloc(sizeof(intercode));
    ic->kind = ICN_ASSIGN;
    ic->res = res;
    ic->op1 = op1;
    ic->use_addr = 0;
    list_add_before(&code_head,&(ic->list));
    return res;
}

#define handle_double_const(arith_op) {\
    if(op1->kind == OP_INT && op2->kind == OP_INT){\
        int val_int = op1->val_int arith_op op2->val_int;\
        res_tmp = make_int(val_int);\
        return res_tmp;\
    }\
    else if(op1->kind == OP_FLOAT && op2->kind == OP_FLOAT){\
        float val_float = op1->val_float arith_op op2->val_float;\
        res_tmp = make_float(val_float);\
        return res_tmp;\
    }\
}

#define arith_common(ic_type) {\
    if(use_addr == 1){\
        res_tmp = make_new_op_addr(res_tmp_str);\
    }\
    else{\
        res_tmp = make_new_op_var(res_tmp_str);\
    }\
    intercode* ic = malloc(sizeof(intercode));\
    ic->kind = ic_type;\
    ic->res = res_tmp;\
    ic->op1 = op1;\
    ic->op2 = op2;\
    ic->use_addr = use_addr;\
    list_add_before(&code_head,&(ic->list));\
    return res_tmp;\
}

operand* gen_plus(operand* op1,operand* op2,int use_addr,char* res_tmp_str){
    operand* res_tmp ;
    handle_double_const(+);
    /*handle single const operand*/
    operand* op_const = NULL;
    operand* op_var;
    if(op1->kind == OP_INT){
        op_const = op1;
        op_var = op2;
    }
    if(op2->kind == OP_INT){
        op_const = op2;
        op_var = op1;
    }
    if(op_const && op_const->val_int == 0){
        return op_var;
    }
    /*no const operand*/
    arith_common(ICN_PLUS);
}

operand* gen_minus(operand* op1,operand* op2,int use_addr,char* res_tmp_str){
    operand* res_tmp ;
    handle_double_const(-);
    /*handle single const operand*/
    operand* op_const = NULL;
    operand* op_var;
    if(op2->kind == OP_INT){
        op_const = op2;
        op_var = op1;
    }
    if(op_const && op_const->val_int == 0){
        return op_var;
    }
    /*no const operand*/
    arith_common(ICN_MINUS);
}

operand* gen_mul(operand* op1,operand* op2,int use_addr,char* res_tmp_str){
    operand* res_tmp ;
    handle_double_const(*);
    /*handle single const operand*/
    operand* op_const = NULL;
    operand* op_var;
    if(op1->kind == OP_INT){
        op_const = op1;
        op_var = op2;
    }
    if(op2->kind == OP_INT){
        op_const = op2;
        op_var = op1;
    }
    if(op_const && op_const->val_int == 1){
        return op_var;
    }
    if(op_const && op_const->val_int == 1){
        return op_const;
    }
    /*no const operand*/
    arith_common(ICN_MUL);
}

operand* gen_div(operand* op1,operand* op2,int use_addr,char* res_tmp_str){
    operand* res_tmp ;
    handle_double_const(/);
    /*handle single const operand*/
    operand* op_const = NULL;
    operand* op_var;
    if(op1->kind == OP_INT && op1->val_int == 0){
        return op1;
    }
    if(op2->kind == OP_INT){
        op_const = op2;
        op_var = op1;
    }
    if(op_const && op_const->val_int == 1){
        return op_var;
    }
    /*no const operand*/
    arith_common(ICN_DIV);
}

#undef handle_double_const
#undef arith_common
















