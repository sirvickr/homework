#ifndef PROVINCE_H_
#define PROVINCE_H_

// ќбласть

#include "Region.h"

class Country;

class Province: public Region {
public:
	Province(const std::string& name);
	virtual ~Province();

	// override: наследуемый метод об€зан быть объ€вленным в базовом классе
	void Display(bool brief) const override;

	void ParentCountry(Country* parent) {
		country = parent;
	}
	Country* ParentCountry() {
		return country;
	}

private:
	// hosting country
	Country* country = nullptr;
};

#endif /* PROVINCE_H_ */
