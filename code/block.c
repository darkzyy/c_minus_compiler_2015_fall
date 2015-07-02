#include<stdlib.h>
#include<stdio.h>
#include"intercode.h"
#include"code_print.h"

/*
intercode* pp;
code_block* last_cb;
intercode* last_ic;
*/
#define code_type (list_entry(p,intercode,list)->kind)
#define pp list_entry(p,intercode,list)
#define last_cb list_entry(block_head.prev,code_block,list)
#define prev_ic(cur) list_entry(cur->list.prev,intercode,list)
#define next_ic(cur) list_entry(cur->list.next,intercode,list)


void find_leader(){
    ListHead* p = NULL;
    list_foreach(p,&code_head){
        intercode* leader = NULL;
        if(code_type == ICN_LABEL || code_type == ICN_FUNC){
            leader = pp;
            if(block_head.prev != &block_head && leader->list.prev != &code_head){
                last_cb->end = prev_ic(leader);
            }
            code_block* cb = malloc(sizeof(code_block));
            cb->start = leader;
            cb->end = NULL;
            list_add_before(&block_head,&(cb->list));
        }
        if(code_type == ICN_GOTO || code_type == ICN_IF){
            last_cb->end = pp;
            if(pp->list.next != &code_head && next_ic(pp)->kind != ICN_LABEL &&
                        next_ic(pp)->kind != ICN_FUNC){
                leader = next_ic(pp);
                code_block* cb = malloc(sizeof(code_block));
                cb->start = leader;
                cb->end = NULL;
                list_add_before(&block_head,&(cb->list));
            }
        }
    }
    p = &code_head;
    last_cb->end = prev_ic(pp);
}


#undef code_type
#undef last_cb
#undef prev_ic
#undef next_ic
#undef pp

#define pp list_entry(p,code_block,list)

void print_block(){
    ListHead* p = NULL;
    list_foreach(p,&block_head){
        printf("------------+++++++++++------------\n");
        print_intercode(pp->start);
        print_intercode(pp->end);
        printf("------------+++++++++++------------\n");
    }
}

#undef pp












