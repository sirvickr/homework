#ifndef COUNTRY_H_
#define COUNTRY_H_

// ������

#include "Region.h"

class Country: public Region {
public:
	Country(const std::string& name);
	virtual ~Country();

	// override: ����������� ����� ������ ���� ����������� � ������� ������
	void Display(bool brief) const override;

};

#endif /* COUNTRY_H_ */
