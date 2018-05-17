#ifndef ANIMAL_H_
#define ANIMAL_H_

// ��������

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
	// ��� (�����)
	const std::string& kind;
	// ���
	double weight;
};

#endif /* ANIMAL_H_ */
