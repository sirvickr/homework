#include "tab.h"
#include "input.h"

#include <iomanip>
#include <iostream>

void tab(std::ostream& os, const std::function<double(double)>& fn) {
	auto xMin = input(std::cin, std::cout, -5.0, "Input xMin [-5.0]");
	auto xMax = input(std::cin, std::cout, +5.0, "Input xMax [+5.0]");
	auto dx = input(std::cin, std::cout, 0.5, "Input dx [0.5]");
	
	os << " x\tf(x)\n";
	for(auto x = xMin; x <= xMax; x += dx) {
		os << " " << std::fixed << std::setprecision(3) << x << '\t' << fn(x) << std::endl;
	}
	os.flush();
}
