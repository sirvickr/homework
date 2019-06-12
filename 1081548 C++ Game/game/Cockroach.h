#pragma once

#include <string>

struct SDL_Renderer;
struct SDL_Texture;

class Cockroach
{
public:
	enum class Orient : int {
		up, down, left, right
	};
	Cockroach(int fieldW, int fieldH, SDL_Renderer* renderer, const char* imageName, Orient orient, int d = 1);
	~Cockroach();

	bool initGraphics(SDL_Renderer* renderer, const char* imageName);
	// ������� ��������� ��� (����� ���������)
	void move();
	// ���������� ���� �� ������
	void draw();
	// ����� �� ������� ������?
	bool away() const;
	// ��������� �� �����?
	bool evade(int x, int y) const;
	SDL_Renderer* renderer() const {
		return _renderer;
	}
	SDL_Texture* image() const {
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
	SDL_Renderer* _renderer = nullptr;
	SDL_Texture* _image = nullptr;
};
