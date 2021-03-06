#include<stdio.h>
#include"intercode.h"

static void print_operand(operand* op){
    if(op->kind == OP_INT){
        printf("#%d",op->val_int);
    }
    else if(op->kind == OP_FLOAT){
        printf("#%f",op->val_float);
    }
    else{
        printf("%s",op->var_str);
    }
}
/*
intercode* pp;
*/

#define print_arith(arith_op) {\
    print_operand(ic->icn_arith.result);\
    printf(" := ");\
    print_operand(ic->icn_arith.op_left);\
    printf(" %c ",arith_op);\
    print_operand(ic->icn_arith.op_right);\
    printf("\n");\
}

void print_intercode(intercode* ic){
    if(ic->kind == ICN_ASSIGN){
        print_operand(ic->icn_assign.left);
        printf(" := ");
        print_operand(ic->icn_assign.right);
        printf(" \n");
    }
    else if(ic->kind == ICN_PLUS){
        print_arith('+');
    }
    else if(ic->kind == ICN_MINUS){
        print_arith('-');
    }
    else if(ic->kind == ICN_MUL){
        print_arith('*');
    }
    else if(ic->kind == ICN_DIV){
        print_arith('/');
    }
    else if(ic->kind == ICN_LABEL){
        printf("LABEL %s :\n",ic->icn_label.label);
    }
    else if(ic->kind == ICN_FUNC){
        printf("FUNCTION %s :\n",ic->icn_label.label);
    }
    else if(ic->kind == ICN_ADDR){
        print_operand(ic->icn_addr.left);
        printf(" := &");
        print_operand(ic->icn_addr.right);
        printf("\n");
    }
    else if(ic->kind == ICN_REFER){
        print_operand(ic->icn_refer.left);
        printf(" := *");
        print_operand(ic->icn_refer.right);
        printf("\n");
    }
    else if(ic->kind == ICN_REFER_ASSIGN){
        printf("*");
        print_operand(ic->icn_refer_assign.left);
        printf(" := ");
        print_operand(ic->icn_refer_assign.right);
        printf("\n");
    }
    else if(ic->kind == ICN_GOTO){
        printf("GOTO %s\n",ic->icn_label.label);
    }
    else if(ic->kind == ICN_IF){
        printf("IF ");
        print_operand(ic->icn_if.op_left);
        printf(" %s ",ic->icn_if.relop);
        print_operand(ic->icn_if.op_right);
        printf(" GOTO %s\n",ic->icn_if.label);
    }
    else if(ic->kind == ICN_RETURN){
        printf("RETURN ");
        print_operand(ic->icn_single_var.var);
        printf("\n");
    }
    else if(ic->kind == ICN_DEC){
        printf("DEC ");
        print_operand(ic->icn_dec.var);
        printf(" %d\n",ic->icn_dec.size);
    }
    else if(ic->kind == ICN_ARG){
        printf("ARG ");
        print_operand(ic->icn_single_var.var);
        printf("\n");
    }
    else if(ic->kind == ICN_ARG_ADDR){
        printf("ARG &");
        print_operand(ic->icn_single_var.var);
        printf("\n");
    }
    else if(ic->kind == ICN_CALL){
        print_operand(ic->icn_call.result);
        printf(" := CALL %s\n",ic->icn_call.func);
    }
    else if(ic->kind == ICN_PARAM){
        printf("PARAM ");
        print_operand(ic->icn_single_var.var);
        printf("\n");
    }
    else if(ic->kind == ICN_READ){
        printf("READ ");
        print_operand(ic->icn_single_var.var);
        printf("\n");
    }
    else if(ic->kind == ICN_WRITE){
        printf("WRITE ");
        print_operand(ic->icn_single_var.var);
        printf("\n");
    }
    else{
        printf("error code \n");
    }
}
#undef print_arith

#define pp list_entry(p,intercode,list)
void print_code(){
    ListHead* p=NULL;
    ListHead* h = &code_head;
    list_foreach(p,h){
        print_intercode(pp);
    }
}

#undef pp
