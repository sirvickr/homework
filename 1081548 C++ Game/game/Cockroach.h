#pragma once

#include <string>

struct SDL_Renderer;
struct SDL_Surface;

class Cockroach
{
public:
	enum class Orient : int {
		up, down, left, right
	};
	Cockroach(int fieldW, int fieldH, SDL_Surface* renderer, const char* imageName, Orient orient, int d = 1);
	~Cockroach();

	bool initGraphics(SDL_Surface* renderer, const char* imageName);
	// сделать следующий шаг (смена координат)
	void move();
	// нарисовать себя на экране
	void draw();
	// вышел за пределы экрана?
	bool away() const;
	// увернулся от тапка?
	bool evade(int x, int y) const;
	SDL_Surface* screen() const {
		return _screen;
	}
	SDL_Surface* image() const {
		return _image;
	}
	Orient orient() const {
		return _orient;
	}
	void orient(Orient value) {
		_orient = value;
	}
	int d() const {
		return _d;
	}
	void d(int value) {
		_d = value;
	}
	int x() const {
		return _x; 
	}
	void x(int value) {
		_x = value;
	}
	int y() const {
		return _y;
	}
	void y(int value) {
		_y = value;
	}
	int w() const {
		return _w;
	}
	int h() const {
		return _h;
	}

private:
	Orient _orient = Orient::right;
	int _d = 0;
	int _x = 0;
	int _y = 0;
	int _w = 0;
	int _h = 0;
	int _fieldW = 0;
	int _fieldH = 0;
	SDL_Surface* _screen = nullptr;
	SDL_Surface* _image = nullptr;
};
