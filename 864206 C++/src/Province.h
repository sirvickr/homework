#ifndef PROVINCE_H_
#define PROVINCE_H_

// �������

#include "Region.h"

class Province: public Region {
public:
	Province(const std::string& name);
	virtual ~Province();

	// override: ����������� ����� ������ ���� ����������� � ������� ������
	void Display() override;
};

#endif /* PROVINCE_H_ */
