#include<string.h>
#include<stdio.h>
#include"multi_tree.h"
#include"symtab.h"
#include"cmmtypes.h"
#include"debug.h"
#include"gen_code_tools.h"
#include"assert.h"

extern Type* type_int;
extern Type* type_float;
extern Type* type_error;

static int inside_func = 0;
static int inside_paradec = 0;
static int is_left_val = 0;

typedef void (*ft)(MTnode*);
extern void gen(MTnode* root);


static void Func_Program(MTnode* root){
    Log2("Func_Program");
    list_init(&code_head);
    list_init(&block_head);
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
}
static void Func_ExtDef3(MTnode* root){
    Log2("Func_ExtDef3 : Spec FunDec CompSt");
    gen(ch(1));
    inside_func = 1;
    gen(ch(2));
    inside_func = 0;
}
static void Func_ExtDef4(MTnode* root){
    Log2("Func_ExtDef4");
    assert(0);
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
    s->op = make_op(OP_VAR,NULL);
    if(inside_func){
        if(root->syn_type != type_int && root->syn_type != type_float){
            //example DEC v2 8
            intercode* ic = malloc(sizeof(intercode));
            ic->kind = ICN_DEC;
            ic->icn_dec.var = s->op;
            ic->icn_dec.size = root->syn_type->size;
            list_add_before(&code_head,&(ic->list));
        }
    }
    else if(inside_paradec){
        if(s->val_type!=type_int && s->val_type!=type_float){
            Log3();
            s->op->kind = OP_ADDR;
            gen_single_var(s->op,PARAM);
            Log3();
        }
        else{
            gen_single_var(s->op,PARAM);
        }
    }
}
static void Func_VarDec2(MTnode* root){
    Log2("Func_VarDec2");
    if(inside_func){
        char* id = get_var_id(root)->str;
        symbol* s = find_sym(&var_tab,id);
        assert(s);
        s->op = make_op(OP_VAR,NULL);
        intercode* ic = malloc(sizeof(intercode));
        ic->kind = ICN_DEC;
        ic->icn_dec.var = s->op;
        ic->icn_dec.size = root->syn_type->size;       
        list_add_before(&code_head,&(ic->list));
    }
    else if(inside_paradec){
        assert(0); //array as Parameter
    }
}
static void Func_FunDec1(MTnode* root){
    Log2("Func_FunDec1 : ID ( VarList )");
    gen_label(ch(0)->str,FUNC);
    inside_paradec = 1;
    gen(ch(2));
    inside_paradec = 0;
}
static void Func_FunDec2(MTnode* root){
    Log2("Func_FunDec2 : ID ()");
    gen_label(ch(0)->str,FUNC);
}
static void Func_VarList1(MTnode* root){
    Log2("Func_VarList1");
    gen(ch(0));
    gen(ch(2));
}
static void Func_VarList2(MTnode* root){
    Log2("Func_VarList2");
    gen(ch(0));
}
static void Func_ParamDec(MTnode* root){
    Log2("Func_ParamDec");
    gen(ch(1));
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
    gen(ch(0));
}
static void Func_Stmt2(MTnode* root){
    Log2("Func_Stmt2");
    gen(ch(0));
}
static void Func_Stmt3(MTnode* root){//return
    Log2("Func_Stmt3");
    if(ch(1)->type == Exp17){
        int tmp = get_int_val(ch(1));
        operand* const_val = make_op(OP_INT,(void*)&tmp);
        gen_single_var(const_val,RETURN);
    }
    else if(ch(1)->type == Exp18){
        float tmp = get_float_val(ch(1));
        operand* const_val = make_op(OP_FLOAT,(void*)&tmp);
        gen_single_var(const_val,RETURN);
    }
    else{
        gen(ch(1));
        gen_single_var(ch(1)->op,RETURN);
    }
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
    Log3("GOTO:");
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
    gen(ch(2));

    //gen code2.1
    is_left_val += 1;
    gen(ch(0));
    if(ch(0)->op->kind == OP_VAR){
        gen_assign(ch(0)->op,ch(2)->op,0,NULL);
    }
    else if(ch(0)->op->kind == OP_ADDR){
        gen_refer_assign(ch(0)->op,ch(2)->op);
    }
    else{
        assert(0);
    }
    if(is_left_val != 0){
        is_left_val -= 1;
    }

    //gen code2.2
    if(root->op == NULL){
        root->op = make_op(OP_VAR,NULL);
    }
    gen_assign(root->op,ch(0)->op,0,NULL);
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


#define arith(arith_op){\
    intercode* ic = malloc(sizeof(intercode));\
    ic->kind = ICN_##arith_op;\
    root->op = make_op(OP_VAR,NULL);\
    ic->icn_arith.result = root->op;\
    ic->icn_arith.op_left = ch(0)->op;\
    ic->icn_arith.op_right = ch(2)->op;\
    list_add_before(&code_head,&(ic->list));\
}

