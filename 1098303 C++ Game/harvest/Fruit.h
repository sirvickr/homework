#pragma once

struct SDL_Renderer;
struct SDL_Texture;

class Fruit
{
public:
	enum Type {
		apple = 0,
		pear = 3,
	};
	enum State {
		ripe,
		unripe,
		sick,
		dead
	};

	Fruit(SDL_Renderer* renderer, Type type, State state);
	virtual ~Fruit();
	
	Type type() const {
		return m_type;
	}
	State state() const {
		return m_state;
	}

	// Пересчитать координату y при падении
	void move();
	// Функция отрисовки
	void draw();
	// Фрукт упал?
	bool fell() const;
	//
	static Fruit* generateFruit(SDL_Renderer* renderer);

private:
	static const char* PictureNames[];
	SDL_Renderer* m_renderer;
	SDL_Texture* m_image = nullptr;
	Type m_type;
	State m_state;
	int m_x = 0, m_y = 0, m_dy = 1;
	int m_width = 0, m_height = 0;
};

