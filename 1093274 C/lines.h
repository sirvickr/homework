#ifndef __LINES_H__
#define __LINES_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "words.h"

// односвязный список для хранения строк текста

// структура, представляющая строку текста в списке
typedef struct line_t {
	char* text;
	struct line_t* next; // указатель на следующий элемент списка
	word_list_t words;
} line_t;

// добавление строки в список
line_t* add_line( line_t** head, const char* text );
// освобождает список
void lines_clear( line_t** head ) ;
// печатает список
void lines_print( line_t* head );
// находит строку в списке строк
int lines_find( line_t* head, const char* text );
// упорядочение слов в строках
void lines_process( line_t* head, const char* delimeters );

#endif // __LINES_H__
