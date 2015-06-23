#ifndef __MULTI_TREE_H__
#define __MULTI_TREE_H__

#include<stdio.h>
#include<stdlib.h>
#include"syntax.tab.h"

typedef struct MTnode{
	int type;
	int children_amount;
	char* str;
	YYLTYPE location;
 	struct MTnode** children_list;
}MTnode;

MTnode* create_node(MTnode* node_list[],int list_len,char* str,YYLTYPE* loc,int type);

void pre_tranverse(MTnode* root);

void add_node(MTnode* par,MTnode* child);


enum{
	TERMINALS=300,
	NONTERM
};

#endif
