//This file is used for semantic analyzation
#include<string.h>
#include"multi_tree.h"
#include"symtab.h"
#include"cmmtypes.h"
#include"syntax.tab.h"
#include"semantic.h"
#include"debug.h"

//#define __ARRAY_WIDTH_CMP__

int inside_struct;
int inside_func_para;
int inside_func_compst;
int func_dec;
int func_def;
int global;

void init_sem(){
    inside_struct = 0;
    inside_func_para = 0;
    inside_func_compst = 0;
    func_dec=0;
    func_def=0;
    global = 0;
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
    if(type_cmp(alx->type,aly->type)!=0){
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

Type* field_find(FieldList* fl,char* id){
    if(fl==NULL){
        return NULL;
    }
    else{
        if(strcmp(id,fl->name)==0){
            return fl->type;
        }
        else{
            return field_find(fl->next,id);
        }
    }
}

/*tranverse the tree 
 * and build the ID table
 * for hash table testing
 */

typedef void (*fk)(MTnode*);
extern fk func_table[60];

void sem(MTnode* root){
    if(root->type == TYPE)
        func_table[59](root);
    else
        func_table[root->type - Program](root);
}

void Func_Program(MTnode* root)
{
    Log("Program");
    sem(ch(0));
}
void Func_ExtDefList1(MTnode* root)
{
    Log("ExtDefList1");
    sem(ch(0));
    sem(ch(1));
}
void Func_ExtDefList2(MTnode* root)
{
    Log("ExtDefList2");
}
void Func_ExtDef1(MTnode* root)
{
    Log("ExtDef1");
    sem(ch(0));
    ch(1)->inh_type = 
        chst(0);//ExtDecList
    global = 1;
    sem(ch(1));
    global = 0;
}
void Func_ExtDef2(MTnode* root)
{
    Log("ExtDef2");
    sem(ch(0));
}
void Func_ExtDef3(MTnode* root)
{
    Log("ExtDef3: Func Def");
    sem(ch(0));
    ch(1)->inh_type = 
        chst(0);
    ch(2)->inh_type = 
        chst(0);
    ch(2)->inh_ctrl = 1;

    func_def = 1;
    sem(ch(1));

    inside_func_compst = 1;
    sem(ch(2));
    inside_func_compst = 0;
    func_def = 0;
}
void Func_ExtDef4(MTnode* root)
{
    Log("ExtDef4");
    sem(ch(0));
    ch(1)->inh_type = 
        chst(0);
    func_dec = 1;
    sem(ch(1));
    func_dec = 0;
}
void Func_ExtDecList1(MTnode* root)
{
    Log("ExtDecList1");
    ch(0)->inh_type = root->inh_type;
    sem(ch(0));
}
void Func_ExtDecList2(MTnode* root)
{
    Log("ExtDecList2");
    ch(0)->inh_type = root->inh_type;
    sem(ch(0));
    ch(2)->inh_type = root->inh_type;
    sem(ch(2));
}
void Func_Specifier1(MTnode* root)
{
    Log("Specifier1");
    sem(ch(0));//basic
    root->syn_type = chst(0);
}
void Func_Specifier2(MTnode* root)
{
    Log("Specifier2");
    sem(ch(0));//struct
    root->syn_type = chst(0);
}
void Func_StructSpecifier1(MTnode* root)
{
    Log("StructSpecifier1");
    inside_struct += 1;
    sem(ch(1));//OptTag
    sem(ch(3));//DefList
    root->syn_type = malloc(sizeof(Type));
    root->syn_type->kind = structure;
    root->syn_type->fl = ch(3)->syn_fl;
    if(ch(1)->type == OptTag){
        //add this struct to symtab
        char* struct_id = ch(1)->str;
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
}
void Func_StructSpecifier2(MTnode* root)
{
    Log("StructSpecifier2");
    sem(ch(1));//Tag
    root->syn_type = chst(1);
}
void Func_OptTag(MTnode* root)
{
    Log("OptTag");
    MTnode* struct_id = ch(0);
    symbol* s=find_sym(&struct_tab,struct_id->str);
    if(s!=NULL){
        printf("Error type 16 at Line %d: Duplicated name \"%s\".\n",
                    struct_id->location.first_line,struct_id->str);
    }
    root->str = ch(0)->str;
    //trying: auto upload id
}
void Func_Tag(MTnode* root)
{
    Log("Tag");
    MTnode* struct_id = ch(0);
    symbol* s=find_sym(&struct_tab,struct_id->str);
    if(s==NULL){
        printf("Error type 17 at Line %d: Undefined structure \"%s\".\n",
                    struct_id->location.first_line,struct_id->str);
    }
    root->syn_type = (s==NULL)? NULL : s->val_type;
}
void Func_VarDec1(MTnode* root)
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
        s = find_sym(&var_tab,ch(0)->str);
        if(s!=NULL){
            err=1;
            printf("Error type 15 at Line %d: field's is same as a var \"%s\".\n",
                        var_id->location.first_line,var_id->str);
        }
        root->syn_type = root->inh_type;
        if(err){
        }
        else{//new field -> field_tab
            symbol* var_sym = malloc(sizeof(symbol));
            var_sym->dim = root->inh_dim;
            var_sym->id_name = var_id->str;
            var_sym->val_type = root->syn_type;
            add_sym_node(&field_tab,var_sym);
            Log("#======------added field %s------======#",var_sym->id_name);
        }
    }
    else if(global||func_def){
        int err = 0;
        MTnode* var_id = get_var_id(root);
        symbol* s = find_sym(&field_tab,var_id->str);
        if(s!=NULL){
            err=1;
            printf("Error type 3 at Line %d: Redefined var \"%s\".\n",
                        var_id->location.first_line,var_id->str);
        }
        s = find_sym(&var_tab,ch(0)->str);
        if(s!=NULL){
            err=1;
            printf("Error type 3 at Line %d: Redefined var \"%s\".\n",
                        var_id->location.first_line,var_id->str);
        }
        s = find_sym(&struct_tab,ch(0)->str);
        if(s!=NULL){
            err=1;
            printf("Error type 3 at Line %d: Redefined var \"%s\".\n",
                        var_id->location.first_line,var_id->str);
        }
        root->syn_type = root->inh_type;
        if(err){
            Log("___________errr_________");
        }
        else{//new field -> var_tab
            symbol* var_sym = malloc(sizeof(symbol));
            var_sym->dim = root->inh_dim;
            var_sym->id_name = var_id->str;
            var_sym->val_type = root->syn_type;
            add_sym_node(&var_tab,var_sym);
            if(root->syn_type == type_error){
                Log("Add errer var");
            }
            Log("#======------added global var %s------======#",var_sym->id_name);
        }
    }
    else if(func_dec){
        root->syn_type = root->inh_type;
    }
}
void Func_VarDec2(MTnode* root)
{
    Log("VarDec2");
    if(inside_struct){
        int err = 0;
        MTnode* var_id = get_var_id(root);
        symbol* s = find_sym(&field_tab,var_id->str);
        if(s!=NULL){
            err=1;
            printf("Error type 15 at Line %d: Redefined field \"%s\".\n",
                        var_id->location.first_line,var_id->str);
        }
        s = find_sym(&var_tab,ch(0)->str);
        if(s!=NULL){
            err=1;
            printf("Error type 15 at Line %d: field's is same as a var \"%s\".\n",
                        var_id->location.first_line,var_id->str);
        }
        if(err){
            root->syn_type = type_error;
        }
        else{
            MTnode* main_part = ch(0);
            main_part->inh_type = root->inh_type;
            main_part->inh_dim = root->inh_dim+1;
            sem(main_part);
            root->syn_type = malloc(sizeof(Type));
            root->syn_type->kind = array;
            root->syn_type->array.elem = main_part->syn_type;
            root->syn_type->array.size = ch(2)->valt;
            Log("find array dim:%d",root->inh_dim);
            //find array's id, and change id's type 
            MTnode* var_id = get_var_id(root);
            symbol* s = find_sym(&field_tab,var_id->str);
            s->val_type = root->syn_type;
        }
    }
    else if(global||func_def){
        int err = 0;
        MTnode* var_id = get_var_id(root);
        symbol* s = find_sym(&field_tab,var_id->str);
        if(s!=NULL){
            err=1;
            printf("Error type 3 at Line %d: Redefined var \"%s\".\n",
                        var_id->location.first_line,var_id->str);
        }
        s = find_sym(&var_tab,ch(0)->str);
        if(s!=NULL){
            err=1;
            printf("Error type 3 at Line %d: Redefined var \"%s\".\n",
                        var_id->location.first_line,var_id->str);
        }
        s = find_sym(&struct_tab,ch(0)->str);
        if(s!=NULL){
            err=1;
            printf("Error type 3 at Line %d: Redefined var \"%s\".\n",
                        var_id->location.first_line,var_id->str);
        }
        if(err){
            root->syn_type = type_error;
        }
        else{
            MTnode* main_part = ch(0);
            main_part->inh_type = root->inh_type;
            main_part->inh_dim = root->inh_dim+1;
            sem(main_part);
            root->syn_type = malloc(sizeof(Type));
            root->syn_type->kind = array;
            root->syn_type->array.elem = main_part->syn_type;
            root->syn_type->array.size = ch(2)->valt;
            Log("find global array dim:%d",root->inh_dim);
            //find array's id, and change id's type 
            MTnode* var_id = get_var_id(root);
            symbol* s = find_sym(&var_tab,var_id->str);
            s->val_type = root->syn_type;
        }
    }
    else if(func_dec){
        root->syn_type = root->inh_type;
        MTnode* main_part = ch(0);
        main_part->inh_type = root->inh_type;
        main_part->inh_dim = root->inh_dim+1;
        sem(main_part);
        root->syn_type = malloc(sizeof(Type));
        root->syn_type->kind = array;
        root->syn_type->array.elem = main_part->syn_type;
        root->syn_type->array.size = ch(2)->valt;
    }
}
void Func_FunDec1(MTnode* root)
{
    Log("FunDec1");
    MTnode* func_id = get_var_id(root);
    if(inside_func_compst){
        printf("Error type 20 at Line %d: Func in Func \"%s\".\n",
                    func_id->location.first_line,func_id->str);
    }
    if(inside_struct){
        printf("Error type 20 at Line %d: Func in Struct \"%s\".\n",
                    func_id->location.first_line,func_id->str);
    }
    symbol* s = find_sym(&func_tab,func_id->str);
    if(s!=NULL && func_def && s->def_ed==1){ //redefine
        printf("Error type 4 at Line %d: Func Redefined \"%s\".\n",
                    func_id->location.first_line,func_id->str);
    }
    if(s!=NULL && func_dec && (s->dec_ed==1||s->def_ed==1)){ 
        //defined or declared,and redeclaring now
        if(type_cmp(s->val_type,root->inh_type)!=0){
            printf("Error type 19 at Line %d: Inconsistent declaration\
                        of function \"%s\".\n",func_id->location.first_line,func_id->str);
        }
    }
    MTnode* varl = ch(2);
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
        if(func_dec){
            s->line = locl;
        }
        add_sym_node(&func_tab,s);
        Log("#======------added func %s------======#",func_id->str);
    }
    else{ 
        if(arg_cmp(varl->syn_al,s->func_arg)!=0){
            printf("Error type 19 at Line %d: Inconsistent declaration\
                        of function \"%s\".\n",func_id->location.first_line,func_id->str);
        }
        al_free(varl->syn_al);
        s->dec_ed = func_dec;
        s->def_ed = func_def;
    }
}
void Func_FunDec2(MTnode* root)
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
        if(s->argamt != 0 || type_cmp(s->val_type,root->inh_type)){
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
        if(func_dec){
            s->line = locl;
        }
        s->func_arg = NULL;
        Log("#======------adding func %s------======#",func_id->str);
        add_sym_node(&func_tab,s);
        Log("#======------added func %s------======#",func_id->str);
    }
    else{
        s->dec_ed = func_dec;
        s->def_ed = func_def;
    }
}
void Func_VarList1(MTnode* root)
{
    Log("VarList1");
    sem(ch(0));
    sem(ch(2));
    root->syn_al = malloc(sizeof(ArgList));
    root->syn_al->next = ch(2)->syn_al;
    root->syn_al->type = chst(0);
}
void Func_VarList2(MTnode* root)
{
    Log("VarList2");
    sem(ch(0));
    root->syn_al = malloc(sizeof(ArgList));
    root->syn_al->next = NULL;
    root->syn_al->type = chst(0);
}
void Func_ParamDec(MTnode* root)
{
    Log("ParamDec");
    sem(ch(0));
    ch(1)->inh_type = chst(0);
    sem(ch(1));
    root->syn_type = chst(1);
}
void Func_CompSt(MTnode* root)
{
    Log("CompSt, Contrl:%d",root->inh_ctrl);
    if(ch(2)->type != StmtList2){
        ch(2)->inh_type = root->inh_type; //control
        ch(2)->inh_ctrl = root->inh_ctrl;
    }
    else if(root->inh_ctrl != 0){
        printf("Warning: Control reaches end at Line %d\n",locl);
    }
    sem(ch(1));
    sem(ch(2));
}
void Func_StmtList1(MTnode* root)
{
    Log("StmtList1,Control: %d",root->inh_ctrl);
    if(ch(1)->type == StmtList2){
        ch(0)->inh_ctrl = root->inh_ctrl;
    }
    else{
        ch(0)->inh_ctrl = 0;
    }
    ch(0)->inh_type = root->inh_type;
    ch(1)->inh_type = root->inh_type;
    ch(1)->inh_ctrl = root->inh_ctrl;
    sem(ch(0));
    sem(ch(1));
}
void Func_StmtList2(MTnode* root)
{
    Log("StmtList2");
}
void Func_Stmt1(MTnode* root)
{
    Log("Stmt1");
    if(root->inh_ctrl==1){
        printf("Warning: Control reaches end at Line %d\n",locl);
    }
    sem(ch(0));
}
void Func_Stmt2(MTnode* root)
{
    Log("Stmt2");
    ch(0)->inh_type = root->inh_type;
    ch(0)->inh_ctrl = root->inh_ctrl;
    sem(ch(0));
}
void Func_Stmt3(MTnode* root)
{
    Log("Stmt3");
    sem(ch(1));
    if(type_cmp(root->inh_type,chst(1))!=0){
        printf("Error type 8 at Line %d: Type mismatched for return.\n",locl);
    }
}
void Func_Stmt4(MTnode* root)
{
    Log("Stmt4");
    if(root->inh_ctrl==1){
        printf("Warning: Control might reach end at Line %d\n",locl);
    }
    ch(4)->inh_type = root->inh_type;
    ch(4)->inh_ctrl = root->inh_ctrl;
    sem(ch(2));
    if(type_cmp(chst(2),type_int)!=0){
        printf("Error type 7 at Line %d: condition val is not integer.\n",locl);
    }
    sem(ch(4));
}
void Func_Stmt5(MTnode* root)
{
    Log("Stmt5");
    ch(4)->inh_type = root->inh_type;
    ch(4)->inh_ctrl = root->inh_ctrl;
    ch(6)->inh_type = root->inh_type;
    ch(6)->inh_ctrl = root->inh_ctrl;
    sem(ch(2));
    if(type_cmp(chst(2),type_int)!=0){
        printf("Error type 7 at Line %d: condition val is not integer.\n",locl);
    }
    sem(ch(4));
    sem(ch(6));
}
void Func_Stmt6(MTnode* root)
{
    Log("Stmt6");
    if(root->inh_ctrl==1){
        printf("Warning: Control might reach end at Line %d\n",locl);
    }
    ch(4)->inh_type = root->inh_type;
    ch(4)->inh_ctrl = root->inh_ctrl;
    sem(ch(2));
    if(type_cmp(chst(2),type_int)!=0){
        printf("Error type 7 at Line %d: condition val is not integer.\n",locl);
    }
    sem(ch(4));
}
void Func_DefList1(MTnode* root)
{
    Log("DefList1");
    if(inside_struct){
        MTnode* def = ch(0);
        MTnode* defl = ch(1);
        sem(def);
        sem(defl);
        root->syn_fl = def->syn_fl;
        if(defl->syn_fl!=NULL){
            def->syn_fl->next = defl->syn_fl;
            def->syn_fl->tail = defl->syn_fl->tail;
        }
    }
    else{
        sem(ch(0));
        sem(ch(1));
    }
}
void Func_DefList2(MTnode* root)
{
    Log("DefList2");
    root->syn_fl=NULL;
}
void Func_Def(MTnode* root)
{
    Log("Def");
    if(inside_struct){
        MTnode* spec = ch(0);
        MTnode* decl = ch(1);
        sem(spec);
        decl->inh_type = spec->syn_type;
        sem(decl);
        root->syn_fl = decl->syn_fl;
    }
    else{
        MTnode* spec = ch(0);
        MTnode* decl = ch(1);
        sem(spec);
        decl->inh_type = spec->syn_type;
        sem(decl);
    }
}
void Func_DecList1(MTnode* root)
{
    Log("DecList1");
    if(inside_struct){
        MTnode* dec = ch(0);
        dec->inh_type = root->inh_type;
        sem(dec);
        root->syn_fl = dec->syn_fl;
    }
    else{
        ch(0)->inh_type = root->inh_type;
        sem(ch(0));
    }
}
void Func_DecList2(MTnode* root)
{
    Log("DecList2");
    if(inside_struct){
        MTnode* dec = ch(0);
        MTnode* decl = ch(2);
        dec->inh_type = root->inh_type;
        decl->inh_type = root->inh_type;
        sem(dec);
        sem(decl);
        root->syn_fl = dec->syn_fl;
        dec->syn_fl->next = decl->syn_fl;
        dec->syn_fl->tail = decl->syn_fl->tail;
    }
    else{
        ch(0)->inh_type = root->inh_type;
        ch(2)->inh_type = root->inh_type;
        sem(ch(0));
        sem(ch(2));
    }
}
void Func_Dec1(MTnode* root)
{
    Log("Dec1");
    if(inside_struct){
        MTnode* vardec = ch(0);
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
    else{
        MTnode* vardec = ch(0);
        vardec->inh_type = root->inh_type;
        vardec->inh_dim = 0;
        sem(vardec);
    }
}
void Func_Dec2(MTnode* root)
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
    else{
        MTnode* vardec = ch(0);
        vardec->inh_type = root->inh_type;
        vardec->inh_dim = 0;
        sem(vardec);
        sem(ch(2));
        if(type_cmp(vardec->syn_type,chst(2))!=0){
            printf("Error type 5 at Line %d: Type mismatched for assignment.\n",locl);
        }
    }
}
void Func_TYPE(MTnode* root)
{
    Log("TYPE");
    if(strcmp(root->str,"int")==0){
        Log("TYPE : int");
        root->syn_type=type_int;
    }
    else{
        Log("TYPE : float");
        root->syn_type=type_float;
    }
}
void Func_Exp1(MTnode* root)
{
    Log("Exp1");
    if(ch(0)->type!=Exp16 &&
                ch(0)->type!=Exp14 &&
                ch(0)->type!=Exp15){
        printf("Error type 6 at Line %d: The left_hand side of an assignment must be a varia_ble.\n",
                    locl);
    }
    sem(ch(0));
    sem(ch(2));
    if(type_cmp(chst(0),chst(2))!=0){
        printf("Error type 5 at Line %d: Type mismatched for assignment.\n",
                    locl);
    }
    root->syn_type = chst(2);
}
void Func_Exp2(MTnode* root)
{
    Log("Exp2");
    sem(ch(0));
    sem(ch(2));
    if(chst(0)!=type_int||chst(2)!=type_int){
        printf("Error type 7 at Line %d: Type mismatched for AND.\n",locl);
    }
    root->syn_type = type_int;
}
void Func_Exp3(MTnode* root)
{
    Log("Exp3");
    sem(ch(0));
    sem(ch(2));
    if(chst(0)!=type_int||chst(2)!=type_int){
        printf("Error type 7 at Line %d: Type mismatched for OR.\n",locl);
    }
    root->syn_type = type_int;
}
void Func_Exp4(MTnode* root)
{
    Log("Exp4");
    sem(ch(0));
    sem(ch(2));
    if(chst(0)!=chst(2)||(chst(0)!=type_int&&chst(0)!=type_float)){
        printf("Error type 7 at Line %d: Type mismatched for RELOP.\n",locl);
    }
    root->syn_type = type_int;
}
void Func_Exp5(MTnode* root)
{
    Log("Exp5");
    sem(ch(0));
    sem(ch(2));
    if(chst(0)!=chst(2)||(chst(0)!=type_int&&chst(0)!=type_float)){
        printf("Error type 7 at Line %d: Type mismatched for PLUS.\n",locl);
    }
    root->syn_type = chst(0);
}
void Func_Exp6(MTnode* root)
{
    Log("Exp6");
    sem(ch(0));
    sem(ch(2));
    if(chst(0)!=chst(2)||(chst(0)!=type_int&&chst(0)!=type_float)){
        printf("Error type 7 at Line %d: Type mismatched for MINUS.\n",locl);
    }
    root->syn_type = chst(0);
}
void Func_Exp7(MTnode* root)
{
    Log("Exp7");
    sem(ch(0));
    sem(ch(2));
    if(chst(0)!=chst(2)||(chst(0)!=type_int&&chst(0)!=type_float)){
        printf("Error type 7 at Line %d: Type mismatched for MUL.\n",locl);
    }
    root->syn_type = chst(0);
}
void Func_Exp8(MTnode* root)
{
    Log("Exp8");
    sem(ch(0));
    sem(ch(2));
    if(chst(0)!=chst(2)||(chst(0)!=type_int&&chst(0)!=type_float)){
        printf("Error type 7 at Line %d: Type mismatched for DIV.\n",locl);
    }
    root->syn_type = chst(0);
}
void Func_Exp9(MTnode* root)
{
    Log("Exp9");
    sem(ch(1));
    root->syn_type = chst(1);
}
void Func_Exp10(MTnode* root)
{
    Log("Exp10");
    sem(ch(1));
    if(chst(1)!=type_int&&chst(1)!=type_float){
        printf("Error type 7 at Line %d: Type mismatched for NEG.\n",locl);
    }
    root->syn_type = chst(1);
}
void Func_Exp11(MTnode* root)
{
    Log("Exp11");
    sem(ch(1));
    if(chst(1)!=type_int){
        printf("Error type 7 at Line %d: Type mismatched for NOT.\n",locl);
    }
    root->syn_type = chst(1);
}
void Func_Exp12(MTnode* root)
{
    Log("Exp12");
    symbol* s = find_sym(&func_tab,ch(0)->str);
    if(s==NULL){
        printf("Error type 2 at Line %d: Undefined function \"%s\".\n",
                    locl,ch(0)->str);
        root->syn_type = type_error;
    }
    else{
        sem(ch(2));
        if(arg_cmp(s->func_arg,ch(2)->syn_al)==0){
            root->syn_type = s->val_type;
        }
        else{
            printf("Error type 9 at Line %d: Function \"%s\" is not applicable\
                        for arguments provided here.\n",locl,s->id_name);
            root->syn_type = type_error;
        }
    }
}
void Func_Exp13(MTnode* root)
{
    Log("Exp13");
    symbol* s = find_sym(&func_tab,ch(0)->str);
    if(s==NULL){
        printf("Error type 2 at Line %d: Undefined function \"%s\".\n",
                    locl,ch(0)->str);
        root->syn_type = type_error;
    }
    else if(s->argamt==0){
        root->syn_type = s->val_type;
    }
    else{
        printf("Error type 9 at Line %d: Function \"%s\" is not applicable\
                    for arguments provided here.\n",locl,s->id_name);
        root->syn_type = type_error;
    }
}
void Func_Exp14(MTnode* root)
{
    Log("Exp14");
    sem(ch(0));
    sem(ch(2));
    if(chst(0)->kind != array){
        printf("Error type 10 at Line %d: use'[]'after non-array exp.\n",locl);
        root->syn_type = type_error;
    }
    else if(chst(2) != type_int){
        printf("Error type 12 at Line %d: use'[]'around non-int exp.\n",locl);
        root->syn_type = type_error;
    }
    else{
        root->syn_type = chst(0)->array.elem;
    }
}
void Func_Exp15(MTnode* root)
{
    Log("Exp15");
    sem(ch(0));
    if(chst(0)->kind != structure){
        printf("Error type 13 at Line %d: Illegal use of \".\".\n",locl);
        root->syn_type = type_error;
    }
    else {
        Type* t = field_find(chst(0)->fl,ch(2)->str);
        if(t==NULL){
            printf("Error type 14 at Line %d: Non_existent field \"%s\".\n",
                        locl,ch(2)->str);
            root->syn_type = type_error;
        }
        else{
            root->syn_type = t;
        }
    }
}
void Func_Exp16(MTnode* root)
{
    Log("Exp16");
    symbol* s = find_sym(&var_tab,ch(0)->str);
    if(s==NULL){
        printf("Error type 1 at Line %d: Undefined variable \"%s\".\n",
                    locl,ch(0)->str);
        root->syn_type = type_error;
    }
    else{
        root->syn_type = s->val_type;
    }
}
void Func_Exp17(MTnode* root)
{
    Log("Exp17");
    root->syn_type = type_int;
}
void Func_Exp18(MTnode* root)
{
    Log("Exp18");
    root->syn_type = type_float;
}
void Func_Args1(MTnode* root)
{
    Log("Args1");
    sem(ch(0));
    sem(ch(2));
    root->syn_al = malloc(sizeof(ArgList));
    root->syn_al->next = ch(2)->syn_al;
    root->syn_al->type = chst(0);
}
void Func_Args2(MTnode* root)
{
    Log("Args2");
    sem(ch(0));
    root->syn_al = malloc(sizeof(ArgList));
    root->syn_al->next = NULL;
    root->syn_al->type = chst(0);
}
void Func_EMPTY(MTnode* root)
{
    Log("empty");
}

