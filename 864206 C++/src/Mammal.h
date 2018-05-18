#ifndef MAMMAL_H_
#define MAMMAL_H_

// �������������

#include "Animal.h"

class Mammal: public Animal {
public:
	Mammal();
	Mammal(const std::string& kind, double weight);
	virtual ~Mammal();
};

#endif /* MAMMAL_H_ */
