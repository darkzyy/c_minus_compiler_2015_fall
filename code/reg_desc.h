#ifndef __REG_DESC_H__
#define __REG_DESC_H__

struct var_desc;
typedef struct var_desc var_desc;

struct reg_desc{
    var_desc* vd[100];
    int cur_vd_no;
};
typedef struct reg_desc reg_desc;

void reg_desc_init();
void add_var_in_reg(int regno, var_desc* vd);
void del_var_in_reg(int regno, var_desc* vd);

#endif
