#include "lines.h"

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

void lines_process( line_t* head, const char* delimeters ) {
	line_t* curr = head;
	while( curr ) {
		char* pch = strtok( curr->text, delimeters );
		while( pch != NULL ) {
    		add_word( &curr->words, pch );
			pch = strtok( NULL, delimeters );
		}

		curr = curr->next;
	}
	curr = head; // DEBUG
	while( curr ) {
     	int index = 0;

		printf( "%s:\n", curr->text );
		
		sort_words( curr->words.head, curr->words.tail );
     	
     	pint_words( &curr->words );

		word_t* word = curr->words.head;
		while( word != NULL ) {
			#if 0
    		printf( " >> word: %s\n", word->text );
			#else
			char* pc = word->text;
			printf( " >> word: " );
			while( *pc != '\0' ) {
				printf( "%c", *pc );
				curr->text[ index++ ] = *pc;
				pc++;
			}
			curr->text[ index ] = '\0';
			printf( "\t>>: %s\n", curr->text );
			#endif
			word = word->next;
		}

		curr = curr->next;
	}
}
