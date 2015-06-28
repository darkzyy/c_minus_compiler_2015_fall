//This file is used for semantic analyzation
#include"multi_tree.h"
#include"symtab.h"
#include"cmmtypes.h"
#include"syntax.tab.h"

void init_basic_type(){
    type_int = malloc(sizeof(type));
    type_float = malloc(sizeof(type));
    type_int->kind = basic;
    type_int->basic = INT; 
    type_float->kind = basic;
    type_float->basic = FLOAT; 
}

char* get_spec_name(MTnode* spec){  //not tested!!
    if(spec->type!=Specifier){
        printf("error refer\n");
        return NULL;
    }
    else if(spec->children_list[0]->type==TYPE){//int & float
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

FieldList* add_field(MTnode* root){
    if(spec->type!=DefList){
        printf("error refer\n");
        return NULL;
    }
    if(root->children_amount=0){
        return NULL;
    }
    else{
        fl = malloc(sizeof(FieldList));
        fl->name = get_spec_name( root->children_list[0]->children_list[0]);
        //          Def         Specifier
        fl->type = addtype(root->children_list[0]);
        fl->next = add_field(root->children_list[1]);
        return fl;
    }
}

Type* add_type(MTnode* root){
    switch(root->type){
        case StructSpecifier:
            {
                if(root->children_amount==5){
                    Type* tp = malloc(sizeof(Type));
                    tp->kind=structure;
                    fl=add_type(root->children_list[3]);
                    return tp;
                }
                else{
                    char* struct_name = root->children_list[1]->children_list[0]->str;
                    //              Tag                 ID  
                    symbol* s;
                    s = find_sym(&struct_tab,struct_name);
                    return s->val_type;
                }
            }
        case Def:
            {
                return add_type(root->children_list[0]);
            }
        case Specifier:
            {
                return add_type(root->children_list[0]);
            }
        case TYPE:
            {
                if(strcmp(root->str,"int")==0){
                    return type_int;
                }
                else{
                    return type_float;
                }
            }
    }
}

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

