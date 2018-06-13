#ifndef __LIST1_H__
#define __LIST1_H__

struct LIST1_ITEM;

// ��������� �������� ������������ ������
typedef struct LIST1_ITEM {
	struct LIST1_ITEM* next;
	void* data;
} LIST1_ITEM;

// ����������� ������

typedef void (*LIST_ITEM_FREE)(void* item);
typedef void (*LIST_ITEM_UPD)(void* item);
// ���� ����� 0, ��������� ����������� �� ���� ��������
typedef int (*LIST_ITEM_PROC)(void* item, int index, void* param);
typedef int (*LIST_ITEM_FIND)(void* item, void* param);
typedef int (*LIST_ITEM_COMP)(void* item1, void* item2, void* param);
// ��������� ������������ ������
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
// ���������� ������� ��������� �����
void* list1_curr_fwd(LIST1* list, int wrap);
// ���������� ������� ��������� �����
void* list1_curr_rev(LIST1* list, int wrap);
//
void* list1_front(LIST1* list);
//
void* list1_back(LIST1* list);
//
void* list1_curr(LIST1* list);
// ������� ������� ������
// ���������� ������� ������� (��������, �����)
void* list1_erase(LIST1* list, LIST_ITEM_FIND cb, void* param);
// ������� ������� ������� ������
// ���������� ����� ������� �������
void* list1_erase_current(LIST1* list);
//
int list1_get_current_index(LIST1* list);
//
int list1_set_current_index(LIST1* list, int index);
// ����� �������� (��������� ������� � ����� ��� ���������� �����)
int list1_search(LIST1* list, LIST_ITEM_FIND check, void* param);
// ���������� ������
void list1_sort(LIST1* list, LIST_ITEM_COMP compare, void* param);

#endif
