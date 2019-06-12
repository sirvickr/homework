#pragma once

#include <SDL.h>
#include <SDL_image.h>

class Image
{
public:
	Image();
	~Image();
private:
	SDL_Texture* image;
};

