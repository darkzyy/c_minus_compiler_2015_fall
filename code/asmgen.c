#include<string.h>
#include"asmgen.h"
#include"intercode.h"

#define code_type (list_entry(p,intercode,list)->kind)
#define pp list_entry(p,intercode,list)

char* ret_addr = "cur_ret_addr";
int cur_offset = -0;// negative, always
int param_amt;

void save_all(){
}

void process_block(){
}

mips_inc* new_inc(){
    mips_inc* mi = malloc(sizeof(mips_inc));
    memset(mi,0,sizeof(mips_inc));
    return mi;
}

mips_inc* insert_new_inc(){
    mips_inc* mi = new_inc();
    list_add_before(&asm_head,&(mi->list));
    return mi;
}


void asmgen(){
    ListHead* p = NULL;
    list_foreach(p,(&code_head)){
        switch(code_type){
            case ICN_LABEL:
                {
                    mips_inc* mi = insert_new_inc();
                    mi->type = asm_label;
                    mi->label = pp->label;
                    break;
                }
            case ICN_FUNC:
                {
                    find_var(ret_addr)->in_reg[31] = 1;
                    cur_offset = -0;
                    param_amt = 0;
                    break;
                }
            case ICN_ASSIGN:
                {
                    break;
                }
            case ICN_PLUS:
                {
                    break;
                }
            case ICN_MINUS:
                {
                    break;
                }
            case ICN_MUL:
                {
                    break;
                }
            case ICN_DIV:
                {
                    break;
                }
            case ICN_GOTO:
                {
                    break;
                }
            case ICN_IF:
                {
                    break;
                }
            case ICN_RETURN:
                {
                    if(find_var(ret_addr)->in_reg[31]){
                        mips_inc* mi = insert_new_inc();
                        mi->type
                    }
                    break;
                }
            case ICN_DEC:
                {
                    break;
                }
            case ICN_ARG:
                {
                    break;
                }
            case ICN_ARG_ADDR:
                {
                    break;
                }
            case ICN_CALL:
                {
                    break;
                }
            case ICN_PARAM:
                {
                    param_amt += 1;
                    break;
                }
            case ICN_READ:
                {
                    break;
                }
            case ICN_WRITE:
                {
                    break;
                }
        }
    }
}

#undef pp
#undef code_type
