#ifndef __INTERCODE_ENUM_H__
#define __INTERCODE_ENUM_H__

#include"list.h"

//#define __USE_UNION__

enum intercodeno{
    ICN_LABEL = 400,    //LABEL x :: x<= char*
    ICN_FUNC,           //f <= char*
    ICN_ASSIGN,         //x<=tmp var; y<=operand
    ICN_PLUS,           //x<=tmp_var; y z <= operand
    ICN_MINUS,
    ICN_MUL,
    ICN_DIV,
    ICN_ADDR,           //tmp_var
    ICN_REFER,          //tmp_var
    ICN_REFER_ASSIGN,   //tmp_var
    ICN_GOTO,           //LABEL <= char*
    ICN_IF,             //x y <= operand ; z <= char*
    ICN_RETURN,         //operand
    ICN_DEC,            //tmp_var
    ICN_ARG,            //operand
    ICN_CALL,           //f <= char*
    ICN_PARAM,          //tmp_var
    ICN_READ,           //tmp_var
    ICN_WRITE           //operand
};

struct operand{
    enum{ OP_INT,OP_FLOAT,OP_VAR } kind;
    char* var_str;
    int val_int;
    float val_float;
};

typedef struct operand operand;

struct intercode{
    int kind;
    struct{ char* label; }                                  icn_label;
    struct{ char* func; }                                   icn_func;
    struct{ operand* left , *right; }                   icn_assign;
    struct{ operand* result , *op_left , *op_right; }    icn_plus;
    struct{ operand* result , *op_left , *op_right; }    icn_minus;
    struct{ operand* result , *op_left , *op_right; }    icn_mul;
    struct{ operand* result , *op_left , *op_right; }    icn_viv;
    struct{ operand* left , *right; }                           icn_addr;
    struct{ operand* left , *right; }                           icn_refer;
    struct{ operand* left , *right; }                           icn_refer_assign;
    struct{ char* label; }                                  icn_goto;
    struct{ operand* op_left , *op_right ; char* label; }     icn_if;
    struct{ operand* return_val; }                           icn_return;
    struct{ operand* var ; int size;/*size % 4 = 0*/}          icn_dec;
    struct{ operand* arg; }                                  icn_arg;
    struct{ operand* result , *func; }                            icn_call;
    struct{ operand* param; }                                  icn_param;
    struct{ char* read_dst; }                               icn_read;
    struct{ operand* write_src; }                            icn_write;
    ListHead list;
};

typedef struct intercode intercode ;

#endif
