#ifndef __NEW_TOOLS_H__
#define __NEW_TOOLS_H__

#include"intercode.h"

operand* make_new_op_var(char* var_str);

operand* make_new_op_addr(char* var_str);

operand* make_int(int val);

operand* make_float(float val);

#endif
