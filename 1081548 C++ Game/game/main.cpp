#include <iostream>

#include "Game.h"
#include "StartScreen.h"
#include "InputWindow.h"
#include "Utils.h"

static const char* gameName = "Cockroach race";

int main(int argc, char* argv[])
{
	if (!initializeGraphics()) {
		std::cerr << "Failed to initialize graphics" << std::endl;
		return 1;
	}

	StartScreen screen(600, 400, gameName);
	screen.show();

	std::string userName = "";
	bool requestUserName = true;
	while (requestUserName) {
		InputWindow dialog(400, 100, "input player name");
		dialog.show();
		if (!dialog.confirmed()) {
			std::cout << "cancelled by user" << std::endl;
			return 0;
		}
		userName = dialog.input();
		if (userName.size() > 0) {
			requestUserName = false;
		}
		else {
			std::cout << "user name is empty" << std::endl;
		}
	}

	std::cout << "user: " << userName << std::endl;
	Game game(800, 600, gameName, userName);
	game.show();

	finalizeGraphics();

	return 0;
}
