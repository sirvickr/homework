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

class GameObject {
public:
	GameObject(ConsoleColor color, int face, int x = 0, int y = 0): mColor(color), mFace(face), mX(x), mY(y) {
	}
	int x() const {
		return mX;
	}
	void x(int value) {
		mX = value;
	}
	int y() const {
		return mY;
	}
	void y(int value) {
		mY = value;
	}
	void locate(int x, int y) {
		mX = x;
		mY = y;
	}
	void show() {
		gotoXY(mX, mY); // устанавливаем курсор в (x,y)
		SetColor(mColor, Black);
		printf("%c", mFace); // выводим символ цвета экрана
	}
	void hide() {
		gotoXY(mX, mY); // устанавливаем курсор в (x,y)
		SetColor(Black, Black);
		printf("%c", mFace); // выводим символ цвета экрана
	}
	void move(int dir) {
		// стираем объект
		hide();
		switch (dir) {
		case 0: if (mY > Y_MIN) mY--; break;
		case 1: if (mY < Y_MAX) mY++; break;
		case 2: if (mX > X_MIN) mX--; break;
		case 3: if (mX < X_MAX) mX++; break;
		}
		// рисуем объект
		show();
	}
	void moveKey(char key) {
		int dir = -1;
		switch (key) {
		case 'W': case 'w': dir = 0;/*if (y > Y_MIN) y--;*/ break;
		case 'S': case 's': dir = 1;/*if (y < Y_MAX) y++;*/ break;
		case 'A': case 'a': dir = 2;/*if (x > X_MIN) x--;*/ break;
		case 'D': case 'd': dir = 3;/*if (x < X_MAX) x++;*/ break;
		}
		move(dir);
	}

private:
	ConsoleColor mColor;
	int mFace;
	int mX;
	int mY;
};

int collision(const GameObject& a, const GameObject& b) {
	if ((abs(a.x() - b.x()) < 2) && (abs(a.y() - b.y()) < 2)) {
		return 1;
	}
	return 0;
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

	int xB, yB, faceB = 2, dir; // direction;
	xB = rand() % (X_MAX - X_MIN + 1) + X_MIN; // задаём координату x
	yB = rand() % (Y_MAX - Y_MIN + 1) + Y_MIN; // задаём координату y

	GameObject hero(Yellow, faceG, xG, yG);
	hero.show();
	GameObject bot(LightMagenta, faceB, xB, yB);
	bot.show();
	GameObject explosion(LightRed, 15);

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
			hero.moveKey(key);	// вызов функции управляемого движения
		}
		dir = rand() % 4;
		bot.move(dir);
		Sleep(200); // ждать 1/5 секунды
		if (collision(hero, bot)) {
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

			hero.hide();

			bot.hide();
			
			explosion.x(bot.x());
			explosion.y(bot.y());
			// рисуем взрыв
			explosion.show();
			Sleep(1000);
			// стираем взрыв
			explosion.hide();

			hero.x(rand() % (X_MAX - X_MIN) + X_MIN); // задаём координату x
			hero.y(rand() % (Y_MAX - Y_MIN) + Y_MIN); // задаём координату y
			hero.show();

			bot.x(rand() % (X_MAX - X_MIN) + X_MIN); // задаём координату x
			bot.y(rand() % (Y_MAX - Y_MIN) + Y_MIN); // задаём координату y
			bot.show();
		}
	} while (key != ESC && life != 0);
	system("cls");
	gotoXY(20, 10); // устанавливаем курсор в (x,y)
	SetColor(LightRed, Black);
	printf("you lose"); // стираем взрыв
	Sleep(3000);
	return 0;
}
