#ifndef REGION_H_
#define REGION_H_

// ������ (��������������� �������)

#include <string>
#include <vector>

class Animal;

class Region {
public:
	Region(const std::string& name);
	virtual ~Region() = 0;

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

	virtual void Display() = 0;

protected:
	// ��� (�����)
	std::string kind;
	// ��������
	std::string name;
	// �����
	std::vector< Animal* > animals;
};

#endif /* REGION_H_ */
