#ifndef PROVINCE_H_
#define PROVINCE_H_

// ќбласть

#include "Region.h"

class Province: public Region {
public:
	Province(const std::string& name);
	virtual ~Province();

	// override: наследуемый метод об€зан быть объ€вленным в базовом классе
	void Display() override;
};

#endif /* PROVINCE_H_ */
