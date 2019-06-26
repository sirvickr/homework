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
	// ���������� ����
	int show() override;
	// �������� ����������� �� �����
	void loadResults();
	// ���������� ����������� � ����
	void saveResults();

private:
	void spawnCockroach(int index, int speed);
	
	Objects::iterator replaceCockroach(Objects::iterator it, int index, int speed);

	bool showScore();

private:
	// ����� ��� ����
	_TTF_Font* _menuFont = nullptr;
	// ������ ��������� �� ������
	//Beetles beetles;
	// ������ �������� �� ������
	Objects objects;

	// ��� ������������
	std::string _userName;
	// ������� ������ ���������
	size_t _killCount = 0;
	// ���������� ��������� 10 �������������
	Results _results;
};

