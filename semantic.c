//This file is used for semantic analyzation
#include"multi_tree.h"
#include"symtab.h"
#include"cmmtypes.h"
#include"syntax.tab.h"
#include"semantic.h"
#include"debug.h"

int inside_struct;
int inside_func_para;
int inside_func_compst;
int func_dec;
int func_def;
int global;

void init_basic_type(){
    type_int = malloc(sizeof(Type));
    type_int->kind = basic;
    type_int->basic = INT; 
    type_float = malloc(sizeof(Type));
    type_float->kind = basic;
    type_float->basic = FLOAT; 
    type_error = malloc(sizeof(Type));
    type_float->kind = semantic_error;
    inside_struct = 0;
    inside_func_para = 0;
    inside_func_compst = 0;
    func_dec=0;
    func_def=0;
    global = 1;
}

MTnode* get_var_id(MTnode* dec){
    MTnode* vardec = dec->children_list[0];
    while(vardec->type!=ID){
        vardec = vardec->children_list[0];
    }
    return vardec;
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

int argamt_count(ArgList* al){
    int count = 0;
    while(al!=NULL){
        count++;
        al = al->next;
    }
    return count;
}
int arg_cmp(ArgList* alx,ArgList* aly){
    if(alx==NULL||aly==NULL){
        if(alx==NULL&&aly==NULL){
            return 0;
        }
        else{
            return 1;
        }
    }
    if(alx->type != aly->type){
        return 1;
    }
    else{
        return arg_cmp(alx->next,aly->next);
    }
}
void al_free(ArgList* al){
    if(al!=NULL){
        al_free(al->next);
        free(al);
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
                Log("Program");
                sem(root->children_list[0]);
                break;
            }
        case ExtDefList1:
            {
                Log("ExtDefList1");
                sem(root->children_list[0]);
                sem(root->children_list[1]);
                break;
            }
        case ExtDefList2:
            {
                Log("ExtDefList2");
                break;
            }
        case ExtDef1:
            {
                Log("ExtDef1");
                sem(root->children_list[0]);
                root->children_list[1]->inh_type = 
                    root->children_list[0]->syn_type;//ExtDecList
                global = 1;
                sem(root->children_list[1]);
                global = 0;
                break;
            }
        case ExtDef2:
            {
                Log("ExtDef2");
                sem(root->children_list[0]);
                break;
            }
        case ExtDef3: // func definition
            {
                Log("ExtDef3: Func Def");
                sem(root->children_list[0]);
                root->children_list[1]->inh_type = 
                    root->children_list[0]->syn_type;
                root->children_list[2]->inh_type = 
                    root->children_list[0]->syn_type;

                func_def = 1;
                sem(root->children_list[1]);
                func_def = 0;

                inside_func_compst = 1;
                sem(root->children_list[2]);
                inside_func_compst = 0;
                break;
            }
        case ExtDef4: // func delaration !!
            {
                Log("ExtDef4");
                sem(root->children_list[0]);
                root->children_list[1]->inh_type = 
                    root->children_list[0]->syn_type;
                func_dec = 1;
                sem(root->children_list[1]);
                func_dec = 0;
                break;
            }
        case ExtDecList1:
            {
                Log("ExtDecList1");
                root->children_list[0]->inh_type = root->inh_type;
                sem(root->children_list[0]);
                break;
            }
        case ExtDecList2:
            {
                Log("ExtDecList2");
                root->children_list[0]->inh_type = root->inh_type;
                sem(root->children_list[0]);
                root->children_list[2]->inh_type = root->inh_type;
                sem(root->children_list[2]);
                break;
            }
        case Specifier1:
            {
                Log("Specifier1");
                sem(root->children_list[0]);//basic
                root->syn_type = root->children_list[0]->syn_type;
                break;
            }
        case Specifier2:
            {
                Log("Specifier2");
                sem(root->children_list[0]);//struct
                root->syn_type = root->children_list[0]->syn_type;
                break;
            }
        case StructSpecifier1:
            {
                Log("StructSpecifier1");
                inside_struct += 1;
                sem(root->children_list[1]);//OptTag
                sem(root->children_list[3]);//DefList
                root->syn_type = malloc(sizeof(Type));
                root->syn_type->kind = structure;
                root->syn_type->fl = root->children_list[3]->syn_fl;
                if(root->children_list[1]->type == OptTag){
                    //add this struct to symtab
                    char* struct_id = root->children_list[1]->str;
                    symbol* struct_sym = malloc(sizeof(symbol));
                    struct_sym->dim = 0;
                    struct_sym->id_name = struct_id;
                    struct_sym->val_type = root->syn_type;
                    add_sym_node(&struct_tab,struct_sym);
                    Log("#======------added struct %s------======#",struct_id);
                }
                if(inside_struct>0){
                    inside_struct -= 1;
                }
                break;
            }
        case StructSpecifier2:
            {
                Log("StructSpecifier2");
                sem(root->children_list[1]);//Tag
                root->syn_type = root->children_list[1]->syn_type;
                break;
            }
        case OptTag:
            {
                Log("OptTag");
                MTnode* struct_id = root->children_list[0];
                symbol* s=find_sym(&struct_tab,struct_id->str);
                if(s!=NULL){
                    printf("Error type 16 at Line %d: Duplicated name \"%s\".\n",
                                struct_id->location.first_line,struct_id->str);
                }
                root->str = root->children_list[0]->str;
                //trying: auto upload id
                break;
            }
        case Tag:
            {
                Log("Tag");
                MTnode* struct_id = root->children_list[0];
                symbol* s=find_sym(&struct_tab,struct_id->str);
                if(s==NULL){
                    printf("Error type 17 at Line %d: Undefined structure \"%s\".\n",
                                struct_id->location.first_line,struct_id->str);
                }
                root->syn_type = (s==NULL)? NULL : s->val_type;
                break;
            }
        case VarDec1:
            {
                Log("VarDec1");
                if(inside_struct){
                    int err = 0;
                    MTnode* var_id = get_var_id(root);
                    symbol* s = find_sym(&field_tab,var_id->str);
                    if(s!=NULL){
                        err=1;
                        printf("Error type 15 at Line %d: Redefined field \"%s\".\n",
                                    var_id->location.first_line,var_id->str);
                    }
                    s = find_sym(&var_tab,root->children_list[0]->str);
                    if(s!=NULL){
                        err=1;
                        printf("Error type 15 at Line %d: field's is same as a var \"%s\".\n",
                                    var_id->location.first_line,var_id->str);
                    }
                    root->syn_type = root->inh_type;
                    if(err){
                        break;
                    }
                    else{//new field -> field_tab
                        symbol* var_sym = malloc(sizeof(symbol));
                        var_sym->dim = root->inh_dim;
                        var_sym->id_name = var_id->str;
                        var_sym->val_type = root->syn_type;
                        add_sym_node(&field_tab,var_sym);
                        Log("#======------added field %s------======#",var_sym->id_name);
                        break;
                    }
                }
                else if(global){
                    int err = 0;
                    MTnode* var_id = get_var_id(root);
                    symbol* s = find_sym(&field_tab,var_id->str);
                    if(s!=NULL){
                        err=1;
                        printf("Error type 3 at Line %d: Redefined var \"%s\".\n",
                                    var_id->location.first_line,var_id->str);
                    }
                    s = find_sym(&var_tab,root->children_list[0]->str);
                    if(s!=NULL){
                        err=1;
                        printf("Error type 3 at Line %d: Redefined var \"%s\".\n",
                                    var_id->location.first_line,var_id->str);
                    }
                    s = find_sym(&struct_tab,root->children_list[0]->str);
                    if(s!=NULL){
                        err=1;
                        printf("Error type 3 at Line %d: Redefined var \"%s\".\n",
                                    var_id->location.first_line,var_id->str);
                    }
                    root->syn_type = root->inh_type;
                    if(err){
                        break;
                    }
                    else{//new field -> field_tab
                        symbol* var_sym = malloc(sizeof(symbol));
                        var_sym->dim = root->inh_dim;
                        var_sym->id_name = var_id->str;
                        var_sym->val_type = root->syn_type;
                        add_sym_node(&var_tab,var_sym);
                        Log("#======------added global var %s------======#",var_sym->id_name);
                        break;
                    }
                }
                break;
            }
        case VarDec2:
            {
                Log("DefDec2");
                if(inside_struct){
                    int err = 0;
                    MTnode* var_id = get_var_id(root);
                    symbol* s = find_sym(&field_tab,var_id->str);
                    if(s!=NULL){
                        err=1;
                        printf("Error type 15 at Line %d: Redefined field \"%s\".\n",
                                    var_id->location.first_line,var_id->str);
                    }
                    s = find_sym(&var_tab,root->children_list[0]->str);
                    if(s!=NULL){
                        err=1;
                        printf("Error type 15 at Line %d: field's is same as a var \"%s\".\n",
                                    var_id->location.first_line,var_id->str);
                    }
                    if(err){
                        root->syn_type = type_error;
                        break;
                    }
                    else{
                        MTnode* main_part = root->children_list[0];
                        main_part->inh_type = root->inh_type;
                        main_part->inh_dim = root->inh_dim+1;
                        sem(main_part);
                        root->syn_type = malloc(sizeof(Type));
                        root->syn_type->kind = array;
                        root->syn_type->array.elem = main_part->syn_type;
                        root->syn_type->array.size = root->children_list[2]->valt;
                        Log("find array dim:%d",root->inh_dim);
                    }
                }
                else if(global){
                    int err = 0;
                    MTnode* var_id = get_var_id(root);
                    symbol* s = find_sym(&field_tab,var_id->str);
                    if(s!=NULL){
                        err=1;
                        printf("Error type 3 at Line %d: Redefined var \"%s\".\n",
                                    var_id->location.first_line,var_id->str);
                    }
                    s = find_sym(&var_tab,root->children_list[0]->str);
                    if(s!=NULL){
                        err=1;
                        printf("Error type 3 at Line %d: Redefined var \"%s\".\n",
                                    var_id->location.first_line,var_id->str);
                    }
                    s = find_sym(&struct_tab,root->children_list[0]->str);
                    if(s!=NULL){
                        err=1;
                        printf("Error type 3 at Line %d: Redefined var \"%s\".\n",
                                    var_id->location.first_line,var_id->str);
                    }
                    if(err){
                        root->syn_type = type_error;
                        break;
                    }
                    else{
                        MTnode* main_part = root->children_list[0];
                        main_part->inh_type = root->inh_type;
                        main_part->inh_dim = root->inh_dim+1;
                        sem(main_part);
                        root->syn_type = malloc(sizeof(Type));
                        root->syn_type->kind = array;
                        root->syn_type->array.elem = main_part->syn_type;
                        root->syn_type->array.size = root->children_list[2]->valt;
                        Log("find global array dim:%d",root->inh_dim);
                    }
                }
                break;
            }
        case FunDec1:
            {
                Log("FunDec1");
                MTnode* func_id = get_var_id(root);
                if(inside_func_compst){
                    printf("Error type 20 at Line %d: Func in Func \"%s\".\n",
                                func_id->location.first_line,func_id->str);
                    break;
                }
                if(inside_struct){
                    printf("Error type 20 at Line %d: Func in Struct \"%s\".\n",
                                func_id->location.first_line,func_id->str);
                    break;
                }
                symbol* s = find_sym(&func_tab,func_id->str);
                if(s!=NULL && func_def && s->def_ed==1){ //redefine
                    printf("Error type 4 at Line %d: Func Redefined \"%s\".\n",
                                func_id->location.first_line,func_id->str);
                    break;
                }
                if(s!=NULL && func_dec && (s->dec_ed==1||s->def_ed==1)){ 
                    //defined or declared,and redeclaring now
                    if(s->argamt != 0 || s->val_type != root->inh_type){
                        printf("Error type 19 at Line %d: Inconsistent declaration\
 of function \"%s\".\n",func_id->location.first_line,func_id->str);
                        break;
                    }
                }
                MTnode* varl = root->children_list[2];
                sem(varl);
                if(s==NULL){
                    s = malloc(sizeof(symbol));
                    s->id_name = func_id->str;
                    s->val_type = root->inh_type;
                    s->func_arg = varl->syn_al;
                    s->argamt=argamt_count(varl->syn_al);
                    s->property = 0;
                    s->dec_ed = func_dec;
                    s->def_ed = func_def;
                    add_sym_node(&func_tab,s);
                    Log("#======------added func %s------======#",func_id->str);
                }
                else{ 
                    if(arg_cmp(varl->syn_al,s->func_arg)!=0){
                        printf("Error type 19 at Line %d: Inconsistent declaration\
 of function \"%s\".\n",func_id->location.first_line,func_id->str);
                        break;
                    }
                    al_free(varl->syn_al);
                }
            }
        case FunDec2:
            {//no arguement
                Log("FunDec2");
                int err = 0;
                MTnode* func_id = get_var_id(root);
                if(inside_func_compst){
                    err = 1;
                    printf("Error type 20 at Line %d: Func in Func \"%s\".\n",
                                func_id->location.first_line,func_id->str);
                }
                if(inside_struct){
                    err = 1;
                    printf("Error type 20 at Line %d: Func in Struct \"%s\".\n",
                                func_id->location.first_line,func_id->str);
                }
                symbol* s = find_sym(&func_tab,func_id->str);
                if(s!=NULL && func_def && s->def_ed==1){ //redefine
                    err = 1;
                    printf("Error type 4 at Line %d: Func Redefined \"%s\".\n",
                                func_id->location.first_line,func_id->str);
                }
                if(s!=NULL && func_dec && (s->dec_ed==1||s->def_ed==1)){ 
                    //defined or declared,and redeclaring now
                    if(s->argamt != 0 || s->val_type != root->inh_type){
                        printf("Error type 19 at Line %d: Inconsistent declaration\
 of function \"%s\".\n",func_id->location.first_line,func_id->str);
                    }
                }
                if((!err)&&s==NULL){ // not defined or declared  , func_id -> symtab
                    s = malloc(sizeof(symbol));
                    s->id_name = func_id->str;
                    s->val_type = root->inh_type;
                    s->argamt=0;
                    s->property = 0;
                    s->dec_ed = func_dec;
                    s->def_ed = func_def;
                    s->func_arg = NULL;
                    add_sym_node(&func_tab,s);
                    Log("#======------added func %s------======#",func_id->str);
                }
                break;
            }
        case DefList1:
            {
                Log("DefList1");
                if(inside_struct){
                    MTnode* def = root->children_list[0];
                    MTnode* defl = root->children_list[1];
                    sem(def);
                    sem(defl);
                    root->syn_fl = def->syn_fl;
                    if(defl->syn_fl!=NULL){
                        def->syn_fl->next = defl->syn_fl;
                        def->syn_fl->tail = defl->syn_fl->tail;
                    }
                }
                break;
            }
        case DefList2:
            {
                Log("DefList2");
                root->syn_fl=NULL;
                break;
            }
        case Def:
            {
                Log("Def");
                if(inside_struct){
                    MTnode* spec = root->children_list[0];
                    MTnode* decl = root->children_list[1];
                    sem(spec);
                    decl->inh_type = spec->syn_type;
                    sem(decl);
                    root->syn_fl = decl->syn_fl;
                }
                break;
            }
        case DecList1:
            {
                Log("DecList1");
                if(inside_struct){
                    MTnode* dec = root->children_list[0];
                    dec->inh_type = root->inh_type;
                    sem(dec);
                    root->syn_fl = dec->syn_fl;
                }
                break;
            }
        case DecList2:
            {
                Log("DecList2");
                if(inside_struct){
                    MTnode* dec = root->children_list[0];
                    MTnode* decl = root->children_list[2];
                    dec->inh_type = root->inh_type;
                    decl->inh_type = root->inh_type;
                    sem(dec);
                    sem(decl);
                    root->syn_fl = dec->syn_fl;
                    dec->syn_fl->next = decl->syn_fl;
                    dec->syn_fl->tail = decl->syn_fl->tail;
                }
                break;
            }
        case Dec1:
            {
                Log("Dec1");
                if(inside_struct){
                    MTnode* vardec = root->children_list[0];
                    vardec->inh_type = root->inh_type;
                    vardec->inh_dim = 0;
                    sem(vardec);
                    root->syn_fl = malloc(sizeof(FieldList));
                    root->syn_fl->type = vardec->syn_type;
                    //require vardec modify its syn_type
                    MTnode* dec_id = get_var_id(root);
                    root->syn_fl->name = dec_id->str;
                    root->syn_fl->next = NULL;
                    root->syn_fl->tail = NULL;
                }
                break;
            }
        case Dec2:
            {
                Log("Dec2");
                if(inside_struct){
                    printf("Error type 15 at Line %d: Init Struct Field .",
                                root->location.first_line);
                    root->syn_fl = malloc(sizeof(FieldList));
                    root->syn_fl->type = type_error;
                    MTnode* dec_id = get_var_id(root);
                    root->syn_fl->name = dec_id->str;
                    root->syn_fl->next = NULL;
                    root->syn_fl->tail = NULL;
                }
                break;
            }
        case TYPE:
            {
                Log("TYPE");
                if(strcmp(root->str,"int")==0){
                    root->syn_type=type_int;
                }
                else{
                    root->syn_type=type_float;
                }
                break;

            }
        case EMPTY:
            {
                Log("empty");
                break;
            }
        default:
            {
                Log("Default : %d",root->type);
                break;
            }
    }
}
