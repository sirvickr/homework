#pragma once

#include "ScreenObject.h"

class Cockroach : public ScreenObject
{
public:
	enum class Orient : int {
		up, down, left, right
	};
	Cockroach(int fieldW, int fieldH, SDL_Surface* screen, const char* imageName, Orient orient, int d = 1);
	~Cockroach() = default;

	bool initGraphics(const char* imageName);
	// сделать следующий шаг (смена координат)
	void move();
	// вышел за пределы экрана?
	bool away() const;
	// увернулся от тапка?
	bool evade(int x, int y) const;
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

private:
	Orient _orient = Orient::right;
	int _d = 0;
	int _fieldW = 0;
	int _fieldH = 0;
};
