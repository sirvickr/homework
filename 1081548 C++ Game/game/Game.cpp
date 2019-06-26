#include "Game.h"
#include "Utils.h"
#include "Menu.h"
#include "Cockroach.h"
#include "Image.h"
#include "Text.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <list>
// ��� ��������� ��������������� ����� (���)
#include <random>  
#include <ctime>

using namespace std;

static const char* fileName = "config";

const char* imgNames[] = {
	"res/crU.png",
	"res/crD.png",
	"res/crL.png",
	"res/crR.png",
};

Game::Game(int width, int height, const string& title, const std::string& userName)
:	Window(width, height, title), _userName(userName), _results(10)
{
	loadResults();
}

Game::~Game()
{
	saveResults();
}

int Game::show()
{
	// ������ ������
	_menuFont = TTF_OpenFont("res/arial.ttf", 32);
	Text* txtScore = new Text(_screen, "res/arial.ttf", 32);
	Text* txtTips = new Text(_screen, "res/arial.ttf", 18);
	if (!txtScore || !txtTips) {
		return 3;
	}
	// ��������� ���
	Image *background = new Image(_screen, "res/background.jpg");
	objects.push_back(background);
	// ��������� ������
	Image *cross = new Image(_screen, "res/target.png");
	objects.push_back(cross);
	// ��������� ���������
	if (background == nullptr || cross == nullptr) {
		logSDLError(cout, "Loading images");
		return 4;
	}
	cross->locate(_scrWidth / 2 - cross->w() / 2, _scrHeight / 2 - cross->h() / 2);
	// ���������� ���
	mt19937 gen;
	gen.seed(static_cast<uint32_t>(time(0)));
	uniform_int_distribution<> uidOrient(0, 3);
	uniform_int_distribution<> uidSpeed(1, 3);
	// ������ ��������� ������ ���������
	size_t initCount = 5;
	for (size_t i = 0; i < initCount; ++i) {
		auto index = uidOrient(gen);
		spawnCockroach(index, uidSpeed(gen));
	}

	_killCount = 0;

	// ��������� ����������� ������� �� 1 ���
	const int crossDelta = 15;
	// ������� SDL
	SDL_Event evt;
	// ����� ���� - 1 ���
	constexpr uint32_t gameTime = 60000; // ms
	// ����� ������ ���� - 50 ��
	constexpr uint32_t stepDelay = 50; // ms
	// ����� ���� � �����
	constexpr uint32_t stepCount = gameTime / stepDelay;
	// ������� �����
	uint32_t stepCounter = 0;
	// ���������������� ����
	Menu menu(_window);
	// ���� ��������� �������
	while (_active) {
		if (stepCounter++ == stepCount) {
			if (showScore()) {
				break;
			}
			else {
				_killCount = 0;
			}
		}
		SDL_Delay(stepDelay);
		// ��������� ������� ������������
		bool pressed = false;
		while (SDL_PollEvent(&evt)) {
			switch (evt.type) {
			case SDL_QUIT:
				stop();
				break;
			case SDL_KEYDOWN:
				//cout << " \nscancode " << evt.key.keysym.scancode << " vk " << evt.key.keysym.sym << endl;
				switch (evt.key.keysym.scancode)
				{
				case SDL_SCANCODE_UP:
					cross->moveY(-crossDelta);
					pressed = true;
					break;
				case SDL_SCANCODE_DOWN:
					cross->moveY(crossDelta);
					pressed = true;
					break;
				case SDL_SCANCODE_LEFT:
					cross->moveX(-crossDelta);
					pressed = true;
					break;
				case SDL_SCANCODE_RIGHT:
					cross->moveX(crossDelta);
					pressed = true;
					break;
				case SDL_SCANCODE_ESCAPE:
					if (1 == menu.show(SDL_GetWindowSurface(_window), _menuFont, { "Continue", "Exit" })) {
						stop();
					}
					break;
				case SDL_SCANCODE_SPACE:
					cout << "\nmenu index = " << menu.show(SDL_GetWindowSurface(_window), _menuFont, { "Continue", "Exit", "Three", "Four" }) << endl;
					break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				cout << " \nmouse " << evt.button.x << " " << evt.button.y << endl;
				//stop();
				break;
			}
		}

		// ��������� �����

		// ��������� �������� �� ������
		for (auto it = begin(objects); it != end(objects); ) {
			(*it)->draw();
			// ��������� ��������-���������
			Cockroach* cockroach = dynamic_cast<Cockroach*>((*it));
			if (cockroach) {
				// ��������� �� ������ �������?
				if (!pressed || cockroach->evade(cross->x() + cross->w() / 2, cross->y() + cross->h() / 2)) {
					cockroach->move();
					if (cockroach->away()) {
						// ������ - ������� ������ ��������, ������ ����������
						it = replaceCockroach(it, uidOrient(gen), uidSpeed(gen));
					}
					else {
						it++;
					}
				}
				else {
					// �� ���������.. ������� ������ ��������, ������ �������
					it = replaceCockroach(it, uidOrient(gen), uidSpeed(gen));
					_killCount++;
				}
			}
			else {
				it++;
			}
		}
		// ����� - ���������� �����
		{
			ostringstream os;
			os << "Score: " << _killCount;
			txtScore->print(10, 10, os.str(), { 0, 100, 0, 255 });
		}
		// ����� - ��������� �� ����������
		txtTips->print(10, _scrHeight - 30, "Use 'Left', 'Right', 'Up' and 'Down' arrows to move the target, Esc to show user menu", { 50, 150, 0, 255 });
		// ����� ����� � ����
		SDL_UpdateWindowSurface(_window);
	} // while(active)
	cout << _userName << ": " << _killCount << endl;

	// ������� ��������

	// �������� ������ ���������
	for (auto it = begin(objects); it != end(objects); it++) {
		delete (*it);
	}
	
	// ������������ �������� SDL

	delete txtScore;
	txtScore = nullptr;
	delete txtTips;
	txtTips = nullptr;

	TTF_CloseFont(_menuFont);
	_menuFont = nullptr;

	return 0;
}

void Game::loadResults()
{
	ifstream is(fileName, ifstream::binary);
	if (is)
	{
		// ������� ������ �����
		is.seekg(0, is.end);
		size_t length = is.tellg();
		cout << "length = " << length << endl;
		// ������� ��������� ������ � ������ �����
		is.seekg(0, is.beg);
		char* buffer = new char[length];
		is.read(buffer, length);

		string user;
		int score = 0;

		if (is)
		{
			for (size_t curr = 0, prev = 0, index = 0; curr < length;)
			{
				if ('\0' == buffer[curr])
				{
					if (curr < length - sizeof(int))
					{
						user = &buffer[prev];
						score = *(int*)(&buffer[curr + 1]);
						cout << user << ": score " << score << endl;
						_results[index].user = user;
						_results[index].score = score;
						_results[index].empty = false;
						if (++index == _results.size())
							break;
						curr += (sizeof(int) + 1);
						prev = curr;
					}
				}
				else
				{
					curr++;
				}
			}
		}
		is.close();
		delete[] buffer;
	}
}

void Game::saveResults()
{
	ofstream os(fileName, ofstream::binary);
	if (os)
	{
		Results::iterator it;
		// ���� �������� ������������, ����� ��������
		for (it = begin(_results); it != end(_results); ++it)
		{
			cout << "[" << (it - begin(_results)) << "]" << it->user << ": " << it->score << "(" << boolalpha << it->empty << ")" << endl;
			if (!it->empty && it->user == _userName)
			{
				it->score = _killCount;
				cout << "updated " << _userName << ": " << _killCount << endl;
				break;
			}
		}
		if (it == end(_results))
		{
			// �� �����, ���������
			for (auto& result : _results)
			{
				if (result.empty)
				{
					result.user = _userName;
					result.score = _killCount;
					result.empty = false;
					cout << "added " << result.user << ": " << result.score << endl;
					break;
				}
			}
		}
		for (const auto& result : _results)
		{
			if (!result.empty)
			{
				cout << "saving " << result.user << ": " << result.score << endl;
				// ��������� ��� ������������ + ������ ���������� ������
				os.write(result.user.c_str(), result.user.size() + 1);
				// ��������� ��������� ������������
				os.write((char*)&result.score, sizeof(result.score));
			}
		}
	}
}

void Game::spawnCockroach(int index, int speed)
{
	objects.push_back(new Cockroach(_scrWidth, _scrHeight, _screen, imgNames[index],
		static_cast<Cockroach::Orient>(index), speed));
}

Game::Objects::iterator Game::replaceCockroach(Objects::iterator it, int index, int speed) {
	delete *it;
	auto result = objects.erase(it);
	spawnCockroach(index, speed);
	return result;
}

bool Game::showScore()
{
	Menu menu(_window);
	return (1 == menu.show(SDL_GetWindowSurface(_window), _menuFont, { "Continue", "Exit" }));
}
