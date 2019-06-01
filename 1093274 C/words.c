#include "words.h"
#include "utils.h"

// добавление в список
void add_word( word_list_t* list, const char* word ) {
	if( word == NULL )
		return;
    word_t *item = ( word_t* )malloc( sizeof( word_t ) );
    item->text = ( char* )malloc( strlen( word ) );
    strcpy( item->text, word );
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
 
// удаление
void remove_words( word_list_t* list ) {
    word_t *temp, *item = list->head;
    while( item != NULL ) {
    	if( item->text != NULL )
    		free( item->text );
        temp = item;
        item = item->next;
        free( temp );
    }
    list->head = list->tail = NULL;
}
 
// вывод содержимого списка в консоль
void print_words( word_list_t* list ) {
    word_t *item = list->head;
    while ( item != NULL ) {
        printf( "%s ", item->text );
        item = item->next;
    }
	printf( "\n" );
}

// быстрая сортировка (quick sort)
void sort_words( word_t *left, word_t *right ) {
    word_t *start;
    word_t *curr; 
    char* p;
 
    // сортировка окончена - выход
    if ( left == right )
    	return;
 
    // установка двух рабочих указателей - Start и Current
    start = left;
    curr = start->next;
 
    // итерация по списку слева направо
    while ( 1 ) {
        // элемент с максимальным значением помещается в начало списка
        if ( consonant_count( start->text ) < consonant_count( curr->text ) ) {
            p = curr->text;
            curr->text = start->text;
            start->text = p;
        }   
         
        if ( curr == right )
        	break;
        curr = curr->next;
    }
 
    // переключение First и Current - максимум попадает в правый конец списка
    p = left->text;
    left->text = curr->text;
    curr->text = p;
 
 
    // сохранение Current
    word_t* temp = curr;
 
    // рекурсия
    curr = curr->prev;
    if( curr != NULL ) {
        if ( ( left->prev != curr ) && ( curr->next != left ) )
            sort_words( left, curr );
    }
 
    curr = temp;
    curr = curr->next;
    if( curr != NULL ) {
        if ( ( curr->prev != right ) && ( right->next != curr ) )
            sort_words( curr, right );
    }
}
