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

class Game : public Window
{
	struct UserResult {
		bool empty = true;
		std::string user;
		int score = 0;
	};
	using Beetles = std::list<Cockroach*>;
	using Results = std::vector<UserResult>;

public:
	Game(int width, int height, const std::string& title, const std::string& userName);
	virtual ~Game();
	int show() override;

private:
	void spawnCockroach(int index, int speed);
	
	Beetles::iterator replaceCockroach(Beetles::iterator it, int index, int speed);

	bool showScore();

private:
	// ������ ��������� �� ������
	Beetles beetles;

	// ��� ������������
	std::string _userName;
	// ������� ������ ���������
	size_t _killCount = 0;
	// ���������� ��������� 10 �������������
	Results _results;
};

