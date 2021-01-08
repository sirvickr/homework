#include "bilist.h"

#include <stdlib.h>

// создаёт объект списка
bilist_t* bilist_create() {
	bilist_t* list = (bilist_t*)malloc(sizeof(bilist_t));
	list->head = NULL;
	list->tail = NULL;
	list->count = 0;
	return list;
}

// удаляет содержимое списка из памяти
void bilist_clear(bilist_t* list, callback_t release) {
	binode_t* curr = list->head;
	while(curr) {
		binode_t* next = curr->next;
		release(curr->data);
		free(curr);
		curr = next;
	}
	list->head = list->tail = NULL;
	list->count = 0;
}

// удаляет содержимое и объект списка из памяти
void bilist_destroy(bilist_t* books, callback_t release) {
	bilist_clear(books, release);
	free(books);
}

// вставляет элемент в конец списка
void bilist_push_back(bilist_t* list, void* data) {
	binode_t* item = (binode_t*)malloc(sizeof(binode_t));
	item->data = data;
	item->next = NULL;
	if(list->tail) { // список не пуст
		item->prev = list->tail;
		list->tail->next = item;
	} else {
		item->prev = NULL;
		list->head = item;
	}
	list->tail = item;
	list->count++;
}

// вставляет элемент в список перед curr
void bilist_insert(bilist_t* list, binode_t* curr, void* data) {
	binode_t* item = (binode_t*)malloc(sizeof(binode_t));
	item->data = data;
	item->next = curr;
	if(curr) {
		item->next->prev = curr->prev;
	} else {
		item->prev = NULL;
	}
	if(list->tail) { // список не пуст
		item->prev = list->tail;
		list->tail->next = item;
	} else {
		list->head = item;
		list->tail = item;
	}
	list->count++;
}

// удаляет элемент item из списка
void bilist_remove(bilist_t* list, binode_t* item, callback_t release) {
	if(!list->count)
		return;
	if(item->prev) {
		item->prev->next = item->next;
	}
	if(item->next) {
		item->next->prev = item->prev;
	}
	list->count--;
	if(!list->count) {
		list->head = NULL;
		list->tail = NULL;
	}
	if(release)
		release(item->data);
	free(item);
}

// возвращает размер списка
int bilist_size(const bilist_t* list) {
	return list->count;
}

// возвращает элемент по индексу
binode_t* bilist_item(bilist_t* list, int index) {
	binode_t* curr = list->head;
	int n = 0;
	while(curr) {
		if(index == n)
			return curr;
		curr = curr->next;
		n++;
	}
	return NULL;
}

// возвращает данные по индексу
void* bilist_get_data(bilist_t* list, int index) {
	binode_t* curr = list->head;
	int n = 0;
	while(curr) {
		if(index == n)
			return curr->data;
		curr = curr->next;
	}
	return NULL;
}

// заменяет данные элемента
void bilist_set_data(bilist_t* list, binode_t* item, void* data, callback_t release) {
	if(!list->count)
		return;
	if(release)
		release(item->data);
	item->data = data;
	free(item);
}

// печатает список
void bilist_print(const bilist_t* list, writer_t printer, FILE* stream, const char* title) {
	if(title)
		fprintf(stream, "%s:\n", title);
	binode_t* curr = list->head;
	int index = 0;
	while(curr) {
		fprintf(stream, "%d\t", index++);
		printer(curr->data, stream);
		curr = curr->next;
	}
}

// считывает список
void bilist_read(bilist_t* list, reader_t reader, FILE* stream) {
	void* data;
	while(data = reader(stream)) {
		bilist_push_back(list, data);
	}
}

// ищет элемент
void* bilist_find_data(const bilist_t* list, match_t match, const void* value) {
	binode_t* curr = list->head;
	while(curr) {
		if(match(curr->data, value))
			return curr->data;
		curr = curr->next;
	}
	return NULL;
}

static void bilist_swap(binode_t *a, binode_t *b)
{
	void* temp = a->data;
	a->data = b->data;
	b->data = temp;
}

// сортирует список
void bilist_bubble_sort(bilist_t* list, compare_t compare) {
	binode_t *left; // будет всегда указывать на начало списка
	binode_t *right = NULL; // будет всегда указывать на начало списка
	int swapped;
	do {	
		swapped = 0;
		left = list->head;
		while(left->next != right) {
			if(compare(left->data, left->next->data) > 0) {
				bilist_swap(left, left->next); 
                swapped = 1; 
			}
			left = left->next;
		}
		// поскольку максимальный элемент уже в конце списка, присваимаем его указателю right, 
		// чтобы не проверять понапрасну уже отсортированный список
		right = left;

	} while(swapped);
}
