#include "Game.h"
#include "Cockroach.h"

#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <SDL.h>
#include <SDL_image.h>
#include <list>
// для генерации пседвослучайных чисел (ПСЧ)
#include <random>  
#include <ctime>

//using namespace std;

Game::Game(int scrWidth, int scrHeight)
:	_active(true), _scrWidth(scrWidth), _scrHeight(scrHeight)
{
}


Game::~Game()
{
}

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
	// генераторы ПСЧ
	std::mt19937 gen;
	gen.seed(static_cast<uint32_t>(time(0)));
	std::uniform_int_distribution<> uidOrient(0, 3);
	std::uniform_int_distribution<> uidCount(2, 3);
	std::uniform_int_distribution<> uidSpeed(1, 3);

	//static const int delta = 2;
	// up, down, left, right
	StartParams startParams[] = {
		{ _scrWidth / 2,  _scrHeight    , -1, "img/crU.png" },
		{ _scrWidth / 2,  0             ,  1, "img/crD.png" },
		{ _scrWidth,      _scrHeight / 2, -1, "img/crL.png" },
		{ 0,              _scrHeight / 2,  1, "img/crR.png" },
	};

	auto initCount = static_cast<size_t>(4);
	//	auto initCount = static_cast<size_t>(uidCount(gen));
	for (size_t i = 0; i < initCount; ++i) {
		//	Cockroach::Orient orient = Cockroach::Orient::right;
		Cockroach::Orient orient = static_cast<Cockroach::Orient>(i & 3);
		//	Cockroach::Orient orient = static_cast<Cockroach::Orient>(uidOrient(gen));
		const auto& startPoint = startParams[static_cast<int>(orient)];
		beetles.push_back(new Cockroach(_renderer, startPoint.imgName, orient, 
			startPoint.x, startPoint.y, startPoint.delta * uidSpeed(gen)));
	}

	const int crossDelta = 10;
	size_t killCount = 0;
	SDL_Event evt;
	while (_active) {
		SDL_Delay(50);
		std::cout << ".";
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
		// Отрисовка сцены
#if 1
		// очистка экрана
		SDL_RenderClear(_renderer);
		// заполнение фона
#if 1
		renderTexture(background, 0, 0);
#else
		const int TILE_SIZE = 200;
		int xTiles = _scrWidth / TILE_SIZE;
		int yTiles = _scrHeight / TILE_SIZE;
		//Draw the tiles by calculating their positions
		for (int i = 0; i < xTiles * yTiles; ++i) {
			int x = i % xTiles;
			int y = i / xTiles;
			renderTexture(background, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
		}
#endif
		// тараканы
		for (auto it = std::begin(beetles); it != std::end(beetles); ) {
			(*it)->draw();
			if (!pressed || (*it)->evade(x + iW / 2, y + iH / 2)) { // центр прицела
				(*it++)->move();
			}
			else { // не увернулся..
				//std::cout << "################### ++++ ###################" << std::endl;
				it = beetles.erase(it);
				killCount++;
			}
		}
		// прицел
		renderTexture(cross, x, y);

		SDL_RenderPresent(_renderer);
#endif
	}

	for (auto it = std::begin(beetles); it != std::end(beetles); it++) {
		delete (*it);
	}

	SDL_DestroyTexture(cross);

	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();

	_window = nullptr;
	_renderer = nullptr;

	return 0;
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
