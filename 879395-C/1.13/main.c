/*
1.13. Отформатировать абзацы текста в заданных границах и с красной строкой (без переноса слов). 
Абзац — фрагмент текста между пустыми строками.
Тестирование:
make
./task sample.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

// точка входа программы
int main( int argc, char* argv[] )
{
	// исходный файл
	FILE *file = NULL;
	// буфер для содержимого исходного файла
	char *line = NULL;
	// размер выделенного под строку буфера
	size_t size = 0;
	// длина прочитанной строки
	ssize_t read;
	// заданная граница (длина) строки
	size_t line_bound = 50; 
	// разделитель слов
	const char* delim = " ";

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
		// признак нового абзаца
		int is_new_block = 1;
		// общая длина напечатанной строки 
		size_t line_len = 0;
		// читаем в цикле каждую строку файла
		while( ( read = getline( &line, &size, file ) ) != -1 ) {
			///printf("(%lu)", read);
			if( 1 == read ) { // пустая строка (прочитан только символ перевода строки)
				printf("!\n");
				is_new_block = 1; // отмечаем, что нужно будет сделать отступ ("красную строку")
			} else if( read > 1 ) { // строка из одного или более слов
				line[ read - 1 ] = '\0'; // удаляем символ перевода строки ('\n')
				if( is_new_block ) {
					is_new_block = 0;
					printf("  ");
				}
				// разбиваем строку на слова и выводим их поочерёдно, пока не достигнута граница
				char* word = strtok( line, delim );
				while( word != NULL ) {
					printf("%s ", word);
					line_len += strlen( word );
					if( line_len >= line_bound ) {
						printf("\n");
						line_len = 0;
					}
					word = strtok( NULL, delim );
				}
			}
		} // while( read )
	} while( 0 );

	// освобождаем ресурсы
	if( file )
		fclose( file );
	if( line )
		free( line );
	
	return 0;
}
