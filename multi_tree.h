#ifndef __MULTI_TREE_H__
#define __MULTI_TREE_H__

#include<stdio.h>
#include<stdlib.h>
#include"syntax.tab.h"
#include"cmmtypes.h"

typedef struct MTnode{
	int type;
	int children_amount;
	char* str;
    union {
        int valt;
        double valf;
    };
    Type* inh_type;
    Type* syn_type;
    FieldList* syn_fl;
	YYLTYPE location;
	struct MTnode** children_list;
}MTnode;

MTnode* create_node(MTnode* node_list[],int list_len,char* str,YYLTYPE* loc,int type);

void pre_tranverse(MTnode* root);

void add_node(MTnode* par,MTnode* child);


enum{
	TERMINALS=300,
	Program,
    ExtDefList1,
    ExtDefList2,
    ExtDef1,
    ExtDef2,
    ExtDef3,
    ExtDef4,
    ExtDecList1,
    ExtDecList2,
    Specifier1,
    Specifier2,
    StructSpecifier1,
    StructSpecifier2,
    OptTag,
    Tag,
    VarDec1,
    VarDec2,
    FunDec1,
    FunDec2,
    VarList1,
    VarList2,
    ParamDec,
    CompSt,
    StmtList1,
    StmtList2,
    Stmt1,
    Stmt2,
    Stmt3,
    Stmt4,
    Stmt5,
    Stmt6,
    DefList1,
    DefList2,
    Def,
    DecList1,
    DecList2,
    Dec1,
    Dec2,
    Exp1,
    Exp2,
    Exp3,
    Exp4,
    Exp5,
    Exp6,
    Exp7,
    Exp8,
    Exp9,
    Exp10,
    Exp11,
    Exp12,
    Exp13,
    Exp14,
    Exp15,
    Exp16,
    Exp17,
    Exp18,
    Args1,
    Args2,
	EMPTY
};

#endif
