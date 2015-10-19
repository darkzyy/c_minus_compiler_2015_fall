//This file is used for semantic analyzation
#include"multi_tree.h"
#include"symtab.h"
#include"cmmtypes.h"
#include"syntax.tab.h"
#include"semantic.h"

int inside_struct;

void init_basic_type(){
    type_int = malloc(sizeof(Type));
    type_float = malloc(sizeof(Type));
    type_int->kind = basic;
    type_int->basic = INT; 
    type_float->kind = basic;
    type_float->basic = FLOAT; 
}

char* get_spec_name(MTnode* spec){  //not tested!!
    if(spec->type!=Specifier1 && spec->type!=Specifier2){
        printf("error refer\n");
        return NULL;
    }
    else if(spec->type==Specifier1){//int & float
        return spec->children_list[0]->str;
    }
    else{
        MTnode* ss = spec->children_list[0];//StructSpecifier
        if(ss->children_amount==2){
            return ss->children_list[1]->children_list[0]->str;
        }
        else{
            MTnode* opttag = ss->children_list[1];
            if(opttag->children_amount==0){
                return NULL;
            }
            else{
                return opttag->children_list[0]->str;
            }
        }
    }
}

/*tranverse the tree 
 * and build the ID table
 * for hash table testing
 */

void sem(MTnode* root){
    switch(root->type){
        case Program:
            {
                sem(root->children_list[0]);
            }
        case ExtDefList:
            {
                sem(root->children_list[0]);
                sem(root->children_list[1]);
            }
        case ExtDef1:
            {
            }
        case Specifier1:
            {
                if(root->type==TYPE){
                }
            }
        default:{
                }
    }
}

