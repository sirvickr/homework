#pragma once

struct btree
{
	int value;
	struct btree *left, *right;
};

// ��������� ������� � ������
void Ins_Btree(int val, btree **q);
//����� ����������� ������
void Print_Btree(btree *p, int w);
