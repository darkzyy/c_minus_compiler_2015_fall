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
                break;
            }
        case ExtDefList:
            {
                sem(root->children_list[0]);
                sem(root->children_list[1]);
                break;
            }
        case ExtDef1:
            {
                sem(root->children_list[0]);
                root->children_list[1]->inh_type = 
                    root->children_list[0]->syn_type;//ExtDecList
                sem(root->children_list[1]);
                break;
            }
        case ExtDef2:
            {
                sem(root->children_list[0]);
                break;
            }
        case ExtDef3: // func definition
            {
                sem(root->children_list[0]);
                root->children_list[1]->inh_type = 
                    root->children_list[0]->syn_type;//func def
                sem(root->children_list[1]);
                break;
            }
        case ExtDef4: // func delaration !!
            {
                sem(root->children_list[0]);
                sem(root->children_list[1]);
                break;
            }
        case ExtDecList1:
            {
                root->children_list[0]->inh_type =
                    root->inh_type;
                sem(root->children_list[0]);
                break;
            }
        case ExtDecList2:
            {
                root->children_list[0]->inh_type =
                    root->inh_type;
                sem(root->children_list[0]);
                root->children_list[2]->inh_type =
                    root->inh_type;
                sem(root->children_list[2]);
                break;
            }
        case Specifier1:
            {
                sem(root->children_list[0]);//basic
                break;
            }
        case Specifier2:
            {
                sem(root->children_list[0]);//struct
                break;
            }
        case StructSpecifier1:
            {
                sem(root->children_list[1]);//OptTag
                sem(root->children_list[3]);//DefList
                root->syn_type = malloc(sizeof(Type));
                root->syn_type->kind = structure;
                root->syn_type->fl = 
                    root->children_list[3]->syn_fl;
                break;
            }
        case StructSpecifier2:
            {
                sem(root->children_list[1]);//Tag
                break;
            }
        case OptTag:
            {
                MTnode* struct_id = root->children_list[0];
                symbol* s=find_sym(&struct_tab,struct_id->str);
                if(s!=NULL){
                    printf("Error type 16 at Line %d: Duplicated name \"%s\".\n",
                                struct_id->location.first_line,struct_id->str);
                }
                break;
            }
        case TYPE:
            {
                if(strcmp(root->str,"int")==0){
                    root->syn_type=type_int;
                }
                else{
                    root->syn_type=type_float;
                }
                break;

            }
        default:{
                }
    }
}

