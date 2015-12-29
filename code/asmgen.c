#include<string.h>
#include<assert.h>
#include"asmgen.h"
#include"intercode.h"
#include"var_desc.h"
#include"reg_desc.h"

#define code_type (list_entry(p,intercode,list)->kind)
#define pp list_entry(p,intercode,list)

char* ret_addr = "cur_ret_addr";
char* tmp_res = "tmp_res";
char* tmp_op1 = "tmp_op1";
char* tmp_op2 = "tmp_op2";
char* cur_func_name;//save return addr
int cur_offset = -0;// negative, always
int param_amt;

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

void save_all(){
}

int is_empty(int regno){
    int i;
    for(i=0;i<registers[regno].cur_vd_no;i++){
        if(registers[regno].vd[i] == 0){
            return 1;
        }
        else if(registers[regno].vd[i] != (void*)-1){
            return 0;
        }
    }
    return 1;
}

int is_in_reg(char* var_str){
    int* a = find_var(var_str)->in_reg;
    int i;
    for(i=3;i<25;i++){
        if(a[i]){
            return i;
        }
    }
    return -1;
}

void store(var_desc* vd, int regno){
    mips_inc* mi = insert_new_inc();
    mi->type = asm_sw;
    mi->op1 = regno;
    mi->dst = 29;//%sp
    mi->imm = vd->offset;
}

void flush_reg(int regno){
    int i;
    for(i=0;i<100;i++){
        if(registers[regno].vd[i] == 0){
            break;
        }
        else if(registers[regno].vd[i] !=(void*) -1){
            store(registers[regno].vd[i],regno);
            registers[regno].vd[i] = 0;
            registers[regno].vd[i]->is_in_mem = 1;
        }
    }
    registers[regno].cur_vd_no = 0;
}

int alloc_reg(char* var_str){
    //find a register for var
    int i;
    int victim = -1;
    for(i=3;i<=25;i++){
        if(is_empty(i)){
            victim = i;
            break;
        }
    }
    if(victim == -1){
        for(i=3;i<=25;i++){
            if(!find_var(tmp_op1)->in_reg[i] &&
                        !find_var(tmp_op1)->in_reg[i] &&
                        !find_var(tmp_res)->in_reg[i] ){
                flush_reg(i);
                victim = i;
                break;
            }
        }
    }
    find_var(var_str)->in_reg[victim] = 1;
    return 0;
}

int get_reg(char* var_str){
    //if var is not in reg.alloc a register;
    //else return its register
    if(is_in_reg(var_str) == -1){
        return alloc_reg(var_str);
    }
    else{//in reg
        return is_in_reg(var_str);
    }
}

void del_tmpvar_from_reg(){
    int i;
    for(i=3;i<=25;i++){
        del_var_in_reg(i,find_var(tmp_op1));
        del_var_in_reg(i,find_var(tmp_op2));
        del_var_in_reg(i,find_var(tmp_res));
    }
    memset(find_var(tmp_op1)->in_reg,0,32*sizeof(int));
    memset(find_var(tmp_op2)->in_reg,0,32*sizeof(int));
    memset(find_var(tmp_res)->in_reg,0,32*sizeof(int));
}

