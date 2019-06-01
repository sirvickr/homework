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

int get_line( line_t* lines, line_t*** ppp, char* text ) {
	size_t length = strlen( text );
	// при вводе с консоли, последним символом будет \0', а предпоследним - '\n'
	if( length < 1 ) {
		fprintf( stderr, "input error\n" );
		return 0;
	}
	text[ --length ] = '\0'; // удаляем '\n' (заменяем '\n' на '\0')

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
#if 0
	const char* debug_lines[] = {
		"one two three four\n",
		"appalling;great astonisged so-so;well-known;splendid outstanding awful yandex.ru excellent astounded\n",
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
#if 0
	delimeters[ 0 ] = ' ';
	delimeters[ 1 ] = ';';
	delimeter_count = 2;
#else
	printf( "input delimeters:\n" );
	int sum = 0;
	while( 1 ) { 

		int c = getchar();
		if ( c != '\n' && c != '\r' ) {
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

	lines_process( lines, delimeters );
	lines_print( lines );

	return 0;
}
