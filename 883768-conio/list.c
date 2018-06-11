#include "list.h"
#include <stdlib.h>
#include <string.h>

static LIST1_ITEM* create_item(void* data);
static void delete_item(LIST1* list, LIST1_ITEM* item);

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
	curr = list->head;
	while(curr) {
		LIST1_ITEM* next = curr->next;
		delete_item(list, curr);
		curr = next;
	}
	memset(list, 0x00, sizeof(LIST1));
	return 0;
}

int list1_push_front(LIST1* list, void* data) {
	LIST1_ITEM* item = create_item(data);
	if(!item)
		return -1;
	if(list->head) {
		item->next = list->head;
	} else { // добавляется первый элемент
		list->curr = item;
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
	} else { // добавляется первый элемент
		list->head = item;
		list->curr = item;
	}
	list->tail = item;
	list->count++;
	return 0;
}

int list1_for_each(LIST1* list, LIST_ITEM_PROC cb, void* param) {
	LIST1_ITEM* curr;
	int i = 0;
	if(!cb)
		return -1;
	curr = list->head;
	while(curr) {
		cb(curr->data, i++, param);
		curr = curr->next;
	}
	return 0;
}

// Прокрутить текущий указатель вперёд
void* list1_curr_fwd(LIST1* list, int wrap) {
	if(!list || !list->curr)
		return NULL;
	list->curr = list->curr->next;
	if(!list->curr && wrap)
		list->curr = list->head;
	return list->curr->data;
}

// Прокрутить текущий указатель назад
void* list1_curr_rev(LIST1* list, int wrap) {
	LIST1_ITEM *curr, *prev;
	if(!list || !list->head)
		return NULL;
	curr = list->head;
	prev = list->head;
	while(curr) {
		if(curr == list->curr) {
			if(curr == list->head) {
				if(wrap) {
					list->curr = list->tail;
				}
			} else {
				list->curr = prev;
			}
			break;
		}
		prev = curr;
		curr = curr->next;
	}
	return list->curr->data;
}

void* list1_front(LIST1* list) {
	if(!list->head)
		return NULL;
	return list->head->data;
}

void* list1_back(LIST1* list) {
	if(!list->tail)
		return NULL;
	return list->tail->data;
}

void* list1_curr(LIST1* list) {
	if(!list->curr)
		return NULL;
	return list->curr->data;
}

void* list1_erase(LIST1* list, LIST_ITEM_FIND cb, void* param) {
	LIST1_ITEM *curr, *prev;
	if(!list || !list->head)
		return NULL;
	curr = list->head;
	prev = list->head;
	while(curr) {
		LIST1_ITEM* next = curr->next;
		if(cb(curr->data, param)) {
			// сместить текущий указатель при необходимости
			if(curr == list->curr) {
				list->curr = prev;
			}
			// удалить найденный элемент вместе с данными
			delete_item(list, curr);
		} else {
			prev = curr;
		}
		curr = next;
	}
	if(!list->curr)
		return NULL;
	return list->curr->data;
}

void* list1_erase_current(LIST1* list) {
	LIST1_ITEM *curr, *prev;
	if(!list || !list->head)
		return NULL;
	curr = list->head;
	prev = list->head;
	while(curr) {
		LIST1_ITEM *next = curr->next;
		if(curr == list->curr) {
			// определить новый текущий элемент
			prev->next = next;
			list->curr = next;
			if(curr == list->head) {
				list->head = next;
			}
			if(curr == list->tail) {
				if(list->head == list->tail)
					list->tail = NULL;
				else
					list->tail = prev;
			}
			if(!list->curr) {
				list->curr = prev;
			}
			// удалить старый текущий элемент вместе с данными
			delete_item(list, curr);
			break;

		}
		prev = curr;
		curr = next;
	}

	if(!list->curr)
		return NULL;
	return list->curr->data;
}

// вспомогательные функции

// создать элемент списка - контейнер данных
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

// удалить элемент списка - контейнер данных
void delete_item(LIST1* list, LIST1_ITEM* item) {
	if(list->item_free)
		list->item_free(item->data);
	free(item);
	list->count--;
}

