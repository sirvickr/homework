#include "Fruit.h"

#include <SDL.h>
#include <SDL_image.h>
#include <cstdlib>

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
	int type = (rand() % 2) * 3;
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
