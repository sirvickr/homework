#include "Game.h"
#include "Cockroach.h"

#include <iostream>
#include <sstream>
#include <chrono>
#include <SDL.h>
#include <SDL_image.h>
#include <list>
// ��� ��������� ��������������� ����� (���)
#include <random>  
#include <ctime>

//using namespace std;

Game::Game(int scrWidth, int scrHeight)
:	_active(true), _scrWidth(scrWidth), _scrHeight(scrHeight)
{
	userName = "user1";
}


Game::~Game()
{
}

const char* imgNames[] = {
	"img/crU.png",
	"img/crD.png",
	"img/crL.png",
	"img/crR.png",
};
/*
	StartParams startParams[] = {
		{ _scrWidth,  _scrWidth / 2,  _scrHeight    , -1, "img/crU.png" },
		{ _scrWidth,  _scrWidth / 2,  0             ,  1, "img/crD.png" },
		{ _scrHeight, _scrWidth,      _scrHeight / 2, -1, "img/crL.png" },
		{ _scrHeight, 0,              _scrHeight / 2,  1, "img/crR.png" },
	};
*/

int Game::run()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		logSDLError(std::cout, "IMG_Init");
		SDL_Quit();
		return 1;
	}
	_window = SDL_CreateWindow("Cockroach hunt", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, _scrWidth, _scrHeight, SDL_WINDOW_SHOWN);
	if (!_window) {
		logSDLError(std::cout, "SDL_CreateWindow");
		return 1;
	}
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!_renderer) {
		logSDLError(std::cout, "SDL_CreateRenderer");
		return 1;
	}

	SDL_Texture *background = loadTexture("img/background.jpg");
	SDL_Texture *cross = loadTexture("img/target.png");
	if (background == nullptr || cross == nullptr) {
		logSDLError(std::cout, "Loading images");
		return 4;
	}
	int iW, iH;
	SDL_QueryTexture(cross, nullptr, nullptr, &iW, &iH);
	int x = _scrWidth / 2 - iW / 2;
	int y = _scrHeight / 2 - iH / 2;
	// ���������� ���
	std::mt19937 gen;
	gen.seed(static_cast<uint32_t>(time(0)));
	std::uniform_int_distribution<> uidOrient(0, 3);
	std::uniform_int_distribution<> uidSpeed(1, 3);
	// ������ ��������� ������ ���������
	size_t initCount = 5;
	for (size_t i = 0; i < initCount; ++i) {
		auto index = uidOrient(gen);
		beetles.push_back(new Cockroach(_scrWidth, _scrHeight, _renderer, imgNames[index], 
			static_cast<Cockroach::Orient>(index), uidSpeed(gen)));
	}

	// ��������� ����������� ������� �� 1 ���
	const int crossDelta = 15;
	// ������� SDL
	SDL_Event evt;
	// ������� ������ ���������
	size_t killCount = 0;
	// ����� ���� - 1 ���
	constexpr uint32_t gameTime = 60000; // ms
	// ����� ������ ���� - 50 ��
	constexpr uint32_t stepDelay = 50; // ms
	// ����� ���� � �����
	constexpr uint32_t stepCount = gameTime / stepDelay;
	// ������� �����
	uint32_t stepCounter = 0;
	while (_active) {
		if (stepCounter++ == stepCount) {
			break;
		}
		SDL_Delay(stepDelay);
		std::cout << ".";
		// ��������� ������� ������������
		bool pressed = false;
		while (SDL_PollEvent(&evt)) {
			switch (evt.type) {
			case SDL_QUIT:
				stop();
				break;
			case SDL_KEYDOWN:
				//std::cout << " \nscancode " << evt.key.keysym.scancode << " vk " << evt.key.keysym.sym << std::endl;
				switch (evt.key.keysym.scancode)
				{
				case SDL_SCANCODE_UP:
					y -= crossDelta;
					pressed = true;
					break;
				case SDL_SCANCODE_DOWN:
					y += crossDelta;
					pressed = true;
					break;
				case SDL_SCANCODE_LEFT:
					x -= crossDelta;
					pressed = true;
					break;
				case SDL_SCANCODE_RIGHT:
					x += crossDelta;
					pressed = true;
					break;
				case SDL_SCANCODE_ESCAPE:
					stop();
					break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				std::cout << " \nmouse " << evt.button.x << " " << evt.button.y << std::endl;
				//stop();
				break;
			}
		}

		// ��������� �����

		// ������� ������
		SDL_RenderClear(_renderer);
		// ���������� ����
		renderTexture(background, 0, 0);
		// ��������� � ��������� ���������
		for (auto it = std::begin(beetles); it != std::end(beetles); ) {
			(*it)->draw();
			if (!pressed || (*it)->evade(x + iW / 2, y + iH / 2)) { // ����� �������
				(*it)->move();
				if ((*it)->away()) {
					// ������ - ������� ������ ��������, ������ ����������
					it = replaceCockroach(it, uidOrient(gen), uidSpeed(gen));
					/*delete (*it);
					it = beetles.erase(it);
					auto index = uidOrient(gen);
					beetles.push_back(new Cockroach(_scrWidth, _scrHeight, _renderer, imgNames[index],
						static_cast<Cockroach::Orient>(index), uidSpeed(gen)));*/
				}
				else {
					it++;
				}
			}
			else {
				// �� ���������.. ������� ������ ��������, ������ �������
				it = replaceCockroach(it, uidOrient(gen), uidSpeed(gen));
				/*delete (*it);
				it = beetles.erase(it);
				auto index = uidOrient(gen);
				beetles.push_back(new Cockroach(_scrWidth, _scrHeight, _renderer, imgNames[index],
					static_cast<Cockroach::Orient>(index), uidSpeed(gen)));*/
				killCount++;
			}
		}
		// ������
		renderTexture(cross, x, y);
		// ����� ����� � ����
		SDL_RenderPresent(_renderer);
	} // while(active)
	std::cout << "killCount for " << userName << " = " << killCount << std::endl;

	// ������� ��������

	// �������� ������ ���������
	for (auto it = std::begin(beetles); it != std::end(beetles); it++) {
		delete (*it);
	}
	
	// ������������ �������� SDL
	
	SDL_DestroyTexture(cross);
	
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();

	_window = nullptr;
	_renderer = nullptr;

	return 0;
}

Game::Beetles::iterator Game::replaceCockroach(Beetles::iterator it, int index, int speed) {
	delete *it;
	Beetles::iterator result = beetles.erase(it);
	beetles.push_back(new Cockroach(_scrWidth, _scrHeight, _renderer, imgNames[index],
		static_cast<Cockroach::Orient>(index), speed));
	return result;
}

void Game::stop()
{
	_active = false;
}

SDL_Texture* Game::loadTexture(const std::string &file) {
	SDL_Texture *texture = IMG_LoadTexture(_renderer, file.c_str());
	if (texture == nullptr) {
		logSDLError(std::cout, "loadTexture");
	}
	return texture;
}

void Game::renderTexture(SDL_Texture *tex, int x, int y, int w, int h) {
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(_renderer, tex, nullptr, &dst);
}

void Game::renderTexture(SDL_Texture *tex, int x, int y) {
	int w, h;
	SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
	renderTexture(tex, x, y, w, h);
}

void Game::logSDLError(std::ostream &os, const std::string &msg) {
	os << msg << " failed: " << SDL_GetError() << std::endl;
}
