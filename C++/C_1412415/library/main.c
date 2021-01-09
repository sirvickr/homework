// Личная библиотека

#include "bilist.h"
#include "book.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MENU_CREATE 1 // добавление новой записи в список
#define MENU_UPDATE 2 // удаление записи из списка
#define MENU_DELETE 3 // редактирование записи в списке
#define MENU_SORT   4 // сортировка списка
#define MENU_OUTPUT 5 // вывод на экран содержимого списка
#define MENU_LOAD   6 // загрузка данных из файла в список
#define MENU_SAVE   7 // запись списка в файл
#define MENU_SEARCH 8 // извлечение информации из списка по указанным критериям
#define MENU_EXIT   9 // выход из программы

#define MAX_FILE_NAME 256

// проверка выхода индекса за границы диапазона
int check_index(unsigned count, unsigned index) {
	if(count == 0)
		return 0;
	if(index >= count) {
		printf("Индекс может быть от 0 до %u\n", count - 1);
		fflush(stdout);
		return 0;
	}
	return 1;
}

// точка входа программы
int main( int argc, char* argv[] )
{
	unsigned i, menu;
	book_t book;
	FILE* file;
	char file_name[MAX_FILE_NAME];
	bilist_t* books = bilist_create();
	do {
		printf("[%d]\tДобавление новой записи в список\n", MENU_CREATE);
		printf("[%d]\tРедактирование записи в списке\n", MENU_UPDATE);
		printf("[%d]\tУдаление записи из списка\n", MENU_DELETE);
		printf("[%d]\tСортировка списка\n", MENU_SORT);
		printf("[%d]\tВывод на экран содержимого списка\n", MENU_OUTPUT);
		printf("[%d]\tЗагрузка данных из файла в список\n", MENU_LOAD);
		printf("[%d]\tЗапись списка в файл\n", MENU_SAVE);
		printf("[%d]\tИзвлечение информации из списка по указанным критериям\n", MENU_SEARCH);
		printf("[%d]\tВыход\n", MENU_EXIT);
		printf("Выберите действие: ");
		fflush(stdout);
		scanf("%u", &menu);

		switch(menu) {
		case MENU_CREATE:
			bilist_push_back(books, input_book(stdin, TRUE));
			break;
		case MENU_UPDATE:
			printf("Введите индекс элемента: ");
			fflush(stdout);
			scanf("%u", &i);
			if(!check_index(bilist_size(books), i))
				break;
			bilist_set_data(books, bilist_item(books, i), input_book(stdin, TRUE), book_destroy);
			break;
		case MENU_DELETE:
			printf("Введите индекс элемента: ");
			fflush(stdout);
			scanf("%u", &i);
			if(!check_index(bilist_size(books), i))
				break;
			bilist_remove(books, bilist_item(books, i), book_destroy);
			break;
		case MENU_SORT:
			printf("Введите индекс поля, по которому будет выполняться поиск (0 - год, 1 - шифр, 2 - фамилия автора, 3 - название книги): ");
			fflush(stdout);
			scanf("%u", &i);
			switch(i) {
			case 0:
				bilist_bubble_sort(books, book_compare_by_year);
				bilist_print(books, print_book, stdout, "Список книг");
				break;
			case 1:
				bilist_bubble_sort(books, book_compare_by_code);
				bilist_print(books, print_book, stdout, "Список книг");
				break;
			case 2:
				bilist_bubble_sort(books, book_compare_by_author);
				bilist_print(books, print_book, stdout, "Список книг");
				break;
			case 3:
				bilist_bubble_sort(books, book_compare_by_name);
				bilist_print(books, print_book, stdout, "Список книг");
				break;
			default:
				printf("Номер может быть от 0 до 3\n");
			}
			fflush(stdout);
			break;
		case MENU_OUTPUT:
			bilist_print(books, print_book, stdout, "Список книг");
			break;
		case MENU_LOAD:
			printf("Введите имя файла: ");
			fflush(stdout);
			scanf ("%255s", file_name);
			file = fopen(file_name, "rt");
			if(!file) {
				fprintf(stderr, "Не удалось открыть файл \"%s\" для записи\n", file_name);
				break;
			}
			bilist_clear(books, book_destroy);
			bilist_read(books, read_book, file);
			fclose(file);
			break;
		case MENU_SAVE:
			printf("Введите имя файла: ");
			fflush(stdout);
			scanf ("%255s", file_name);
			file = fopen(file_name, "wt");
			if(!file) {
				fprintf(stderr, "Не удалось открыть файл \"%s\" для записи\n", file_name);
				break;
			}
			bilist_print(books, print_book, file, NULL);
			fclose(file);
			break;
		case MENU_SEARCH:
			printf("Введите индекс поля, по которому будет выполняться поиск (0 - год, 1 - шифр, 2 - фамилия автора, 3 - название книги): ");
			fflush(stdout);
			scanf("%u", &i);
			switch(i) {
			case 0:
				input_book_year(&book, stdin, TRUE);
				print_book(bilist_find_data(books, book_compare_year, &book.year), stdout);
				break;
			case 1:
				input_book_code(&book, stdin, TRUE);
				print_book(bilist_find_data(books, book_compare_code, book.code), stdout);
				break;
			case 2:
				input_book_author(&book, stdin, TRUE);
				print_book(bilist_find_data(books, book_compare_author, book.author), stdout);
				break;
			case 3:
				input_book_name(&book, stdin, TRUE);
				print_book(bilist_find_data(books, book_compare_name, book.name), stdout);
				break;
			default:
				printf("Номер может быть от 0 до 3\n");
			}
			break;
		case MENU_EXIT:
			printf("Завершение работы\n");
			break;
		default:
			printf("Неверное значение\n");
		}
		printf("---------------------------------------------------------------------\n");
	} while(menu != MENU_EXIT);

	bilist_destroy(books, book_destroy);
	return 0;
}
