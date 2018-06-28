#include "Menu.h"

using namespace std;

Menu::Menu(int timeout) : timeout(timeout) {
	currUser = users.end();
	#ifdef __DEBUG__
	users["a"] = "a";
	users["b"] = "b";
	users["c"] = "c";
	users["d"] = "d";
	users["e"] = "e";
	songs.push_back(new Song("aa", "aaa", 1));
	songs.push_back(new Song("bb", "bbb", 2));
	songs.push_back(new Song("cc", "ccc", 3));
	songs.push_back(new Song("dd", "ddd", 4));
	#endif
	startTime = chrono::steady_clock::now();
	checkTimeout();
}

Menu::~Menu() {
	for(auto song : songs) {
		delete song;
	}
}

int Menu::run() {
	int counter = 0;
	int menu = -1;
    while (menu != 0) {
        cout << "\n\n����� ����������\n\n";
        cout << "[0] ���������� ������\n";
        cout << "[1] ����� � ������� ������ ������\n";
        cout << "[2] ������������������ � ������� ������ ������\n";
        cout << endl << "��� �����: ";
        cin.sync();
        cin >> menu;
		cout << endl;
		
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
		if(menu != 0 && checkTimeout()) {
			break;
		}
		
    } // while()

	return 0;
}

bool Menu::signIn() {
	Menu::Users::value_type user = inputUser();
	currUser = users.find(user.first);
	if(currUser == users.end() || currUser->second != user.second) {
		cout << "�������� ��������� ����� ������������ � ������" << endl;
		currUser = users.end();
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
	currUser = users.insert(user).first;
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
        cout << "[2] ����� c����� �����\n";
        cout << "[3] ������ ����� ��� ������\n";
        cout << "[4] ������������� �� �����\n";
        cout << "[5] �������� �����\n";
        cout << "[6] ������������� ���������� � �����\n";
        cout << "[7] ������� �����\n";
        cout << "[8] ����� �����\n";

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
        	printSongs(songs, PrintSong());
        	break;
        case 3:
        	printSongs(songs, PrintAvailableSong(userSongs, currUser));
        	break;
        case 4:
        	selectSong();
        	break;
        case 5:
        	addSong();
        	break;
        case 6:
        	editSong();
        	break;
        case 7:
        	delSong();
        	break;
        case 8:
        	findSong();
        	break;
        }
        if(counter++ == 10) {
        	cout << "emergency exit" << endl;
        	break;
		}
		if(checkTimeout()) {
        	exit = true;
        	break;
		}
    } // while()
    
	currUser = users.end();

	return exit;
}

void Menu::selectSong() {
	try {
		printSongs(songs, PrintAvailableSong(userSongs, currUser));
		size_t index = inputIndex();
		Song* song = songs[index];
		song->vote();
		userSongs.push_back(UserSongs::value_type(song, currUser));
		printSongs(songs, PrintAvailableSong(userSongs, currUser));
	} catch(InvalidOrdinalError&) {
		cout << "������������ ��������" << endl;
	}
}

void Menu::addSong() {
	Song* song = inputSong();
	const auto it = find(songs.begin(), songs.end(), song);
	if(it != songs.end()) {
		cout << "����� ����� ��� ����" << endl;
		return;
	}
	songs.push_back(song);
	for(size_t i = 0; i < songs.size(); i++) 
		songs[i]->setId(i + 1);
	printSongs(songs, PrintSong());
}

void Menu::editSong() {
	try {
		size_t index = inputIndex();
		songs[index] = inputSong();
		printSongs(songs, PrintSong());
	} catch(InvalidOrdinalError&) {
		cout << "������������ ��������" << endl;
	}
}

void Menu::delSong() {
	try {
		size_t index = inputIndex();
		songs.erase(songs.begin() + index);
		for(size_t i = 0; i < songs.size(); i++) 
			songs[i]->setId(i + 1);
		printSongs(songs, PrintSong());
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
		it = findSong(FindSongByName(s));
    	break;
    case 2:
		it = findSong(FindSongBySinger(s));
    	break;
    default:
    	return;
	}
	if(it == songs.end()) {
		cout << "����� �� �������" << endl;
		return;
	}
	cout << "����� �������, � ����� " << (*it)->getId() << endl;
}

Menu::Users::value_type Menu::inputUser() {
	string name, pass;
	cout << "������� ��� ������������: ";
	cin >> name;
	cout << "������� ������: ";
	cin >> pass;
	return Users::value_type(name, pass);
}

Song* Menu::inputSong() {
	string name, singer;
	cout << "������� �������� �����: ";
	cin >> name;
	cout << "������� �����������: ";
	cin >> singer;
	return new Song(name, singer);
}

size_t Menu::inputIndex() {
	size_t index;
	cout << "������� ����� �����: ";
	cin >> index;
	if(0 == index || index > songs.size())
		throw InvalidOrdinalError(index);
	return index - 1;
}

void Menu::PrintSong::operator()(const Song* song) {
	cout << setw(4) << song->getId() << ". " << setw(7) << song->getVotes() << " \"" << song->getName() << "\" (" << song->getSinger() << ")" << endl;
}

void Menu::PrintAvailableSong::operator()(const Song* song) {
	Menu::UserSongs::iterator it = find(userSongs.begin(), userSongs.end(), Menu::UserSongs::value_type(song, currUser));
	if(it == userSongs.end()) {
		cout << setw(4) << song->getId() << ". " << setw(7) << song->getVotes() << " \"" << song->getName() << "\" (" << song->getSinger() << ")" << endl;
	}
}

bool Menu::checkTimeout() {
	bool result = false;
	auto now = chrono::steady_clock::now();
	auto delta = chrono::duration_cast<chrono::seconds>(now - startTime).count();
	auto rest = timeout - delta;
	if(rest <= 0) {
	   	cout << "������� ����������:" << endl;
	   	Songs program = createConcertProgram<2>();
    	printSongs(program, PrintSong());
    	result = true;
	} else {
	   	cout << "�� ������ �������� �������� ������: " << rest << endl;
	}
	return result;
}

bool FindSongByName::operator()(const Song* song) {
	return song->getName() == field;
}

bool FindSongBySinger::operator()(const Song* song) {
	return song->getSinger() == field;
}

// �������-�������� ��� ���������� ����� �� �������� ��������
bool CompareSongsRating(const Song* a, const Song* b) {
	return a->getVotes() > b->getVotes();
}

