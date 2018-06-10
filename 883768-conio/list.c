#include "list.h"
#include <stdlib.h>
#include <string.h>

LIST1* list1_create() {
	LIST1* list = (LIST1*)malloc(sizeof(LIST1));
	if(NULL == list)
		return NULL;
	memset(list, 0x00, sizeof(LIST1));
	list->item_free = free;
	return list;
}

void list1_release(LIST1* list) {
	LIST1_ITEM* curr = list->head;
	while(curr) {
		LIST1_ITEM* next = curr->next;
		list->item_free(curr->data);
		free(curr);
		curr = next;
	}
	list->head = NULL;
	list->tail = NULL;
	free(list);
}

