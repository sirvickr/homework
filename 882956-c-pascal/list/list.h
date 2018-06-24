#pragma once

struct List
{
	int value;
	List *next;
	List(int val = 0, List *p = nullptr) //�����������
	{
		value = val;
		next = p;
	}
};

//��������� �������� � ������
List *Ins_first(int n, List *head);
//��������� �������� � �����
List *Add_last(int n, List *head);
//��������� �� ��������
List*Ins_Sort(int n, List *head);
// ��������� �� ������
List* Ins_Number(int Num, int n, List *head);
// �������� ������
List* Creat_list();
// �������������� �������� ������
List* Creat_random_list(int count);
// �������� �� ������
List* Del_first(int &n, List *head);
// �������� �� �����
List* Del_last(int &n, List *head);
// �������� �� ��������
List* Del_value(int n, List *head);
// �������� �� ������
List* Del_Number(int Num, List*head);
//������ ������
void Print_list(List *head);
//�������� ������ �� ��������� ���� ������ �������
int Concat_lists(List* L1, List* L2, List** L3);
