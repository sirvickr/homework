#ifndef MENU_H
#define MENU_H

#include <string>
#include <map>
#include <vector>

#include "Song.h"

class PrintSong {
public:
	PrintSong(size_t startIndex) : index(startIndex) {
	}
	void operator()(const Song& song);
private:
	size_t index;
};

class FindSongByName {
public:
	FindSongByName(const std::string& field) : field(field) {
	}
	bool operator()(const Song& song);
private:
	std::string field;
};

class FindSongBySinger {
public:
	FindSongBySinger(const std::string& field) : field(field) {
	}
	bool operator()(const Song& song);
private:
	std::string field;
};

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
	Menu();
	int run();
private:
	typedef std::map<std::string, std::string> Users;
	Users users;
	typedef std::vector<Song> Songs;
	Songs songs;
private:
	bool signIn();
	bool signUp();
	bool runAuth();
	void printSongs();
	void selectSong();
	void addSong();
	void editSong();
	void delSong();
	void findSong();
	Users::value_type inputUser();
	Song inputSong();
	size_t inputIndex();
};

#endif
