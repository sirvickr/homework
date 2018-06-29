#ifndef SONG_H
#define SONG_H

#include <string>

// Песня
class Song
{
	public:
		// Конструктор
		Song(const std::string& name = "", 
			const std::string& singer = "", size_t id = 0);
		// Методы доступа к полям класса
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
		inline size_t getVotes() const {
			return votes;
		}
		// Выбор песни (голосование) 
		inline size_t vote() {
			return ++votes;
		}
		// Обнулить результаты голосования
		inline void resetVotes() {
			votes = 0;
		}
	private:
		// Название
		std::string name;
		// Исполнитель
		std::string singer;
		// Идентификатор
		size_t id;
		// Количество голосов
		size_t votes;
};

#endif
