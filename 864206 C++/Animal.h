#ifndef ANIMAL_H_
#define ANIMAL_H_

// ��������

#include <string>

class Province;

class Animal {
public:
	Animal(const std::string& kind, double weight);
	virtual ~Animal() = 0; // ��������� ��������� ���������� ������������ ������

	inline std::string Kind() const {
		return kind;
	}

	inline void Weight(double value) {
		weight = value;
	}
	inline double Weight() const {
		return weight;
	}

	inline void Home(Province* value) {
		home = value;
	}
	inline Province* Home() const {
		return home;
	}

	virtual void Display() const = 0;

	static size_t InstanceCount();

protected:
	// ���
	std::string kind;
	// ���
	double weight;
	//
	Province* home;

private:
	static size_t instCount;

};

#endif /* ANIMAL_H_ */
