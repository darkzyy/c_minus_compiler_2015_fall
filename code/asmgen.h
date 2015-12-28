#ifndef __ASMGEN_H__
#define __ASMGEN_H__

#include "list.h"

enum {
    asm_label=0,
    asm_li,
    asm_move,
    asm_addi,
    asm_add,
    asm_sub,
    asm_mul,
    asm_div,
    asm_mflo,
    asm_lw,
    asm_sw,
    asm_j,
    asm_jal,
    asm_jr,
    asm_beq,
    asm_bne,
    asm_bgt,
    asm_blt,
    asm_bge,
    asm_ble
};

struct mips_inc{
    int type;
    int dst;//register
    int op1,op2;
    char* label;
    ListHead list;
};
typedef struct mips_inc mips_inc;

ListHead* asm_head;







#endif
