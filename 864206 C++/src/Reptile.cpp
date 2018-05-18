#include "Reptile.h"

Reptile::Reptile() : Animal("reptile", weight) {
}

Reptile::Reptile(const std::string& kind, double weight) : Animal(kind, weight) {
}

Reptile::~Reptile() {
}

