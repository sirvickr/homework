#ifndef __LIST1_H__
#define __LIST1_H__

struct LIST1_ITEM;

// структура элемента односвязного списка
typedef struct LIST1_ITEM {
	struct LIST1_ITEM* next;
	void* data;
} LIST1_ITEM;

// односвязный список

typedef void (*LIST_ITEM_FREE)(void*);
typedef void (*LIST_ITEM_PROC)(void* data, void* param);
// структура односвязного списка
typedef struct LIST1 {
	struct LIST1_ITEM* head;
	struct LIST1_ITEM* tail;
	LIST_ITEM_FREE item_free;
	int count;
} LIST1;

//
LIST1* list1_new();
//
int list1_delete(LIST1* list);
//
int list1_init(LIST1* list);
//
int list1_clear(LIST1* list);
//
int list1_push_front(LIST1* list, void* data);
//
int list1_push_back(LIST1* list, void* item);
//
int list1_for_each(LIST1* list, LIST_ITEM_PROC cb, void* param);

#endif
