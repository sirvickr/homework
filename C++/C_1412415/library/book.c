// Личная библиотека

#include "book.h"

#include <stdlib.h>
#include <string.h>

book_t* book_create() {
	book_t* book = (book_t*)malloc(sizeof(book_t));
	book->year = 0;
	book->code[0] = '\0';
	book->author[0] = '\0';
	book->name[0] = '\0';
	return book;
}

book_t* book_create_init(unsigned year, const char* code, const char* author, const char* name) {
	book_t* book = (book_t*)malloc(sizeof(book_t));
	book->year = year;
	strcpy(book->code, code);
	strcpy(book->author, author);
	strcpy(book->name, name);
	return book;
}

// функции ввода

void book_destroy(void* book) {
	free(book);
}

void input_book_year(book_t* book, FILE* stream, int prompt) {
	if(prompt) {
		printf("Введите год выпуска: ");
		fflush(stdout);
	}
	fscanf(stream, "%u", &book->year);
}

void input_book_code(book_t* book, FILE* stream, int prompt) {
	if(prompt) {
		printf("Введите шифр книги: ");
		fflush(stdout);
	}
	fscanf(stream, "%255s", book->code); 
}

void input_book_author(book_t* book, FILE* stream, int prompt) {
	if(prompt) {
		printf("Введите фамилию автора: ");
		fflush(stdout);
	}
	fscanf(stream, "%255s", book->author); 
}

void input_book_name(book_t* book, FILE* stream, int prompt) {
	if(prompt) {
		printf("Введите название книги: ");
		fflush(stdout);
	}
	fscanf(stream, "%255s", book->name); 
}

void* input_book(FILE* stream, int prompt) {
	book_t* book = book_create();
	input_book_year(book, stream, prompt);
	input_book_code(book, stream, prompt);
	input_book_author(book, stream, prompt);
	input_book_name(book, stream, prompt);
	return book;
}

void* read_book(FILE* stream) {
	book_t* book = book_create();
	int index;
	int scan_result = fscanf(stream, "%d\t%u\t%s\t%s\t%s\n", &index, &book->year, book->code, book->author, book->name);
	if(scan_result != 5) {
		free(book);
		book = NULL;
	}
	return book;
}

// функция вывода

void print_book(void* ptr, FILE* stream) {
	const book_t* book = (const book_t*)ptr;
	if(book) {
		fprintf(stream, "%u\t%s\t%s\t%s\n", book->year, book->code, book->author, book->name);
	} else {
		fprintf(stream, "Книга не найдена\n");
	}
}

// функции обратного вызова для поиска

int book_compare_year(const void* ptr, const void* value) {
	const book_t* book = (const book_t*)ptr;
	int year = *((int*)value);
	return book->year == year;
}

int book_compare_code(const void* ptr, const void* value) {
	const book_t* book = (const book_t*)ptr;
	const char* code = value;
	return (strcmp(book->code, code) == 0);
}

int book_compare_author(const void* ptr, const void* value) {
	const book_t* book = (const book_t*)ptr;
	const char* author = value;
	return (strcmp(book->author, author) == 0);
}

int book_compare_name(const void* ptr, const void* value) {
	const book_t* book = (const book_t*)ptr;
	const char* name = value;
	return (strcmp(book->name, name) == 0);
}

// функции обратного вызова для сортировки

int book_compare_by_year(const void* lhs, const void* rhs) {
	const book_t* a = (const book_t*)lhs;
	const book_t* b = (const book_t*)rhs;
	return a->year - b->year;
}

int book_compare_by_code(const void* lhs, const void* rhs) {
	const book_t* a = (const book_t*)lhs;
	const book_t* b = (const book_t*)rhs;
	return strcmp(a->code, b->code);
}

int book_compare_by_author(const void* lhs, const void* rhs) {
	const book_t* a = (const book_t*)lhs;
	const book_t* b = (const book_t*)rhs;
	return strcmp(a->author, b->author);
}

int book_compare_by_name(const void* lhs, const void* rhs) {
	const book_t* a = (const book_t*)lhs;
	const book_t* b = (const book_t*)rhs;
	return strcmp(a->name, b->name);
}
