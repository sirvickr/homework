#ifndef ANIMAL_H_
#define ANIMAL_H_

// Животное

#include <string>

class Animal {
public:
	Animal(const std::string& kind, double weight);
	virtual ~Animal() = 0;

	inline std::string Kind() const {
		return kind;
	}

	inline void Weight(double value) {
		weight = value;
	}
	inline double Weight() const {
		return weight;
	}

protected:
	// вид (класс)
	const std::string& kind;
	// вес
	double weight;
};

#endif /* ANIMAL_H_ */
