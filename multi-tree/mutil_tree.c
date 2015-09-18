#include<stdio.h>
#include<stdlib.h>

typedef struct MTnode{
	int type;
	int children_amount;
	int content;
 	struct MTnode** children_list;
}MTnode;

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

int main(){
	MTnode* node0 = create_node(NULL,0,401);
	MTnode* node1 = create_node(NULL,0,402);
	MTnode* node2 = create_node(NULL,0,403);
	MTnode* node3 = create_node(NULL,0,404);
	MTnode* node4 = create_node(NULL,0,405);
	MTnode* node5 = create_node(NULL,0,406);
	MTnode* node6 = create_node(NULL,0,407);
	MTnode* node7 = create_node(NULL,0,408);
	MTnode* node8 = create_node(NULL,0,409);
	MTnode* node9 = create_node(NULL,0,410);
	MTnode** list = malloc(10*sizeof(MTnode*));
	list[0] = node0;
	list[1] = node1;
	list[2] = node2;
	list[3] = node3;
	list[4] = node4;
	list[5] = node5;
	list[6] = node6;
	list[7] = node7;
	list[8] = node8;
	list[9] = node9;
	MTnode* root = create_node(list,10,500);
	pre_tranverse(root);
	return 0;
}
