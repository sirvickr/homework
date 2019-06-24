#include "Fruit.h"
#include "Geometry.h"

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <cstdlib>

using namespace std;

extern int ScreenWidth;
extern int ScreenHeight;

const char* Fruit::PictureNames[] = {
	"img/apple-ripe.png",
	"img/apple-unripe.png",
	"img/apple-sick.png",
	"img/apple-dead.png",
	"img/pear-ripe.png",
	"img/pear-unripe.png",
	"img/pear-sick.png",
	"img/pear-dead.png",
};

Fruit* Fruit::generateFruit(SDL_Renderer* renderer)
{
	int type = (rand() % 2) * pear;
	int state = rand() % 4;
	return new Fruit(renderer, static_cast<Type>(type), static_cast<State>(state));
}

Fruit::Fruit(SDL_Renderer* renderer, Type type, State state)
	: m_renderer(renderer), m_type(type), m_state(state)
{
	int imgIndex = type + state;
	m_image = IMG_LoadTexture(renderer, PictureNames[imgIndex]);
	if (m_image) {
		SDL_QueryTexture(m_image, nullptr, nullptr, &m_width, &m_height);
		m_x = rand() % (ScreenWidth - m_width);
		m_dy = 2 + (rand() % 2);
		m_y = 0;
	}
}

Fruit::~Fruit()
{
	if (m_image) {
		SDL_DestroyTexture(m_image);
		m_image = nullptr;
	}
}

void Fruit::move()
{
	m_y += m_dy;
}

void Fruit::draw()
{
	SDL_Rect dst;
	dst.x = m_x;
	dst.y = m_y;
	dst.w = m_width;
	dst.h = m_height;
	SDL_RenderCopy(m_renderer, m_image, nullptr, &dst);
}

bool Fruit::fell() const
{
	return m_y + m_height > ScreenHeight;
}

bool Fruit::caught(int x, int y) const
{
	vector<Geometry::point> polygon {
		{ double(m_x),           double(m_y) },
		{ double(m_x + m_width), double(m_y) },
		{ double(m_x + m_width), double(m_y + m_height) },
		{ double(m_x),           double(m_y + m_height) }
	};
	return Geometry::point_in_polygon(Geometry::point(x, y), polygon);
}

int Fruit::score() const
{
	switch (m_state) {
	case ripe:
		return 2;
	case unripe:
		return 1;
	case sick:
		return -1;
	case dead:
		return -2;
	}
	return 0;
}
