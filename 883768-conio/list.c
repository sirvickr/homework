#include "list.h"
#include <stdlib.h>
#include <string.h>

static LIST1_ITEM* create_item(void* data);

LIST1* list1_new() {
	LIST1* list = (LIST1*)malloc(sizeof(LIST1));
	if(-1 == list1_init(list))
		return NULL;
	return list;
}

int list1_delete(LIST1* list) {
	if(!list1_clear(list))
		return -1;
	free(list);
	return 0;
}

int list1_init(LIST1* list) {
	if(!list)
		return -1;
	memset(list, 0x00, sizeof(LIST1));
	list->item_free = free;
	return 0;
}

int list1_clear(LIST1* list) {
	LIST1_ITEM* curr;
	if(!list)
		return -1;
	if(!list->item_free)
		return -1;
	curr = list->head;
	while(curr) {
		LIST1_ITEM* next = curr->next;
		list->item_free(curr->data);
		free(curr);
		curr = next;
	}
	list->head = NULL;
	list->tail = NULL;
	return 0;
}

int list1_push_front(LIST1* list, void* data) {
	LIST1_ITEM* item = create_item(data);
	if(!item)
		return -1;
	if(list->head) {
		item->next = list->head;
	} else {
		list->tail = item;
	}
	list->head = item;
	list->count++;
	return 0;
}

int list1_push_back(LIST1* list, void* data) {
	LIST1_ITEM* item = create_item(data);
	if(!item)
		return -1;
	if(list->head) {
		list->tail->next = item;
	} else {
		list->head = item;
	}
	list->tail = item;
	list->count++;
	return 0;
}

int list1_for_each(LIST1* list, LIST_ITEM_PROC cb, void* param) {
	LIST1_ITEM* curr;
	if(!cb)
		return -1;
	curr = list->head;
	while(curr) {
		cb(curr->data, param);
		curr = curr->next;
	}
	return 0;
}

// вспомогательные функции

LIST1_ITEM* create_item(void* data) {
	LIST1_ITEM* item;
	if(NULL == data)
		return NULL;
	item = (LIST1_ITEM*)malloc(sizeof(LIST1_ITEM));
	if(!item)
		return NULL;
	memset(item, 0x00, sizeof(LIST1_ITEM));
	item->data = data;
	return item;
}

