#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"multi_tree.h"
#include"syntax.tab.h"

MTnode* create_node(MTnode* node_list[],int list_len,char* str,YYLTYPE* loc,int type){
	MTnode* new_node = malloc(sizeof(MTnode));
	new_node->children_amount = list_len;
	new_node->str = str;
	new_node->location.first_column = loc->first_column;
	new_node->location.first_line = loc->first_line;
	new_node->location.last_column = loc->last_column;
	new_node->location.last_line = loc->last_line;
	new_node->type = type;
	new_node->children_list = node_list;
	new_node->is_const = 0;
    new_node->op = NULL;
    if(type == INT){
        new_node->valt = atoi(str);
	    new_node->is_const = 1;
    }
    else if(type == FLOAT){
        new_node->valf = atof(str);
	    new_node->is_const = 1;
    }
	return new_node;
}

void pre_tranverse2(MTnode* root,int indent){
	int i=0;
	//if(strlen(root->str)>0){
	if(root->type!=EMPTY){
		while(i++<indent){
			printf("  ");
		}
		if(root->type==RELOP){
			printf("RELOP: %s\n",root->str);
		}
		else if(root->type==TYPE){
			printf("TYPE: %s\n",root->str);
		}
		else if(root->type==INT){
			printf("INT: %d\n",root->valt);
		}
		else if(root->type==FLOAT){
			printf("FLOAT: %.6lf\n",root->valf);
		}
		else if(root->type==ID){
			printf("ID: %s\n",root->str);
		}
		else if(root->type>TERMINALS){
			printf("%s (%d)\n",root->str,root->location.first_line);
		}
		else{
			printf("%s\n",root->str);
		}
	}
	for(i=0;i<root->children_amount;++i){
		pre_tranverse2(root->children_list[i],indent+1);
	}
}
void pre_tranverse(MTnode* root){
	pre_tranverse2(root,0);
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

MTnode* get_var_id(MTnode* dec){
    MTnode* vardec = dec->children_list[0];
    while(vardec->type!=ID){
        vardec = vardec->children_list[0];
    }
    return vardec;
}

