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
        add_sym_name(&var_tab,root->str);
    }
    switch(root->type){
        case OptTag:
            {
                if(root->children_amount!=0){ // OptTag is not empty
                    MTnode* struct_id = root->children_list[0];
                    symbol* s=find_sym(&struct_tab,struct_id->str);
                    if(s!=NULL){
                        printf("Error type 16 at Line %d: Duplicated name \"%s\".\n",
                                    struct_id->location.first_line,struct_id->str);
                    }
                    else{
                        add_sym_name(&struct_tab,struct_id->str);
                    }
                }
                break;
            }
        default:{
                }
    }
    int i;
    for(i=0;i<root->children_amount;i++){
        build_table(root->children_list[i]);
    }
}

