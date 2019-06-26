#pragma once

#include <ostream>
#include <list>
#include <vector>

#include "Window.h"

struct SDL_Window;
struct SDL_Surface;
struct SDL_Color;
struct _TTF_Font;

class Cockroach;
class ScreenObject;

class Game : public Window
{
	struct UserResult {
		bool empty = true;
		std::string user;
		int score = 0;
	};
	using Beetles = std::list<Cockroach*>;
	using Objects = std::list<ScreenObject*>;
	using Results = std::vector<UserResult>;

public:
	Game(int width, int height, const std::string& title, const std::string& userName);
	virtual ~Game();
	// отобразить окно
	int show() override;
	// загрузка результатов из файла
	void loadResults();
	// сохранение результатов в файл
	void saveResults();

private:
	void spawnCockroach(int index, int speed);
	
	Objects::iterator replaceCockroach(Objects::iterator it, int index, int speed);

	bool showScore();

private:
	// шрифт для меню
	_TTF_Font* _menuFont = nullptr;
	// список тараканов на экране
	//Beetles beetles;
	// список объектов на экране
	Objects objects;

	// имя пользователя
	std::string _userName;
	// счётчик убитых тараканов
	size_t _killCount = 0;
	// результаты последних 10 пользователей
	Results _results;
};

