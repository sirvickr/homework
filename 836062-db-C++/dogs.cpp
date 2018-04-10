#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

using namespace std;

struct dog {                // ��������� ����������� ���� ������ � �������
    char kind[100];
    char name[100];
    int date;
    char medals[100];
};

#define N 30                // ������������ ����� ������� � ���� ������
dog dogs_database[N];
int current = 0;

void newDog() {                                                    // ���������� ������
    cout << "������� ������ ������: ";                              // ��������� ��� ����, ������� � ����
    cin.sync();
    cin.getline(dogs_database[current].kind, 50);
    cout << "������� ������: ";
    cin.sync();
    cin.getline(dogs_database[current].name, 50);
    cout << "������� ��� ��������: ";
    cin >> dogs_database[current].date;
    cout << "������� ���������� � �������(������, �������, ������, ����� ������): ";
    cin.sync();
    cin.getline(dogs_database[current].medals, 50);
    current++;
}

void Edit() {                                                           // �������������� ����������
    char str[100];
    int flag = -1;                                                                  // ��������������� ����, � �������� �������� ������, ����� �� �� ������ ������
    cout << endl << "������� ������ ������ ���������� � ������� ����� ��������: ";  // ������ ��� ������� ������
    cin.sync();
    cin.getline(str, 50);
    for (int i = 0; i < current; i++) {                                             // �������� �� ���� ����
        cout << dogs_database[i].name;
        if ((strcmp(str, dogs_database[i].name) == 0)) {                            // ���������� ��������� ������ � �������
            flag = i;                                                               // ���� ������� ����������, �� ���������� ��� ������
        }
    }
    if (flag == -1) {                                                               // ���������, ����� �� ������
        cout << "������ �� �������" << endl;                                       // ����� �����, ��� �� �����
    } else {
    cout << "������� ����� ������ ������: ";                                        // ��������� ��� ����, ������� � ���������
    cin.sync();
    cin.getline(dogs_database[current].kind, 50);
    cout << "������� ����� ������: ";
    cin.sync();
    cin.getline(dogs_database[current].name, 50);
    cout << "������� ����� ���� ��������: ";
    cin >> dogs_database[current].date;
    cout << "������� ����� ���������� � �������(������, �������, ������, ����� ������): ";
    cin.sync();
    cin.getline(dogs_database[current].medals, 50);
    }
}

void deleteDog() {                                                                 // �������� ���������� � ������
    char str[100];
    cout << endl << "������� ������ ������ ����������� � ������� ���������� �������: ";
    cin.sync();
    cin.getline(str, 50);
    for (int i = 0; i < current; i++) {                                             // �������� �� ���� ����
        if (strcmp(str ,dogs_database[i].name) == 0) {                              // ���������� ���� ������, ���������� � ������� ��� ����� �������
            for (int j = i; j < current - 1; j++)                                   // ���� �����, �� �������� ���� ������ �� ������� �����
                dogs_database[j] = dogs_database[j + 1];
            current--;                                                              // ��������� ���-��
        }
    }
}

void fileOut() {
    cout << endl << "������� ��� ����� ��� ���������� ����������: " << endl;        // ������ ��� ����� ��� ������
    char buf[30];
    cin >> buf;
    FILE *file = fopen(buf, "a+");                                                     // ��������� ����
    for (int i = 0; i < current; i++) {                                             // ������� � ���� ���������� � ������ ������
        fprintf(file, "%s\n", dogs_database[i].kind);
        fprintf(file, "%s\n", dogs_database[i].name);
        fprintf(file, "%d\n", dogs_database[i].date);
        fprintf(file, "%s\n", dogs_database[i].medals);
    }
    fclose(file);                                                                   // ��������� ����
}

void fileIn() {
    char buf[30];
    cout << endl << "������� ��� ����� ��� ���������� ����������: " << endl;
    cin >> buf;
    FILE *file = fopen(buf, "r");                                                     // ��������� ����
    while (!feof(file)) {                                                           // ������ ���������� �� �����
        fscanf(file, "%s\n", dogs_database[current].kind);
        fscanf(file, "%s\n", dogs_database[current].name);
        fscanf(file, "%d\n", &dogs_database[current].date);
        fscanf(file, "%s\n", dogs_database[current].medals);
        cout << dogs_database[current].kind << " " << current << endl;
        cout << dogs_database[current].name << " " << current << endl;
        cout << dogs_database[current].date << " " << current << endl;
        cout << dogs_database[current].medals << " " << current << endl;
        current++;                                                                  // ����������� ���-�� �������
    }
    fclose(file);                                                                   // ��������� ����
}

void List() {                                                                  // �������� ������ ����� ������������ ������ � �������� ��������
    char buf[30];
    cout << endl << "������� ������ ������: ";
    cin.sync();
    cin.getline(buf, 30);
    for (int i = 0; i < current; i++) {                                             // �������� �� ���� ������
        if ((strcmp(buf, dogs_database[i].kind) == 0)) {                               // ���� � ������� ������ ������ ��������� � ���������, ��
            if (strstr(dogs_database[i].medals, "������") != NULL) {                // ��������� ���� �� � �� ������� ������, ���� ���� - ������� ���������� � ��� �� �����
                cout << dogs_database[i].kind << " ";
                cout << dogs_database[i].name << " ";
                cout << dogs_database[i].date << " ";
                cout << dogs_database[i].medals << endl;
            }
        }
    }
}

void solveTask() {                                                                // ������� ������� �������
    int avg = 0;
    float res = 0;
    for (int i = 0; i < current; i++) {                                             // �������� �� ����
        avg = avg + 2018 - dogs_database[i].date;                                   // ������� ���-�� ��� ��� ������ ������(������� ��� - ���� ��������), ���������
        cout << dogs_database[i].date << " " << current << endl;
    }
    res = avg / current;                                                            // ������� ������� ���������������, �������
    cout << "������� ������� �����: " << res;
}


int main() {
    //system("chcp 1251");                        // ��� ��������� �������� ����� � �������
    setlocale(LC_ALL, "RUS");                   // ��������� �������� �����
    int menu = 0;
    while (menu != 8) {
        cout << endl << endl;                                                           // ����
        cout << "[1] �������� ���������� � ������" << endl;                             // �������� ������ � ������ � ���� ������
        cout << "[2] ������������� ���������� � ������" << endl;                        // �������� ������ � ������
        cout << "[3] ������� ���������� � ������" << endl;                              // ������� ������
        cout << "[4] �������� � ����" << endl;                                          // ������ � ����
        cout << "[5] ������� �� �����" << endl;                                         // ������ �� �����
        cout << "[6] �������� ������ ����� ������������ ������ � �������� ��������" << endl;       // �������� ������ � ��������� �����������
        cout << "[7] ������������ ������� �������" << endl;    // �� �������
        cout << "[8] �����" << endl << endl;                                            // ��������� ���������
        cout << "��� �����: ";
        cin.sync();
        cin >> menu;                                                                    // ��������� ����� ������������

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
