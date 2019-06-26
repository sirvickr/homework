#pragma once

#include <SDL_rect.h>

struct SDL_Surface;

class ScreenObject
{
public:
	ScreenObject(SDL_Surface* screen);
	virtual ~ScreenObject();

	// ���������� ���� �� ������
	virtual void draw(SDL_Rect* dest = nullptr);
	// �������� �����������
	void clear();
	// ������ �������
	SDL_Surface* screen() const {
		return _screen;
	}
	SDL_Surface* surface() const {
		return _surface;
	}
	void surface(SDL_Surface* value);
	int x() const {
		return _pos.x;
	}
	void x(int value) {
		_pos.x = value;
	}
	int y() const {
		return _pos.y;
	}
	void y(int value) {
		_pos.y = value;
	}
	int w() const {
		return _pos.w;
	}
	int h() const {
		return _pos.h;
	}
	SDL_Rect pos() const {
		return _pos;
	}
	// ����������� �� �����������
	void locate(int x, int y) {
		_pos.x = x;
		_pos.y = y;
	}
	// ����������� �� ��� X
	void moveX(int delta) {
		_pos.x += delta;
	}
	// ����������� �� ��� Y
	void moveY(int delta) {
		_pos.y += delta;
	}
	// �������� ���������� �� �����������?
	bool inside(int x, int y) const {
		return (x >= _pos.x && x <= _pos.x + _pos.w && y >= _pos.y && y <= _pos.y + _pos.h);
	}

protected:
	SDL_Surface* _screen = nullptr;
	SDL_Surface* _surface = nullptr;
	SDL_Rect _pos = { 0, 0, 0, 0 };
};

