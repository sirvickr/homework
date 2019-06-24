#include "Game.h"
#include "Menu.h"
#include "Fruit.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <list>
#include <random>  
#include <ctime>

using namespace std;

extern int ScreenWidth;
extern int ScreenHeight;

Game::Game(const std::string& name)
: m_user(name), m_run(true)
{
}

Game::~Game()
{
	if (m_font) {
		TTF_CloseFont(m_font);
		m_font = nullptr;
	}
}

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
	m_window = SDL_CreateWindow("Harverst", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN);
	if (!m_window) {
		logSDLError(cout, "SDL_CreateWindow");
		return 1;
	}
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!m_renderer) {
		logSDLError(cout, "SDL_CreateRenderer");
		return 2;
	}
	// Создаём шрифт
	m_font = TTF_OpenFont("fonts\\calibri.ttf", 28);
	if (m_font == nullptr) {
		logSDLError(cout, "TTF_OpenFont");
		return 3;
	}
	SDL_Texture *background = IMG_LoadTexture(m_renderer, "img/background.jpg");
	SDL_Texture *basket = IMG_LoadTexture(m_renderer, "img/basket.png");
	if (background == nullptr || basket == nullptr) {
		logSDLError(cout, "Loading images");
		return 4;
	}

	loadStats();

	int basketWidth, basketHeight;
	SDL_QueryTexture(basket, nullptr, nullptr, &basketWidth, &basketHeight);
	int basketX = ScreenWidth / 2 - basketWidth / 2;
	int basketY = ScreenHeight - basketWidth;
	// Создаём начальный список объектов
	size_t initCount = 5;
	for (size_t i = 0; i < initCount; ++i) {
		fruits.push_back(Fruit::generateFruit(m_renderer));
	}

	// Дальность перемещения корзины за 1 шаг
	const int crossDelta = 15;
	// Событие SDL
	SDL_Event evt;
	// Начельное оличество жизней
	size_t initLifeCount = 5;
	// Текущее количество жизней
	size_t lifeCount = initLifeCount;
	// Текущее количество очков
	int score = 0;
	// Задержка шага игры - 20 мс
	const uint32_t stepDelay = 50; // ms
	// Меню пользователя
	Menu menu(m_window, m_renderer);
	// Цикл обработки событий
	while (m_run) {
		if (0 == lifeCount) {
			if (showScore()) {
				break;
			}
			else {
				lifeCount = initLifeCount;
			}
		}
		SDL_Delay(stepDelay);
		// Обработка событий пользователя
		while (SDL_PollEvent(&evt)) {
			switch (evt.type) {
			case SDL_QUIT:
				m_run = false;
				break;
			case SDL_KEYDOWN:
				switch (evt.key.keysym.scancode)
				{
				case SDL_SCANCODE_LEFT:
					basketX -= crossDelta;
					break;
				case SDL_SCANCODE_RIGHT:
					basketX += crossDelta;
					break;
				case SDL_SCANCODE_ESCAPE:
					if (1 == menu.show(SDL_GetWindowSurface(m_window), m_font, { "Continue", "Exit" })) {
						m_run = false;
					}
					break;
				}
				break;
			}
		}

		// Отрисовка сцены

		// Очистка экрана
		SDL_RenderClear(m_renderer);
		// Фон
		renderTexture(background, 0, 0);
		// Корзина
		renderTexture(basket, basketX, basketY);
		// Обработка и прорисовка фруктов
		for (auto it = begin(fruits); it != end(fruits);) {
			(*it)->draw();
			if (!(*it)->caught(basketX + basketWidth / 2, basketY + basketHeight / 2)) { // центр корзины
				(*it)->move();
				if ((*it)->fell()) {
					// Упал, заменяем фрукт
					it = newFruit(it);
				}
				else {
					it++;
				}
			}
			else {
				// Поймали фрукт, создаём новый и подсчитываем очки
				int delta = (*it)->score();
				score += delta;
				if (-2 == delta)
					lifeCount--;
				it = newFruit(it);
			}
		}
		// Количество жизней
		{
			ostringstream os;
			os << "Lives: " << lifeCount << " Score: " << score;
			SDL_Texture *image = renderText(os.str().c_str(), m_font, { 150, 100, 0, 255 });
			if (image) {
				// Получаем размеры текста, чтобы разместить его на экране
				int w, h;
				SDL_QueryTexture(image, NULL, NULL, &w, &h);
				renderTexture(image, 20, 20);
			}
			else {
				logSDLError(cout, "renderText");
			}
		}
		// Отобразить сцену
		SDL_RenderPresent(m_renderer);
	} // while(active)
	
	m_stats[m_user] = score;
	cout << "lifeCount for " << m_user << " = " << lifeCount << endl;

	// Запись достижений в файл
	saveStats();

	// Очистка ресурсов

	cout << "del objects" << endl;
	// Удаление фруктов
	for (auto it = begin(fruits); it != end(fruits); it++) {
		delete (*it);
	}

	// освобождение ресурсов SDL
	
	cout << "del font" << endl;
	TTF_CloseFont(m_font);

	cout << "del basket" << endl;
	SDL_DestroyTexture(basket);
	
	cout << "del renderer" << endl;
	SDL_DestroyRenderer(m_renderer);
	cout << "del wnd" << endl;
	SDL_DestroyWindow(m_window);
	cout << "quit" << endl;
	SDL_Quit();

	return 0;
}

