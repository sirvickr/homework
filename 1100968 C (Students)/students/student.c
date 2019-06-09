#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

// Добавление в список нового элемента
void Students_Add_Item( Students* list, Student* item )
{
    // заносим новый элемент в список
    if ( list->head == NULL ) {
        list->head = list->tail = item;
        item->next = item->prev = NULL;
    } else {
        list->tail->next = item;
        item->prev = list->tail;
        list->tail = item;
        item->next = NULL;
    }
}

// Добавление в список по переданным параметрам
void Students_Add_Data( Students* list, const char* name, unsigned char age, float score )
{
	// выделяем память под новый элемент
    Student *item = ( Student* )malloc( sizeof( Student ) );
	// копируем данные
    strcpy( item->name, name );
    item->age = age;
    item->score = score;
    Students_Add_Item( list, item );
}

// Редактирование элемента списка по указателю
void Students_Edit( Student* item, const char* name, unsigned char age, float score )
{
    strcpy( item->name, name );
    item->age = age;
    item->score = score;
}

// Редактирование элемента списка по индексу
void Students_Edit_Index( Students* list, int index, const char* name, unsigned char age, float score )
{
	int i = 0;
    Student* item = list->head;
    while( item != NULL ) {
    	if( i++ == index ) {
	        Students_Edit( item, name, age, score );
	        break;
		}
        item = item->next;
    }
}

// Удаление из списка по элементу
void Students_Del( Students* list, Student* item )
{
	if( item->prev ) {
		item->prev->next = item->next;
	} else {
		list->head = item->next;
	}
	if( item->next ) {
		item->next->prev = item->prev;
	} else {
		list->tail = item->prev;
	}
    free( item );
}

// Удаление из списка по индексу
void Students_Del_Index( Students* list, int index )
{
	int i = 0;
    Student* item = list->head;
    while( item != NULL ) {
    	if( i++ == index ) {
	        Students_Del( list, item );
	        break;
		}
        item = item->next;
    }
}

// Печать элемента списка
void Students_Print( Students* list )
{
	int i = 0;
    Student *item = list->head;
    if( item == NULL ) {
		printf( "List is empty\n" );
    	return;
	}
	printf( "Num\tName\tAge\tScore\n" );
    while ( item != NULL ) {
        printf( "%d\t%s\t%d\t%.2f\n", ++i, item->name, (int)item->age, item->score );
        item = item->next;
    }
	printf( "\n" );
}

// Удаление всего списка
void Students_Destroy( Students* list )
{
    Student *temp, *item = list->head;
    while( item != NULL ) {
        temp = item;
        item = item->next;
        free( temp );
    }
    list->head = list->tail = NULL;
}

