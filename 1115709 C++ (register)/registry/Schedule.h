#pragma once

#include "DbItem.h"

class TimeSchedule : public DbItem
{
public:
	static const TCHAR* Week[7];
	TimeSchedule();
	~TimeSchedule() = default;

	void serialize(tostream& stream) const override;
	bool deserialize(tistream& stream) override;

	const int DoctorID() const {
		return _idDoc;
	}
	void DoctorID(int value) {
		_idDoc = value;
	}

	const int Day() const {
		return _day;
	}
	void Day(int value) {
		_day = value;
	}

	const tstring& Patient() const {
		return _patient;
	}
	void Patient(const tstring& value) {
		_patient = value;
	}

	const tstring& Time() const {
		return _time;
	}
	void Time(const tstring& value) {
		_time = value;
	}

	const tstring& Span() const {
		return _duration;
	}
	void Span(const tstring& value) {
		_duration = value;
	}

private:
	int _idDoc = 0;
	int _day = 0;
	tstring _patient;
	tstring _time;
	tstring _duration;
};

