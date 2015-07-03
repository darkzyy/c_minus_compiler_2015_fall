#include<stdio.h>
#include"intercode.h"
#include"debug.h"

void print_operand_var(operand* op){
    if(op->kind == OP_INT){
        printf("#%d",op->val_int);
    }
    else if(op->kind == OP_FLOAT){
        printf("#%f",op->val_float);
    }
    else if(op->kind == OP_VAR){
        printf("%s",op->var_str);
    }
    else{
        printf("*%s",op->var_str);
    }
}
void print_operand_addr(operand* op){
    if(op->kind == OP_INT){
        printf("#%d",op->val_int);
    }
    else if(op->kind == OP_FLOAT){
        printf("#%f",op->val_float);
    }
    else if(op->kind == OP_VAR && op->var_str[0] != 't'){
        printf("&%s",op->var_str);
    }
    else{
        printf("%s",op->var_str);
    }
}


/*
   intercode* pp;
   */

#define print_arith_var(arith_op) {\
    print_operand_var(ic->res);\
    printf(" := ");\
    print_operand_var(ic->op1);\
    printf(" %c ",arith_op);\
    print_operand_var(ic->op2);\
    printf("\n");\
    return;\
}
#define print_arith_addr(arith_op) {\
    print_operand_addr(ic->res);\
    printf(" := ");\
    print_operand_addr(ic->op1);\
    printf(" %c ",arith_op);\
    print_operand_addr(ic->op2);\
    printf("\n");\
    return;\
}

void print_intercode(intercode* ic){
    assert(ic);
    if(ic->use_addr == 0){
        if(ic->kind == ICN_ASSIGN){
            print_operand_var(ic->res);
            printf(" := ");
            print_operand_var(ic->op1);
            printf(" \n");
            return;
        }
        else if(ic->kind == ICN_PLUS){
            print_arith_var('+');
        }
        else if(ic->kind == ICN_MINUS){
            print_arith_var('-');
        }
        else if(ic->kind == ICN_MUL){
            print_arith_var('*');
        }
        else if(ic->kind == ICN_DIV){
            print_arith_var('/');
        }
    }
    else{
        if(ic->kind == ICN_ASSIGN){
            print_operand_addr(ic->res);
            printf(" := ");
            print_operand_addr(ic->op1);
            printf(" \n");
            return;
        }
        else if(ic->kind == ICN_PLUS){
            print_arith_addr('+');
        }
        else if(ic->kind == ICN_MINUS){
            print_arith_addr('-');
        }
        else if(ic->kind == ICN_MUL){
            print_arith_addr('*');
        }
        else if(ic->kind == ICN_DIV){
            print_arith_addr('/');
        }
    }
    if(ic->kind == ICN_LABEL){
        printf("LABEL %s :\n",ic->label);
    }
    else if(ic->kind == ICN_FUNC){
        printf("FUNCTION %s :\n",ic->label);
    }
    else if(ic->kind == ICN_GOTO){
        printf("GOTO %s\n",ic->label);
    }
    else if(ic->kind == ICN_IF){
        printf("IF ");
        print_operand_var(ic->op1);
        printf(" %s ",ic->relop);
        print_operand_var(ic->op2);
        printf(" GOTO %s\n",ic->label);
    }
    else if(ic->kind == ICN_RETURN){
        printf("RETURN ");
        print_operand_var(ic->res);
        printf("\n");
    }
    else if(ic->kind == ICN_DEC){
        printf("DEC ");
        print_operand_var(ic->res);
        printf(" %d\n",ic->size);
    }
    else if(ic->kind == ICN_ARG){
        printf("ARG ");
        print_operand_var(ic->res);
        printf("\n");
    }
    else if(ic->kind == ICN_ARG_ADDR){
        if(ic->res->kind==OP_VAR){
            printf("ARG &");
            print_operand_var(ic->res);
        }
        else{
            printf("ARG ");
            print_operand_addr(ic->res);
        }
        printf("\n");
    }
    else if(ic->kind == ICN_CALL){
        print_operand_var(ic->res);
        printf(" := CALL %s\n",ic->label);
    }
    else if(ic->kind == ICN_PARAM){
        printf("PARAM ");
        printf("%s",ic->res->var_str); //special case
        printf("\n");
    }
    else if(ic->kind == ICN_READ){
        printf("READ ");
        print_operand_var(ic->res);
        printf("\n");
    }
    else if(ic->kind == ICN_WRITE){
        printf("WRITE ");
        print_operand_var(ic->res);
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
