#pragma once

struct List
{
	int value;
	List *next;
	List(int val = 0, List *p = nullptr) //конструктор
	{
		value = val;
		next = p;
	}
};

//включение элемента в начало
List *Ins_first(int n, List *head);
//включение элемента в конец
List *Add_last(int n, List *head);
//включение по значению
List*Ins_Sort(int n, List *head);
// включение по номеру
List* Ins_Number(int Num, int n, List *head);
// создание списка
List* Creat_list();
// автоматическое создание списка
List* Creat_random_list(int count);
// удаление из начала
List* Del_first(int &n, List *head);
// удаление из конца
List* Del_last(int &n, List *head);
// удаление по значению
List* Del_value(int n, List *head);
// удаление по номеру
List* Del_Number(int Num, List*head);
//печать списка
void Print_list(List *head);
//создание списка из элементов двух других списков
int Concat_lists(List* L1, List* L2, List** L3);
