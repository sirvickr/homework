#ifndef __LIST1_H__
#define __LIST1_H__

// односвязный список

// структура элемента односвязного списка
typedef struct LIST1_ITEM {
	struct LIST1_ITEM* next;
	void* data;
} LIST1_ITEM;

// структура односвязного списка
typedef struct LIST1 {
	struct LIST1_ITEM* head;
	struct LIST1_ITEM* tail;
	void (*item_free)(void* item);
	int count;
} LIST1;

//
LIST1* list1_create();
//
void list1_release(LIST1* list);

#endif
