#include <iostream>
#include <string>
#include "list.h"

using namespace std;

//��������� �������� � ������
List *Ins_first(int n, List *head)
{
	List *q = new List(n, head);
	return q;
}
//��������� �������� � �����
List *Add_last(int n, List *head)
{
	List *q = new List(n), *p = head;
	// ���� ������ ����
	if (head == nullptr) return q;
	// ����� � ����� ������
	while (p->next != nullptr)
		p = p->next;
	p->next = q;
	return head;
}
//��������� �� ��������
List*Ins_Sort(int n, List *head)
{
	List *q = new List(n), *p = head;
	if (head == nullptr) return q;
	// ��������� � ������
	if (n < head->value)
	{
		q->next = head;
		return q;
	}
	// ��������� � ���������
	while (p->next != nullptr)
		if (n < p->next->value)
		{
			q->next = p->next;
			p->next = q;
			break;
		}
		else
			p = p->next;
	// �������� ���������
	p->next = q;
	return head;
}
// ��������� �� ������
List* Ins_Number(int Num, int n, List *head)
{
	List *p = head, *q = new List(n);
	int i = 1;
	if (head == nullptr) return q;
	// �������� �� ������ �1�
	if (Num == 1)
	{
		q->next = head;
		return q;
	}
	// ���� ������� ��� ���������
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
	// �������� ���������
	if (Num == i + 1)
		p->next = q;
	else
		cout << "NO THIS NUMBER" << endl;
	return head;
}
// �������� ������
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
// �������������� �������� ������
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
// �������� �� ������
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
// �������� �� �����
List* Del_last(int &n, List *head)
{
	List *p = head;
	if (p == nullptr)
	{
		cout << "LIST EMPTY!" << endl;
		return nullptr;
	}
	// ���� ���� ������� � ������
	if (head->next == nullptr)
	{
		n = head->value;
		delete head;
		return nullptr;
	}
	// ��������� �� ������������� �������
	while (p->next->next != nullptr)
		p = p->next;
	// ���������� ��������� ��������
	n = p->next->value;
	delete p->next;
	p->next = nullptr;
	return head;
}
// �������� �� ��������
List* Del_value(int n, List *head)
{
	List *p = head, *t;
	if (head == nullptr)
	{
		cout << "LIST EMPTY!" << endl;
		return nullptr;
	}
	// ������� ������ �������
	if (head->value == n)
	{
		t = head;
		head = head->next;
		delete t;
		return head;
	}
	// ���� � ������ ��������
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
// �������� �� ������
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
//������ ������
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
//�������� ������ �� ��������� ���� ������ �������
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
