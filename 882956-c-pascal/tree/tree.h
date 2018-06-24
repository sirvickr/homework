#pragma once

struct btree
{
	int value;
	struct btree *left, *right;
};

// Включение вершины в дерево
void Ins_Btree(int val, btree **q);
//Вывод содержимого дерева
void Print_Btree(btree *p, int w);
