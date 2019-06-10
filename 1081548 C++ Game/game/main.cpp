#include <windows.h>
#include <iostream>
#include <sstream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 40;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

SDL_Texture* LoadImage(const std::string& file){
	SDL_Texture *image = NULL;
	SDL_Surface *bmp = SDL_LoadBMP(file.c_str());
	if (bmp != NULL){
		image = SDL_CreateTextureFromSurface(renderer, bmp);
		SDL_FreeSurface(bmp);
	}
	else {
		std::cout << SDL_GetError() << std::endl;
		//MessageBox(NULL, "SDL_LoadBMP Error", file.c_str(), MB_ICONSTOP);
	}
	return image;
}

/*
* Loads an image into a texture on the rendering device
* @param file The image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == NULL){
		//logSDLError(std::cout, "LoadTexture");
		//MessageBox(NULL, "IMG_LoadTexture Error", file.c_str(), MB_ICONSTOP);
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
	SDL_RenderCopy(ren, tex, NULL, &dst);
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
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	renderTexture(tex, ren, x, y, w, h);
}

void ApplySurface(int x, int y, SDL_Texture *tex, SDL_Renderer *rend){
	SDL_Rect pos;
	pos.x = x;
	pos.y = y;
	SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);
	SDL_RenderCopy(rend, tex, NULL, &pos);
}

#if 1
int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
		//MessageBox(NULL, "SDL_Init failed", "Error", MB_ICONSTOP);
		return 1;
	}
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
		//logSDLError(std::cout, "IMG_Init");
		//MessageBox(NULL, "IMG_Init failed", "Error", MB_ICONSTOP);
		SDL_Quit();
		return 1;
	}
	#if 1
	window = SDL_CreateWindow("Lesson 2", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	#else
	window = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	#endif
	if (window == NULL){
		std::cout << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
		//MessageBox(NULL, "SDL_CreateWindow failed", "Error", MB_ICONSTOP);
		return 1;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL){
		std::cout << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
		//MessageBox(NULL, "SDL_CreateRenderer failed", "Error", MB_ICONSTOP);
		return 1;
	}
	
	#if 0
	SDL_Texture *background = loadTexture("img/background.png", renderer);
	SDL_Texture *image = loadTexture("img/image.png", renderer);
	if (background == NULL || image == NULL) {
		//MessageBox(NULL, "LoadImage failed", "Error", MB_ICONSTOP);
		return 4;
	}
	#else
	SDL_Texture *background = LoadImage("img/background.bmp");
	SDL_Texture *image = LoadImage("img/image.bmp");
	if (background == NULL || image == NULL) {
		//MessageBox(NULL, "LoadImage failed", "Error", MB_ICONSTOP);
		return 4;
	}
	/*SDL_Surface *bmp = SDL_LoadBMP("img/hello.bmp");
	if (bmp == NULL){
		std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
		//MessageBox(NULL, "SDL_LoadBMP failed", "Error", MB_ICONSTOP);
		return 1;
	}
	SDL_Texture *image = SDL_CreateTextureFromSurface(renderer, bmp);
	SDL_FreeSurface(bmp);
	if (image == NULL){
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		//MessageBox(NULL, "SDL_CreateTextureFromSurface failed", "Error", MB_ICONSTOP);
		return 1;
	}*/
	#endif
	
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, background, NULL, NULL);
	SDL_RenderPresent(renderer);
	
	SDL_Delay(2000);

	//Determine how many tiles we'll need to fill the screen
	int xTiles = SCREEN_WIDTH / TILE_SIZE;
	int yTiles = SCREEN_HEIGHT / TILE_SIZE;
	//Draw the tiles by calculating their positions
	/*for (int i = 0; i < xTiles * yTiles; ++i){
		int x = i % xTiles;
		int y = i / xTiles;
		renderTexture(background, renderer, x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE,
			TILE_SIZE);
	}
	// Draw foreground
	int iW, iH;
	SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
	int x = SCREEN_WIDTH / 2 - iW / 2;
	int y = SCREEN_HEIGHT / 2 - iH / 2;
	renderTexture(image, renderer, x, y);*/

	/*SDL_Event e;
	bool quit = false;
	while (!quit){
		while (SDL_PollEvent(&e)){
			if (e.type == SDL_QUIT){
				quit = true;
			}
			if (e.type == SDL_KEYDOWN){
				quit = true;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN){
				quit = true;
			}
		}
		//Render the scene
		SDL_RenderClear(renderer);
		renderTexture(image, renderer, x, y);
		SDL_RenderPresent(renderer);
	}*/

	SDL_DestroyTexture(image);
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

