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

static inline int get_int_val(MTnode* root){
    return ch(0)->valt;
}
static inline int get_float_val(MTnode* root){
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

typedef void (*ft)(MTnode*);
extern void gen(MTnode* root);

static void print_operand(operand* op){
    if(op->kind == OP_INT){
        printf(" #%d ",op->val_int);
    }
    else if(op->kind == OP_FLOAT){
        printf(" #%d ",op->val_float);
    }
    else{
        printf(" %s ",op->var_str);
    }
}

#define code_type (list_entry(p,intercode,list)->kind)
#define pp list_entry(p,intercode,list)
#define print_arith(arith_op)\
    print_operand(pp->icn_plus.result);\
    printf(":=");\
    print_operand(pp->icn_plus.op_left);\
    printf("%c",arith_op);\
    print_operand(pp->icn_plus.op_right);\
    printf("\n");

void print_code(){
    ListHead* p=NULL;
    ListHead* h = &code_head;
    list_foreach(p,h){
        Log2("------list entry addr : %p",pp);
        if(code_type == ICN_ASSIGN){
            print_operand(pp->icn_assign.left);
            printf(":=");
            print_operand(pp->icn_assign.right);
            printf("\n");
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
static void Func_Stmt3(MTnode* root){
    Log2("Func_Stmt3");
}
static void Func_Stmt4(MTnode* root){
    Log2("Func_Stmt4");
}
static void Func_Stmt5(MTnode* root){
    Log2("Func_Stmt5");
}
static void Func_Stmt6(MTnode* root){
    Log2("Func_Stmt6");
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
    intercode* ic = malloc(sizeof(intercode));
    ic->kind = ICN_ASSIGN;
    ic->icn_assign.left = s->op;
    ic->icn_assign.right = ch(2)->op;
    list_add_before(&code_head,&(ic->list));
    Log2("------intercode addr:%p",ic);

}
static void Func_Exp1(MTnode* root){
    Log2("Func_Exp1 : ASSIGN");
    ch(2)->op = malloc(sizeof(operand));
    gen(ch(2));

    //gen code2.1
    char* id = get_var_id(root)->str;
    symbol* s = find_sym(&var_tab,id);
    intercode* ic = malloc(sizeof(intercode));
    ic->kind = ICN_ASSIGN;
    ic->icn_assign.left = s->op;
    ic->icn_assign.right = ch(2)->op;
    list_add_before(&code_head,&(ic->list));
    Log2("------intercode addr:%p",ic);

    if(root->op != NULL){
        //gen code2.2
        ic = malloc(sizeof(intercode));
        ic->kind = ICN_ASSIGN;
        ic->icn_assign.left = root->op;
        ic->icn_assign.right = s->op;
        list_add_before(&code_head,&(ic->list));
    }
    Log2("------intercode addr:%p",ic);
}
static void Func_Exp2(MTnode* root){
    Log2("Func_Exp2");
}
static void Func_Exp3(MTnode* root){
    Log2("Func_Exp3");
}
static void Func_Exp4(MTnode* root){
    Log2("Func_Exp4");
}

#define arith(arith_op)\
    ch(0)->op = malloc(sizeof(operand));\
    gen(ch(0));\
    ch(2)->op = malloc(sizeof(operand));\
    gen(ch(2));\
    intercode* ic = malloc(sizeof(intercode));\
    ic->kind = ICN_##arith_op;\
    root->op->kind = OP_VAR;\
    root->op->var_str = get_var_no();\
    ic->icn_plus.result = root->op;\
    ic->icn_plus.op_left = ch(0)->op;\
    ic->icn_plus.op_right = ch(2)->op;\
    list_add_before(&code_head,&(ic->list));\
    Log2("------intercode addr:%p",ic);


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
}
static void Func_Exp11(MTnode* root){
    Log2("Func_Exp11");
}
static void Func_Exp12(MTnode* root){
    Log2("Func_Exp12");
}
static void Func_Exp13(MTnode* root){
    Log2("Func_Exp13");
}
static void Func_Exp14(MTnode* root){
    Log2("Func_Exp14");
}
static void Func_Exp15(MTnode* root){
    Log2("Func_Exp15");
}
static void Func_Exp16(MTnode* root){
    Log2("Func_Exp16");
    //gen code
    intercode* ic = malloc(sizeof(intercode));
    ic->kind = ICN_ASSIGN;
    ic->icn_assign.left = root->op;
    ic->icn_assign.left->kind = OP_VAR;
    ic->icn_assign.left->var_str = get_var_no();

    char* id = get_var_id(root)->str;
    symbol* s = find_sym(&var_tab,id);
    assert(s);
    Log2("current ID: %s",id);
    ic->icn_assign.right = s->op;
    list_add_before(&code_head,&(ic->list));


    //modify its operand infomations for upper level
    Log2("added ID");
    Log2("------intercode addr:%p",ic);
}
static void Func_Exp17(MTnode* root){
    Log2("Func_Exp17");

    //gen code
    intercode* ic = malloc(sizeof(intercode));
    ic->kind = ICN_ASSIGN;
    ic->icn_assign.left = root->op;
    ic->icn_assign.left->kind = OP_VAR;
    ic->icn_assign.left->var_str = get_var_no();

    int val = get_int_val(root);
    ic->icn_assign.right = malloc(sizeof(operand));
    ic->icn_assign.right->kind = OP_INT;
    ic->icn_assign.right->val_int = val;
    list_add_before(&code_head,&(ic->list));

    Log2("added int: %d",val);
    Log2("------intercode addr:%p",ic);
}
static void Func_Exp18(MTnode* root){
    Log2("Func_Exp18");

    //gen code
    intercode* ic = malloc(sizeof(intercode));
    ic->kind = ICN_ASSIGN;
    ic->icn_assign.left = root->op;
    ic->icn_assign.left->kind = OP_VAR;
    ic->icn_assign.left->var_str = get_var_no();

    float val = get_float_val(root);
    ic->icn_assign.right = malloc(sizeof(operand));
    ic->icn_assign.right->kind = OP_FLOAT;
    ic->icn_assign.right->val_float = val;
    list_add_before(&code_head,&(ic->list));

    Log2("added float");
    Log2("------intercode addr:%p",ic);
}
static void Func_Args1(MTnode* root){
    Log2("Func_Args1");
}
static void Func_Args2(MTnode* root){
    Log2("Func_Args2");
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
