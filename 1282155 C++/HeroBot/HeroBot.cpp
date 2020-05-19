// HeroBot.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <stdio.h>	// printf();
#include <stdlib.h> // system("cls"); srand();rand();
#include <time.h>   // time()
#include <conio.h> // _getch();
#include <Windows.h> // SetConsoleCursorPosition();

#include <iostream>

#define ESC 27
#define X_MIN 20
#define X_MAX 25
#define Y_MIN 10
#define Y_MAX 15

enum ConsoleColor
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};

void SetColor(ConsoleColor text, ConsoleColor background)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (WORD)((background << 4) | text));
}

void gotoXY(int x, int y)
{
	COORD p = { x, y };
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Получение дескриптора устройства стандартного вывода
	SetConsoleCursorPosition(hConsole, p); // Перемещение каретки по заданным координатам
}

void moveForward(int *u, int *v, int face, char key) {
	int x = *u, y = *v; // разименовывание переменных
	//стираем героя
	gotoXY(x, y); // устанавливаем курсор в (x,y)
	SetColor(Black, Black);
	printf("%c", face); // выводим символ цвета экрана
	switch (key) {
	case 'W':
	case 'w': if (y > Y_MIN) y--; break;
	case 'S':
	case 's': if (y < Y_MAX) y++; break;
	case 'A':
	case 'a': if (x > X_MIN) x--; break;
	case 'D':
	case 'd': if (x < X_MAX) x++; break;
	}
	// рисуем героя
	gotoXY(x, y);
	SetColor(Yellow, Black); // выводим символ желтого цвета 
	printf("%c", face);
	*u = x;
	*v = y;
}

int collision(int xG, int yG, int xB, int yB) {
	if ((abs(xG - xB) < 2) && (abs(yG - yB) < 2)) {
		return 1;
	}
	return 0;
}

void moveBot(int *u, int *v, int face, int dir) {
	int x = *u, y = *v;
	// стирае бота
	gotoXY(x, y); // устанавливаем курсор в (x,y)
	SetColor(Black, Black);
	printf("%c", face); // выводим символ цвета экрана
	switch (dir) {
	case 0: if (y > Y_MIN) y--; break;
	case 1: if (y < Y_MAX) y++; break;
	case 2: if (x > X_MIN) x--; break;
	case 3: if (x < X_MAX)x++; break;
	}
	// рисукм бота
	gotoXY(x, y);
	SetColor(LightMagenta, Black); // выводим символ цвета LightMagenta
	printf("%c", face);
	Sleep(200); // ждать 1/5 секунды
	*u = x;
	*v = y;
}

void drowPole() {
	for (int i = X_MIN - 1; i <= X_MAX + 1; i++) {
		gotoXY(i, Y_MIN - 1); // устанавливаем курсор в (x,y)
		SetColor(LightGreen, Black);
		printf("%c", 245); // выводим выводим верхнюю часть поля
		gotoXY(i, Y_MAX + 1); // устанавливаем курсор в (x,y)
		SetColor(LightGreen, Black);
		printf("%c", 245); // выводим выводим нижнюю часть поля
	}
	for (int i = Y_MIN - 1; i <= Y_MAX + 1; i++) {
		gotoXY(X_MIN - 1, i); // устанавливаем курсор в (x,y)
		SetColor(LightGreen, Black);
		printf("%c", 245); // выводим выводим верхнюю часть поля
		gotoXY(X_MAX + 1, i); // устанавливаем курсор в (x,y)
		SetColor(LightGreen, Black);
		printf("%c", 245); // выводим выводим нижнюю часть поля
	}
}

int main()
{
	srand((unsigned)time(NULL));
	drowPole();
	int xG, yG, faceG = 1;
	char key = 0;
	xG = rand() % (X_MAX - X_MIN + 1) + X_MIN; // задаём координату x
	yG = rand() % (Y_MAX - Y_MIN + 1) + Y_MIN; // задаём координату y
	gotoXY(xG, yG); // устанавливаем курсор в (x,y)
	SetColor(Yellow, Black);
	printf("%c", faceG); // выводим ГЕРОЯ
	int xB, yB, faceB = 2, dir; // direction;
	xB = rand() % (X_MAX - X_MIN + 1) + X_MIN; // задаём координату x
	yB = rand() % (Y_MAX - Y_MIN + 1) + Y_MIN; // задаём координату y
	gotoXY(xB, yB); // устанавливаем курсор в (x,y)
	SetColor(LightMagenta, Black);
	printf("%c", faceB); // выводим БОТА
	int life = 3;
	for (int i = 0; i < life; i++) {
		gotoXY(i + 2, 2); // устанавливаем курсор в (x,y)
		SetColor(LightRed, Black);
		printf("%c", 3); // выводим ЖИЗНЬ
	}
	do {
		drowPole();
		if (_kbhit()) {
			key = _getch(); //чтобы работал getch, нужно создать проект
			moveForward(&xG, &yG, faceG, key);	// вызов функции управляемого движения
		}
		dir = rand() % 4;
		moveBot(&xB, &yB, faceB, dir); // вызов функции неуправляемого движения
		if (collision(xG, yG, xB, yB)) {
			for (int i = 0; i < life; i++) {
				gotoXY(i + 2, 2); // устанавливаем курсор в (x,y)
				SetColor(Black, Black);
				printf("%c", 3); // выводим ЖИЗНЬ
			}
			life--;
			for (int i = 0; i < life; i++) {
				gotoXY(i + 2, 2); // устанавливаем курсор в (x,y)
				SetColor(LightRed, Black);
				printf("%c", 3); // выводим ЖИЗНЬ
			}
			gotoXY(xG, yG); // устанавливаем курсор в (x,y)
			SetColor(Black, Black);
			printf("%c", faceG); // стираем ГЕРОЯ
			gotoXY(xB, yB); // устанавливаем курсор в (x,y)
			SetColor(Black, Black);
			printf("%c", faceB); // стираем БОТА
			gotoXY(xB, yB); // устанавливаем курсор в (x,y)
			SetColor(LightRed, Black);
			printf("%c", 15); // рисуем взрыв
			Sleep(1000);
			gotoXY(xB, yB); // устанавливаем курсор в (x,y)
			SetColor(Black, Black);
			printf("%c", 15); // стираем взрыв
			xG = rand() % (X_MAX - X_MIN) + X_MIN; // задаём координату x
			yG = rand() % (Y_MAX - Y_MIN) + Y_MIN; // задаём координату y
			gotoXY(xG, yG); // устанавливаем курсор в (x,y)
			SetColor(Yellow, Black);
			printf("%c", faceG); // рисуем ГЕРОЯ
			xB = rand() % (X_MAX - X_MIN) + X_MIN; // задаём координату x
			yB = rand() % (Y_MAX - Y_MIN) + Y_MIN; // задаём координату y
			gotoXY(xB, yB); // устанавливаем курсор в (x,y)
			SetColor(LightMagenta, Black);
			printf("%c", faceB); // рисуем БОТА
		}
	} while (key != ESC && life != 0);
	system("cls");
	gotoXY(20, 10); // устанавливаем курсор в (x,y)
	SetColor(LightRed, Black);
	printf("you lose"); // стираем взрыв
	Sleep(3000);
	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
