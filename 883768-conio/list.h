#ifndef __LIST1_H__
#define __LIST1_H__

struct LIST1_ITEM;

// структура элемента односвязного списка
typedef struct LIST1_ITEM {
	struct LIST1_ITEM* next;
	void* data;
} LIST1_ITEM;

// односвязный список

typedef void (*LIST_ITEM_FREE)(void* item);
typedef void (*LIST_ITEM_UPD)(void* item);
// если вернёт 0, обработка прекратится на этой итерации
typedef int (*LIST_ITEM_PROC)(void* item, int index, void* param);
typedef int (*LIST_ITEM_FIND)(void* item, void* param);
typedef int (*LIST_ITEM_COMP)(void* item1, void* item2, void* param);
// структура односвязного списка
typedef struct LIST1 {
	struct LIST1_ITEM *head, *tail, *curr;
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
int list1_for_each(LIST1* list, LIST_ITEM_PROC find, void* param);
// Прокрутить текущий указатель вперёд
void* list1_curr_fwd(LIST1* list, int wrap);
// Прокрутить текущий указатель назад
void* list1_curr_rev(LIST1* list, int wrap);
//
void* list1_front(LIST1* list);
//
void* list1_back(LIST1* list);
//
void* list1_curr(LIST1* list);
// Удалить элемент списка
// возвращает текущий элемент (возможно, новый)
void* list1_erase(LIST1* list, LIST_ITEM_FIND cb, void* param);
// Удалить текущий элемент списка
// возвращает новый текущий элемент
void* list1_erase_current(LIST1* list);
//
int list1_get_current_index(LIST1* list);
//
int list1_set_current_index(LIST1* list, int index);
// поиск элемента (установит текущий и вернёт его порядковый номер)
int list1_search(LIST1* list, LIST_ITEM_FIND check, void* param);
// сортировка списка
void list1_sort(LIST1* list, LIST_ITEM_COMP compare, void* param);

#endif
