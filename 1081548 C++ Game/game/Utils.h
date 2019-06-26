#pragma once

#include <string>
#include <ostream>

// инициализация графических библиотек
bool initializeGraphics();
// освобождение графических библиотек
void finalizeGraphics();
// вывод ошибки на экран
void logSDLError(std::ostream &os, const std::string &msg);
