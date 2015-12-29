#include<string.h>
#include<assert.h>
#include"asmgen.h"
#include"intercode.h"
#include"var_desc.h"
#include"reg_desc.h"
#include"debug.h"
#include"asmprint.h"

#define code_type (list_entry(p,intercode,list)->kind)
#define pp list_entry(p,intercode,list)

char* ret_addr = "cur_ret_addr";
char* tmp_res = "tmp_res";
char* tmp_op1 = "tm1p_oasdp";
char* tmp_op2 = "tmi2p_op";

char* cur_func_name;//save return addr
int cur_offset = -0;// negative, always
int param_amt;
int arg_amt = 0;

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
    vd->is_in_mem = 1;
}

void flush_reg(int regno){
    int i;
    for(i=0;i<registers[regno].cur_vd_no;i++){
        if(registers[regno].vd[i] == 0){
            break;
        }
        else if(registers[regno].vd[i] !=(void*) -1){
            store(registers[regno].vd[i],regno);
            registers[regno].vd[i]->is_in_mem = 1;
            registers[regno].vd[i] = 0;
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
            if((!find_var(tmp_op1) || !find_var(tmp_op1)->in_reg[i]) &&
                        (!find_var(tmp_op2) || !find_var(tmp_op2)->in_reg[i]) &&
                        (!find_var(tmp_res) || !find_var(tmp_res)->in_reg[i]) ){
                flush_reg(i);
                victim = i;
                break;
            }
        }
    }
    find_var(var_str)->in_reg[victim] = 1;
    add_var_in_reg(victim,find_var(var_str));
    /*
    if(enable_debug){
        print_reg();
        print_var();
    }
    */
    Log4("allocated %s for %s",reg_str[victim],var_str);
    return victim;
}

int get_reg(char* var_str){
    //if var is not in reg.alloc a register;
    //else return its register
    if(is_in_reg(var_str) == -1){
        Log4();
        return alloc_reg(var_str);
    }
    else{//in reg
        Log4();
        /*
        if(enable_debug){
            print_reg();
            print_var();
        }
        */
        if(strcmp(var_str,tmp_op2) == 0){
            Log4("fking no : %d",is_in_reg(var_str));
        }
        return is_in_reg(var_str);
    }
}

