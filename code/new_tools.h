#ifndef __NEW_TOOLS_H__
#define __NEW_TOOLS_H__

#include"intercode.h"

operand* make_new_op_var(char* var_str);
operand* make_new_op_addr(char* var_str);

operand* make_int(int val);
operand* make_float(float val);

operand* gen_assign_addr(operand* res,operand* op1);
operand* gen_assign_var(operand* res,operand* op1);

operand* gen_plus(operand* op1,operand* op2,int use_addr,char* res_tmp_str);
operand* gen_minus(operand* op1,operand* op2,int use_addr,char* res_tmp_str);
operand* gen_mul(operand* op1,operand* op2,int use_addr,char* res_tmp_str);
operand* gen_div(operand* op1,operand* op2,int use_addr,char* res_tmp_str);

#endif
