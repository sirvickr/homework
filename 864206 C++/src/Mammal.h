#ifndef MAMMAL_H_
#define MAMMAL_H_

// ћлекопитающее

#include "Animal.h"

class Mammal: public Animal {
public:
	Mammal();
	Mammal(const std::string& kind, double weight);
	virtual ~Mammal();

	// override: наследуемый метод об€зан быть объ€вленным в базовом классе
	void Display() const override;
};

#endif /* MAMMAL_H_ */
