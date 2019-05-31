/*
Водится массив строк текста с неизвестным количеством строк. 
Ввод текста заканчивается, если повторно введена любая ранее введенная строка. 
Затем ввести массив символов разделителей. 
Признаком окончания ввода этого массива является ввод символа, код которого больше кодов всех ранее введенных символов разделителей. 
Преобразовать исходный массив таким образом, чтобы в каждой строке текста слова были упорядочены в порядке возрастания количества согласных букв. 
Символы разделители не переставляется и при необходимости сторону. 
Вывести преобразованный текст.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#include "words.h"
#include "lines.h"
#include "utils.h"

#define MAX_INPUT_SIZE 4096

static char input_buffer[ MAX_INPUT_SIZE ];
static char delimeters[ MAX_INPUT_SIZE ];

word_list_t word_list;

int get_line( line_t* lines, line_t*** ppp, char* text ) {
	size_t length = strlen( text );
	// при вводе с консоли, последним символом будет \0', а предпоследним - '\n'
	if( length < 1 ) {
		fprintf( stderr, "input error\n" );
		return 0;
	}
	text[ --length ] = '\0'; // удаляем '\n' (заменяем '\n' на '\0')

	printf( "text (len = %lu) %s\n", length, text );

	if( lines_find( lines, text ) ) {
		printf( "duplicate line, input finished\n" );
		return 0;
	}
	
	line_t* line = add_line( *ppp, text );

	**ppp = line;
	*ppp = &line->next;

	return 1;
}

// точка входа программы
int main( int argc, char* argv[] )
{
	// список строк
	line_t* lines = NULL;
	// вспомогательные переменные для работы со списком
	line_t* tail = NULL; // указатель на конец (для добавления элементов)
	//line_t* line = NULL; // указатель для работы с текущим элементом
	line_t** pp = &lines; //
	// количество разделителей
	int delimeter_count = 0; 
#if 1
	const char* debug_lines[] = {
		"one two three four\n",
		"first;seconddd third fourthh fifth\n",
	};
	{
		int count = sizeof( debug_lines ) / sizeof( debug_lines[ 0 ] );
		for( int i = 0; i < count; ++i ) {
			strcpy( input_buffer, debug_lines[ i ] );
			if( 0 == get_line( lines, &pp, input_buffer ) ) {
				break;
			}
		}
	}
#else
	while( 1 ) { 

		printf( "input string: " );
		if( fgets( input_buffer, MAX_INPUT_SIZE, stdin ) ) {
			if( 0 == get_line( lines, &pp, input_buffer ) ) {
				break;
			}
		} else {
			printf( "input error\n" );
		}

	} // while( 1 )
#endif
	lines_print( lines );
#if 1
	delimeters[ 0 ] = ' ';
	delimeters[ 1 ] = ';';
	delimeter_count = 2;
#else
	printf( "input delimeters:\n" );
	int sum = 0;
	while( 1 ) { 

		int c = getchar();
		if ( c != '\n' && c != '\r' ) {
			//printf( "You entered: '%c' (c %d sum %d)\n", c, c, sum + c );
			if( sum > 0 && c > sum ) {
				printf( "input finished\n" );
				break;
			}
			sum += c;
			delimeters[ delimeter_count++ ] = c;
		}

	} // while( 1 )
#endif
	delimeters[ delimeter_count ] = '\0';
	printf( "delimeters:\n" );
	for( int i = 0; i < delimeter_count; ++i ) {
		printf( "%d)\t'%c'\n", i, delimeters[ i ] );
	}

	printf( "------------------------------------------------------------\n" );
	lines_process( lines, delimeters );
	printf( "------------------------------------------------------------\n" );
	lines_print( lines );
    /*add_word( &word_list, "hundred" ); // 100
    add_word( &word_list, "twelve" ); // 12
    add_word( &word_list, "six" ); // 56
    add_word( &word_list, "seven" ); // 67
 
    pint_words( &word_list );
    sort_words( word_list.head, word_list.tail );
    pint_words( &word_list );
    remove_words( &word_list );*/
	printf( "------------------------------------------------------------\n" );

	return 0;
}
