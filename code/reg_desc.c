#include <string.h>
#include "uthash.h"
#include "reg_desc.h"

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
    for(i=0;i<100;i++){
        if(registers[regno].vd[i] == 0){
            return;
        }
        else if(registers[regno].vd[i] == vd){
            registers[regno].vd[i] = (void*)-1;
            if(registers[regno].cur_vd_no == i){
                registers[regno].cur_vd_no -= 1;
            }
        }
    }
}
