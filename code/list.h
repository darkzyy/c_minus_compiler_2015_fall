#ifndef __LIST_H__
#define __LIST_H__

#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#endif

////////////////////////////////////////////////////////////////////////////
//
// example:
//      pcb_free = list_entry(pcb_free->list.next , PCB , list);
//      list_entry(pcb_free->list.next , PCB , list)->list ...
///////////////////////////
//      ListHead *it;
//      list_foreach(it,&read_queue);
///////////////////////////
//      
//
////////////////////////////////////////////////////////////////////////////

#include <assert.h>

struct ListHead {
	struct ListHead *prev, *next;
};
typedef struct ListHead ListHead;

#ifdef ENVIRONMENT64
#define list_entry(ptr, type, member) \
	((type*)((char*)(ptr) - (long long)(&((type*)0)->member)))
#else
#define list_entry(ptr, type, member) \
	((type*)((char*)(ptr) - (int)(&((type*)0)->member)))
#endif


static inline void
list_add(ListHead *prev, ListHead *next, ListHead *data) {
	assert(data != NULL);
	data->prev = prev;
	data->next = next;
	if (prev != NULL) prev->next = data;
	if (next != NULL) next->prev = data;
}

static inline void
list_add_before(ListHead *list, ListHead *data) {
	list_add(list->prev, list, data);
}

static inline void
list_add_after(ListHead *list, ListHead *data) {
	list_add(list, list->next, data);
}

static inline void
list_del(ListHead *data) {
	assert(data != NULL);
	ListHead *prev = data->prev;
	ListHead *next = data->next;
	if (prev != NULL) prev->next = next;
	if (next != NULL) next->prev = prev;
}

static inline void
list_init(ListHead *list) {
	list->prev = list->next = list;
}

static inline int
list_empty(ListHead *list) {
	return list == list->next;
}

#define list_foreach(ptr, head) \
	for ((ptr) = (head)->next; (ptr) != (head); (ptr) = (ptr)->next)

#endif

