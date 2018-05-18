#ifndef REPTILE_H_
#define REPTILE_H_

// ��������������

#include "Animal.h"

class Reptile: public Animal {
public:
	Reptile();
	Reptile(const std::string& kind, double weight, bool toxic);
	virtual ~Reptile();

	inline void Toxic(bool value) {
		toxic = value;
	}
	inline bool Toxic() const {
		return toxic;
	}

	// override: ����������� ����� ������ ���� ����������� � ������� ������
	void Display() const override;

private:
	bool toxic;
};

#endif /* REPTILE_H_ */
