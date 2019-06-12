//#include <windows.h>
#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <SDL.h>
#include <SDL_image.h>
#include <list>

#include "Cockroach.h"

const int SCREEN_WIDTH = 1000;// 640;
const int SCREEN_HEIGHT = 700;// 480;
const int TILE_SIZE = 40;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message to
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(std::ostream &os, const std::string &msg) {
	os << msg << " failed: " << SDL_GetError() << std::endl;
}
#if 0
SDL_Texture* LoadImage(const std::string& file){
	SDL_Texture *image = nullptr;
	SDL_Surface *bmp = SDL_LoadBMP(file.c_str());
	if (bmp != nullptr){
		image = SDL_CreateTextureFromSurface(renderer, bmp);
		SDL_FreeSurface(bmp);
	}
	else {
		logSDLError(std::cout, "SDL_CreateTextureFromSurface");
	}
	return image;
}
#endif
/*
* Loads an image into a texture on the rendering device
* @param file The image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr){
		logSDLError(std::cout, "loadTexture");
	}
	return texture;
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
* width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
* @param w The width of the texture to draw
* @param h The height of the texture to draw
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, nullptr, &dst);
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
	int w, h;
	SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
	renderTexture(tex, ren, x, y, w, h);
}

void ApplySurface(int x, int y, SDL_Texture *tex, SDL_Renderer *rend){
	SDL_Rect pos;
	pos.x = x;
	pos.y = y;
	SDL_QueryTexture(tex, nullptr, nullptr, &pos.w, &pos.h);
	SDL_RenderCopy(rend, tex, nullptr, &pos);
}

using Beetles = std::list<Cockroach*>;

Beetles beetles;

struct StartParams {
	SDL_Point pt;
	int delta;
	const char* imgName;
};

static const int delta = 1;
// up, down, left, right
StartParams startParams[] = {
	{ { SCREEN_WIDTH / 2,  SCREEN_HEIGHT     }, -delta, "img/crU.jpg" },
	{ { SCREEN_WIDTH / 2,  0                 },  delta, "img/crD.jpg" },
	{ { SCREEN_WIDTH,      SCREEN_HEIGHT / 2 }, -delta, "img/crL.jpg" },
	{ { 0,                 SCREEN_HEIGHT / 2 },  delta, "img/crR.jpg" },
};

#if 1
int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
		logSDLError(std::cout, "IMG_Init");
		SDL_Quit();
		return 1;
	}
	#if 1
	window = SDL_CreateWindow("Lesson 2", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	#else
	window = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	#endif
	if (window == nullptr){
		logSDLError(std::cout, "SDL_CreateWindow");
		return 1;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr){
		logSDLError(std::cout, "SDL_CreateRenderer");
		return 1;
	}
	
	#if 1
	SDL_Texture *background = loadTexture("img/Lesson3/background.png", renderer);
//	SDL_Texture *image = loadTexture("img/Lesson3/image.png", renderer);
	SDL_Texture *cross = loadTexture("img/target.png", renderer);
	/*SDL_Texture *crU = loadTexture("img/crU.jpg", renderer);
	SDL_Texture *crD = loadTexture("img/crD.jpg", renderer);
	SDL_Texture *crL = loadTexture("img/crL.jpg", renderer);
	SDL_Texture *crR = loadTexture("img/crR.jpg", renderer);*/
	if (background == nullptr || cross == nullptr/* || crU == nullptr || crD == nullptr || crL == nullptr || crR == nullptr*/) {
		logSDLError(std::cout, "Loading images");
		return 4;
	}
	#else
	SDL_Texture *background = LoadImage("img/Lesson2/background.bmp");
	SDL_Texture *image = LoadImage("img/Lesson2/image.bmp");
	if (background == nullptr || image == nullptr) {
		logSDLError(std::cout, "LoadImage");
		return 4;
	}
	#endif
	/*
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, background, nullptr, nullptr);
	*/
#if 0
	//Determine how many tiles we'll need to fill the screen
	int xTiles = SCREEN_WIDTH / TILE_SIZE;
	int yTiles = SCREEN_HEIGHT / TILE_SIZE;
	//Draw the tiles by calculating their positions
	for (int i = 0; i < xTiles * yTiles; ++i){
		int x = i % xTiles;
		int y = i / xTiles;
		renderTexture(background, renderer, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
	}
#endif
	int iW, iH;
	SDL_QueryTexture(cross, nullptr, nullptr, &iW, &iH);
	int x = SCREEN_WIDTH / 2 - iW / 2;
	int y = SCREEN_HEIGHT / 2 - iH / 2;
#if 1
	// Draw foreground
	renderTexture(cross, renderer, x, y);

	SDL_RenderPresent(renderer);
#endif

	Cockroach::Orient orient = Cockroach::Orient::down;
	const auto& startPoint = startParams[static_cast<int>(orient)];
	//SDL_Texture *cr = loadTexture(startPoint.imgName, renderer);
	SDL_Texture *cr = loadTexture("img/crU.jpg", renderer);
	beetles.push_back(new Cockroach(renderer, cr, startPoint.imgName, orient, startPoint.pt.x, startPoint.pt.y, startPoint.delta));

	const int delta = 10;
	SDL_Event evt;
	bool quit = false;
	while (!quit){
		SDL_Delay(50);
		std::cout << ".";
		while (SDL_PollEvent(&evt)){
			switch (evt.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				//std::cout << " \nscancode " << evt.key.keysym.scancode << " vk " << evt.key.keysym.sym << std::endl;
				switch (evt.key.keysym.scancode)
				{
				case SDL_SCANCODE_UP:
					y -= delta;
					break;
				case SDL_SCANCODE_DOWN:
					y += delta;
					break;
				case SDL_SCANCODE_LEFT:
					x -= delta;
					break;
				case SDL_SCANCODE_RIGHT:
					x += delta;
					break;
				case SDL_SCANCODE_ESCAPE:
					quit = true;
					break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				std::cout << " \nmouse " << evt.button.x << " " << evt.button.y << std::endl;
				//quit = true;
				break;
			}
		}
		// Отрисовка сцены
#if 1
		// очистка экрана
		SDL_RenderClear(renderer);
		// заполнение фона
		//Determine how many tiles we'll need to fill the screen
		int xTiles = SCREEN_WIDTH / TILE_SIZE;
		int yTiles = SCREEN_HEIGHT / TILE_SIZE;
		//Draw the tiles by calculating their positions
		for (int i = 0; i < xTiles * yTiles; ++i) {
			int x = i % xTiles;
			int y = i / xTiles;
			renderTexture(background, renderer, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
		}
		// тараканы
		for (auto it = std::begin(beetles); it != std::end(beetles); it++) {
			(*it)->draw();
			// центр прицела
			if ((*it)->evade(x + iW / 2, y + iH / 2)) {
				(*it)->move();
			}
			else { // не увернулся..
				std::cout << "############################################" << std::endl;
				std::cout << "################### ++++ ###################" << std::endl;
				std::cout << "############################################" << std::endl;
			}
		}
		// прицел
		renderTexture(cross, renderer, x, y);

		SDL_RenderPresent(renderer);
#endif
	}

	for (auto it = std::begin(beetles); it != std::end(beetles); it++) {
		delete (*it);
	}

	SDL_DestroyTexture(cross);
	SDL_DestroyTexture(cr);
	/*SDL_DestroyTexture(crU);
	SDL_DestroyTexture(crD);
	SDL_DestroyTexture(crL);
	SDL_DestroyTexture(crR);*/

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}

#else
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		//MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Caption",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		640, /* width */
		480, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		//MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}
#endif

