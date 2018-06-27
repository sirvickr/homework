#include "Song.h"

Song::Song(const std::string& name, const std::string& singer)
: name(name), singer(singer), votes(0)
{
}

bool operator==(const Song& a, const Song& b) {
	return a.getName() == b.getName() && a.getSinger() == b.getSinger();
}

