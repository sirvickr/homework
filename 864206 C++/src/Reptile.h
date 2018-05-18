#ifndef REPTILE_H_
#define REPTILE_H_

// Пресмыкающееся

#include "Animal.h"

class Reptile: public Animal {
public:
	Reptile();
	Reptile(const std::string& kind, double weight = 0.0);
	virtual ~Reptile();
};

#endif /* REPTILE_H_ */
