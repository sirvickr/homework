#pragma once

#include <string>
#include <list>

struct SDL_Window;
struct SDL_Surface;
struct SDL_Color;
struct _TTF_Font;

class ScreenObject;

class Window
{
public:
	Window(int width, int height, const std::string& title = "");
	virtual ~Window();

	void stop();

protected:
	virtual int show() = 0;

protected:
	using Objects = std::list<ScreenObject*>;
	// ����������� �������
	SDL_Window* _window = nullptr;
	SDL_Surface* _screen = nullptr;
	
	// ������ �������� �� ������
	Objects objects;

	// ������� ����
	int _scrWidth, _scrHeight;

	// ���� ���������� ����� �������� �������
	bool _active;

	// ��������� ����
	const std::string _title;
};

