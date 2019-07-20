#pragma once

#include "NamedItem.h"

class Doctor : public NamedItem
{
public:
	// Список доступных специализаций
	static const size_t specializationCount = 6;
	static const tstring specializations[specializationCount];
	Doctor();
	Doctor(int id, const tstring& name,
		const tstring& birthDate,
		const tstring& specialization,
		int cabinetNumber);
	~Doctor() = default;

	void serialize(tostream& stream) const override;
	bool deserialize(tistream& stream) override;

	const tstring& BirthDate() const {
		return _birthDate;
	}
	void BirthDate(const tstring& value) {
		_birthDate = value;
	}

	const int CabinetNumber() const {
		return _cabinetNumber;
	}
	void CabinetNumber(int value) {
		_cabinetNumber = value;
	}

	const tstring& Specialization() const {
		return _specialization;
	}
	void Specialization(const tstring& value) {
		_specialization = value;
	}

private:
	tstring _birthDate;
	int _cabinetNumber = 0;
	tstring _specialization;
};

