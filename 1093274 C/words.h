#ifndef __WORDS_H__
#define __WORDS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// элемент двусвязного списка
typedef struct word_t
{
    //int nInteger;
    char* text;
    struct word_t *prev;
    struct word_t *next;
} word_t;

// двусвязный список слов
typedef struct _word_list_t
{
	// указатели на первый и последний элементы списка
    struct word_t *head;
    struct word_t *tail;
} word_list_t;

// добавление в список
void add_word( word_list_t* list, const char* p );
// удаление
void remove_words( word_list_t* list );
// вывод содержимого списка в консоль
void pint_words( word_list_t* list );
// быстрая сортировка (quick sort)
void sort_words( word_t *left, word_t *right );

#endif // __WORDS_H__