#define handle_double_const(arith_op) {\
    if((ch(0)->is_const == 1)&&(ch(2)->is_const == 1)){\
        if(ch(0)->op->kind == OP_INT){\
            int tmp = ch(0)->op->val_int arith_op ch(2)->op->val_int;\
            root->op = make_op(OP_INT,&tmp);\
            root->is_const = 1;\
            root->valt = tmp;\
            return;\
        }\
        else{\
            float tmp = ch(0)->op->val_float arith_op ch(2)->op->val_float;\
            root->op = make_op(OP_FLOAT,&tmp);\
            root->is_const = 1;\
            root->valf = tmp;\
            return;\
        }\
    }\
}

static void Func_Exp5(MTnode* root){
    Log3("Func_Exp5");
    Log3("root addr : %p",root);
    gen(ch(0));
    gen(ch(2));
    handle_double_const(+);
    operand* op_const = NULL;
    operand* op_var;
    if(ch(0)->op->kind == OP_INT){
        op_const = ch(0)->op;
        op_var = ch(2)->op;
    }
    if(ch(2)->op->kind == OP_INT){
        op_const = ch(2)->op;
        op_var = ch(0)->op;
    }
    if(op_const && op_const->val_int == 0){
        root->op = op_var;
        return;
    }
    arith(PLUS);
}
static void Func_Exp6(MTnode* root){
    Log3("Func_Exp6");
    Log3("root addr : %p",root);
    gen(ch(0));
    gen(ch(2));
    handle_double_const(-);
    operand* op_const = NULL;
    operand* op_var;
    if(ch(2)->op->kind == OP_INT){
        op_const = ch(2)->op;
        op_var = ch(0)->op;
    }
    if(op_const && op_const->val_int == 0){
        root->op = op_var;
        return;
    }
    arith(MINUS);
}
static void Func_Exp7(MTnode* root){
    Log3("Func_Exp7");
    Log3("root addr : %p",root);
    gen(ch(0));
    gen(ch(2));
    handle_double_const(*);
    Log3("ch0 addr : %p",ch(0));
    operand* op_const = NULL;
    operand* op_var;
    if(ch(0)->op->kind == OP_INT){
        op_const = ch(0)->op;
        op_var = ch(2)->op;
    }
    if(ch(2)->op->kind == OP_INT){
        op_const = ch(2)->op;
        op_var = ch(0)->op;
    }
    if(op_const && op_const->val_int == 1){
        root->op = op_var;
        return;
    }
    if(op_const && op_const->val_int == 0){
        root->op = op_const;
        return;
    }
    arith(MUL);
}
static void Func_Exp8(MTnode* root){
    Log3("Func_Exp8");
    gen(ch(0));
    gen(ch(2));
    handle_double_const(/);
    operand* op_const = NULL;
    operand* op_var;
    if(ch(2)->op->kind == OP_INT){
        op_const = ch(2)->op;
        op_var = ch(0)->op;
    }
    if(op_const && op_const->val_int == 1){
        root->op = op_var;
        return;
    }
    arith(DIV);
}
#undef handle_double_const
#undef arith
static void Func_Exp9(MTnode* root){
    Log2("Func_Exp9");
    gen(ch(1));
    root->op = ch(1)->op;
    root->is_const = ch(1)->is_const;
    root->al = ch(1)->al;
}
static void Func_Exp10(MTnode* root){
    Log2("Func_Exp10");
    //prepare operand for ch(1)
    ch(1)->op = malloc(sizeof(operand));
    gen(ch(1));

    if(root->op == NULL){
        root->op = make_op(OP_VAR,NULL);
    }
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
static void gen_args(Argl* al){
    if(al==NULL){
        return;
    }
    gen_args(al->next);
    if(al->is_basic){
        gen_single_var(al->op,ARG);
    }
    else{
        gen_single_var(al->op,ARG_ADDR);
    }
}
static void Func_Exp12(MTnode* root){
    Log3("Func_Exp12 : ID( Args )");
    if(root->op == NULL){
        root->op = make_op(OP_VAR,NULL);
    }

    ch(2)->al = malloc(sizeof(Argl));
    ch(2)->al->next = NULL;
    if(strcmp(ch(0)->str,"write")==0){
        int tmp = is_left_val ;
        is_left_val = 0;
        gen(ch(2));
        is_left_val = tmp;
        gen_single_var(ch(2)->al->op,WRITE);
    }
    else{
        is_left_val += 1;
        gen(ch(2));
        if(is_left_val>0){
            is_left_val -= 1;
        }
        gen_args(ch(2)->al);
        gen_call(root->op,ch(0)->str);
    }
}
static void Func_Exp13(MTnode* root){
    Log2("Func_Exp13 : ID()");
    if(root->op == NULL){
        root->op = make_op(OP_VAR,NULL);
    }
    if(strcmp(ch(0)->str,"read")==0){
        gen_single_var(root->op,READ);
    }
    else{
        gen_call(root->op,ch(0)->str);
    }
}
static void Func_Exp14(MTnode* root){
    Log2("Func_Exp14 : Exp [ Exp ]");
    is_left_val += 1;
    gen(ch(0)); // expect ch(0) to prepare addr
    if(is_left_val>0){
        is_left_val -= 1;
    }
    operand* ch0_addr;
    if(ch(0)->op->kind == OP_VAR){
        ch0_addr = malloc(sizeof(operand));
        ch0_addr->kind = OP_ADDR;
        ch0_addr->var_str = get_var_no();
        gen_addr(ch0_addr,ch(0)->op);
    }
    else{
        ch0_addr = ch(0)->op;
    }
    gen(ch(2));
    ////MUL:
    operand* tmp;
    if(!ch(2)->is_const){
        intercode* ic = malloc(sizeof(intercode));
        ic->kind = ICN_MUL;
        tmp = make_op(OP_VAR,NULL);
        ic->icn_arith.result = tmp;
        ic->icn_arith.op_left = make_op(OP_INT,(void*)&chst(0)->array.elem->size);
        ic->icn_arith.op_right = ch(2)->op;
        list_add_before(&code_head,&(ic->list));
        Log2("------intercode addr : %p",ic);
    }
    else{
        tmp = make_op(OP_VAR,NULL);
        tmp->kind = OP_INT;
        tmp->val_int = chst(0)->array.elem->size * ch(2)->op->val_int;
    }
    ////ADD:
    if(root->op==NULL){
        root->op = make_op(OP_VAR,NULL);
    }
    root->op->kind = OP_ADDR;
    intercode* ic = malloc(sizeof(intercode));
    ic->kind = ICN_PLUS;
    ic->icn_arith.result = root->op;
    ic->icn_arith.op_left = tmp;
    ic->icn_arith.op_right = ch0_addr;
    list_add_before(&code_head,&(ic->list));
    Log2("------intercode addr : %p",ic);

    if(!is_left_val){
        operand* val = make_op(OP_VAR,NULL);
        gen_refer(val,root->op);
        root->op = val;
    }
}
static void Func_Exp15(MTnode* root){ //!!!!!!!!!!!!!!!!!!1
    Log3("Func_Exp15 : Exp1 . Exp2");
    /*Exp1 is always ID || Exp14 || Exp15*/
    //get Exp1 addr
    is_left_val += 1;
    //ch(0)->op = malloc(sizeof(operand));
    gen(ch(0));
    if(is_left_val>0){
        is_left_val -= 1;
    }
    operand* ch0_addr;
    if(ch(0)->op->kind == OP_VAR){
        ch0_addr = malloc(sizeof(operand));
        ch0_addr->kind = OP_ADDR;
        ch0_addr->var_str = get_var_no();
        Log3();
        gen_addr(ch0_addr,ch(0)->op);
        Log3();
    }
    else{
        ch0_addr = ch(0)->op;
    }
    //get offset  ,and add to addr
    char* id = ch(2)->str;
    int offset = get_field_offset(chst(0)->fl,id);
    intercode* ic = malloc(sizeof(intercode));
    ic->kind = ICN_PLUS;
    if(root->op==NULL){
        root->op = make_op(OP_VAR,NULL);
    }
    root->op->kind = OP_ADDR;
    ic->icn_arith.result = root->op;
    ic->icn_arith.op_left = malloc(sizeof(operand));
    ic->icn_arith.op_left->kind = OP_INT;
    ic->icn_arith.op_left->val_int = offset;
    ic->icn_arith.op_right = ch0_addr;
    list_add_before(&code_head,&(ic->list));
    Log2("------intercode addr : %p",ic);
    if(!is_left_val){
        operand* val = make_op(OP_VAR,NULL);
        gen_refer(val,root->op);
        root->op = val;
    }
}
static void Func_Exp16(MTnode* root){
    Log2("Func_Exp16");
    if(!is_left_val){
        if(root->op==NULL){
            root->op = make_op(OP_VAR,NULL);
        }
        char* id = get_var_id(root)->str;
        symbol* s = find_sym(&var_tab,id);
        assert(s);
        gen_assign(root->op,s->op,0,NULL);
    }
    else{
        char* id = get_var_id(root)->str;
        symbol* s = find_sym(&var_tab,id);
        assert(s);
        root->op = s->op;
        if(root->op->kind!=OP_ADDR){
            root->op->kind = OP_VAR;
        }
    }
}
static void Func_Exp17(MTnode* root){
    Log2("Func_Exp17");
    root->valt = get_int_val(ch(0));
    root->op = make_op(OP_INT,get_int_addr(ch(0)));
    root->is_const = 1;
}
static void Func_Exp18(MTnode* root){
    Log2("Func_Exp18");
    root->valf = get_float_val(ch(0));
    root->op = make_op(OP_FLOAT,get_float_addr(ch(0)));
    root->is_const = 1;
}
static void Func_Args1(MTnode* root){
    Log2("Func_Args1 : Exp , Args");
    gen(ch(0));
    root->al->op = ch(0)->op;
    if(ch(0)->syn_type != type_int && ch(0)->syn_type != type_float){
        root->al->is_basic = 0;
    }
    else{
        root->al->is_basic = 1;
    }
    root->al->next = malloc(sizeof(Argl));
    root->al->next->next = NULL;
    ch(2)->al = root->al->next;
    gen(ch(2));
}
static void Func_Args2(MTnode* root){
    Log2("Func_Args2 : Exp");
    gen(ch(0));
    if(ch(0)->syn_type != type_int && ch(0)->syn_type != type_float){
        root->al->is_basic = 0;
    }
    else{
        root->al->is_basic = 1;
    }
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

