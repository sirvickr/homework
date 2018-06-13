#include "list.h"
#include <stdlib.h>
#include <string.h>

static LIST1_ITEM* create_item(void* data);
static void delete_item(LIST1* list, LIST1_ITEM* item);
static void merge(LIST1_ITEM *a, LIST1_ITEM *b, LIST1_ITEM **c, LIST_ITEM_COMP compare, void* param);
static void split(LIST1_ITEM *src, LIST1_ITEM **low, LIST1_ITEM **high);
static void mergeSort(LIST1_ITEM **head, LIST_ITEM_COMP compare, void* param);

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
				list->head = next; // если это последний элемент, next равен 0
			}
			if(curr == list->tail) {
				if(list->head == NULL)
					list->tail = NULL; // если это последний элемент, list->head тут уже равен 0
				else
					list->tail = prev;
			}
			if(!list->curr && list->head) {
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

int list1_get_current_index(LIST1* list) {
	int i = 0;
	LIST1_ITEM* curr = list->head;
	while(curr) {
		if(curr == list->curr)
			return i;
		i++;
		curr = curr->next;
	}
	return 0;
}

int list1_set_current_index(LIST1* list, int index) {
	int i = 0;
	LIST1_ITEM* curr = list->head;
	while(curr) {
		if(i == index) {
			list->curr = curr;
			break;
		}
		i++;
		curr = curr->next;
	}
	return 0;
}

int list1_search(LIST1* list, LIST_ITEM_FIND check, void* param) {
	int i = 0;
	LIST1_ITEM* curr = list->head;
	while(curr) {
		if(check(curr->data, param)) {
			list->curr = curr;
			break;
		}
		i++;
		curr = curr->next;
	}
	return i;
}

void list1_sort(LIST1* list, LIST_ITEM_COMP compare, void* param) {
	mergeSort(&list->head, compare, param);
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

void mergeSort(LIST1_ITEM **head, LIST_ITEM_COMP compare, void* param) {
	LIST1_ITEM *low  = NULL;
	LIST1_ITEM *high = NULL;
	if ((*head == NULL) || ((*head)->next == NULL)) {
		return;
	}
	split(*head, &low, &high);
	mergeSort(&low, compare, param);
	mergeSort(&high, compare, param);
	merge(low, high, head, compare, param);
}

void split(LIST1_ITEM *src, LIST1_ITEM **low, LIST1_ITEM **high) {
	LIST1_ITEM* fast = NULL;
	LIST1_ITEM* slow = NULL;

	if (src == NULL || src->next == NULL) {
		(*low) = src;
		(*high) = NULL;
		return;
	}
	slow = src;
	fast = src->next;
	while (fast != NULL) {
		fast = fast->next;
		if (fast != NULL) {
			fast = fast->next;
			slow = slow->next;
		}
	}
	(*low) = src;
	(*high) = slow->next;
	slow->next = NULL;
}

void merge(LIST1_ITEM *a, LIST1_ITEM *b, LIST1_ITEM **c, LIST_ITEM_COMP compare, void* param) {
	LIST1_ITEM tmp;
    *c = NULL;
    if (a == NULL) {
        *c = b;
        return;
    }
    if (b == NULL) {
        *c = a;
        return;
    }
	if (compare(a->data, b->data, param)) { // a->value < b->value
		*c = a;
        a = a->next;
    } else {
        *c = b;
        b = b->next;
    }
    tmp.next = *c;
	while (a && b) {
		if (compare(a->data, b->data, param)) { // a->value < b->value
            (*c)->next = a;
            a = a->next;
        } else {
            (*c)->next = b;
            b = b->next;
        }
        (*c) = (*c)->next;
    }
    if (a) {
        while (a) {
            (*c)->next = a;
            (*c) = (*c)->next;
            a = a->next;
        }
    }
    if (b) {
        while (b) {
            (*c)->next = b;
            (*c) = (*c)->next;
            b = b->next;
        }
    }
    *c = tmp.next;
}

