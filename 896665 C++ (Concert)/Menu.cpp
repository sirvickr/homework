#include <clocale>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "Menu.h"

using namespace std;

Menu::Menu() {
	users["a"] = "a";
	songs.push_back(Song("aa", "aaa"));
	songs.push_back(Song("bb", "bbb"));
	songs.push_back(Song("cc", "ccc"));
	songs.push_back(Song("dd", "ddd"));
}

int Menu::run() {
	setlocale(LC_ALL, "Russian");
	int counter = 0;
	int menu = -1;
    while (menu != 0) {
        cout << "\n\n����� ����������\n\n";
        cout << "[0] ���������� ������\n";
        cout << "[1] ����� � ������� ������ ������\n";
        cout << "[2] ������������������ � ������� ������ ������\n";

        cout  << endl;

        cout << "��� �����: ";
        cin.sync();
        cin >> menu;

        switch(menu) {
        case 1:
            if(signIn() && runAuth())
				menu = 0;
        	break;
        case 2:
            if(signUp() && runAuth())
				menu = 0;
        	break;
    	}
        if(counter++ == 10) {
        	cout << "emergency exit" << endl;
        	break;
		}
    }

	return 0;
}

bool Menu::signIn() {
	Menu::Users::value_type user = inputUser();
	Users::const_iterator it = users.find(user.first);
	if(it == users.end() || it->second != user.second) {
		cout << "�������� ��������� ����� ������������ � ������" << endl;
		return false;
	}
	cout << "����������� ��������" << endl;
	return true;
}

bool Menu::signUp() {
	Menu::Users::value_type user = inputUser();
	Users::const_iterator it = users.find(user.first);
	if(it != users.end()) {
		cout << "����� ������������ ��� ����������" << endl;
		return false;
	}
	users[user.first] = user.second;
	cout << "����������� ���������" << endl;
	return true;
}

bool Menu::runAuth() {
	bool exit = false;
	bool running = true;
	int counter = 0;
	int menu = -1;
    while (running) {
        cout << "\n\n������� ����\n\n";
        cout << "[0] ���������� ������\n";
        cout << "[1] ����� �� ������� ������\n";
        cout << "[2] ������ �����\n";
        cout << "[3] ������������� �� �����\n";
        cout << "[4] �������� �����\n";
        cout << "[5] ������������� ���������� � �����\n";
        cout << "[6] ������� �����\n";
        cout << "[7] ����� �����\n";

        cout  << endl;

        cout << "��� �����: ";
        cin.sync();
        cin >> menu;

        switch(menu) {
        case 0:
        	running = false;
        	exit = true;
        	break;
        case 1:
        	running = false;
        	break;
        case 2:
        	printSongs();
        	break;
        case 3:
        	selectSong();
        	break;
        case 4:
        	addSong();
        	break;
        case 5:
        	editSong();
        	break;
        case 6:
        	delSong();
        	break;
        case 7:
        	findSong();
        	break;
        }
        if(counter++ == 10) {
        	cout << "emergency exit" << endl;
        	break;
		}
    }

	return exit;
}

void Menu::printSongs() {
	if(songs.empty())
		cout << "������ ����� ����" << endl;
	else {
   		cout << setw(4) << "�" << "  ������� �����" << endl;
   		PrintSong printSong(1);
    	for_each(songs.begin(), songs.end(), printSong);
	}
}

void Menu::selectSong() {
	try {
		size_t index = inputIndex();
		songs[index].vote();
		printSongs();
	} catch(InvalidOrdinalError&) {
		cout << "������������ ��������" << endl;
	}
}

void Menu::addSong() {
	Song song = inputSong();
	Songs::const_iterator it = find(songs.begin(), songs.end(), song);
	if(it != songs.end()) {
		cout << "����� ����� ��� ����" << endl;
		return;
	}
	songs.push_back(song);
	printSongs();
}

void Menu::editSong() {
	try {
		size_t index = inputIndex();
		Song song = inputSong();
		songs[index] = song;
		printSongs();
	} catch(InvalidOrdinalError&) {
		cout << "������������ ��������" << endl;
	}
}

void Menu::delSong() {
	try {
		size_t index = inputIndex();
		songs.erase(songs.begin() + index);
		printSongs();
	} catch(InvalidOrdinalError&) {
		cout << "������������ ��������" << endl;
	}
}

void Menu::findSong() {
	int menu = -1;
	string s;
    cout << "�� ������ ��������� ������ �����?";
    cout << "[0] �������� �����\n";
    cout << "[1] ��������\n";
    cout << "[2] �����������" << endl;
    cout << "��� �����: ";
    cin.sync();
    cin >> menu;
    cout << "������� �������� ��� ������: ";
    cin >> s;

	Songs::iterator it = songs.end();
    switch(menu) {
    case 1:
		it = find_if(songs.begin(), songs.end(), FindSongByName(s));
    	break;
    case 2:
		it = find_if(songs.begin(), songs.end(), FindSongBySinger(s));
    	break;
    default:
    	return;
	}
	if(it == songs.end()) {
		cout << "����� �� �������" << endl;
		return;
	}
	cout << "����� �������, � ����� � ������ - " << distance(songs.begin(), it) + 1 << endl;
}

Menu::Users::value_type Menu::inputUser() {
	string name, pass;
	cout << "������� ��� ������������: ";
	cin >> name;
	cout << "������� ������: ";
	cin >> pass;
	return Users::value_type(name, pass);
}

Song Menu::inputSong() {
	string name, singer;
	cout << "������� �������� �����: ";
	cin >> name;
	cout << "������� �����������: ";
	cin >> singer;
	return Song(name, singer);
}

size_t Menu::inputIndex() {
	size_t index;
	cout << "������� ����� �����: ";
	cin >> index;
	if(0 == index || index > songs.size())
		throw InvalidOrdinalError(index);
	return index - 1;
}

void PrintSong::operator()(const Song& song) {
	cout << setw(4) << index++ << ". " << setw(7) << song.getVotes() << " \"" << song.getName() << "\" (" << song.getSinger() << ")" << endl;
}

bool FindSongByName::operator()(const Song& song) {
	return song.getName() == field;
}

bool FindSongBySinger::operator()(const Song& song) {
	return song.getSinger() == field;
}

