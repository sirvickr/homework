#ifndef __STUDENT_H__
#define __STUDENT_H__

#define MAX_NAME_LENGTH 100

// Двусвязный список для хранения информации о студентах

// элемент двусвязного списка
typedef struct Student {
	struct Student* next;
	struct Student* prev;
	char name[ MAX_NAME_LENGTH ];
	unsigned char age;
	float score;
} Student;

// структура двусвязного списка
typedef struct Students
{
	// указатели на первый и последний элемент списка
    struct Student *head;
    struct Student *tail;
} Students;

// Добавление в список нового элемента
void Students_Add_Item( Students* list, Student* item );
// Добавление в список по переданным параметрам
void Students_Add_Data( Students* list, const char* name, unsigned char age, float score );
// Редактирование элемента списка по указателю
void Students_Edit( Student* item, const char* name, unsigned char age, float score );
// Редактирование элемента списка по индексу
void Students_Edit_Index( Students* list, int index, const char* name, unsigned char age, float score );
// Удаление из списка по элементу
void Students_Del( Students* list, Student* item );
// Удаление из списка по индексу
void Students_Del_Index( Students* list, int index );
// Печать элемента списка
void Students_Print( Students* list );
// Удаление всего списка
void Students_Destroy( Students* list );

#endif // __STUDENT_H__

