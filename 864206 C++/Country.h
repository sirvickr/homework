#ifndef COUNTRY_H_
#define COUNTRY_H_

// Страна

#include "Region.h"

class Country: public Region {
public:
	Country(const std::string& name);
	virtual ~Country();

	// override: наследуемый метод обязан быть объявленным в базовом классе
	void Display(bool brief) const override;

};

#endif /* COUNTRY_H_ */
