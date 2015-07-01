#include<string.h>
#include<stdio.h>
#include"multi_tree.h"
#include"symtab.h"
#include"cmmtypes.h"
#include"debug.h"
#include"intercode.h"
#include"assert.h"

extern Type* type_int;
extern Type* type_float;
extern Type* type_error;

static ListHead code_head;
static int current_var_no = 0;
static int current_label_no = 0;
static int inside_func = 0;
static operand* zero;
static operand* one;

typedef void (*ft)(MTnode*);
extern void gen(MTnode* root);

void op01_init(){
    zero = malloc(sizeof(operand));
    zero->kind = OP_INT;
    zero->val_int = 0;
    one = malloc(sizeof(operand));
    one->kind = OP_INT;
    one->val_int = 1;
}

static inline int get_int_val(MTnode* root){
    return ch(0)->valt;
}
static inline float get_float_val(MTnode* root){
    Log2("float val: %f\n",ch(0)->valf);
    return ch(0)->valf;
}
static inline char* get_var_no(){
    char* v = malloc(6);
    v[0] = 'v';
    sprintf(v+1,"%04d",current_var_no);
    Log2("------var no: %s used",v);
    current_var_no += 1;
    return v;
}
static inline char* get_new_label(){
    char* label = malloc(10);
    strncpy(label,"label",5);
    sprintf(label+5,"%04d",current_label_no);
    current_label_no += 1;
    return label;
}


static void gen_assign(operand* left,operand* right,int right_kind,void* right_val){
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
    ic->icn_assign.left = left;
    ic->icn_assign.right = right;
    list_add_before(&code_head,&(ic->list));
}

#define gen_label(s,suffix) {\
    intercode* ic = malloc(sizeof(intercode));\
    ic->kind = ICN_##suffix;\
    ic->icn_label.label = s;\
    list_add_before(&code_head,&(ic->list));\
}

#define gen_single_var(op,suffix) {\
    intercode* ic = malloc(sizeof(intercode));\
    ic->kind = ICN_##suffix;\
    ic->icn_single_var.var = op;\
    list_add_before(&code_head,&(ic->list));\
}

static void gen_if(operand* op1,operand* op2,char* relop,char* label){
    intercode* ic = malloc(sizeof(intercode));
    ic->kind = ICN_IF;
    ic->icn_if.op_left = op1;
    ic->icn_if.op_right = op2;
    ic->icn_if.relop = relop;
    ic->icn_if.label = label;
    list_add_before(&code_head,&(ic->list));
}

static void gen_call(operand* op,char* func){
    intercode* ic = malloc(sizeof(intercode));
    ic->kind = ICN_CALL;
    ic->icn_call.result = op;
    ic->icn_call.func = func;
    list_add_before(&code_head,&(ic->list));
}

