#ifndef REGION_H_
#define REGION_H_

// ������ (��������������� �������)

#include <string>

class Region {
public:
	Region(const std::string& name);
	virtual ~Region() = 0; // ��������� ��������� ���������� ������������ ������

	inline void Name(const std::string& value) {
		name = value;
	}
	inline std::string Name() const {
		return name;
	}

	static size_t InstanceCount();

	virtual void Display(bool brief) const = 0;

protected:
	// ��������
	std::string name;

private:
	static size_t instCount;
};

#endif /* REGION_H_ */
