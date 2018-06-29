#include <clocale>
#include <sstream>

#include "Concert.h"

// Точка входа приложения
int main(int argc, char** argv) {
	// Установка кириллицы в консоли
	setlocale(LC_CTYPE, "Russian");
	// Получение таймаута из параметра командной строки
	int timeout = 120;
	if(argc > 1) {
		std::istringstream iss;
		iss.str(argv[1]);
		int value = 0;
		if(iss >> value) {
			timeout = value;
		}
	}
	// Основной объект приложения
	Concert concert(timeout);
	return concert.run();
}

