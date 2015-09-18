#include<stdio.h>
#include<stdlib.h>
#include "./multi_tree.h"

MTnode* create_node(MTnode* node_list[],int list_len,int type){
	MTnode* new_node = malloc(sizeof(MTnode));
	new_node->children_amount = list_len;
	new_node->type = type;
	new_node->children_list = node_list;
	return new_node;
}

void pre_tranverse(MTnode* root){
	int i;
	printf("type:%d \n",root->type);
	for(i=0;i<root->children_amount;++i){
		pre_tranverse(root->children_list[i]);
	}
}

void add_node(MTnode* par,MTnode* child){
	MTnode** new_list = 
		malloc(sizeof(MTnode*)*(par->children_amount+1));
	int i;
	for(i=0;i<par->children_amount;++i){
		new_list[i]=par->children_list[i];
	}
	new_list[i]=child;
	free(par->children_list);
	par->children_list = new_list;
}
