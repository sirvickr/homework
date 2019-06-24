#include "Game.h"
#include "Menu.h"
#include "Cockroach.h"
#include "Fruit.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <list>
// для генерации пседвослучайных чисел (ПСЧ)
#include <random>  
#include <ctime>

using namespace std;

extern int ScreenWidth;
extern int ScreenHeight;

Game::Game(int scrWidth, int scrHeight)
:	_active(true)
{
	userName = "user1";
}


Game::~Game()
{
	if (_font) {
		TTF_CloseFont(_font);
		_font = nullptr;
	}
}

const char* imgNames[] = {
	"img/apple-ripe.png",
	"img/apple-sick.png",
	"img/pear-ripe.png",
	"img/pear-sick.png",
};

//static const char* fontFileName = "res/sample.ttf";
static const char* fontFileName = "res/arial.ttf";
//static const char* fontFileName = "c:\\windows\\fonts\\arial.ttf";
static const int fontSize = 32;
SDL_Color fontColor = { 0, 100, 0, 255 };//255

int Game::run()
{
	srand(static_cast<unsigned int>(time(0)));
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		logSDLError(cout, "SDL_Init");
		return 1;
	}
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		logSDLError(cout, "IMG_Init");
		SDL_Quit();
		return 1;
	}
	if (TTF_Init() != 0) {
		logSDLError(cout, "TTF_Init");
		SDL_Quit();
		return 1;
	}
	_window = SDL_CreateWindow("Cockroach hunt", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN);
	if (!_window) {
		logSDLError(cout, "SDL_CreateWindow");
		return 1;
	}
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!_renderer) {
		logSDLError(cout, "SDL_CreateRenderer");
		return 2;
	}
	// Создаём шрифт
	_font = TTF_OpenFont(fontFileName, fontSize);
	if (_font == nullptr) {
		logSDLError(cout, "TTF_OpenFont");
		return 3;
	}
	SDL_Texture *background = IMG_LoadTexture(_renderer, "img/background.jpg");
	SDL_Texture *basket = IMG_LoadTexture(_renderer, "img/basket.png");
	if (background == nullptr || basket == nullptr) {
		logSDLError(cout, "Loading images");
		return 4;
	}
	int basketWidth, basketHeight;
	SDL_QueryTexture(basket, nullptr, nullptr, &basketWidth, &basketHeight);
	int basketX = ScreenWidth / 2 - basketWidth / 2;
	int basketY = ScreenHeight - basketWidth;
	// генераторы ПСЧ
	mt19937 gen;
	gen.seed(static_cast<uint32_t>(time(0)));
	uniform_int_distribution<> uidOrient(0, 3);
	uniform_int_distribution<> uidSpeed(1, 3);
	// создаём начальный список тараканов
	size_t initCount = 5;
	for (size_t i = 0; i < initCount; ++i) {
		auto index = uidOrient(gen);
		beetles.push_back(new Cockroach(ScreenWidth, ScreenHeight, _renderer, imgNames[index],
			static_cast<Cockroach::Orient>(index), uidSpeed(gen)));
		fruits.push_back(Fruit::generateFruit(_renderer));
	}
	cout << "fruit count = " << fruits.size() << endl;

	// Дальность перемещения корзины за 1 шаг
	const int crossDelta = 15;
	// Событие SDL
	SDL_Event evt;
	// счётчик убитых тараканов
	size_t killCount = 0;
	// время игры - 1 мин
	constexpr uint32_t gameTime = 60000; // ms
	// время одного шага - 50 мс
	constexpr uint32_t stepDelay = 50; // ms
	// время игры в шагах
	constexpr uint32_t stepCount = gameTime / stepDelay;
	// счётчик шагов
	uint32_t stepCounter = 0;
	// пользовательское меню
	Menu menu(_window, _renderer);
	// цикл обработки событий
	while (_active) {
		if (stepCounter++ == stepCount) {
			if (showScore()) {
				break;
			}
			else {
				killCount = 0;
			}
		}
		SDL_Delay(stepDelay);
		cout << ".";
		// Обработка событий пользователя
		while (SDL_PollEvent(&evt)) {
			switch (evt.type) {
			case SDL_QUIT:
				stop();
				break;
			case SDL_KEYDOWN:
				//cout << " \nscancode " << evt.key.keysym.scancode << " vk " << evt.key.keysym.sym << endl;
				switch (evt.key.keysym.scancode)
				{
				case SDL_SCANCODE_LEFT:
					basketX -= crossDelta;
					break;
				case SDL_SCANCODE_RIGHT:
					basketX += crossDelta;
					break;
				case SDL_SCANCODE_ESCAPE:
					if (1 == menu.show(SDL_GetWindowSurface(_window), _font, { "Continue", "Exit" })) {
						stop();
					}
					break;
				case SDL_SCANCODE_SPACE:
					cout << "\nmenu index = " << menu.show(SDL_GetWindowSurface(_window), _font, { "Continue", "Exit", "Three", "Four" }) << endl;
					break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				cout << " \nmouse " << evt.button.x << " " << evt.button.y << endl;
				//stop();
				break;
			}
		}

		// Отрисовка сцены

		// Очистка экрана
		SDL_RenderClear(_renderer);
		// Фон
		renderTexture(background, 0, 0);
		// Корзина
		renderTexture(basket, basketX, basketY);
#if 0
		// обработка и отрисовка тараканов
		for (auto it = begin(beetles); it != end(beetles); ) {
			(*it)->draw();
			if (!pressed || (*it)->evade(basketX + basketWidth / 2, basketY + basketHeight / 2)) { // центр прицела
				(*it)->move();
				if ((*it)->away()) {
					// убежал - создать нового таракана, взамен сбежавшего
					it = replaceCockroach(it, uidOrient(gen), uidSpeed(gen));
					/*delete (*it);
					it = beetles.erase(it);
					auto index = uidOrient(gen);
					beetles.push_back(new Cockroach(ScreenWidth, ScreenHeight, _renderer, imgNames[index],
						static_cast<Cockroach::Orient>(index), uidSpeed(gen)));*/
				}
				else {
					it++;
				}
			}
			else {
				// не увернулся.. создать нового таракана, взамен убитого
				it = replaceCockroach(it, uidOrient(gen), uidSpeed(gen));
				/*delete (*it);
				it = beetles.erase(it);
				auto index = uidOrient(gen);
				beetles.push_back(new Cockroach(ScreenWidth, ScreenHeight, _renderer, imgNames[index],
					static_cast<Cockroach::Orient>(index), uidSpeed(gen)));*/
				killCount++;
			}
		}
#else
		// Обработка и прорисовка фруктов
		//int udx = 0;
		for (auto it = begin(fruits); it != end(fruits);) {
			(*it)->draw();
			(*it)->move();
			if ((*it)->fell()) {
				// Упал, заменяем фрукт
				it = newFruit(it);
				/*delete (*it);
				it = beetles.erase(it);
				auto index = uidOrient(gen);
				beetles.push_back(new Cockroach(ScreenWidth, ScreenHeight, _renderer, imgNames[index],
					static_cast<Cockroach::Orient>(index), uidSpeed(gen)));*/
			}
			else {
				it++;
			}
			//cout << "draw fruit " << udx++ << endl;

			/*if (!pressed || (*it)->evade(basketX + basketWidth / 2, basketY + basketHeightbasket
			/ 2)) { // центр прицела
				(*it)->move();
				if ((*it)->away()) {
					// убежал - создать нового таракана, взамен сбежавшего
					it = replaceCockroach(it, uidOrient(gen), uidSpeed(gen));
				}
				else {
					it++;
				}
			}
			else {
				// не увернулся.. создать нового таракана, взамен убитого
				it = replaceCockroach(it, uidOrient(gen), uidSpeed(gen));
				killCount++;
			}*/
		}
		//cout << "drawn fruits" << endl;
#endif
		// Количество жизней
		{
			ostringstream os;
			os << "Score: " << killCount;
			SDL_Texture *image = renderText(os.str().c_str(), _font, fontColor);
			if (image) {
				// Получаем размеры текста, чтобы разместить его на экране
				int w, h;
				SDL_QueryTexture(image, NULL, NULL, &w, &h);
				renderTexture(image, ScreenWidth - w, ScreenHeight - h);
			}
			else {
				logSDLError(cout, "renderText");
			}
		}
		// Отобразить сцену
		SDL_RenderPresent(_renderer);
	} // while(active)
	cout << "killCount for " << userName << " = " << killCount << endl;

	// Очистка ресурсов

	// удаление списка тараканов
	for (auto it = begin(beetles); it != end(beetles); it++) {
		delete (*it);
	}
	// Удаление фруктов
	for (auto it = begin(fruits); it != end(fruits); it++) {
		delete (*it);
	}

	// освобождение ресурсов SDL
	
	TTF_CloseFont(_font);
	_font = nullptr;

	SDL_DestroyTexture(basket);
	
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
	beetles.push_back(new Cockroach(ScreenWidth, ScreenHeight, _renderer, imgNames[index],
		static_cast<Cockroach::Orient>(index), speed));
	return result;
}

