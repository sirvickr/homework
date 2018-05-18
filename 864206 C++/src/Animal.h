#ifndef ANIMAL_H_
#define ANIMAL_H_

// ∆ивотное

#include <string>

class Animal {
public:
	Animal(const std::string& kind, double weight);
	virtual ~Animal() = 0; // запрещаем создавать экземпл€ры абстрактного класса

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
	// вид
	const std::string& kind;
	// вес
	double weight;

private:
	static const size_t MAX_ANIMALS_COUNT = 20;
	static Animal* animals[MAX_ANIMALS_COUNT];
	static size_t animalsCount;

};

#endif /* ANIMAL_H_ */
