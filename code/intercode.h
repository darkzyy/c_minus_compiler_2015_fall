#ifndef __INTERCODE_ENUM_H__
#define __INTERCODE_ENUM_H__

#include"list.h"

//#define __USE_UNION__

enum intercodeno{
    ICN_LABEL = 400,
    ICN_FUNC,
    ICN_ASSIGN,
    ICN_PLUS,
    ICN_MINUS,
    ICN_MUL,
    ICN_DIV,
    ICN_ADDR,
    ICN_REFER,
    ICN_REFER_ASSIGN,
    ICN_GOTO,
    ICN_IF,
    ICN_RETURN,
    ICN_DEC,
    ICN_ARG,
    ICN_CALL,
    ICN_PARAM,
    ICN_READ,
    ICN_WRITE
};

#ifdef __USE_UNION__
struct operand{
    enum{ VARIABLE,CONST_INT,CONST_FLOAT,LABEL,FUNC_NAME,VAR_TMP } kind;
    union{
        char* var_str;
        int value_int;
        float value_float;
        char* label;
        char* func;
        char* var_tmp_str;
    };
};
#else
struct operand{
    enum{ VARIABLE,CONST_INT,CONST_FLOAT,LABEL,FUNC_NAME,VAR_TMP } kind;
    char* var_str;
    int value_int;
    float value_float;
    char* label;
    char* func;
    char* var_tmp_str;
};
#endif
typedef struct operand operand;

#ifdef __USE_UNION__
struct intercode{
    int kind;
    union{
        struct{ operand label; }icn_label;
        struct{ operand func; }icn_func;
        struct{ operand left,right; }icn_assign;
        struct{ operand result,op_left,op_right; }icn_plus;
        struct{ operand result,op_left,op_right; }icn_minus;
        struct{ operand result,op_left,op_right; }icn_mul;
        struct{ operand result,op_left,op_right; }icn_div;
        struct{ operand left,right; }icn_addr;
        struct{ operand left,right; }icn_refer;
        struct{ operand left,right; }icn_refer_assign;
        struct{ operand label; }icn_goto;
        struct{ operand op_left,op_right,label; }icn_if;
        struct{ operand return_val; }icn_return;
        struct{ operand size;/*size % 4 = 0*/ }icn_dec;
        struct{ operand arg; }icn_arg;
        struct{ operand left,func; }icn_call;
        struct{ operand param; }icn_param;
        struct{ operand read_dst; }icn_read;
        struct{ operand write_src; }icn_write;
    };
    ListHead list;
};

#else
struct intercode{
    int kind;
    struct{ operand label; }                        icn_label;
    struct{ operand func; }                         icn_func;
    struct{ operand left , right; }                 icn_assign;
    struct{ operand result , op_left , op_right; }  icn_plus;
    struct{ operand result , op_left , op_right; }  icn_minus;
    struct{ operand result , op_left , op_right; }  icn_mul;
    struct{ operand result , op_left , op_right; }  icn_div;
    struct{ operand left , right; }                 icn_addr;
    struct{ operand left , right; }                 icn_refer;
    struct{ operand left , right; }                 icn_refer_assign;
    struct{ operand label; }                        icn_goto;
    struct{ operand op_left , op_right , label; }   icn_if;
    struct{ operand return_val; }                   icn_return;
    struct{ operand size;/*size % 4 = 0*/}          icn_dec;
    struct{ operand arg; }                          icn_arg;
    struct{ operand left , func; }                  icn_call;
    struct{ operand param; }                        icn_param;
    struct{ operand read_dst; }                     icn_read;
    struct{ operand write_src; }                    icn_write;
    ListHead list;
};

#endif

typedef struct intercode intercode ;


#endif
