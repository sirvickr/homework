#include <clocale>
#include <sstream>

#include "Menu.h"

int main(int argc, char** argv) {
	setlocale(LC_CTYPE, "Russian");
	int timeout = 120;
	if(argc > 1) {
		std::istringstream iss;
		iss.str(argv[1]);
		int value = 0;
		if(iss >> value) {
			timeout = value;
		}
	}
	Menu menu(timeout);
	return menu.run();
}

