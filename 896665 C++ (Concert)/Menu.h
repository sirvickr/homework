#ifndef MENU_H
#define MENU_H

#include <string>
#include <map>
#include <vector>
#include <list>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <chrono>

#include "Song.h"

class FindSongByName {
public:
	FindSongByName(const std::string& field) : field(field) {
	}
	bool operator()(const Song* song);
private:
	std::string field;
};

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

class Menu
{
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
	Menu(int timeout);
	~Menu();
	int run();
public:
	typedef std::map<std::string, std::string> Users;
	typedef std::vector< Song* > Songs;
	typedef std::vector< std::pair<const Song*, Users::iterator> > UserSongs;
private:
	Users users;
	Songs songs;
	std::chrono::steady_clock::time_point startTime;
	// время до начала концерта
	int timeout;

	UserSongs userSongs;
	Users::iterator currUser;
	
	struct PrintSong {
		void operator()(const Song* song);
	};

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
	bool signIn();
	bool signUp();
	bool runAuth();
	void selectSong();
	void addSong();
	void editSong();
	void delSong();
	void findSong();
	Users::value_type inputUser();
	Song* inputSong();
	size_t inputIndex();
	bool checkTimeout();
	template<class Finder>
	Songs::iterator findSong(const Finder& finder) {
		return find_if(songs.begin(), songs.end(), finder);
	}
	template<class Container, class Print>
	void printSongs(Container container, Print print) {
		if(container.empty())
			std::cout << "Список песен пуст" << std::endl;
		else {
	   		std::cout << std::setw(4) << "№" << "  Рейтинг Песня" << std::endl;
	    	std::for_each(container.begin(), container.end(), print);
		}
	}
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

