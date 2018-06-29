#include "Concert.h"

using namespace std;

Concert::Concert(int timeout) : timeout(timeout) {
	currUser = users.end();
	// В любой конфигурации с "Debug" для удобства отладки
	// в систему изначально добавляется набор условных пользователей
	// и набор из 4 песен
	// Для отключения этой возможности нужно пересобрать проект
	// в любой из конфигураций с "Release"
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
        cout << "\n\nДОБРО ПОЖАЛОВАТЬ\n\n";
        cout << "[0] Завершение работы\n";
        cout << "[1] Войти в систему подачи заявок\n";
        cout << "[2] Зарегистрироваться в системе подачи заявок\n";
        cout << endl << "Ваш выбор: ";
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
		cout << "Неверное сочетание имени пользователя и пароля" << endl;
		currUser = users.end();
		return false;
	}
	cout << "Авторизация пройдена" << endl;
	return true;
}

bool Concert::signUp() {
	Concert::Users::value_type user = inputUser();
	Users::const_iterator it = users.find(user.first);
	if(it != users.end()) {
		cout << "Такой пользователь уже существует" << endl;
		return false;
	}
	currUser = users.insert(user).first;
	cout << "Регистрация завершена" << endl;
	return true;
}

bool Concert::runAuth() {
	bool exit = false;
	bool running = true;
	int counter = 0;
	int menu = -1;
    while (running) {
        cout << "\n\nГЛАВНОЕ МЕНЮ\n\n";
        cout << "[0] Завершение работы\n";
        cout << "[1] Выйти из учётной записи\n";
        cout << "[2] Общий cписок песен\n";
        cout << "[3] Список песен для выбора\n";
        cout << "[4] Проголосовать за песню\n";
        cout << "[5] Добавить песню\n";
        cout << "[6] Редактировать информацию о песне\n";
        cout << "[7] Удалить песню\n";
        cout << "[8] Найти песню\n";

        cout  << endl;

        cout << "Ваш выбор: ";
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
		cout << "Недопустимое значение" << endl;
	}
}

void Concert::addSong() {
	Song* song = inputSong(songs.size() + 1);
	const auto it = find(songs.begin(), songs.end(), song);
	if(it != songs.end()) {
		cout << "Такая песня уже есть" << endl;
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
		cout << "Недопустимое значение" << endl;
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
		cout << "Недопустимое значение" << endl;
	}
}

void Concert::findSong() {
	int menu = -1;
	string s;
    cout << "По какому параметру искать песню?\n";
    cout << "[0] Отменить поиск\n";
    cout << "[1] Название\n";
    cout << "[2] Исполнитель" << endl;
    cout << "Ваш выбор: ";
    cin.sync();
    cin >> menu;
    cout << "Введите значение для поиска: ";
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
		cout << "Песня не найдена" << endl;
		return;
	}
	cout << "Песня найдена, её номер " << (*it)->getId() << endl;
}

Concert::Users::value_type Concert::inputUser() {
	string name, pass;
	cout << "Введите имя пользователя: ";
	cin >> name;
	cout << "Введите пароль: ";
	cin >> pass;
	return Users::value_type(name, pass);
}

Song* Concert::inputSong(size_t id) {
	string name, singer;
	cout << "Введите название песни: ";
	cin >> name;
	cout << "Введите исполнителя: ";
	cin >> singer;
	return new Song(name, singer, id);
}

size_t Concert::inputIndex() {
	size_t index;
	cout << "Введите номер песни: ";
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
	   	cout << "Концерт начинается:" << endl;
	   	Songs program = createConcertProgram<2>();
    	printSongs(program, PrintSong());
    	result = true;
	} else {
	   	cout << "До начала концерта осталось секунд: " << rest << endl;
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