Game::Fruits::iterator Game::newFruit(Fruits::iterator fruit) {
	delete *fruit;
	auto result = fruits.erase(fruit);
	fruits.push_back(Fruit::generateFruit(_renderer));
	return result;
}

bool Game::showScore()
{
	Menu menu(_window, _renderer);
	return (1 == menu.show(SDL_GetWindowSurface(_window), _font, { "Continue", "Exit" }));
}

void Game::stop()
{
	_active = false;
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

SDL_Texture* Game::renderText(const string &message, const string &fontFile,
	SDL_Color color, int fontSize)
{
	//Open the font
	TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == nullptr) {
		logSDLError(cout, "TTF_OpenFont");
		return nullptr;
	}
	SDL_Texture* texture = renderText(message, font, color);
	TTF_CloseFont(font);
	return texture;
}

SDL_Texture* Game::renderText(const string &message, TTF_Font* font, SDL_Color color)
{
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface *surface = TTF_RenderText_Blended(font, message.c_str(), color);
	if (!surface) {
		TTF_CloseFont(font);
		logSDLError(cout, "TTF_RenderText");
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, surface);
	if (texture == nullptr) {
		logSDLError(cout, "CreateTexture");
	}
	SDL_FreeSurface(surface);
	return texture;
}

void Game::logSDLError(ostream &os, const string &msg) {
	os << msg << " failed: " << SDL_GetError() << endl;
}
