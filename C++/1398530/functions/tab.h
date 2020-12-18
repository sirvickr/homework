#pragma once

#include <functional>

// модуль для вычисления функции f(x)
void tab(std::ostream& os, const std::function<double(double)>& fn);
