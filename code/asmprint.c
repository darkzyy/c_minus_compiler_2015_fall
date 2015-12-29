#include<stdio.h>
#include"debug.h"
#include"asmgen.h"
#include"asmprint.h"

#define pp list_entry(p,mips_inc,list)

void print_asm(){
    ListHead* p = NULL;
    Log4();
    FILE* f = stdout;
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
