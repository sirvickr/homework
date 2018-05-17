#include "Animal.h"

Animal::Animal(const std::string& kind, double weight) : kind(kind), weight(weight) {
}

// пустая заглушка для виртуального деструктора
Animal::~Animal() {
}

