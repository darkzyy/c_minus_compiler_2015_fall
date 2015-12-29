#include <string.h>
#include <stdio.h>
#include "uthash.h"
#include "reg_desc.h"
#include "var_desc.h"
#include "asmprint.h"
#include "debug.h"

reg_desc registers[32];

void reg_desc_init(){
    memset(registers,0,32*sizeof(reg_desc));
}

void add_var_in_reg(int regno, var_desc* vd){
    registers[regno].vd[registers[regno].cur_vd_no] = vd;
    registers[regno].cur_vd_no++;
}

void del_var_in_reg(int regno, var_desc* vd){
    int i;
    for(i=0;i<registers[regno].cur_vd_no;i++){
        if(registers[regno].vd[i] == 0){
            return;
        }
        else if(registers[regno].vd[i] == vd){
            if(registers[regno].cur_vd_no-1 == i){
                registers[regno].vd[i] = 0;
                registers[regno].cur_vd_no -= 1;
            }
            else{
                registers[regno].vd[i] = (void*)-1;
            }
        }
    }
}

void print_reg(){
    int i,j;
    for(i=3;i<=25;i++){
        printf("%s :",reg_str[i]);
        for(j=0;j<registers[i].cur_vd_no;j++){
            if(registers[i].vd[j] == 0){
                break;
            }
            else if(registers[i].vd[j] != (void*) -1){
                printf(" %s",(registers[i].vd[j])->var_str);
            }
        }
        printf("    current vd no : %d\n",registers[i].cur_vd_no);
    }
}
