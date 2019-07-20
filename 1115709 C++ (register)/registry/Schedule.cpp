#include "stdafx.h"
#include "Schedule.h"

const TCHAR* TimeSchedule::Week[] = {
	_T("пн"),
	_T("вт"),
	_T("ср"),
	_T("чт"),
	_T("пт"),
	_T("сб"),
	_T("вс"),
};

TimeSchedule::TimeSchedule()
{
}

void TimeSchedule::serialize(tostream& stream) const
{
	stream << _idDoc << '\t' << _day << '\t' << _patient 
		<< '\t' << _time << '\t' << _duration << '\t';
}

bool TimeSchedule::deserialize(tistream& stream)
{
	return (stream >> _idDoc)
		&& (stream >> _day)
		&& (stream >> _patient)
		&& (stream >> _time)
		&& (stream >> _duration);
}
