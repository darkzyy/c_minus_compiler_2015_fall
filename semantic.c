//This file is used for semantic analyzation
#include"multi_tree.h"
#include"symtab.h"
#include"cmmtypes.h"

/*tranverse the tree 
 * and build the ID table
 * for hash table testing
 */
void build_table(MTnode* root){
    if(root->type == ID){
        add_sym_name(root->str);
    }
    int i;
    for(i=0;i<root->children_amount;i++){
        build_table(root->children_list[i]);
    }
}

