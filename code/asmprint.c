#include<stdio.h>
#include"debug.h"
#include"asmgen.h"
#include"asmprint.h"
#include"api.h"

char* reg_str[32] = {
    "$zero",
    "$at",
    "$v0",
    "$v1",
    "$a0",
    "$a1",
    "$a2",
    "$a3",
    "$t0",
    "$t1",
    "$t2",
    "$t3",
    "$t4",
    "$t5",
    "$t6",
    "$t7",
    "$s0",
    "$s1",
    "$s2",
    "$s3",
    "$s4",
    "$s5",
    "$s6",
    "$s7",
    "$t8",
    "$t9",
    "$k0",
    "$k1",
    "$gp",
    "$sp",
    "$fp",
    "$ra"
};

void print_api(FILE* ff){
    fprintf(ff,"%s%s%s%s%s",data_seg,global_seg,text_seg_start,func_read,func_write);
}

#define pp list_entry(p,mips_inc,list)

void print_asm(){
    ListHead* p = NULL;
    FILE* f = stdout;
    print_api(f);
    list_foreach(p,&asm_head){
        switch(list_entry(p,mips_inc,list)->type){
            case asm_label:
                {
                    fprintf(f,"%s:\n",pp->label);
                    break;
                }
            case asm_li:
                {
                    fprintf(f,"  li %s,%d\n",reg_str[pp->dst],pp->imm);
                    break;
                }
            case asm_move:
                {
                    fprintf(f,"  move %s,%s\n",reg_str[pp->dst],reg_str[pp->op1]);
                    break;
                }
            case asm_addi:
                {
                    fprintf(f,"  addi %s,%s,%d\n",reg_str[pp->dst],reg_str[pp->op1],pp->imm);
                    break;
                }
            case asm_add:
                {
                    fprintf(f,"  add %s,%s,%s\n",reg_str[pp->dst],
                                reg_str[pp->op1],reg_str[pp->op2]);
                    break;
                }
            case asm_sub:
                {
                    fprintf(f,"  sub %s,%s,%s\n",reg_str[pp->dst],
                                reg_str[pp->op1],reg_str[pp->op2]);
                    break;
                }
            case asm_mul:
                {
                    fprintf(f,"  mul %s,%s,%s\n",reg_str[pp->dst],
                                reg_str[pp->op1],reg_str[pp->op2]);
                    break;
                }
            case asm_div:
                {
                    fprintf(f,"  div %s,%s,%s\n",reg_str[pp->dst],
                                reg_str[pp->op1],reg_str[pp->op2]);
                    break;
                }
            case asm_mflo:
                {
                    fprintf(f,"  mflo %s\n",reg_str[pp->dst]);
                    break;
                }
            case asm_lw:
                {
                    fprintf(f,"  lw %s,%d(%s)\n",reg_str[pp->dst],pp->imm,reg_str[pp->op1]);
                    break;
                }
            case asm_sw:
                {
                    fprintf(f,"  sw %s,%d(%s)\n",reg_str[pp->op1],pp->imm,reg_str[pp->dst]);
                    break;
                }
            case asm_j:
                {
                    fprintf(f,"  j %s\n",pp->label);
                    break;
                }
            case asm_jal:
                {
                    fprintf(f,"  jal %s\n",pp->label);
                    break;
                }
            case asm_jr:
                {
                    fprintf(f,"  jr %s\n",reg_str[pp->dst]);
                    break;
                }
            case asm_beq:
                {
                    fprintf(f,"  beq %s,%s,%s\n",reg_str[pp->op1],reg_str[pp->op2],pp->label);
                    break;
                }
            case asm_bne:
                {
                    fprintf(f,"  bne %s,%s,%s\n",reg_str[pp->op1],reg_str[pp->op2],pp->label);
                    break;
                }
            case asm_bgt:
                {
                    fprintf(f,"  bgt %s,%s,%s\n",reg_str[pp->op1],reg_str[pp->op2],pp->label);
                    break;
                }
            case asm_blt:
                {
                    fprintf(f,"  blt %s,%s,%s\n",reg_str[pp->op1],reg_str[pp->op2],pp->label);
                    break;
                }
            case asm_bge:
                {
                    fprintf(f,"  bge %s,%s,%s\n",reg_str[pp->op1],reg_str[pp->op2],pp->label);
                    break;
                }
            case asm_ble:
                {
                    fprintf(f,"  ble %s,%s,%s\n",reg_str[pp->op1],reg_str[pp->op2],pp->label);
                    break;
                }
        }
    }
}

#undef pp
