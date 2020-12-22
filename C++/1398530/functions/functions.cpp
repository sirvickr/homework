#include "pch.h"
#include "fx.h"  // модуль для вычисления функции f(x)
#include "yx.h"  // модуль для вычисления функции y(x)
#include "tab.h" // модуль абуляции значений
#include "input.h"

#include <iostream>
#include <fstream>
#include <cmath>

std::ostream& get_ostream() {
	std::cout << "[1]: console\n";
	std::cout << "[2]: file\n";
	auto choice = input(std::cin, std::cout, 1, "Select output medium");

	if (choice == 1)
		return std::cout;

	static std::ofstream file;
	if (!file.is_open()) {
		auto name = input(std::cin, std::cout, std::string("out.txt"), "Select file name");
		file.open(name);
	}
	return file;
}

int main()
{
	// интерфейс программы
	std::cout << "[1]: f(x) = x + sin(x)\n";
	std::cout << "            3,                if x <= -4\n";
	std::cout << "[2]: y(x) = x^2 - 4|x| + 3,   if -4 <= x <= 4\n";
	std::cout << "            3 - (x - 4)^2,    if x > 4\n";
	auto fn = input(std::cin, std::cout, 1, "Input function number");
	auto& output = get_ostream();
	if (!output) {
		std::cerr << "invalid stream" << std::endl;
		return -1;
	}
	switch (fn) {
	case 1:
		tab(output, fx);
		break;
	case 2:
		tab(output, yx);
		break;
	}
	std::cout << "Press any key... ";
	std::cin.get();
	return 0;
}
