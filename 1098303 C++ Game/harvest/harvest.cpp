#include "Game.h"

#include <iostream>
#include <string>

using namespace std;

int ScreenWidth = 800;
int ScreenHeight = 600;

int main(int argc, char* argv[])
{
	string name;
	cout << "Input user name: ";
	cin >> name;
	Game game(name);
	return game.run();
}
