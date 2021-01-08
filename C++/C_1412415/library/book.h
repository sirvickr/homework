#ifndef __MY_LIBRARY_DATABASE_H__
#define __MY_LIBRARY_DATABASE_H__

#include <stdio.h>

#ifndef TRUE
# define TRUE 1
#endif
#ifndef FALSE
# define FALSE 0
#endif

#define MAX_STR_SIZE 256

// структура, описывающая одну книгу

typedef struct book_t {
	unsigned year;
	char code[MAX_STR_SIZE];
	char author[MAX_STR_SIZE];
	char name[MAX_STR_SIZE];
} book_t;

// конструктор по умолчанию
book_t* book_create();

// конструктор с параметрами
book_t* book_create_init(unsigned year, const char* code, const char* author, const char* name);

// деструктор
void book_destroy(void* book);

// функции ввода

void input_book_year(book_t* book, FILE* stream, int prompt);

void input_book_code(book_t* book, FILE* stream, int prompt);

void input_book_author(book_t* book, FILE* stream, int prompt);

void input_book_name(book_t* book, FILE* stream, int prompt);

void* input_book(FILE* stream, int prompt);

void* read_book(FILE* stream);

// функция вывода

void print_book(void* ptr, FILE* stream);

// функции обратного вызова для поиска

int book_compare_year(const void* ptr, const void* value);

int book_compare_code(const void* ptr, const void* value);

int book_compare_author(const void* ptr, const void* value);

int book_compare_name(const void* ptr, const void* value);

// функции обратного вызова для сортировки

int book_compare_by_year(const void* lhs, const void* rhs);

int book_compare_by_code(const void* lhs, const void* rhs);

int book_compare_by_author(const void* lhs, const void* rhs);

int book_compare_by_name(const void* lhs, const void* rhs);

#endif // __MY_LIBRARY_DATABASE_H__
