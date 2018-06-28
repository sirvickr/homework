#ifndef SONG_H
#define SONG_H

#include <string>

class Song
{
	public:
		Song(const std::string& name = "", const std::string& singer = "", size_t id = 0);
		inline void setName(const std::string& value) {
			name = value;
		}
		inline std::string getName() const {
			return name;
		}
		inline void setSinger(const std::string& value) {
			singer = value;
		}
		inline std::string getSinger() const {
			return singer;
		}
		inline void setId(size_t value) {
			id = value;
		}
		inline size_t getId() const {
			return id;
		}
		inline size_t vote() {
			return ++votes;
		}
		inline void resetVotes() {
			votes = 0;
		}
		inline size_t getVotes() const {
			return votes;
		}
	private:
		std::string name;
		std::string singer;
		size_t id;
		size_t votes;
};

bool operator==(const Song& a, const Song& b);

#endif
