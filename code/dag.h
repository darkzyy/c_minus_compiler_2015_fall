#ifndef __DAG_H__
#define __DAG_H__

#include"tempvar_ht.h"
#include"intercode.h"

enum{INVALID=0,VALID,KILLED};

/*
 * use static link list here
 */

#define VAR_LIST_LEN 100

struct dagnode{
    int status;
    int lch;
    int rch;
    int type; // -3: int, -2: float, -1:var
    intercode* ic;
    operand* res;
};

typedef struct dagnode dagnode;













#endif
