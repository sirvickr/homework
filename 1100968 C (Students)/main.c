#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "student.h"

#define MAX_INPUT_LEN 100

char input[ MAX_INPUT_LEN ];

// Предварительные объявления функций
void Add_Student( Students* students );
void Del_Student( Students* students );
void Edit_Student( Students* students );

// Основная функция программы
int main( int argc, char *argv[] ) {
	int length;
	//int counter = 0;
	int menu = -1;
	Students students;
	memset( &students, 0x00, sizeof( students ) );
	
	while ( menu != 0 ) {
		printf( "\n\nMain menu\n" );
		printf( "[0] Exit\n" );
		printf( "[1] Print students\n" );
		printf( "[2] Add student\n" );
		printf( "[3] Edit student's info\n" );
		printf( "[4] Delete student\n" );

		printf("\n");

		printf( "Your choice: " );
		length = strlen( gets( input ) );
		if( length == 1 && isdigit( input[ 0 ] ) ) {
			menu = atoi( input );
			switch ( menu ) {
			case 1:
				Students_Print( &students );
				break;
			case 2:
				Add_Student( &students );
				break;
			case 3:
				Edit_Student( &students );
				break;
			case 4:
				Del_Student( &students );
				break;
			default:
				printf( "Unknown choice\n" );
			}
		} else {
			printf( "Not number\n" );
		}
	}

	// освобождение ресурсов перед выходом
	Students_Destroy( &students );
	printf( "Finished\n" );
	return 0;
}

// Запрашивает у пользователя информацию о студенте
void Read_Student( char* name, unsigned char* age, float* score )
{
	char input[ MAX_INPUT_LEN ];
	printf( "Enter name: " );
	gets( name );
	printf( "Enter age: " );
	gets( input );
	*age = ( unsigned char )atoi( input );
	printf( "Enter score: " );
	gets( input );
	*score = atof( input );
}

// Запрашивает у пользователя порядковый номер студента
int Read_Index()
{
	char input[ MAX_INPUT_LEN ];
	printf( "Enter student's number': " );
	gets( input );
	return atoi( input );
}

// Добавление нового студента
void Add_Student( Students* students )
{
	char name[ MAX_NAME_LENGTH ];
	unsigned char age;
	float score;
	Read_Student( name, &age, &score );
	Students_Add_Data( students, name, age, score );
}

// Редактирование информации о выбранном студенте
void Edit_Student( Students* students )
{
	char name[ MAX_NAME_LENGTH ];
	unsigned char age;
	float score;
	int index = Read_Index();
	if( index > 0 ) {
		Read_Student( name, &age, &score );
		Students_Edit_Index( students, index - 1, name, age, score );
	}
}

// Удаление выбранного студента из списка
void Del_Student( Students* students )
{
	int index = Read_Index();
	if( index > 0 ) {
		Students_Del_Index( students, index - 1 );
	}
}

