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
