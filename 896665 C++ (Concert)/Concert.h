#ifndef CONCERT_H
#define CONCERT_H

#include <string>
#include <map>
#include <vector>
#include <list>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <chrono>

#include "Song.h"

// Функциональный тип для фильтра песен по названию
class FindSongByName {
public:
	FindSongByName(const std::string& field) : field(field) {
	}
	bool operator()(const Song* song);
private:
	std::string field;
};

// Функциональный тип для фильтра песен по исполнителю
class FindSongBySinger {
public:
	FindSongBySinger(const std::string& field) : field(field) {
	}
	bool operator()(const Song* song);
private:
	std::string field;
};

// Функция-предикат для сортировки песен по убыванию рейтинга
bool CompareSongsRating(const Song* a, const Song* b);

// Концерт
class Concert
{
	// Исключение - недопустимый порядковый номер
	class InvalidOrdinalError {
	public:
		InvalidOrdinalError(size_t value) : value(value) {
		}
		inline size_t getValue() const {
			return value;
		}
	private:
		size_t value;
	};
public:
	// Конструктор
	Concert(int timeout);
	// Деструктор
	~Concert();
	// Основная рабочая функция системы приёма заявок
	// (общее меню для всех пользователей)
	int run();
public:
	// Псевдонимы для типов
	using Users = std::map< std::string, std::string >;
	using Songs = std::vector< Song* > ;
	using UserSongs = std::vector< std::pair< const Song*, Users::iterator > >;
private:
	// Зарегистрированные пользователи
	Users users;
	// Песни, достпуные для выбора пользователями
	Songs songs;
	// Время запуска системы приём заявок
	std::chrono::steady_clock::time_point startTime;
	// Время до начала концерта
	int timeout;
	// Выбранные песни
	UserSongs userSongs;
	// Авторизованный в данный момент пользователь
	Users::iterator currUser;
	// Функциональный тип для безусловной распечатки песни
	struct PrintSong {
		void operator()(const Song* song);
	};
	// Функциональный тип для условной распечатки песни
	class PrintAvailableSong {
	public:
		PrintAvailableSong(UserSongs& userSongs, Users::iterator currUser) 
		: userSongs(userSongs), currUser(currUser) {
		}
		void operator()(const Song* song);
	private:
		UserSongs& userSongs;
		Users::iterator currUser;
	};
private:
	// Авторизация существующего пользователя
	bool signIn();
	// Регистрация нового пользователя
	bool signUp();
	// Меню для авторизованного пользователя
	bool runAuth();
	// Выбор песнипользователем
	void selectSong();
	// Добавление новой песни
	void addSong();
	// Редактирование информации о песне
	void editSong();
	// Удаление песни
	void delSong();
	// Поиск песни по выбранному полю
	void findSong();
	// Ввод данных пользователя
	Users::value_type inputUser();
	// Ввода данных песни
	Song* inputSong();
	// Выбор номера песни
	size_t inputIndex();
	// Проверка оставшегося времени до концерта
	// и его запуск, в случае истечения времени
	bool checkTimeout();
	// Параметризированный поиск песни
	template<class Finder>
	Songs::iterator findSong(const Finder& finder) {
		return find_if(songs.begin(), songs.end(), finder);
	}
	// Распечатка списка песен
	template<class Container, class Print>
	void printSongs(Container container, Print print) {
		if(container.empty())
			std::cout << "Список песен пуст" << std::endl;
		else {
	   		std::cout << std::setw(4) << "№" << "  Рейтинг Песня" << std::endl;
	    	std::for_each(container.begin(), container.end(), print);
		}
	}
	// Формирование программы концерта по результатам гоолования
	template<size_t ConcertSongsCount>
	Songs createConcertProgram() {
		Songs result(songs.size());
		std::copy(songs.begin(), songs.end(), result.begin());
		std::sort(result.begin(), result.end(), CompareSongsRating);
		if(ConcertSongsCount < result.size())
			result.resize(ConcertSongsCount);
		return result;
	}
};

#endif

