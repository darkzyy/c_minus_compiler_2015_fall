#include<string.h>
#include"multi_tree.h"
#include"symtab.h"
#include"cmmtypes.h"
#include"debug.h"
#include"intercode.h"

extern Type* type_int;
extern Type* type_float;
extern Type* type_error;

typedef void (*ft)(MTnode*);
extern void gen(MTnode* root);

static void Func_Program(MTnode* root){
    Log("Func_Program");
    gen(ch(0));
}
static void Func_ExtDefList1(MTnode* root){
    Log("Func_ExtDefList1");
}
static void Func_ExtDefList2(MTnode* root){
    Log("Func_ExtDefList2");
}
static void Func_ExtDef1(MTnode* root){
    Log("Func_ExtDef1");
}
static void Func_ExtDef2(MTnode* root){
    Log("Func_ExtDef2");
}
static void Func_ExtDef3(MTnode* root){
    Log("Func_ExtDef3");
}
static void Func_ExtDef4(MTnode* root){
    Log("Func_ExtDef4");
}
static void Func_ExtDecList1(MTnode* root){
    Log("Func_ExtDecList1");
}
static void Func_ExtDecList2(MTnode* root){
    Log("Func_ExtDecList2");
}
static void Func_Specifier1(MTnode* root){
    Log("Func_Specifier1");
}
static void Func_Specifier2(MTnode* root){
    Log("Func_Specifier2");
}
static void Func_StructSpecifier1(MTnode* root){
    Log("Func_StructSpecifier1");
}
static void Func_StructSpecifier2(MTnode* root){
    Log("Func_StructSpecifier2");
}
static void Func_OptTag(MTnode* root){
    Log("Func_OptTag");
}
static void Func_Tag(MTnode* root){
    Log("Func_Tag");
}
static void Func_VarDec1(MTnode* root){
    Log("Func_VarDec1");
}
static void Func_VarDec2(MTnode* root){
    Log("Func_VarDec2");
}
static void Func_FunDec1(MTnode* root){
    Log("Func_FunDec1");
}
static void Func_FunDec2(MTnode* root){
    Log("Func_FunDec2");
}
static void Func_VarList1(MTnode* root){
    Log("Func_VarList1");
}
static void Func_VarList2(MTnode* root){
    Log("Func_VarList2");
}
static void Func_ParamDec(MTnode* root){
    Log("Func_ParamDec");
}
static void Func_CompSt(MTnode* root){
    Log("Func_CompSt");
}
static void Func_StmtList1(MTnode* root){
    Log("Func_StmtList1");
}
static void Func_StmtList2(MTnode* root){
    Log("Func_StmtList2");
}
static void Func_Stmt1(MTnode* root){
    Log("Func_Stmt1");
}
static void Func_Stmt2(MTnode* root){
    Log("Func_Stmt2");
}
static void Func_Stmt3(MTnode* root){
    Log("Func_Stmt3");
}
static void Func_Stmt4(MTnode* root){
    Log("Func_Stmt4");
}
static void Func_Stmt5(MTnode* root){
    Log("Func_Stmt5");
}
static void Func_Stmt6(MTnode* root){
    Log("Func_Stmt6");
}
static void Func_DefList1(MTnode* root){
    Log("Func_DefList1");
}
static void Func_DefList2(MTnode* root){
    Log("Func_DefList2");
}
static void Func_Def(MTnode* root){
    Log("Func_Def");
}
static void Func_DecList1(MTnode* root){
    Log("Func_DecList1");
}
static void Func_DecList2(MTnode* root){
    Log("Func_DecList2");
}
static void Func_Dec1(MTnode* root){
    Log("Func_Dec1");
}
static void Func_Dec2(MTnode* root){
    Log("Func_Dec2");
}
static void Func_Exp1(MTnode* root){
    Log("Func_Exp1");
}
static void Func_Exp2(MTnode* root){
    Log("Func_Exp2");
}
static void Func_Exp3(MTnode* root){
    Log("Func_Exp3");
}
static void Func_Exp4(MTnode* root){
    Log("Func_Exp4");
}
static void Func_Exp5(MTnode* root){
    Log("Func_Exp5");
}
static void Func_Exp6(MTnode* root){
    Log("Func_Exp6");
}
static void Func_Exp7(MTnode* root){
    Log("Func_Exp7");
}
static void Func_Exp8(MTnode* root){
    Log("Func_Exp8");
}
static void Func_Exp9(MTnode* root){
    Log("Func_Exp9");
}
static void Func_Exp10(MTnode* root){
    Log("Func_Exp10");
}
static void Func_Exp11(MTnode* root){
    Log("Func_Exp11");
}
static void Func_Exp12(MTnode* root){
    Log("Func_Exp12");
}
static void Func_Exp13(MTnode* root){
    Log("Func_Exp13");
}
static void Func_Exp14(MTnode* root){
    Log("Func_Exp14");
}
static void Func_Exp15(MTnode* root){
    Log("Func_Exp15");
}
static void Func_Exp16(MTnode* root){
    Log("Func_Exp16");
}
static void Func_Exp17(MTnode* root){
    Log("Func_Exp17");
}
static void Func_Exp18(MTnode* root){
    Log("Func_Exp18");
}
static void Func_Args1(MTnode* root){
    Log("Func_Args1");
}
static void Func_Args2(MTnode* root){
    Log("Func_Args2");
}
static void Func_EMPTY(MTnode* root){
    Log("Func_EMPTY");
}
static void Func_TYPE(MTnode* root){
    Log("Func_TYPE");
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
