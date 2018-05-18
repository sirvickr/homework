#include "Mammal.h"

Mammal::Mammal() : Animal("mammal", 0.0) {
}

Mammal::Mammal(const std::string& kind, double weight) : Animal(kind, weight) {
}

Mammal::~Mammal() {
}
