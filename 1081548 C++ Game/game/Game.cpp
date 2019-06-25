#include "Game.h"
#include "Menu.h"
#include "Cockroach.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <list>
// для генерации пседвослучайных чисел (ПСЧ)
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

Game::Game(int scrWidth, int scrHeight)
:	_active(true), _scrWidth(scrWidth), _scrHeight(scrHeight), _results(10)
{
	// Загрузка результатов из файла
	ifstream is(fileName, ifstream::binary);
	if (is)
	{
		// Выясним размер файла
		is.seekg(0, is.end);
		size_t length = is.tellg();
		cout << "length = " << length << endl;
		// Смещаем указатель чтения в начало файла
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
	_userName = "at";
}

Game::~Game()
{
	// Сохранение результатов в файл
	ofstream os(fileName, ofstream::binary);
	if(os) 
	{
		Results::iterator it;
		// ищем текущего пользователя, чтобы обновить
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
			// не нашли, добавляем
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
				// сохраняем имя пользователя + символ завершения строки
				os.write(result.user.c_str(), result.user.size() + 1);
				// сохраняем результат пользователя
				os.write((char*)&result.score, sizeof(result.score));
			}
		}
	}
}

int Game::run()
{
	_window = SDL_CreateWindow("Cockroach hunt", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, _scrWidth, _scrHeight + 50, SDL_WINDOW_SHOWN);
	if (!_window) {
		logSDLError(cout, "SDL_CreateWindow");
		return 1;
	}
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!_renderer) {
		logSDLError(cout, "SDL_CreateRenderer");
		return 2;
	}
	// создаём шрифты
	_scoreFont = TTF_OpenFont("res/arial.ttf", 32);
	_tipsFont = TTF_OpenFont("res/arial.ttf", 18);
	if (!_scoreFont || !_tipsFont) {
		logSDLError(cout, "TTF_OpenFont");
		return 3;
	}
	SDL_Texture *txtScore = nullptr;
	SDL_Texture *txtTips = nullptr;
	// загружаем фон
	SDL_Texture *background = loadTexture("res/background.jpg");
	// загружаем прицел
	SDL_Texture *cross = loadTexture("res/target.png");
	// проверяем результат
	if (background == nullptr || cross == nullptr) {
		logSDLError(cout, "Loading images");
		return 4;
	}
	// размеры картинки с прицелом
	int xW, xH;
	SDL_QueryTexture(cross, nullptr, nullptr, &xW, &xH);
	int x = _scrWidth / 2 - xW / 2;
	int y = _scrHeight / 2 - xH / 2;
	// генераторы ПСЧ
	mt19937 gen;
	gen.seed(static_cast<uint32_t>(time(0)));
	uniform_int_distribution<> uidOrient(0, 3);
	uniform_int_distribution<> uidSpeed(1, 3);
	// создаём начальный список тараканов
	size_t initCount = 5;
	for (size_t i = 0; i < initCount; ++i) {
		auto index = uidOrient(gen);
		spawnCockroach(index, uidSpeed(gen));
	}

	_killCount = 0;

	// дальность перемещения прицела за 1 шаг
	const int crossDelta = 15;
	// событие SDL
	SDL_Event evt;
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
				_killCount = 0;
			}
		}
		SDL_Delay(stepDelay);
		cout << ".";
		// Обработка событий пользователя
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
					if (1 == menu.show(SDL_GetWindowSurface(_window), _scoreFont, { "Continue", "Exit" })) {
						stop();
					}
					break;
				case SDL_SCANCODE_SPACE:
					cout << "\nmenu index = " << menu.show(SDL_GetWindowSurface(_window), _scoreFont, { "Continue", "Exit", "Three", "Four" }) << endl;
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

		// очистка экрана
		SDL_RenderClear(_renderer);
		// заполнение фона
		renderTexture(background, 0, 0);
		// обработка и отрисовка тараканов
		for (auto it = begin(beetles); it != end(beetles); ) {
			(*it)->draw();
			if (!pressed || (*it)->evade(x + xW / 2, y + xH / 2)) { // центр прицела
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
				_killCount++;
			}
		}
		// прицел
		renderTexture(cross, x, y);
		// текст - количество очков
		{
			ostringstream os;
			os << "Score: " << _killCount;
			showText(os.str(), 10, 10, txtScore, _scoreFont, { 0, 100, 0, 255 });
		}
		// текст - подсказки по управлению
		showText("Use 'Left', 'Right', 'Up' and 'Down' arrows to move the target, Esc to show user menu", 10, _scrHeight + 10, txtTips, _tipsFont, { 50, 150, 0, 255 });
		// показ сцены в окне
		SDL_RenderPresent(_renderer);
	} // while(active)
	cout << _userName << ": " << _killCount << endl;

	// Очистка ресурсов

	// удаление списка тараканов
	for (auto it = begin(beetles); it != end(beetles); it++) {
		delete (*it);
	}
	
	// освобождение ресурсов SDL
	
	TTF_CloseFont(_scoreFont);
	_scoreFont = nullptr;
	TTF_CloseFont(_tipsFont);
	_tipsFont = nullptr;

	SDL_DestroyTexture(cross);
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(txtScore);

	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);

	_window = nullptr;
	_renderer = nullptr;

	return 0;
}

void Game::spawnCockroach(int index, int speed)
{
	beetles.push_back(new Cockroach(_scrWidth, _scrHeight, _renderer, imgNames[index],
		static_cast<Cockroach::Orient>(index), speed));
}

Game::Beetles::iterator Game::replaceCockroach(Beetles::iterator it, int index, int speed) {
	delete *it;
	Beetles::iterator result = beetles.erase(it);
	spawnCockroach(index, speed);
	return result;
}

bool Game::showScore()
{
	Menu menu(_window, _renderer);
	return (1 == menu.show(SDL_GetWindowSurface(_window), _scoreFont, { "Continue", "Exit" }));
}

void Game::stop()
{
	_active = false;
}

void Game::showText(const string& text, int x, int y, SDL_Texture* texture, _TTF_Font* font, const SDL_Color& color)
{
	if (texture)
		SDL_DestroyTexture(texture);
	texture = renderText(text.c_str(), font, color);
	if (texture)
	{
		// Получаем размеры текста, чтобы разместить его на экране
		//int w, h;
		//SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		renderTexture(texture, x, y);
	}
	else {
		logSDLError(cout, "renderText");
	}
}

SDL_Texture* Game::loadTexture(const string &file) {
	SDL_Texture *texture = IMG_LoadTexture(_renderer, file.c_str());
	if (texture == nullptr) {
		logSDLError(cout, "loadTexture");
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
	// Сперва рисуем текст на поверхности
	SDL_Surface *surface = TTF_RenderText_Blended(font, message.c_str(), color);
	if (!surface) {
		logSDLError(cout, "TTF_RenderText_Blended");
		return nullptr;
	}
	// Затем загружаем поверхность в текстуру
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
