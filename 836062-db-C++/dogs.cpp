#include <stdio.h>
#include <iostream>
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
    int flag = -1;                                                                  // вспомогательный флаг, с помошьдю которого поймем, нашли ли мы нужную собаку
    cout << endl << "Введите кличку собаки информацию о которой нужно изменить: ";  // вводим имя нужного собаки
    cin.sync();
    cin.getline(str, 50);
    for (int i = 0; i < current; i++) {                                             // проходим по всей базе
        cout << dogs_database[i].name;
        if ((strcmp(str, dogs_database[i].name) == 0)) {                            // сравниваем введенную строку с текущей
            flag = i;                                                               // если находим совпадение, то запоминаем его индекс
        }
    }
    if (flag == -1) {                                                               // проверяем, нашли ли собаку
        cout << "Собака не найдена" << endl;                                       // иначе пишем, что не нашли
    } else {
    cout << "Введите новую породу собаки: ";                                        // считываем все поля, заносим в структуру
    cin.sync();
    cin.getline(dogs_database[current].kind, 50);
    cout << "Введите новую кличку: ";
    cin.sync();
    cin.getline(dogs_database[current].name, 50);
    cout << "Введите новую дату рождения: ";
    cin >> dogs_database[current].date;
    cout << "Введите новую информацию о медалях(золото, серебро, бронза, через пробел): ";
    cin.sync();
    cin.getline(dogs_database[current].medals, 50);
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
    FILE *file = fopen(buf, "r");                                                     // открываем файл
    while (!feof(file)) {                                                           // читаем информацию из файла
        fscanf(file, "%s\n", dogs_database[current].kind);
        fscanf(file, "%s\n", dogs_database[current].name);
        fscanf(file, "%d\n", &dogs_database[current].date);
        fscanf(file, "%s\n", dogs_database[current].medals);
        cout << dogs_database[current].kind << " " << current << endl;
        cout << dogs_database[current].name << " " << current << endl;
        cout << dogs_database[current].date << " " << current << endl;
        cout << dogs_database[current].medals << " " << current << endl;
        current++;                                                                  // увеличиваем кол-во записей
    }
    fclose(file);                                                                   // закрываем файл
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
        avg = avg + 2018 - dogs_database[i].date;                                   // считаем кол-во лет для каждой собаки(текущий год - дата рождения), суммируем
        cout << dogs_database[i].date << " " << current << endl;
    }
    res = avg / current;                                                            // считаем среднее аримфметическое, выводим
    cout << "Средний возраст равен: " << res;
}


int main() {
    //system("chcp 1251");                        // для поддержки русского языка в консоли
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
    }
    return 0;
}