void pre_process_ic(intercode* ic){
    del_tmpvar_from_reg();
    mips_inc* mi;
    if(ic->use_addr){
        if(ic->op1->kind == OP_INT){
            mi = insert_new_inc();
            mi->type = asm_li;
            mi->dst = alloc_reg(tmp_op1);
        }
        else if(ic->op1->kind == OP_VAR && ic->op1->var_str[0] == 'v'){
            //get ic->op1's addr
            mi = insert_new_inc();
            mi->type = asm_addi;
            mi->dst = alloc_reg(tmp_op1);
            mi->op1 = 29; //$sp
            mi->imm = find_var(ic->op1->var_str)->offset;
        }
        else{
            find_var(tmp_op1)->in_reg[get_reg(ic->op1->var_str)] = 1;
        }

        if(ic->op2->kind == OP_INT){
            mi = insert_new_inc();
            mi->type = asm_li;
            mi->dst = alloc_reg(tmp_op2);
        }
        else if(ic->op2->kind == OP_VAR && ic->op2->var_str[0] == 'v'){
            //get ic->op2's addr
            mi = insert_new_inc();
            mi->type = asm_addi;
            mi->dst = alloc_reg(tmp_op2);
            mi->op1 = 29; //$sp
            mi->imm = find_var(ic->op2->var_str)->offset;
        }
        else{
            find_var(tmp_op2)->in_reg[get_reg(ic->op2->var_str)] = 1;
        }

        find_var(tmp_res)->in_reg[get_reg(ic->res->var_str)] = 1;
    }
    else{ // use_val
        if(ic->op1->kind == OP_INT){
            mi = insert_new_inc();
            mi->type = asm_li;
            mi->dst = alloc_reg(tmp_op1);
        }
        else if(ic->op1->kind == OP_ADDR ){
            mi = insert_new_inc();
            mi->type = asm_lw;
            mi->dst = alloc_reg(tmp_op1);
            mi->op1 = 29; //$sp
            mi->imm = find_var(ic->op1->var_str)->offset;
        }
        else{
            find_var(tmp_op1)->in_reg[get_reg(ic->op1->var_str)] = 1;
        }

        if(ic->op2->kind == OP_INT){
            mi = insert_new_inc();
            mi->type = asm_li;
            mi->dst = alloc_reg(tmp_op2);
        }
        else if(ic->op2->kind == OP_ADDR ){
            mi = insert_new_inc();
            mi->type = asm_lw;
            mi->dst = alloc_reg(tmp_op2);
            mi->op1 = 29; //$sp
            mi->imm = find_var(ic->op2->var_str)->offset;
        }
        else{
            find_var(tmp_op2)->in_reg[get_reg(ic->op2->var_str)] = 1;
        }

        if(ic->res->kind == OP_VAR){
            find_var(tmp_res)->in_reg[get_reg(ic->res->var_str)] = 1;
        }
        else{
            get_reg(tmp_res);
        }
    }
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
                    cur_func_name = pp->label;
                    find_var(cur_func_name)->in_reg[31] = 1;
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
                    mips_inc* mi0 = insert_new_inc();
                    mi0->type = asm_add;
                    pre_process_ic(pp);
                    mi0->dst = get_reg(tmp_res);
                    mi0->op1 = get_reg(pp->res->var_str);
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
                    mips_inc* mi0 = insert_new_inc();
                    mi0->type = asm_move;
                    mi0->dst = 2;
                    mi0->op1 = get_reg(pp->res->var_str);

                    if(!find_var(cur_func_name)->in_reg[31]){
                        //check if return val is in 
                        mips_inc* mi1 = insert_new_inc();
                        mi1->type = asm_lw;
                        mi1->dst = 31;
                        mi1->op1 = 29;
                        mi1->imm = find_var(cur_func_name)->offset;
                    }

                    mips_inc* mi2 = insert_new_inc();
                    mi2->op1 = 29;
                    mi2->imm = -4*(param_amt); //put stack pointer back

                    mips_inc* mi3 = insert_new_inc();
                    mi3->type = asm_jal;
                    break;
                }
            case ICN_DEC:
                {
                    add_var(pp->res->var_str);
                    cur_offset -= pp->size;
                    find_var(pp->res->var_str)->offset = cur_offset;
                    break;
                }
            case ICN_ARG:
                {
                    break;
                }
            case ICN_ARG_ADDR:
                {
                    assert(0);
                    break;
                }
            case ICN_CALL:
                {
                    break;
                }
            case ICN_PARAM:
                {
                    add_var(pp->res->var_str);
                    find_var(pp->res->var_str)->offset = param_amt * 4;
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