fk func_table[60] = {
    Func_Program ,
    Func_ExtDefList1 ,
    Func_ExtDefList2 ,
    Func_ExtDef1 ,
    Func_ExtDef2 ,
    Func_ExtDef3 ,
    Func_ExtDef4 ,
    Func_ExtDecList1 ,
    Func_ExtDecList2 ,
    Func_Specifier1 ,
    Func_Specifier2 ,
    Func_StructSpecifier1 ,
    Func_StructSpecifier2 ,
    Func_OptTag ,
    Func_Tag ,
    Func_VarDec1 ,
    Func_VarDec2 ,
    Func_FunDec1 ,
    Func_FunDec2 ,
    Func_VarList1 ,
    Func_VarList2 ,
    Func_ParamDec ,
    Func_CompSt ,
    Func_StmtList1 ,
    Func_StmtList2 ,
    Func_Stmt1 ,
    Func_Stmt2 ,
    Func_Stmt3 ,
    Func_Stmt4 ,
    Func_Stmt5 ,
    Func_Stmt6 ,
    Func_DefList1 ,
    Func_DefList2 ,
    Func_Def ,
    Func_DecList1 ,
    Func_DecList2 ,
    Func_Dec1 ,
    Func_Dec2 ,
    Func_Exp1 ,
    Func_Exp2 ,
    Func_Exp3 ,
    Func_Exp4 ,
    Func_Exp5 ,
    Func_Exp6 ,
    Func_Exp7 ,
    Func_Exp8 ,
    Func_Exp9 ,
    Func_Exp10 ,
    Func_Exp11 ,
    Func_Exp12 ,
    Func_Exp13 ,
    Func_Exp14 ,
    Func_Exp15 ,
    Func_Exp16 ,
    Func_Exp17 ,
    Func_Exp18 ,
    Func_Args1 ,
    Func_Args2 ,
    Func_EMPTY ,
    Func_TYPE
};
