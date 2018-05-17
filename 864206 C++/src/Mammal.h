#ifndef MAMMAL_H_
#define MAMMAL_H_

// ћлекопитающее

#include "Animal.h"

class Mammal: public Animal {
public:
	Mammal(double weight = 0.0);
	virtual ~Mammal();
};

#endif /* MAMMAL_H_ */
