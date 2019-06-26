#include "Image.h"
#include "Utils.h"

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>

Image::Image(SDL_Surface* screen, const char* fileName)
	: ScreenObject(screen), _fileName(fileName)
{
	surface(IMG_Load(fileName));
	if (surface()) {
		logSDLError(std::cerr, "renderText");
	}
}
