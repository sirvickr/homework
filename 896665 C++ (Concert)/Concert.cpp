#include "Concert.h"

using namespace std;

Concert::Concert(int timeout) : timeout(timeout) {
	currUser = users.end();
	// � ����� ������������ � "Debug" ��� �������� �������
	// � ������� ���������� ����������� ����� �������� �������������
	// � ����� �� 4 �����
	// ��� ���������� ���� ����������� ����� ����������� ������
	// � ����� �� ������������ � "Release"
	#ifdef __DEBUG__
	users["a"] = "a";
	users["b"] = "b";
	users["c"] = "c";
	users["d"] = "d";
	users["e"] = "e";
	songs.push_back(new Song("Sonne", "Rammstein", 1));
	songs.push_back(new Song("Twenty One", "Cranberries", 2));
	songs.push_back(new Song("Only Time", "Enya", 3));
	songs.push_back(new Song("Our Farewell", "Within Temptation", 4));
	#endif
	startTime = chrono::steady_clock::now();
	checkTimeout();
}

Concert::~Concert() {
	for(auto song : songs) {
		delete song;
	}
}

int Concert::run() {
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
    
    cin.get();

	return 0;
}

bool Concert::signIn() {
	Concert::Users::value_type user = inputUser();
	currUser = users.find(user.first);
	if(currUser == users.end() || currUser->second != user.second) {
		cout << "�������� ��������� ����� ������������ � ������" << endl;
		currUser = users.end();
		return false;
	}
	cout << "����������� ��������" << endl;
	return true;
}

bool Concert::signUp() {
	Concert::Users::value_type user = inputUser();
	Users::const_iterator it = users.find(user.first);
	if(it != users.end()) {
		cout << "����� ������������ ��� ����������" << endl;
		return false;
	}
	currUser = users.insert(user).first;
	cout << "����������� ���������" << endl;
	return true;
}

bool Concert::runAuth() {
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

void Concert::selectSong() {
	try {
		printSongs(songs, PrintAvailableSong(userSongs, currUser));
		size_t index = inputIndex();
		Song* song = songs[index];
		song->vote();
		userSongs.push_back(UserSongs::value_type(song, currUser));
	} catch(InvalidOrdinalError&) {
		cout << "������������ ��������" << endl;
	}
}

void Concert::addSong() {
	Song* song = inputSong(songs.size() + 1);
	const auto it = find(songs.begin(), songs.end(), song);
	if(it != songs.end()) {
		cout << "����� ����� ��� ����" << endl;
		delete song;
		return;
	}
	songs.push_back(song);
	printSongs(songs, PrintSong());
}

void Concert::editSong() {
	try {
		printSongs(songs, PrintSong());
		size_t index = inputIndex();
		songs[index] = inputSong(index + 1);
		printSongs(songs, PrintSong());
	} catch(InvalidOrdinalError&) {
		cout << "������������ ��������" << endl;
	}
}

void Concert::delSong() {
	try {
		printSongs(songs, PrintSong());
		size_t index = inputIndex();
		songs.erase(songs.begin() + index);
		for(size_t i = 0; i < songs.size(); i++) 
			songs[i]->setId(i + 1);
		printSongs(songs, PrintSong());
	} catch(InvalidOrdinalError&) {
		cout << "������������ ��������" << endl;
	}
}

void Concert::findSong() {
	int menu = -1;
	string s;
    cout << "�� ������ ��������� ������ �����?\n";
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

Concert::Users::value_type Concert::inputUser() {
	string name, pass;
	cout << "������� ��� ������������: ";
	cin >> name;
	cout << "������� ������: ";
	cin >> pass;
	return Users::value_type(name, pass);
}

Song* Concert::inputSong(size_t id) {
	string name, singer;
	cout << "������� �������� �����: ";
	cin >> name;
	cout << "������� �����������: ";
	cin >> singer;
	return new Song(name, singer, id);
}

size_t Concert::inputIndex() {
	size_t index;
	cout << "������� ����� �����: ";
	cin >> index;
	if(0 == index || index > songs.size())
		throw InvalidOrdinalError(index);
	return index - 1;
}

void Concert::PrintSong::operator()(const Song* song) {
	cout << setw(4) << song->getId() << ". " << setw(7) << song->getVotes() << " \"" << song->getName() << "\" (" << song->getSinger() << ")" << endl;
}

void Concert::PrintAvailableSong::operator()(const Song* song) {
	Concert::UserSongs::iterator it = find(userSongs.begin(), userSongs.end(), Concert::UserSongs::value_type(song, currUser));
	if(it == userSongs.end()) {
		cout << setw(4) << song->getId() << ". " << setw(7) << song->getVotes() << " \"" << song->getName() << "\" (" << song->getSinger() << ")" << endl;
	}
}

bool Concert::checkTimeout() {
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

bool CompareSongsRating(const Song* a, const Song* b) {
	return a->getVotes() > b->getVotes();
}

