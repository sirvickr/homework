//---------------------------------------------------------------------------
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#pragma hdrstop
#include "dict.h"
#include "menu.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

// ===================== Функции меню =======================================
// Функция меню <Изменить>
int Edit(MENU* menu) {
	//printf("Edit\n");
	return 0;
}
// Функция меню <Поиск>
int Search(MENU* menu) {
	//printf("Search\n");
	return 0;
}
// Функция меню <Сортировка>
int Sort(MENU* menu) {
	//printf("Sort\n");
	return 0;
}
// Функция меню <Сохранить>
int Save(MENU* menu) {
	//printf("Save\n");
	return 0;
}
// Функция меню <Помощь>. Заполняется кодом пользователя
int Help(MENU* menu) {
	//printf("Help\n");
	return 0;
}
// Функция меню <Выход>. Заполняется кодом пользователя
int Exit(MENU* menu) {
	/*int resp;
	//cout << "Вы уверены, что хотите выйти з программы (y/n)?";
	printf("Quit? (y/n)?");
	resp = getchar();
	if (resp == 'y' || resp == 'Y') {
		return -1;
	} */
	return 0;
}
// Функция меню <Очистить>. Заполняется кодом пользователя
/*int Clear(MENU* menu)
{
	cls(menu, 0);
	menu->curspos.X = 0;
	menu->curspos.Y = 1;
	return 1;
}*/

