#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

using namespace std;

struct dog {                // структура описывающая базу данных о собаках
    char kind[100];
    char name[100];
    int date;
    char medals[100];
};

#define N 30                // максимальное число записей в базе данных
dog dogs_database[N];
int current = 0;

void newDog() {                                                    // добавление собаки
    cout << "Введите породу собаки: ";                              // считываем все поля, заносим в базу
    cin.sync();
    cin.getline(dogs_database[current].kind, 50);
    cout << "Введите кличку: ";
    cin.sync();
    cin.getline(dogs_database[current].name, 50);
    cout << "Введите год рождения: ";
    cin >> dogs_database[current].date;
    cout << "Введите информация о медалях(золото, серебро, бронза, через пробел): ";
    cin.sync();
    cin.getline(dogs_database[current].medals, 50);
    current++;
}

void Edit() {                                                           // редактирование информации
    char str[100];
    cout << endl << "Введите кличку собаки информацию о которой нужно изменить: ";  // вводим имя нужного собаки
    cin.sync();
    cin.getline(str, 50);
    int i;
    for (i = 0; i < current; i++) {                                             // проходим по всей базе
        cout << dogs_database[i].name;
        if ((strcmp(str, dogs_database[i].name) == 0)) {                            // сравниваем введенную строку с текущей
            break;                                                               // если находим совпадение, сразу выходим
        }
    }
    if (i < current) {                                                               // проверяем, нашли ли собаку
	    cout << "Введите новую породу собаки: ";                                     // считываем все поля, заносим в структуру
	    cin.sync();
	    cin.getline(dogs_database[i].kind, 50);
	    cout << "Введите новую кличку: ";
	    cin.sync();
	    cin.getline(dogs_database[i].name, 50);
	    cout << "Введите новую дату рождения: ";
	    cin >> dogs_database[i].date;
	    cout << "Введите новую информацию о медалях(золото, серебро, бронза, через пробел): ";
	    cin.sync();
	    cin.getline(dogs_database[i].medals, 50);
	} else {
	    cout << "Собака не найдена" << endl;                                       // иначе пишем, что не нашли
    }
}

void deleteDog() {                                                                 // удаление информации о собаке
    char str[100];
    cout << endl << "Введите кличку собаки инфоромацию о которой необходимо удалить: ";
    cin.sync();
    cin.getline(str, 50);
    for (int i = 0; i < current; i++) {                                             // проходим по всей базе
        if (strcmp(str ,dogs_database[i].name) == 0) {                              // аналогично ищем собаку, информацию о которой нам нужно удалить
            for (int j = i; j < current - 1; j++)                                   // если нашли, то сдвигаем весь массив на элемент влево
                dogs_database[j] = dogs_database[j + 1];
            current--;                                                              // уменьшаем кол-во
        }
    }
}

void fileOut() {
    cout << endl << "Введите имя файла для сохранения информации: " << endl;        // вводим имя файла для записи
    char buf[30];
    cin >> buf;
    FILE *file = fopen(buf, "a+");                                                     // открываем файл
    for (int i = 0; i < current; i++) {                                             // выводим в файл информацию о каждой собаке
        fprintf(file, "%s\n", dogs_database[i].kind);
        fprintf(file, "%s\n", dogs_database[i].name);
        fprintf(file, "%d\n", dogs_database[i].date);
        fprintf(file, "%s\n", dogs_database[i].medals);
    }
    fclose(file);                                                                   // закрываем файл
}

