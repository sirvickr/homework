#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
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
    cout << endl << "������� ������ ������ ���������� � ������� ����� ��������: ";  // ������ ��� ������� ������
    cin.sync();
    cin.getline(str, 50);
    int i;
    for (i = 0; i < current; i++) {                                             // �������� �� ���� ����
        cout << dogs_database[i].name;
        if ((strcmp(str, dogs_database[i].name) == 0)) {                            // ���������� ��������� ������ � �������
            break;                                                               // ���� ������� ����������, ����� �������
        }
    }
    if (i < current) {                                                               // ���������, ����� �� ������
	    cout << "������� ����� ������ ������: ";                                     // ��������� ��� ����, ������� � ���������
	    cin.sync();
	    cin.getline(dogs_database[i].kind, 50);
	    cout << "������� ����� ������: ";
	    cin.sync();
	    cin.getline(dogs_database[i].name, 50);
	    cout << "������� ����� ���� ��������: ";
	    cin >> dogs_database[i].date;
	    cout << "������� ����� ���������� � �������(������, �������, ������, ����� ������): ";
	    cin.sync();
	    cin.getline(dogs_database[i].medals, 50);
	} else {
	    cout << "������ �� �������" << endl;                                       // ����� �����, ��� �� �����
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
    ifstream file(buf, ios_base::in);                                              // ��������� ����
    if(file) {
    	string line;
    	int index = 0;
    	while(getline(file, line)) {                                              // ������ ���������� �� �����
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
	    for (int i = 0; i < current; i++) {                                             // �������� �� ���� ������
   			cout << "[" << i << "]\t" << dogs_database[i].kind 
			   << "\t" << dogs_database[i].name 
			   << "\t" << dogs_database[i].date 
			   << "\t" << dogs_database[i].medals 
			   << endl;
	    }
	}
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
    	int age = 2018 - dogs_database[i].date;         // ������� ���-�� ��� ��� ������ ������(������� ��� - ���� ��������)
        avg += age;                                   // ���������
        cout << dogs_database[i].date << " " << age << endl;
    }
    res = float(avg) / current;                                                            // ������� ������� ���������������, �������
    cout << "������� ������� �����: " << res;
}

int main() {
	int counter = 0;
    system("chcp 1251");                        // ��� ��������� �������� ����� � �������
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
        if(counter++ == 10) {
        	cout << "emergency exit" << endl;
        	break;
		}
    }
    return 0;
}
