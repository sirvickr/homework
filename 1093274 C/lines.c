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

// разделяет слова в каждой строке и сортирует 
// их по возрастанию количества согласных в слове
void lines_process( line_t* head, const char* delimeters ) {
	line_t* curr;
	// разбиваем каждую строку на слова, имходя из списка разделителей
	// отдельные слова помещаем в список для последующей сортировки
	curr = head;
	while( curr ) {
		char* pch = strtok( curr->text, delimeters );
		while( pch != NULL ) {
    		add_word( &curr->words, pch );
			pch = strtok( NULL, delimeters );
		}
		curr = curr->next;
	}
	// сортируем каждый список слов и склеиваем отсортированные слова обратно в строку
	curr = head;
	while( curr ) {
     	int index = 0;

		sort_words( curr->words.head, curr->words.tail );

		word_t* word = curr->words.head;
		while( word != NULL ) {
			char* pc = word->text;
			while( *pc != '\0' ) {
				curr->text[ index++ ] = *pc;
				pc++;
			}
			curr->text[ index++ ] = delimeters[0];
			curr->text[ index ] = '\0';
			word = word->next;
		}

		curr = curr->next;
	}
}
