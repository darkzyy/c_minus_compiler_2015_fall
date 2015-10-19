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
    Type* tp;
}MTnode;

MTnode* create_node(MTnode* node_list[],int list_len,char* str,YYLTYPE* loc,int type);

void pre_tranverse(MTnode* root);

void add_node(MTnode* par,MTnode* child);


enum{
	TERMINALS=300,
	Program,
    ExtDefList,
    ExtDef1,
    ExtDef2,
    ExtDef3,
    ExtDef4,
    ExtDecList,
    Specifier,
    StructSpecifier,
    OptTag,
    Tag,
    VarDec,
    FunDec,
    VarList,
    ParamDec,
    CompSt,
    StmtList,
    Stmt,
    DefList,
    Def,
    DecList,
    Dec,
    Exp,
    Args,
	EMPTY
};

#endif
