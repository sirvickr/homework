/*
1.11. Реализовать инструкции типа #define и #undef, то есть выполнить указанные подстановки в области их задания.
Пробельные промежутки следует оставить без изменения.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

typedef struct macro_t {
	char head[256];
	char body[256];
	struct macro_t* next;
} macro_t;

int main( int argc, char* argv[] )
{
	struct stat stat_buf;
	FILE *file = NULL, *output = NULL;
	char *buffer = NULL;

	if( argc != 2 ) {
		printf("usage: %s file\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	const char* source = argv[ 1 ];
	const char* destin = "output.c";

	do {
		
		// получаем размер исходного файла
		if( -1 == stat( source, &stat_buf ) ) {
			perror("stat");
			break;
		}
		fprintf( stderr, "source file(\"%s\") size: %lu\n", source, stat_buf.st_size );
		// открываем исходный файл на чтение
		file = fopen( source, "r" );
		if( !file ) {
			fprintf( stderr, "failed to open source file: \"%s\"\n", source );
			break;
		}
		// открываем целевой файл на запись
		//output = fopen( destin, "w" );
		//if( !output ) {
		//	fprintf( stderr, "failed to open source file: \"%s\"\n", source );
		//	break;
		//}
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

		macro_t* macros = NULL;
		macro_t* tail = NULL;
		macro_t* macro = NULL;

		//char macroHead[256];
		//char macroBody[256];
		size_t head_pos = 0;
		size_t body_pos = 0;

		typedef enum { 
			stPlainText, 
			stSkipSpaces, 
			stMacroHead,  
			stMacroBody, 
		} ParserStates;

		ParserStates state = stPlainText;
		// для stSkipSpaces
		ParserStates next_state = stPlainText;
		char* pWr = NULL;
		size_t* pPos = NULL;

		const char* def_str = "#define";
		const size_t def_len = strlen( def_str );
		size_t def_pos = 0;

		for( size_t i = 0; i < stat_buf.st_size; i++ ) {
			char ch = buffer[ i ];
			fprintf( stderr, "%c:", ch );

			switch( state ) {
			case stPlainText:
				///fprintf( stderr, " stPlainText" );
				if( def_str[ def_pos ] == ch ) {
					def_pos++;
					if( def_pos == def_len ) {
						def_pos = 0;
						//
						macro = ( macro_t* )malloc( sizeof( macro_t ) );
						bzero( macro, sizeof( macro_t ) );
						next_state = stMacroHead;
						pWr = macro->head;
						pPos = &head_pos;
						state = stSkipSpaces;
					}
				} else {
					def_pos = 0;
				}
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
					next_state = stMacroBody;
					pWr = macro->body;
					pPos = &body_pos;
					state = stSkipSpaces;
					///fprintf( stderr, " head \"%s\"", macro->head );
				}
				break;
			case stMacroBody:
				///fprintf( stderr, " stMacroBody" );
				// сохраняем символы макроопределения до перевода строки
				if( '\n' != ch ) {
					macro->body[ body_pos++ ] = ch;
				} else {
					macro->body[ body_pos ] = '\0';
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

		fprintf( stderr, "macros:\n" );
		macro = macros;
		while( macro ) {
			fprintf( stderr, "%s: \"%s\"\n", macro->head, macro->body );
			macro = macro->next;
		}

	} while( 0 );

	if( file )
		fclose( file );
	if( output )
		fclose( output );
	
	return 0;
}
