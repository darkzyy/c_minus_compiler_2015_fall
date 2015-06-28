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
	YYLTYPE location;
	struct MTnode** children_list;
    Type* tp;
}MTnode;

MTnode* create_node(MTnode* node_list[],int list_len,char* str,YYLTYPE* loc,int type);

void pre_tranverse(MTnode* root);

void add_node(MTnode* par,MTnode* child);


enum{
	TERMINALS=300,
	Program=301,
    ExtDefList=302,
    ExtDef=303,
    ExtDecList=304,
    Specifier=305,
    StructSpecifier=306,
    OptTag=307,
    Tag=308,
    VarDec=309,
    FunDec=310,
    VarList=311,
    ParamDec=312,
    CompSt=313,
    StmtList=314,
    Stmt=315,
    DefList=316,
    Def=317,
    DecList=318,
    Dec=319,
    Exp=320,
    Args=321,
    ExtDec=322,
	EMPTY=323
};

#endif
