/*
1.11. Реализовать инструкции типа #define и #undef, то есть выполнить указанные подстановки в области их задания.
Пробельные промежутки следует оставить без изменения.
Программа тестировалась на файле test.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

// состояния парсера
typedef enum { 
	stPlainText,  // находимся пока неизвестно где 
	stSkipSpaces, // находимся на пробелах и надо все пропустить
	stMacroHead,  // находимся внутри заголовка макроопределения 
	stMacroBody,  // находимся внутри тела макроопределения
} ParserStates;

// структура, описыпающая одно макроопределения в списке подобных
typedef struct macro_t {
	char head[256]; // заголовок (то, что идёт после #define)
	char body[256]; // тело (то, что идёт после заголовка)
	struct macro_t* next; // указатель на следующий элемент списка
	size_t pos; // текущая позиция для поиска (вспомогательный член)
	size_t start_pos; // стартовая позиция для замены (вспомогательный член)
} macro_t;

// освобождает список
void macros_clear( macro_t** head ) {
	macro_t* curr = *head;
	while( curr ) {
		macro_t* next = curr->next;
		free( curr );
		curr = next;
	}
	*head = NULL;
}

// удаялет элемент с совпадающим заголовком
void macros_remove( macro_t** head, const char* key ) {
	macro_t* prev = *head;
	macro_t* curr = *head;
	while( curr ) {
		macro_t* next = curr->next;
		if( 0 == strcmp( curr->head, key ) ) {
			if(curr != *head) 
				prev->next = curr->next;
			else
				*head = curr->next;
			free( curr );
		}
		prev = curr;
		curr = next;
	}
}

// сбрасывает текущии позиции в макроопределениях
void macros_reset_pos( macro_t* head ) {
	macro_t* curr = head;
	while( curr ) {
		curr->pos = 0;
		curr->start_pos = 0;
		curr = curr->next;
	}
}

// печатает список
void macros_print( macro_t* head ) {
	printf( "macros:\n" );
	macro_t* curr = head;
	while( curr ) {
		printf( "%s: \"%s\"\n", curr->head, curr->body );
		curr = curr->next;
	}
}

// точка входа программы
int main( int argc, char* argv[] )
{
	// исходный файл
	FILE *file = NULL;
	// буфер для содержимого исходного файла
	char *buffer = NULL;
	// список макроопределений
	macro_t* macros = NULL;

	if( argc != 2 ) {
		printf("usage: %s file\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// do {} while(0) - синтаксически удобный выход с помощью break к освобождения ресурсов
	do { 

		const char* source = argv[ 1 ];
		struct stat stat_buf;
		
		// получаем размер исходного файла
		if( -1 == stat( source, &stat_buf ) ) {
			perror("stat");
			break;
		}
		// открываем исходный файл на чтение
		file = fopen( source, "r" );
		if( !file ) {
			fprintf( stderr, "failed to open source file: \"%s\"\n", source );
			break;
		}
		// выделяем блок памяти для содержимого файла
		buffer = ( char* )malloc( stat_buf.st_size + 1 );
		if( !buffer ) {
			fprintf( stderr, "failed to alloc memory\n" );
			break;
		}
		size_t block_size = 1;
		size_t nmemb = stat_buf.st_size;
		size_t read = fread( buffer, block_size, nmemb, file );
		//printf( "read %lu blocks of %lu bytes (%lu)\n", read, block_size, left_to_read );
		if( read == 0 ) {
			fprintf( stderr, "failed to read %lu blocks of %lu bytes\n", nmemb, block_size );
			break;
		}

		// вспомогательные переменные для работы со списком
		macro_t* tail = NULL; // указатель на конец (для добавления элементов)
		macro_t* macro = NULL; // указатель для работы с текущим элементом

		// вспомогательные переменные заполнения заголовка и тела макроопределения
		size_t head_pos = 0; // позиция текущего символа заголовка макроопределения
		size_t body_pos = 0; // позиция текущего символа тела макроопределения

		// текущее состояние парсера
		ParserStates state = stPlainText;
		// следующее состояние для перехода из состояния stSkipSpaces
		ParserStates next_state = stPlainText;
		// указатели для состояния stSkipSpaces - куда сохранить первый непробельный символ
		char* pWr = NULL;
		size_t* pPos = NULL;
		// 1 - define, 0 - undef
		int is_def = 0; 
		// образец "#define" для парсера  
		const char* def_str = "#define";
		const size_t def_len = strlen( def_str );
		size_t def_pos = 0;
		// образец "#undef" для парсера  
		const char* undef_str = "#undef";
		const size_t undef_len = strlen( undef_str );
		size_t undef_pos = 0;
		// позиция замещения (там, где встретился заголовок существующего макроса)  
		size_t curr_macro_start_pos = 0;
		// основной цикл парсера  
		for( size_t i = 0; i < stat_buf.st_size; i++ ) {
			char ch = buffer[ i ];
			///fprintf( stderr, "%c:", ch );

			switch( state ) {
			case stPlainText:
				///fprintf( stderr, " stPlainText" );
				// поиск #define
				if( def_str[ def_pos ] == ch ) {
					// находимся где-то внутри "#define"
					def_pos++;
					if( def_pos == def_len ) {
						// полное совпадение!
						def_pos = 0;
						macros_reset_pos( macros );
						//
						macro = ( macro_t* )malloc( sizeof( macro_t ) );
						bzero( macro, sizeof( macro_t ) );
						next_state = stMacroHead;
						pWr = macro->head;
						pPos = &head_pos;
						state = stSkipSpaces;
						is_def = 1;
					}
				} else {
					def_pos = 0;
				}
				// поиск #undef
				if( undef_str[ undef_pos ] == ch ) {
					// находимся где-то внутри "#undef"
					undef_pos++;
					if( undef_pos == undef_len ) {
						// полное совпадение!
						undef_pos = 0;
						macros_reset_pos( macros );
						//
						macro = ( macro_t* )malloc( sizeof( macro_t ) );
						bzero( macro, sizeof( macro_t ) );
						next_state = stMacroHead;
						pWr = macro->head;
						pPos = &head_pos;
						state = stSkipSpaces;
						is_def = 0;
					}
				} else {
					undef_pos = 0;
				}
				// поиск заголовков макроопределений
				#if 1
				macro_t* curr = macros;
				while( curr ) {
					if( curr->head[ curr->pos ] == ch ) {
						// находимся где-то внутри заголовка очередного макроса
						if( 0 == curr->pos )
							curr->start_pos = i;
						curr->pos++;
						if( '\0' == curr->head[ curr->pos ] ) {
							// полное совпадение!
							buffer[ curr->start_pos ] = '\0';
							printf( "%s", buffer + curr_macro_start_pos );
							printf( "%s", curr->body );
							curr_macro_start_pos = curr->start_pos + strlen( curr->head );
							//fprintf( stderr, " FOUND %s\n", curr->head );

							break;
						}
					} else {
						curr->pos = 0;
					}

					curr = curr->next;
				}
				#endif
				break;
			case stSkipSpaces:
				///fprintf( stderr, " stSkipSpaces" );
				// пропускаем пробелы
				if( !isspace( ch ) ) {
					// сохраняем первый символ макроопределения
					pWr[ (*pPos)++ ] = ch;
					state = next_state;
				}
				break;
			case stMacroHead:
				///fprintf( stderr, " stMacroHead" );
				// сохраняем символы макроопределения до пробела
				if( !isspace( ch ) ) {
					macro->head[ head_pos++ ] = ch;
				} else {
					macro->head[ head_pos ] = '\0';
					head_pos = 0;
					if( is_def ) { // для #define уходим в накопление body для этого head
						next_state = stMacroBody;
						pWr = macro->body;
						pPos = &body_pos;
						state = stSkipSpaces;
					} else { // для #undef удаляем этот head из списка макроопределений
						macros_remove( &macros, macro->head );
						// удаляем временный объект
						free( macro );
						state = stPlainText;
					}
					///fprintf( stderr, " head \"%s\"", macro->head );
				}
				break;
			case stMacroBody:
				///fprintf( stderr, " stMacroBody[%lu]", body_pos );
				// сохраняем символы макроопределения до перевода строки
				if( '\n' != ch ) {
					macro->body[ body_pos++ ] = ch;
				} else {
					macro->body[ body_pos ] = '\0';
					body_pos = 0;
					state = stPlainText;
					// добавляем макроопределение в список
					if( !macros )
						macros = macro;
					if( tail )
						tail->next = macro;
					tail = macro;
					///fprintf( stderr, " body \"%s\"", macro->body );
				}
				break;
			}
			///fprintf( stderr, "\n" );
		} // for( i )

		printf( "%s", buffer + curr_macro_start_pos );

	} while( 0 );

	// освобождаем ресурсы
	if( file )
		fclose( file );
	if( buffer )
		free( buffer );
	if( macros )
		macros_clear( &macros );
	
	return 0;
}
