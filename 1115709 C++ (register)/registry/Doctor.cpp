#include "stdafx.h"
#include "Doctor.h"

using namespace std;

// Список доступных специализаций
const tstring Doctor::specializations[specializationCount] = {
	_T("Терапевт"),
	_T("Офтальмолог"),
	_T("Хирург"),
	_T("Гинеколог"),
	_T("Отоляринголог"),
	_T("Невролог"),
};

Doctor::Doctor()
{
}

Doctor::Doctor(int id, const tstring& name, const tstring& birthDate, 
	const tstring& specialization, int cabinetNumber)
	: NamedItem(id, name), _birthDate(birthDate), _specialization(specialization)
	, _cabinetNumber(cabinetNumber)
{

}

void Doctor::serialize(tostream& stream) const
{
	NamedItem::serialize(stream);
	stream << _birthDate << '\t' << _cabinetNumber << '\t' << _specialization << '\t';
}

bool Doctor::deserialize(tistream& stream)
{
	return NamedItem::deserialize(stream) && (stream >> _birthDate) && (stream >> _cabinetNumber) && (stream >> _specialization);
}
