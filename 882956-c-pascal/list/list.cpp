#include <iostream>
#include <string>
#include "list.h"

using namespace std;

//включение элемента в начало
List *Ins_first(int n, List *head)
{
	List *q = new List(n, head);
	return q;
}
//включение элемента в конец
List *Add_last(int n, List *head)
{
	List *q = new List(n), *p = head;
	// если список пуст
	if (head == nullptr) return q;
	// «идем в конец списка»
	while (p->next != nullptr)
		p = p->next;
	p->next = q;
	return head;
}
//включение по значению
List*Ins_Sort(int n, List *head)
{
	List *q = new List(n), *p = head;
	if (head == nullptr) return q;
	// включение в начало
	if (n < head->value)
	{
		q->next = head;
		return q;
	}
	// включение в «середину»
	while (p->next != nullptr)
		if (n < p->next->value)
		{
			q->next = p->next;
			p->next = q;
			break;
		}
		else
			p = p->next;
	// включаем последним
	p->next = q;
	return head;
}
// включение по номеру
List* Ins_Number(int Num, int n, List *head)
{
	List *p = head, *q = new List(n);
	int i = 1;
	if (head == nullptr) return q;
	// включаем по номеру «1»
	if (Num == 1)
	{
		q->next = head;
		return q;
	}
	// ищем позицию для включения
	while (p->next != nullptr)
		if (Num == i + 1)
		{
			q->next = p->next;
			p->next = q;
			return head;
		}
		else
		{
			i++;
			p = p->next;
		}
	// включить последним
	if (Num == i + 1)
		p->next = q;
	else
		cout << "NO THIS NUMBER" << endl;
	return head;
}
// создание списка
List* Creat_list()
{
	string str;
	List *head = nullptr;
	cout << "Creatlist.Enter numbers: ";
#if 0
	for (;;)
	{
		gets(str);
		if (!str[0]) break;
		head = Ins_first(atoi(str), head);
	}
#else
	do
	{
		cin >> str;
		head = Ins_first(atoi(str.c_str()), head);
	} while (!str.empty());
#endif
	return head;
}
// автоматическое создание списка
List* Creat_random_list(int count)
{
	string str;
	List *head = nullptr;
	for (int i = 0; i < count; i++)
	{
		head = Ins_first(1 + (rand() % 10), head);
	}
	return head;
}
// удаление из начала
List* Del_first(int &n, List *head)
{
	List *p = head;
	if (p == nullptr)
		cout << "LIST EMPTY!" << endl;
	else
	{
		n = p->value;
		head = head->next;
		delete p;
	}
	return head;
}
// удаление из конца
List* Del_last(int &n, List *head)
{
	List *p = head;
	if (p == nullptr)
	{
		cout << "LIST EMPTY!" << endl;
		return nullptr;
	}
	// если один элемент в списке
	if (head->next == nullptr)
	{
		n = head->value;
		delete head;
		return nullptr;
	}
	// переходим на предпоследний элемент
	while (p->next->next != nullptr)
		p = p->next;
	// запоминаем удаляемое значение
	n = p->next->value;
	delete p->next;
	p->next = nullptr;
	return head;
}
// удаление по значению
List* Del_value(int n, List *head)
{
	List *p = head, *t;
	if (head == nullptr)
	{
		cout << "LIST EMPTY!" << endl;
		return nullptr;
	}
	// удаляем первый элемент
	if (head->value == n)
	{
		t = head;
		head = head->next;
		delete t;
		return head;
	}
	// ищем в списке значение
	while (p->next != nullptr)
		if (p->next->value == n)
		{
			t = p->next;
			p->next = p->next->next;
			delete t;
			return head;
		}
		else p = p->next;
		cout << "NO VALUE!" << endl;
		return head;
}
// удаление по номеру
List* Del_Number(int Num, List*head)
{
	List *p = head, *t;
	int i = 1;
	if (head == nullptr)
	{
		cout << "LIST EMPTY!" << endl;
		return nullptr;
	}
	if (Num == 1)
	{
		t = head;
		head = head->next;
		delete t;
		return head;
	}
	while (p->next != nullptr)
		if (Num == i + 1)
		{
			t = p->next;
			p->next = p->next->next;
			delete t;
			return head;
		}
		else
		{
			i++;
			p = p->next;
		}
	cout << "NO THIS NUMBER" << endl;
	return head;
}
//печать списка
void Print_list(List *head)
{
	List *p = head;
	cout << "PRINT LIST:";
	if (p == nullptr)
		cout << " List empty!";
	else
		while (p != nullptr)
		{
			cout << " " << p->value;
			p = p->next;
		}
	cout << endl;
}
//создание списка из элементов двух других списков
int Concat_lists(List* L1, List* L2, List** L3)
{
	int length = 0;
	List *p = L1;
	while (p)
	{
		*L3 = Add_last(p->value, *L3);
		p = p->next;
		length++;
	}
	p = L2;
	while (p)
	{
		*L3 = Add_last(p->value, *L3);
		p = p->next;
		length++;
	}
	return length;
}
