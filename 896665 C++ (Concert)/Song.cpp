#include "Song.h"

Song::Song(const std::string& name, const std::string& singer, size_t id)
: name(name), singer(singer), id(id), votes(0)
{
}
