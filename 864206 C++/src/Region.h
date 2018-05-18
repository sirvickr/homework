#ifndef REGION_H_
#define REGION_H_

// –егион (территориальна€ единица)

#include <string>
#include <vector>

class Animal;

class Region {
public:
	Region(const std::string& name);
	virtual ~Region() = 0; // запрещаем создавать экземпл€ры абстрактного класса

	inline void Kind(const std::string& value) {
		kind = value;
	}
	inline std::string Kind() const {
		return kind;
	}

	inline void Name(const std::string& value) {
		name = value;
	}
	inline std::string Name() const {
		return name;
	}

	void AddAnimal(const Animal* animal);

	virtual void Display() = 0;

protected:
	// вид (класс)
	std::string kind;
	// название
	std::string name;
	// фауна
	std::vector< const Animal* > animals;

private:
	static const size_t MAX_REGIONS_COUNT = 10;
	static Region* regions[MAX_REGIONS_COUNT];
	static size_t regionsCount;

};

#endif /* REGION_H_ */