void save_all(){
    var_desc* vd;
    for(vd=vd_head;vd!=NULL;vd=vd->hh.next){
        if(!vd->is_in_mem){
            mips_inc* mi = insert_new_inc();
            mi->dst = 29;
            mi->type = asm_sw;
            mi->imm = vd->offset;
            mi->op1 = get_reg(vd->var_str);
            dec_reg_contain_var(vd->var_str,mi->op1);
        }
        int i;
        for(i=3;i<=25;i++){
            del_var_in_reg(i,vd);
        }
        vd->is_in_mem = 1;
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
    find_var(tmp_op1)->is_in_mem = 1;
    find_var(tmp_op2)->is_in_mem = 1;
    find_var(tmp_res)->is_in_mem = 1;
}

void pre_process_op1(intercode* ic){
    mips_inc* mi;
    if(ic->use_addr){
        if(ic->op1->kind == OP_INT){
            mi = insert_new_inc();
            mi->type = asm_li;
            mi->dst = alloc_reg(tmp_op1);
            mi->imm = ic->op1->val_int;
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
    }
    else{
        if(ic->op1->kind == OP_INT){
            mi = insert_new_inc();
            mi->type = asm_li;
            mi->dst = alloc_reg(tmp_op1);
            mi->imm = ic->op1->val_int;
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
    }
}

void pre_process_ic(intercode* ic){
    Log4();
    mips_inc* mi;
    if(ic->use_addr){
        if(ic->op1->kind == OP_INT){
            mi = insert_new_inc();
            mi->type = asm_li;
            mi->dst = alloc_reg(tmp_op1);
            mi->imm = ic->op1->val_int;
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
            mi->imm = ic->op2->val_int;
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
            mi->imm = ic->op1->val_int;
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
            mi->imm = ic->op2->val_int;
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

        if(ic->kind!=ICN_IF){
            get_reg(tmp_res);
        }
        /* sth wrong with my brain
        if(ic->kind!=ICN_IF && ic->kind!=ICN_DIV && ic->res->kind == OP_VAR){
            Log4();
            get_reg(tmp_res);
            Log4();
            find_var(tmp_res)->in_reg[get_reg(ic->res->var_str)] = 1;
        }
        else{
            Log4();
            get_reg(tmp_res);
        }
        */
    }
}

void asmgen(){
    /* initiate*/
    reg_desc_init();
    list_init(&asm_head);
    add_var(tmp_op1);
    add_var(tmp_op2);
    add_var(tmp_res);

    ListHead* p = NULL;
    Log4();
    list_foreach(p,(&code_head)){
        switch(code_type){
            case ICN_LABEL:
                {
                    Log4(" ICN_LABEL");
                    mips_inc* mi = insert_new_inc();
                    mi->type = asm_label;
                    mi->label = pp->label;
                    break;
                }
            case ICN_FUNC:
                {
                    Log4(" ICN_FUNC");
                    mips_inc* mi = insert_new_inc();
                    mi->type = asm_label;
                    mi->label = pp->label;
                    cur_func_name = pp->label;
                    add_var(cur_func_name);
                    find_var(cur_func_name)->in_reg[31] = 1;
                    find_var(cur_func_name)->offset = -4; //return addr
                    store(find_var(cur_func_name),31);
                    cur_offset = -4;
                    param_amt = 0;
                    break;
                }
            case ICN_ASSIGN:
                {
                    Log4(" ICN_ASSIGN");
                    pre_process_op1(pp);
                    if(!pp->use_addr && pp->res->kind == OP_ADDR){
                        mips_inc* mi = insert_new_inc();
                        mi->type = asm_sw;
                        mi->op1 = get_reg(tmp_op1);
                        mi->imm = 0;
                        mi->dst = get_reg(pp->res->var_str);
                    }
                    else{
                        if(!find_var(pp->res->var_str)){
                            add_var(pp->res->var_str);
                            cur_offset -= 4;
                            find_var(pp->res->var_str)->offset = cur_offset;
                        }
                        inc_reg_contain_var(pp->res->var_str,get_reg(tmp_op1));
                        add_var_in_reg(get_reg(tmp_op1),find_var(pp->res->var_str));
                        find_var(pp->res->var_str)->is_in_mem = 0;
                    }
                    del_tmpvar_from_reg();
                    break;
                }
            case ICN_PLUS:
                {
                    Log4(" ICN_PLUS");
                    get_reg(tmp_res);
                    pre_process_ic(pp);
                    mips_inc* mi0 = insert_new_inc();
                    mi0->type = asm_add;
                    mi0->dst = get_reg(tmp_res);
                    mi0->op1 = get_reg(tmp_op1);
                    mi0->op2 = get_reg(tmp_op2);

                    if(!pp->use_addr && pp->res->kind == OP_ADDR){
                        mips_inc* mi = insert_new_inc();
                        mi->type = asm_sw;
                        mi->op1 = get_reg(tmp_res);
                        mi->imm = 0;
                        mi->dst = get_reg(pp->res->var_str);
                    }
                    else{
                        if(!find_var(pp->res->var_str)){
                            add_var(pp->res->var_str);
                            cur_offset -= 4;
                            find_var(pp->res->var_str)->offset = cur_offset;
                        }
                        inc_reg_contain_var(pp->res->var_str,get_reg(tmp_res));
                        add_var_in_reg(get_reg(tmp_res),find_var(pp->res->var_str));
                        find_var(pp->res->var_str)->is_in_mem = 0;
                    }

                    del_tmpvar_from_reg();
                    if(!pp->use_addr){
                        find_var(pp->res->var_str)->is_in_mem = 0;
                    }
                    break;
                }
            case ICN_MINUS:
                {
                    Log4(" ICN_MINUS");
                    get_reg(tmp_res);
                    pre_process_ic(pp);
                    mips_inc* mi0 = insert_new_inc();
                    mi0->type = asm_sub;
                    mi0->dst = get_reg(tmp_res);
                    mi0->op1 = get_reg(tmp_op1);
                    mi0->op2 = get_reg(tmp_op2);

                    if(!pp->use_addr && pp->res->kind == OP_ADDR){
                        mips_inc* mi = insert_new_inc();
                        mi->type = asm_sw;
                        mi->op1 = get_reg(tmp_res);
                        mi->imm = 0;
                        mi->dst = get_reg(pp->res->var_str);
                    }
                    else{
                        if(!find_var(pp->res->var_str)){
                            add_var(pp->res->var_str);
                            cur_offset -= 4;
                            find_var(pp->res->var_str)->offset = cur_offset;
                        }
                        inc_reg_contain_var(pp->res->var_str,get_reg(tmp_res));
                        add_var_in_reg(get_reg(tmp_res),find_var(pp->res->var_str));
                        find_var(pp->res->var_str)->is_in_mem = 0;
                    }

                    del_tmpvar_from_reg();
                    if(!pp->use_addr){
                        find_var(pp->res->var_str)->is_in_mem = 0;
                    }
                    break;
                }
            case ICN_MUL:
                {
                    Log4(" ICN_MUL");
                    get_reg(tmp_res);
                    pre_process_ic(pp);
                    mips_inc* mi0 = insert_new_inc();
                    mi0->type = asm_mul;
                    mi0->dst = get_reg(tmp_res);
                    mi0->op1 = get_reg(tmp_op1);
                    mi0->op2 = get_reg(tmp_op2);

                    if(!pp->use_addr && pp->res->kind == OP_ADDR){
                        mips_inc* mi = insert_new_inc();
                        mi->type = asm_sw;
                        mi->op1 = get_reg(tmp_res);
                        mi->imm = 0;
                        mi->dst = get_reg(pp->res->var_str);
                    }
                    else{
                        if(!find_var(pp->res->var_str)){
                            add_var(pp->res->var_str);
                            cur_offset -= 4;
                            find_var(pp->res->var_str)->offset = cur_offset;
                        }
                        inc_reg_contain_var(pp->res->var_str,get_reg(tmp_res));
                        add_var_in_reg(get_reg(tmp_res),find_var(pp->res->var_str));
                        find_var(pp->res->var_str)->is_in_mem = 0;
                    }

                    del_tmpvar_from_reg();
                    if(!pp->use_addr){
                        find_var(pp->res->var_str)->is_in_mem = 0;
                    }
                    break;
                }
            case ICN_DIV:
                {
                    Log4(" ICN_DIV");
                    pre_process_ic(pp);
                    mips_inc* mi0 = insert_new_inc();
                    mi0->type = asm_div;
                    mi0->op1 = get_reg(tmp_op1);
                    mi0->op2 = get_reg(tmp_op2);
                    mips_inc* mi1 = insert_new_inc();
                    mi1->type = asm_mflo;
                    mi1->dst = get_reg(tmp_res);

                    if(!pp->use_addr && pp->res->kind == OP_ADDR){
                        mips_inc* mi = insert_new_inc();
                        mi->type = asm_sw;
                        mi->op1 = get_reg(tmp_res);
                        mi->imm = 0;
                        mi->dst = get_reg(pp->res->var_str);
                    }
                    else{
                        if(!find_var(pp->res->var_str)){
                            add_var(pp->res->var_str);
                            cur_offset -= 4;
                            find_var(pp->res->var_str)->offset = cur_offset;
                        }
                        inc_reg_contain_var(pp->res->var_str,get_reg(tmp_res));
                        add_var_in_reg(get_reg(tmp_res),find_var(pp->res->var_str));
                        find_var(pp->res->var_str)->is_in_mem = 0;
                    }

                    if(!pp->use_addr){
                        find_var(pp->res->var_str)->is_in_mem = 0;
                    }
                    del_tmpvar_from_reg();
                    break;
                }
            case ICN_GOTO:
                {
                    Log4(" ICN_GOTO");
                    save_all();
                    mips_inc* mi = insert_new_inc();
                    mi->type = asm_j;
                    mi->label = pp->label;
                    break;
                }
            case ICN_IF:
                {
                    Log4(" ICN_IF");
                    save_all();
                    pp->use_addr = 0;
                    pre_process_ic(pp);
                    mips_inc* mi = insert_new_inc();
                    mi->label = pp->label;
                    mi->op1 = get_reg(tmp_op1);
                    mi->op2 = get_reg(tmp_op2);
                    if(strcmp(">",pp->relop)==0){
                        mi->type = asm_bgt;
                    }
                    else if(strcmp(">=",pp->relop)==0){
                        mi->type = asm_bge;
                    }
                    else if(strcmp("<",pp->relop)==0){
                        mi->type = asm_blt;
                    }
                    else if(strcmp("<=",pp->relop)==0){
                        mi->type = asm_ble;
                    }
                    else if(strcmp("==",pp->relop)==0){
                        mi->type = asm_beq;
                    }
                    else{ // !=
                        mi->type = asm_bne;
                    }
                    del_tmpvar_from_reg();
                    break;
                }
            case ICN_RETURN:
                {
                    Log4(" ICN_RETURN");
                    if(pp->res->kind == OP_INT){
                        mips_inc* mi0 = insert_new_inc();
                        mi0->type = asm_li;
                        mi0->dst = 2;
                        mi0->imm = pp->res->val_int;
                    }
                    else{
                        mips_inc* mi0 = insert_new_inc();
                        mi0->type = asm_move;
                        mi0->dst = 2;
                        mi0->op1 = get_reg(pp->res->var_str);
                    }

                    if(!find_var(cur_func_name)->in_reg[31]){
                        //check if return val is in 
                        mips_inc* mi1 = insert_new_inc();
                        mi1->type = asm_lw;
                        mi1->dst = 31;
                        mi1->op1 = 29;
                        mi1->imm = find_var(cur_func_name)->offset;
                    }

                    /*
                    mips_inc* mi2 = insert_new_inc();
                    mi2->op1 = 29;
                    mi2->dst = 29;
                    mi2->imm = -4*(param_amt); //put stack pointer back
                    mi2->type = asm_addi;
                    */
                    mips_inc* mi3 = insert_new_inc();
                    mi3->type = asm_jr;
                    mi3->dst = 31;
                    break;
                }
            case ICN_DEC:
                {
                    Log4(" ICN_DEC");
                    add_var(pp->res->var_str);
                    cur_offset -= pp->size;
                    find_var(pp->res->var_str)->offset = cur_offset;
                    find_var(pp->res->var_str)->is_in_mem = 1;
                    break;
                }
            case ICN_ARG:
                {
                    Log4(" ICN_ARG");
                    arg_amt+=1;
                    cur_offset -= 4;
                    /*ARG can not be deref !*/
                    if(pp->res->kind == OP_INT){
                        mips_inc* mi = insert_new_inc();
                        mi->dst = get_reg(tmp_op1);
                        mi->imm = pp->res->val_int;
                        mi->type = asm_li;

                        mi = insert_new_inc();
                        mi->dst = 29;
                        mi->imm = cur_offset;
                        mi->op1 = get_reg(tmp_op1);
                        mi->type = asm_sw;
                    }
                    else{
                        mips_inc* mi = insert_new_inc();
                        if(is_in_reg(pp->res->var_str) == -1){
                            mi->type = asm_lw;
                            mi->dst = alloc_reg(pp->res->var_str);
                            mi->op1 = 29; //$sp
                            mi->imm = find_var(pp->res->var_str)->offset;

                            mi = insert_new_inc();
                            mi->dst = 29;
                            mi->imm = cur_offset;
                            mi->op1 = get_reg(pp->res->var_str);
                            mi->type = asm_sw;
                        }
                        else{
                            mi->dst = 29;
                            mi->imm = cur_offset;
                            mi->op1 = get_reg(pp->res->var_str);
                            mi->type = asm_sw;
                        }
                    }
                    del_tmpvar_from_reg();
                    break;
                }
            case ICN_ARG_ADDR:
                {
                    Log4(" ICN_ARG_ADDR");
                    assert(0);
                    break;
                }
            case ICN_CALL:
                {
                    Log4(" ICN_CALL");
                    save_all();
                    mips_inc* mi = insert_new_inc();
                    /* new stack */
                    mi->type = asm_addi;
                    mi->dst = 29;
                    mi->op1 = 29;
                    mi->imm = cur_offset;
                    /* jal */
                    mi = insert_new_inc();
                    mi->type = asm_jal;
                    mi->label = pp->label;
                    /* stack previous */
                    mi = insert_new_inc();
                    mi->type = asm_addi;
                    mi->dst = 29;
                    mi->op1 = 29;
                    mi->imm = -cur_offset;
                    cur_offset += 4*arg_amt;
                    arg_amt = 0;
                    /* move */
                    if(!find_var(pp->res->var_str)){
                        add_var(pp->res->var_str);
                        cur_offset -= 4;
                        find_var(pp->res->var_str)->offset = cur_offset;
                    }
                    int res_reg = get_reg(pp->res->var_str);
                    add_var_in_reg(res_reg,find_var(pp->res->var_str));
                    inc_reg_contain_var(pp->res->var_str,res_reg);
                    mi = insert_new_inc();
                    mi->type = asm_move;
                    mi->dst = res_reg;
                    mi->op1 = 2;
                    break;
                }
            case ICN_PARAM:
                {
                    Log4(" ICN_PARAM");
                    add_var(pp->res->var_str);
                    find_var(pp->res->var_str)->offset = param_amt * 4;
                    find_var(pp->res->var_str)->is_in_mem = 1;
                    param_amt += 1;
                    break;
                }
            case ICN_READ:
                {
                    Log4(" ICN_READ");
                    save_all();
                    /* jal */
                    mips_inc* mi = insert_new_inc();
                    mi->type = asm_jal;
                    mi->label = pp->label;
                    
                    /* move */
                    if(!find_var(pp->res->var_str)){
                        add_var(pp->res->var_str);
                        cur_offset -= 4;
                        find_var(pp->res->var_str)->offset = cur_offset;
                    }
                    int res_reg = get_reg(pp->res->var_str);
                    add_var_in_reg(res_reg,find_var(pp->res->var_str));
                    inc_reg_contain_var(pp->res->var_str,res_reg);
                    mi = insert_new_inc();
                    mi->type = asm_move;
                    mi->dst = res_reg;
                    mi->op1 = 2;
                    break;
                }
            case ICN_WRITE:
                {
                    Log4(" ICN_WRITE");
                    /*ARG can not be deref !*/
                    save_all();
                    /* move */
                    if(pp->res->kind == OP_INT){
                        mips_inc* mi = insert_new_inc();
                        mi->dst = 4;
                        mi->imm = pp->res->val_int;
                        mi->type = asm_li;
                    }
                    else{
                        mips_inc* mi = insert_new_inc();
                        if(is_in_reg(pp->res->var_str) == -1){
                            mi->type = asm_lw;
                            mi->dst = 4;
                            mi->op1 = 29; //$sp
                            mi->imm = find_var(pp->res->var_str)->offset;
                        }
                        else{
                            /*move*/
                            mi->type = asm_move;
                            mi->dst = 4;
                            mi->dst = get_reg(pp->res->var_str);
                        }
                    }

                    /* jal */
                    mips_inc* mi = insert_new_inc();
                    mi->type = asm_jal;
                    mi->label = pp->label;
                    break;
                }
        }
    }
    Log4();
}

#undef pp
#undef code_type
