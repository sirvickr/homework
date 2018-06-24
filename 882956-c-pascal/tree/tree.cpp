#include <iostream>
#include <iomanip>
#include "tree.h"

// ��������� ������� � ������
void Ins_Btree(int val,	btree **q)
{
	if (*q == NULL) {
		// ����� ����� ��� ����������
		*q = new btree;
		(*q)->left = NULL;
		(*q)->right = NULL;
		(*q)->value = val;
		return;
	}
	if ((*q)->value > val)
		// ��������� � ����� ���������
		Ins_Btree(val, &(*q)->left);
	else
		// ��������� � ������ ���������
		Ins_Btree(val, &(*q)->right);
}

// ����� ����������� ������
void Print_Btree(btree *p, int w)
{
	if (p == NULL)
		return;
	Print_Btree(p->left, w);
	std::cout << " " << std::setw(w) << p->value;
	Print_Btree(p->right, w);
}
