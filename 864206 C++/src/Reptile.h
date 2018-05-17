#ifndef REPTILE_H_
#define REPTILE_H_

// Пресмыкающееся

#include "Animal.h"

class Reptile: public Animal {
public:
	Reptile(double weight = 0.0);
	virtual ~Reptile();
};

#endif /* REPTILE_H_ */
