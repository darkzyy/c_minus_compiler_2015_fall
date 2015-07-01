#ifndef __CODE_PRINT_H__
#define __CODE_PRINT_H__

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

#define code_type (list_entry(p,intercode,list)->kind)
#define pp list_entry(p,intercode,list)
#define print_arith(arith_op) {\
    print_operand(pp->icn_arith.result);\
    printf(" := ");\
    print_operand(pp->icn_arith.op_left);\
    printf(" %c ",arith_op);\
    print_operand(pp->icn_arith.op_right);\
    printf("\n");\
}

static void print_code(){
    ListHead* p=NULL;
    ListHead* h = &code_head;
    list_foreach(p,h){
        Log2("------list entry addr : %p",pp);
        if(code_type == ICN_ASSIGN){
            print_operand(pp->icn_assign.left);
            printf(" := ");
            print_operand(pp->icn_assign.right);
            printf(" \n");
        }
        else if(code_type == ICN_PLUS){
            print_arith('+');
        }
        else if(code_type == ICN_MINUS){
            print_arith('-');
        }
        else if(code_type == ICN_MUL){
            print_arith('*');
        }
        else if(code_type == ICN_DIV){
            print_arith('/');
        }
        else if(code_type == ICN_LABEL){
            printf("LABEL %s :\n",pp->icn_label.label);
        }
        else if(code_type == ICN_FUNC){
            printf("FUNCTION %s :\n",pp->icn_label.label);
        }
        else if(code_type == ICN_ADDR){
            print_operand(pp->icn_addr.left);
            printf(" := &");
            print_operand(pp->icn_addr.right);
            printf("\n");
        }
        else if(code_type == ICN_REFER){
            print_operand(pp->icn_refer.left);
            printf(" := *");
            print_operand(pp->icn_refer.right);
            printf("\n");
        }
        else if(code_type == ICN_REFER_ASSIGN){
            printf("*");
            print_operand(pp->icn_refer_assign.left);
            printf(" := ");
            print_operand(pp->icn_refer_assign.right);
            printf("\n");
        }
        else if(code_type == ICN_GOTO){
            printf("GOTO %s\n",pp->icn_label.label);
        }
        else if(code_type == ICN_IF){
            printf("IF ");
            print_operand(pp->icn_if.op_left);
            printf(" %s ",pp->icn_if.relop);
            print_operand(pp->icn_if.op_right);
            printf(" GOTO %s\n",pp->icn_if.label);
        }
        else if(code_type == ICN_RETURN){
            printf("RETURN ");
            print_operand(pp->icn_single_var.var);
            printf("\n");
        }
        else if(code_type == ICN_DEC){
            printf("DEC ");
            print_operand(pp->icn_dec.var);
            printf(" %d\n",pp->icn_dec.size);
        }
        else if(code_type == ICN_ARG){
            printf("ARG ");
            print_operand(pp->icn_single_var.var);
            printf("\n");
        }
        else if(code_type == ICN_CALL){
            print_operand(pp->icn_call.result);
            printf(" := CALL %s\n",pp->icn_call.func);
        }
        else if(code_type == ICN_PARAM){
            printf("PARAM ");
            print_operand(pp->icn_single_var.var);
            printf("\n");
        }
        else if(code_type == ICN_READ){
            printf("READ ");
            print_operand(pp->icn_single_var.var);
            printf("\n");
        }
        else if(code_type == ICN_WRITE){
            printf("WRITE ");
            print_operand(pp->icn_single_var.var);
            printf("\n");
        }
        else{
            printf("error code \n");
        }
    }
}

#undef print_arith
#undef code_type
#undef pp

#endif
