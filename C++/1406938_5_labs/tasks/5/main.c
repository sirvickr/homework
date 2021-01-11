/* Написать программу, выполняющую следующие действия:
   1. Создание и ввод элементов массива структур 
   2. Установка/получение значений полей элемента массива структур по заданному индексу в массиве (индекс вводится пользователем). '
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_NAME_SIZE 256

#define MENU_OUTPUT 1
#define MENU_ACTION 2
#define MENU_SEARCH 3
#define MENU_SWAP   4
#define MENU_SORT   5
#define MENU_EXIT   6

typedef struct ticket_t {
	unsigned num;
	char name[MAX_NAME_SIZE];
	float price;
} ticket_t;

void input_ticket_num(ticket_t* ticket) {
	printf("Введите номер билета: ");
	scanf("%u", &ticket->num);
}

void input_concert_name(ticket_t* ticket) {
	printf("Введите название спектакля: ");
	scanf ("%255s", ticket->name); 
}

void input_ticket_price(ticket_t* ticket) {
	printf("Введите цену билета: ");
	scanf("%f", &ticket->price);
}

void input_ticket(ticket_t* ticket) {
	input_ticket_num(ticket);
	input_concert_name(ticket);
	input_ticket_price(ticket);
}

ticket_t* create_tickets(unsigned count) {
	ticket_t* tickets = (ticket_t*)malloc(count * sizeof(ticket_t));
	for(unsigned i = 0; i < count; i++) {
		input_ticket(&tickets[i]);
	}
	return tickets;
}

void destroy_tickets(ticket_t* tickets) {
	free(tickets);
}

void print_ticket(const ticket_t* ticket) {
	if(ticket) {
		printf("%u\t\"%s\" (%.2f рублей)\n", ticket->num, ticket->name, ticket->price);
	} else {
		printf("Билет не найден\n");
	}
}

void print_tickets(ticket_t* tickets, unsigned from, unsigned to) {
	for(unsigned i = from; i < to; i++) {
		print_ticket(&tickets[i]);
	}
}

ticket_t* find_ticket_by_num(ticket_t* tickets, unsigned count, unsigned num) {
	for(unsigned i = 0; i < count; i++) {
		if(tickets[i].num == num) {
			return &tickets[i];
		}
	}
	return NULL;
}

ticket_t* find_ticket_by_name(ticket_t* tickets, unsigned count, const char* name) {
	for(unsigned i = 0; i < count; i++) {
		if(strcmp(tickets[i].name, name) == 0) {
			return &tickets[i];
		}
	}
	return NULL;
}

ticket_t* find_ticket_by_price(ticket_t* tickets, unsigned count, float price) {
	for(unsigned i = 0; i < count; i++) {
		if(tickets[i].price == price) {
			return &tickets[i];
		}
	}
	return NULL;
}

// обмен двух билетов местами

void swap_tickets(ticket_t* a, ticket_t* b) {
	ticket_t c;
	memcpy(&c, a, sizeof(ticket_t));
	memcpy(a, b, sizeof(ticket_t));
	memcpy(b, &c, sizeof(ticket_t));
}

// сортировка пузырьком

typedef float (*compare_t)(const ticket_t* a, const ticket_t* b);

void bubble_sort(ticket_t* tickets, unsigned count, compare_t compare) {
	for(unsigned bound = count - 1; bound > 0; bound--) {
		for(unsigned i = 0; i < bound; i++) {
			if(compare(&tickets[i], &tickets[i + 1]) > 0) {
				swap_tickets(&tickets[i], &tickets[i + 1]);
			}
		}
	}
}

// функции обратного вызова для сортировки

float ticket_compare_by_num(const ticket_t* a, const ticket_t* b) {
	return (int)a->num - (int)b->num;
}

float ticket_compare_by_name(const ticket_t* a, const ticket_t* b) {
	return strcmp(a->name, b->name);
}

float ticket_compare_by_price(const ticket_t* a, const ticket_t* b) {
	return a->price - b->price;
}

int check_index(unsigned count, unsigned index) {
	if(count == 0)
		return 0;
	if(index >= count) {
		printf("Индекс может быть от 0 до %u\n", count - 1);
		return 0;
	}
	return 1;
}

// точка входа программы

int main( int argc, char* argv[] )
{
	unsigned i, j, n, menu, action;
	ticket_t ticket;
	ticket_t* ptr = NULL;
	printf("Введите количество билетов: ");
	scanf("%u", &n);
	if(n == 0)
		return 0;
	ticket_t* tickets = create_tickets(n);
	do {
		printf("[%d]\tВывод данных\n", MENU_OUTPUT);
		printf("[%d]\tРабота с данными выбранного билета\n", MENU_ACTION);
		printf("[%d]\tПоиск\n", MENU_SEARCH);
		printf("[%d]\tОбмен двух значений\n", MENU_SWAP);
		printf("[%d]\tСортировка билетов\n", MENU_SORT);
		printf("[%d]\tВыход\n", MENU_EXIT);
		printf("Выберите действие: ");
		scanf("%u", &menu);
		switch(menu) {
		case MENU_OUTPUT:
			printf("Введите индекс начала диапазона: ");
			scanf("%u", &i);
			if(!check_index(n, i))
				break;
			printf("Введите индекс конца диапазона: ");
			scanf("%u", &j);
			if(!check_index(n, j))
				break;
			if(i >= j) {
				printf("Ошибка: начало диапазона должно быть меньше конца диапазона!\n");
				break;
			}
			print_tickets(tickets, i, j);
			break;
		case MENU_ACTION:
			printf("Введите индекс элемента: ");
			scanf("%u", &i);
			if(!check_index(n, i))
				break;
			printf("Введите номер операции, 1 – установка, 2 – получение: ");
			scanf("%u", &action);
			switch(action) {
			case 1:
				input_ticket(&tickets[i]);
				break;
			case 2:
				print_ticket(&tickets[i]);
				break;
			default:
				printf("Неверное значение\n");
			}
			break;
		case MENU_SEARCH:
			printf("Введите индекс поля, по которому будет выполняться поиск (0 - номер, 1 - название, 2 - цена): ");
			scanf("%u", &i);
			switch(i) {
			case 0:
				input_ticket_num(&ticket);
				print_ticket(find_ticket_by_num(tickets, n, ticket.num));
				break;
			case 1:
				input_concert_name(&ticket);
				print_ticket(find_ticket_by_name(tickets, n, ticket.name));
				break;
			case 2:
				input_ticket_price(&ticket);
				print_ticket(find_ticket_by_price(tickets, n, ticket.price));
				break;
			default:
				printf("Номер может быть от 0 до 2\n");
			}
			break;
		case MENU_SWAP:
			printf("Введите индекс первого элемента: ");
			scanf("%u", &i);
			if(!check_index(n, i))
				break;
			printf("Введите индекс второго элемента: ");
			scanf("%u", &j);
			if(!check_index(n, j))
				break;
			if(i == j) {
				printf("Ошибка: индексы не должны совпадать!\n");
				break;
			}
			swap_tickets(&tickets[i], &tickets[j]);
			print_tickets(tickets, 0, n);
			break;
		case MENU_SORT:
			printf("Введите индекс поля, по которому будет выполняться сортировка (0 - номер, 1 - название, 2 - цена): ");
			scanf("%u", &i);
			switch(i) {
			case 0:
				bubble_sort(tickets, n, ticket_compare_by_num);
				print_tickets(tickets, 0, n);
				break;
			case 1:
				bubble_sort(tickets, n, ticket_compare_by_name);
				print_tickets(tickets, 0, n);
				break;
			case 2:
				bubble_sort(tickets, n, ticket_compare_by_price);
				print_tickets(tickets, 0, n);
				break;
			default:
				printf("Номер может быть от 0 до 2\n");
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

	destroy_tickets(tickets);
	return 0;
}
