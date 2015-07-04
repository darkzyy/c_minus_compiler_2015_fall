#ifndef __INTERCODE_H__
#define __INTERCODE_H__

#include"list.h"
#include<stdlib.h>

//#define __USE_UNION__

enum intercodeno{
    ICN_LABEL = 400,    //LABEL x :: x<= char*
    ICN_FUNC,           //f <= char*
    ICN_ASSIGN,         //x<=tmp var; y<=operand
    ICN_PLUS,           //x<=tmp_var; y z <= operand
    ICN_MINUS,
    ICN_MUL,
    ICN_DIV,
    ICN_GOTO,           //LABEL <= char*
    ICN_IF,             //x y <= operand ; z <= char*
    ICN_RETURN,         //operand
    ICN_DEC,            //tmp_var
    ICN_ARG,            //operand
    ICN_ARG_ADDR,            //operand addr
    ICN_CALL,           //f <= char*
    ICN_PARAM,          //tmp_var
    ICN_READ,           //tmp_var
    ICN_WRITE           //operand
};

struct operand{
    enum{ OP_INT=1,OP_FLOAT,OP_VAR,OP_ADDR } kind;
    char* var_str;
    int val_int;
    float val_float;
};
typedef struct operand operand;

struct Argl{
    operand* op;
    int is_basic;
    struct Argl* next;
};
typedef struct Argl Argl;

struct code_block;

struct intercode{
    int kind;
    char* relop;
    char* label;
    operand* res;
    operand* op1;
    operand* op2;
    int size;
    ListHead list;
    int use_addr;
};
typedef struct intercode intercode ;

struct code_block{
    intercode* start;
    intercode* end;
    ListHead list;
};
typedef struct code_block code_block;

operand* zero;
operand* one;
ListHead code_head;
ListHead block_head;
int current_label_no;
int current_var_no;

#endif
