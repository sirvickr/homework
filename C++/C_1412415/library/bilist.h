#ifndef __BIDIRECTIONAL_LINKED_LIST_H__
#define __BIDIRECTIONAL_LINKED_LIST_H__

#include <stdio.h>

// функции обратного вызова для элементов списка
typedef void  (*callback_t)(void*); // для удаления
typedef void  (*writer_t)  (void*, FILE*); // для печати
typedef void* (*reader_t)  (FILE*); // для чтения
typedef int   (*match_t)   (const void*, const void* value); // для поиска
typedef int   (*compare_t) (const void* lhs, const void* rhs); // для сортировки

// элемент двунаправленного списка
typedef struct binode_t {
	struct binode_t* prev;
	struct binode_t* next;
	void* data;
} binode_t;

// структура двунаправленного списка
typedef struct bilist_t {
	struct binode_t* head;
	struct binode_t* tail;
	int count;
} bilist_t;

// создаёт объект списка
bilist_t* bilist_create();

// удаляет содержимое списка из памяти
void bilist_clear(bilist_t* list, callback_t release);

// удаляет содержимое и объект списка из памяти
void bilist_destroy(bilist_t* books, callback_t release);

// вставляет элемент в конец списка
void bilist_push_back(bilist_t* list, void* data);

// вставляет элемент в список перед curr
void bilist_insert(bilist_t* list, binode_t* curr, void* data);

// удаляет элемент item из списка
void bilist_remove(bilist_t* list, binode_t* item, callback_t release);

// возвращает размер списка
int bilist_size(const bilist_t* list);

// возвращает элемент по индексу
binode_t* bilist_item(bilist_t* list, int index);

// возвращает данные по индексу
void* bilist_get_data(bilist_t* list, int index);

// заменяет данные элемента
void bilist_set_data(bilist_t* list, binode_t* item, void* data, callback_t release);

// печатает список
void bilist_print(const bilist_t* list, writer_t printer, FILE* stream, const char* title);

// считывает список
void bilist_read(bilist_t* list, reader_t reader, FILE* stream);

// ищет элемент
void* bilist_find_data(const bilist_t* list, match_t match, const void* value);

// сортирует список
void bilist_bubble_sort(bilist_t* list, compare_t compare);

#endif // __BIDIRECTIONAL_LINKED_LIST_H__
