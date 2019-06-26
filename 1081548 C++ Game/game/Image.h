#pragma once

#include "ScreenObject.h"

#include <string>

class Image : public ScreenObject
{
public:
	Image(SDL_Surface* screen, const char* fileName);
	~Image() = default;
	std::string fileName() const {
		return _fileName;
	}

private:
	// имя файла с картинкой
	std::string _fileName;
};
