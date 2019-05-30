#include "lines.h"
#include "words.h"

// добавление строки в список
line_t* add_line( line_t** head, const char* text ) {
	line_t* line = ( line_t* )malloc( sizeof( line_t ) );
	bzero( line, sizeof( line_t ) );
	line->text = ( char* )malloc( strlen( text ) + 1 );
	strcpy( line->text, text );
	return line;
}

// освобождает список
void lines_clear( line_t** head ) {
	line_t* curr = *head;
	while( curr ) {
		line_t* next = curr->next;
		free( curr );
		curr = next;
	}
	*head = NULL;
}

// печатает список
void lines_print( line_t* head ) {
	printf( "lines:\n" );
	line_t* curr = head;
	while( curr ) {
		//printf( "%s\t(%d)\n", curr->text, consonant_count( curr->text ) );
		printf( "%s\n", curr->text );
		curr = curr->next;
	}
}

// находит строку в списке строк
int lines_find( line_t* head, const char* text ) {
	line_t* curr = head;
	while( curr ) {
		if( 0 == strcmp( curr->text, text ) )
			return 1;
		curr = curr->next;
	}
	return 0;
}