Game::Fruits::iterator Game::newFruit(Fruits::iterator fruit) {
	delete *fruit;
	auto result = fruits.erase(fruit);
	fruits.push_back(Fruit::generateFruit(m_renderer));
	return result;
}

void Game::saveStats()
{
	ofstream file("stats.bin", ios::binary);
	if (file)
	{
		cout << "STATISTICS:" << endl;
		for (const auto& item : m_stats) {
			cout << item.first << ": " << item.second << endl;
			file.write(item.first.c_str(), item.first.size() + 1);
			file.write((char*)(&item.second), sizeof(item.second));
		}
		file.close();
	}
}

void Game::loadStats()
{
	ifstream file("stats.bin", ios::binary);
	if (file)
	{
		// Смещаем указатель чтения в конец файла
		file.seekg(0, file.end);
		// Узнаём размер файла
		size_t size = file.tellg();
		cout << "length = " << size << endl;
		// Смещаем указатель чтения в начало файла
		file.seekg(0, file.beg);
		char* buffer = new char[size];
		file.read(buffer, size);
		if (file) {
			for (size_t i = 0, j = 0, userCount = 0; i < size;)
			{
				if ('\0' == buffer[i])
				{
					if (i < size - sizeof(Stats::mapped_type))
					{
						string name = &buffer[j];
						Stats::mapped_type score = *(Stats::mapped_type*)(&buffer[i + 1]);
						cout << name << ": score " << score << endl;
						m_stats[name] = score;
						if (++userCount == 10) // до 10 пользователей
							break;
						i += (sizeof(Stats::mapped_type) + 1);
						j = i;
					}
				}
				else
				{
					i++;
				}
			}
		}
		delete[] buffer;
		file.close();
	}
	else {
		cout << "failed to open file" << endl;
	}
}

bool Game::showScore()
{
	Menu menu(m_window, m_renderer);
	return (1 == menu.show(SDL_GetWindowSurface(m_window), m_font, { "Continue", "Exit" }));
}

void Game::stop()
{
	m_run = false;
}

void Game::renderTexture(SDL_Texture *tex, int x, int y, int w, int h) {
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(m_renderer, tex, nullptr, &dst);
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
	SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, surface);
	if (texture == nullptr) {
		logSDLError(cout, "CreateTexture");
	}
	SDL_FreeSurface(surface);
	return texture;
}

void Game::logSDLError(ostream &os, const string &msg) {
	os << msg << " failed: " << SDL_GetError() << endl;
}