static void translate_cond(MTnode* root,char* label_true,char* label_false){
    Log2("--translate condition");
    switch(root->type)
    {
        case Exp2:{
                      char* label = get_new_label();
                      translate_cond(ch(0),label,label_false);
                      translate_cond(ch(2),label_true,label_false);
                      break;
                  }
        case Exp3:{
                      char* label = get_new_label();
                      translate_cond(ch(0),label_true,label);
                      translate_cond(ch(2),label_true,label_false);
                      break;
                  }
        case Exp4:{
                      ch(0)->op = malloc(sizeof(operand));
                      ch(2)->op = malloc(sizeof(operand));
                      gen(ch(0));
                      gen(ch(2));
                      gen_if(ch(0)->op,ch(2)->op,ch(1)->str,label_true);
                      gen_label(label_false,GOTO);
                      break;
                  }
        case Exp11:{
                       translate_cond(ch(1),label_false,label_true);
                       break;
                   }
        default:{
                    root->op = malloc(sizeof(operand));
                    gen(root);
                    gen_if(root->op,zero,"!=",label_true);
                    gen_label(label_false,LABEL);
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


static void print_operand(operand* op){
    if(op->kind == OP_INT){
        printf("#%d",op->val_int);
    }
    else if(op->kind == OP_FLOAT){
        printf("#%f",op->val_float);
    }
    else if(op->kind == OP_VAR){
        printf("%s",op->var_str);
    }
    else if(op->kind == OP_ADDR){
        printf("&%s",op->var_str);
    }
    else{
        printf("*%s",op->var_str);
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

void print_code(){
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
            printf("FUNC %s :\n",pp->icn_label.label);
        }
        /*
        else if(code_type == ICN_ADDR){
            print_operand(pp->icn_addr.left);
            printf(" := ");
            print_operand(pp->icn_addr.right);
            printf("&\n");
        }
        else if(code_type == ICN_REFER){
            print_operand(pp->icn_refer.left);
            printf(" := ");
            print_operand(pp->icn_refer.right);
            printf("*\n");
        }
        else if(code_type == ICN_REFER_ASSIGN){
            printf("*");
            print_operand(pp->icn_refer_assign.left);
            printf(" := ");
            print_operand(pp->icn_refer_assign.right);
            printf("\n");
        }
        */
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

static void Func_Program(MTnode* root){
    Log2("Func_Program");
    list_init(&code_head);
    gen(ch(0));
}
static void Func_ExtDefList1(MTnode* root){
    Log2("Func_ExtDefList1");
    gen(ch(0));
    gen(ch(1));
}
static void Func_ExtDefList2(MTnode* root){
    Log2("Func_ExtDefList2");
}
static void Func_ExtDef1(MTnode* root){
    Log2("Func_ExtDef1");
    gen(ch(0));
    gen(ch(1));
}
static void Func_ExtDef2(MTnode* root){
    Log2("Func_ExtDef2");
    gen(ch(0));
}
static void Func_ExtDef3(MTnode* root){
    Log2("Func_ExtDef3");
    gen(ch(0));
    gen(ch(1));
    inside_func = 1;
    gen(ch(2));
    inside_func = 0;
}
static void Func_ExtDef4(MTnode* root){
    Log2("Func_ExtDef4");
    gen(ch(0));
    gen(ch(1));
}
static void Func_ExtDecList1(MTnode* root){
    Log2("Func_ExtDecList1");
    gen(ch(0));
}
static void Func_ExtDecList2(MTnode* root){
    Log2("Func_ExtDecList2");
    gen(ch(0));
    gen(ch(1));
}
static void Func_Specifier1(MTnode* root){
    Log2("Func_Specifier1");
    gen(ch(0));
}
static void Func_Specifier2(MTnode* root){
    Log2("Func_Specifier2");
    gen(ch(0));
}
static void Func_StructSpecifier1(MTnode* root){
    Log2("Func_StructSpecifier1");
    gen(ch(1));
    gen(ch(3));
}
static void Func_StructSpecifier2(MTnode* root){
    Log2("Func_StructSpecifier2");
    gen(ch(1));
}
static void Func_OptTag(MTnode* root){
    Log2("Func_OptTag");
}
static void Func_Tag(MTnode* root){
    Log2("Func_Tag");
}
static void Func_VarDec1(MTnode* root){
    Log2("Func_VarDec1");
    char* id = get_var_id(root)->str;
    symbol* s = find_sym(&var_tab,id);
    assert(s);
    s->op = malloc(sizeof(operand));
    s->op->kind = OP_VAR;
    s->op->var_str = get_var_no();
    if(root->syn_type != type_int && root->syn_type != type_float){
        //example DEC v2 8
        intercode* ic = malloc(sizeof(intercode));
        ic->kind = ICN_DEC;
        ic->icn_dec.var = s->op;
        ic->icn_dec.size = root->syn_type->size;
    }
}
static void Func_VarDec2(MTnode* root){
    Log2("Func_VarDec2");
    char* id = get_var_id(root)->str;
    symbol* s = find_sym(&var_tab,id);
    assert(s);
    s->op = malloc(sizeof(operand));
    s->op->kind = OP_VAR;
    s->op->var_str = get_var_no();
    intercode* ic = malloc(sizeof(intercode));
    ic->kind = ICN_DEC;
    ic->icn_dec.var = s->op;
    ic->icn_dec.size = root->syn_type->size;
}
static void Func_FunDec1(MTnode* root){
    Log2("Func_FunDec1");
}
static void Func_FunDec2(MTnode* root){
    Log2("Func_FunDec2");
}
static void Func_VarList1(MTnode* root){
    Log2("Func_VarList1");
}
static void Func_VarList2(MTnode* root){
    Log2("Func_VarList2");
}
static void Func_ParamDec(MTnode* root){
    Log2("Func_ParamDec");
}
static void Func_CompSt(MTnode* root){
    Log2("Func_CompSt");
    gen(ch(1));
    gen(ch(2));
}
static void Func_StmtList1(MTnode* root){
    Log2("Func_StmtList1");
    gen(ch(0));
    gen(ch(1));
}
static void Func_StmtList2(MTnode* root){
    Log2("Func_StmtList2");
}
static void Func_Stmt1(MTnode* root){
    Log2("Func_Stmt1");
    ch(0)->op = NULL;
    gen(ch(0));
}
static void Func_Stmt2(MTnode* root){
    Log2("Func_Stmt2");
    gen(ch(0));
}
static void Func_Stmt3(MTnode* root){//return
    Log2("Func_Stmt3");
    ch(1)->op = malloc(sizeof(operand));
    gen(ch(1));
    gen_single_var(ch(1)->op,RETURN);
}
static void Func_Stmt4(MTnode* root){//if
    Log2("Func_Stmt4");
    char* label1 = get_new_label();
    char* label2 = get_new_label();
    translate_cond(ch(2),label1,label2);
    gen(ch(4));
}
static void Func_Stmt5(MTnode* root){//if else
    Log2("Func_Stmt5");
    char* label1 = get_new_label();
    char* label2 = get_new_label();
    char* label3 = get_new_label();
    translate_cond(ch(2),label1,label2);
    gen_label(label1,LABEL);
    gen(ch(4));
    gen_label(label3,GOTO);
    gen_label(label2,LABEL);
    gen(ch(6));
    gen_label(label3,LABEL);
}
static void Func_Stmt6(MTnode* root){//while
    Log2("Func_Stmt6");
    char* label1 = get_new_label();
    char* label2 = get_new_label();
    char* label3 = get_new_label();
    gen_label(label1,LABEL);
    translate_cond(ch(2),label2,label3);
    gen_label(label2,LABEL);
    gen(ch(4));
    gen_label(label1,GOTO);
    gen_label(label3,LABEL);
}
static void Func_DefList1(MTnode* root){
    Log2("Func_DefList1");
    gen(ch(0));
    gen(ch(1));
}
static void Func_DefList2(MTnode* root){
    Log2("Func_DefList2");
}
static void Func_Def(MTnode* root){
    Log2("Func_Def");
    gen(ch(1));
}
static void Func_DecList1(MTnode* root){
    Log2("Func_DecList1");
    gen(ch(0));
}
static void Func_DecList2(MTnode* root){
    Log2("Func_DecList2");
    gen(ch(0));
    gen(ch(2));
}
static void Func_Dec1(MTnode* root){
    Log2("Func_Dec1");
    gen(ch(0));
}
static void Func_Dec2(MTnode* root){  
    //  VarDec ASSIGNOP  Exp
    Log2("Func_Dec2");
    gen(ch(0));
    ch(2)->op = malloc(sizeof(operand));
    gen(ch(2));

    //gen code2.1
    char* id = get_var_id(root)->str;
    symbol* s = find_sym(&var_tab,id);
    gen_assign(s->op,ch(2)->op,0,NULL);
}
static void Func_Exp1(MTnode* root){
    Log2("Func_Exp1 : ASSIGN");
    ch(2)->is_left_val = 0;
    ch(2)->op = malloc(sizeof(operand));
    gen(ch(2));

    //gen code2.1
    ch(0)->is_left_val = 1;
    ch(0)->op = malloc(sizeof(operand));
    gen(ch(0));
    gen_assign(ch(0)->op,ch(2)->op,0,NULL);

    //gen code2.2
    if(root->op != NULL){
        gen_assign(root->op,ch(0)->op,0,NULL);
    }
}
static void Func_Exp2(MTnode* root){
    Log2("Func_Exp2");
    bool_translate;
}
static void Func_Exp3(MTnode* root){
    Log2("Func_Exp3");
    bool_translate;
}
static void Func_Exp4(MTnode* root){
    Log2("Func_Exp4");
    bool_translate;
}

#define arith(arith_op)\
    ch(0)->op = malloc(sizeof(operand));\
    gen(ch(0));\
    ch(2)->op = malloc(sizeof(operand));\
    gen(ch(2));\
    root->op->kind = OP_VAR;\
    root->op->var_str = get_var_no();\
    intercode* ic = malloc(sizeof(intercode));\
    ic->kind = ICN_##arith_op;\
    ic->icn_arith.result = root->op;\
    ic->icn_arith.op_left = ch(0)->op;\
    ic->icn_arith.op_right = ch(2)->op;\
    list_add_before(&code_head,&(ic->list));


static void Func_Exp5(MTnode* root){
    arith(PLUS);
}
static void Func_Exp6(MTnode* root){
    arith(MINUS);
}
static void Func_Exp7(MTnode* root){
    arith(MUL);
}
static void Func_Exp8(MTnode* root){
    arith(DIV);
}
#undef arith
static void Func_Exp9(MTnode* root){
    Log2("Func_Exp9");
}
static void Func_Exp10(MTnode* root){
    Log2("Func_Exp10");
    //prepare operand for ch(1)
    ch(1)->op = malloc(sizeof(operand));
    gen(ch(1));

    root->op->kind = OP_VAR;
    root->op->var_str = get_var_no();
    //gen code 
    intercode* ic = malloc(sizeof(intercode));
    ic->kind = ICN_MINUS;
    ic->icn_arith.result = root->op;
    ic->icn_arith.op_left = zero;
    ic->icn_arith.op_right = ch(1)->op;
    list_add_before(&code_head,&(ic->list));
    Log2("------intercode addr:%p",ic);
}
static void Func_Exp11(MTnode* root){
    Log2("Func_Exp11");
    bool_translate;
}
static void Func_Exp12(MTnode* root){
    Log2("Func_Exp12");
}
static void Func_Exp13(MTnode* root){
    Log2("Func_Exp13 : ID()");
    root->op->kind = OP_VAR;
    root->op->var_str = get_var_no();
    if(strcmp(ch(0)->str,"read")==0){
        gen_single_var(root->op,READ);
    }
    else{
        gen_call(root->op,ch(0)->str);
    }
}
static void gen_args(Argl* al){
    if(al==NULL){
        return;
    }
    gen_args(al->next);
    gen_single_var(al->op,ARG);
}
static void Func_Exp14(MTnode* root){
    Log2("Func_Exp14 : ID( Args )");
    root->op->kind = OP_VAR;
    root->op->var_str = get_var_no();
    ch(2)->al = malloc(sizeof(Argl));
    ch(2)->al->next = NULL;
    gen(ch(2));
    if(strcmp(ch(0)->str,"write")==0){
        gen_single_var(ch(2)->al->op,WRITE);
    }
    else{
        gen_args(ch(2)->al);
        gen_call(root->op,ch(0)->str);
    }
}
static void Func_Exp15(MTnode* root){
    Log2("Func_Exp15");
}
static void Func_Exp16(MTnode* root){
    Log2("Func_Exp16");
    if(!root->is_left_val){
        root->op->kind = OP_VAR;
        root->op->var_str = get_var_no();
        char* id = get_var_id(root)->str;
        symbol* s = find_sym(&var_tab,id);
        assert(s);
        gen_assign(root->op,s->op,0,NULL);
    }
    else{
        root->op->kind = OP_VAR;
        char* id = get_var_id(root)->str;
        symbol* s = find_sym(&var_tab,id);
        assert(s);
        root->op = s->op;
    }
}
static void Func_Exp17(MTnode* root){
    Log2("Func_Exp17");
    root->op->kind = OP_VAR;
    root->op->var_str = get_var_no();
    int val = get_int_val(root);
    gen_assign(root->op,NULL,OP_INT,(void*)&val);
    Log2("added int: %d",val);
}
static void Func_Exp18(MTnode* root){
    Log2("Func_Exp18");
    root->op->kind = OP_VAR;
    root->op->var_str = get_var_no();
    float val = get_float_val(root);
    gen_assign(root->op,NULL,OP_FLOAT,(void*)&val);
    Log2("added float: %f",val);
}
static void Func_Args1(MTnode* root){
    Log2("Func_Args1 : Exp , Args");
    ch(0)->op = malloc(sizeof(operand));
    gen(ch(0));
    root->al->op = ch(0)->op;
    root->al->next = malloc(sizeof(Argl));
    root->al->next->next = NULL;
}
static void Func_Args2(MTnode* root){
    Log2("Func_Args2 : Exp");
    ch(0)->op = malloc(sizeof(operand));
    gen(ch(0));
    root->al->op = ch(0)->op;
}
static void Func_EMPTY(MTnode* root){
    Log2("Func_EMPTY");
}
static void Func_TYPE(MTnode* root){
    Log2("Func_TYPE");
}
static ft func_table[60] = {
    Func_Program ,
    Func_ExtDefList1 ,
    Func_ExtDefList2 ,
    Func_ExtDef1 ,
    Func_ExtDef2 ,
    Func_ExtDef3 ,
    Func_ExtDef4 ,
    Func_ExtDecList1 ,
    Func_ExtDecList2 ,
    Func_Specifier1 ,
    Func_Specifier2 ,
    Func_StructSpecifier1 ,
    Func_StructSpecifier2 ,
    Func_OptTag ,
    Func_Tag ,
    Func_VarDec1 ,
    Func_VarDec2 ,
    Func_FunDec1 ,
    Func_FunDec2 ,
    Func_VarList1 ,
    Func_VarList2 ,
    Func_ParamDec ,
    Func_CompSt ,
    Func_StmtList1 ,
    Func_StmtList2 ,
    Func_Stmt1 ,
    Func_Stmt2 ,
    Func_Stmt3 ,
    Func_Stmt4 ,
    Func_Stmt5 ,
    Func_Stmt6 ,
    Func_DefList1 ,
    Func_DefList2 ,
    Func_Def ,
    Func_DecList1 ,
    Func_DecList2 ,
    Func_Dec1 ,
    Func_Dec2 ,
    Func_Exp1 ,
    Func_Exp2 ,
    Func_Exp3 ,
    Func_Exp4 ,
    Func_Exp5 ,
    Func_Exp6 ,
    Func_Exp7 ,
    Func_Exp8 ,
    Func_Exp9 ,
    Func_Exp10 ,
    Func_Exp11 ,
    Func_Exp12 ,
    Func_Exp13 ,
    Func_Exp14 ,
    Func_Exp15 ,
    Func_Exp16 ,
    Func_Exp17 ,
    Func_Exp18 ,
    Func_Args1 ,
    Func_Args2 ,
    Func_EMPTY ,
    Func_TYPE
};


void gen(MTnode* root){
    if(root->type == TYPE){
        func_table[59](root);
    }
    else{
        func_table[root->type - Program](root);
    }
}

#undef bool_translate