void fileIn() {
    char buf[30];
    cout << endl << "Введите имя файла для считывания информации: " << endl;
    cin >> buf;
    ifstream file(buf, ios_base::in);                                              // открываем файл
    if(file) {
    	string line;
    	int index = 0;
    	while(getline(file, line)) {                                              // читаем информацию из файла
    		istringstream iss(line);
			index++;
    		switch(index) {
			case 1:
	    		strcpy(dogs_database[current].kind, line.c_str());
    			cout << " kind = '" << dogs_database[current].kind << "'" << endl;
				break;
			case 2:
	    		strcpy(dogs_database[current].name, line.c_str());
    			cout << " name = '" << dogs_database[current].name << "'" << endl;
				break;
			case 3:
	    		iss >> dogs_database[current].date;
    			cout << " date = '" << dogs_database[current].date << "'" << endl;
				break;
			case 4:
	    		strcpy(dogs_database[current].medals, line.c_str());
				current++;
				index = 0;
				break;
			}
		} // while()
		cout << "#\tkind\tname\tdate\tmedals" << endl;
	    for (int i = 0; i < current; i++) {                                             // проходим по базе данных
   			cout << "[" << i << "]\t" << dogs_database[i].kind 
			   << "\t" << dogs_database[i].name 
			   << "\t" << dogs_database[i].date 
			   << "\t" << dogs_database[i].medals 
			   << endl;
	    }
	}
}

void List() {                                                                  // показать список собак определенной породы с золотыми медалями
    char buf[30];
    cout << endl << "Введите породу собаки: ";
    cin.sync();
    cin.getline(buf, 30);
    for (int i = 0; i < current; i++) {                                             // проходим по базе данных
        if ((strcmp(buf, dogs_database[i].kind) == 0)) {                               // если у текущей собаки порода совпадает с введенной, то
            if (strstr(dogs_database[i].medals, "золото") != NULL) {                // проверяем есть ли у неё золотые медали, если есть - выводим информацию о ней на экран
                cout << dogs_database[i].kind << " ";
                cout << dogs_database[i].name << " ";
                cout << dogs_database[i].date << " ";
                cout << dogs_database[i].medals << endl;
            }
        }
    }
}

void solveTask() {                                                                // считаем средний возраст
    int avg = 0;
    float res = 0;
    for (int i = 0; i < current; i++) {                                             // проходим по базе
    	int age = 2018 - dogs_database[i].date;         // считаем кол-во лет для каждой собаки(текущий год - дата рождения)
        avg += age;                                   // суммируем
        cout << dogs_database[i].date << " " << age << endl;
    }
    res = float(avg) / current;                                                            // считаем среднее аримфметическое, выводим
    cout << "Средний возраст равен: " << res;
}

int main() {
	int counter = 0;
    system("chcp 1251");                        // для поддержки русского языка в консоли
    setlocale(LC_ALL, "RUS");                   // установка русского языка
    int menu = 0;
    while (menu != 8) {
        cout << endl << endl;                                                           // меню
        cout << "[1] Добавить информацию о собаке" << endl;                             // добавить запись о собаке в базу данных
        cout << "[2] Редактировать информацию о собаке" << endl;                        // изменить запись о собаке
        cout << "[3] Удалить информацию о собаке" << endl;                              // удалить собаке
        cout << "[4] Записать в файл" << endl;                                          // запись в файл
        cout << "[5] Считать из файла" << endl;                                         // чтение из файла
        cout << "[6] Показать список собак определенной породы с золотыми медалями" << endl;       // показать список с заданными параметрами
        cout << "[7] Подсчититать средний возраст" << endl;    // по заданию
        cout << "[8] Выход" << endl << endl;                                            // завершить программу
        cout << "Ваш выбор: ";
        cin.sync();
        cin >> menu;                                                                    // считываем выбор пользователя

        if (menu == 1) {
            newDog();
        }
        if (menu == 2) {
            Edit();
        }
        if (menu == 3) {
            deleteDog();
        }
        if (menu == 4) {
            fileOut();
        }
        if (menu == 5) {
            fileIn();
        }
        if (menu == 6) {
            List();
        }
        if (menu == 7) {
            solveTask();
        }
        if(counter++ == 10) {
        	cout << "emergency exit" << endl;
        	break;
		}
    }
    return 0;
}
