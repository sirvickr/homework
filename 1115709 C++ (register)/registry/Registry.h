#pragma once

#include "Doctor.h"
#include "Schedule.h"

class Registry
{
public:
	Registry();
	~Registry();
	
	std::list<DbItem*> Doctors() const {
		return _doctors;
	}
	std::list<DbItem*> Schedule() const {
		return _schedule;
	}

	void AddVisitToSchedule(int idDoc, int day, const tstring& patient, 
		const tstring& time, const tstring& span);

private:
	// ��������� ���������, ���������� ����� ������ ������
	static const TCHAR* doctorsFileName;
	static const TCHAR* scheduleFileName;

	std::list<DbItem*> _doctors;
	std::list<DbItem*> _schedule;

	int maxId(const std::list<DbItem*>& items);
	void deleteById(std::list<DbItem*>& items, int id);
	UniqueItem* findById(const std::list<DbItem*>& items, int id);
	void clearList(std::list<DbItem*>& items);
	int printList(tostream& stream, const std::list<DbItem*>& items, const TCHAR* title = nullptr);
	void saveList(const std::list<DbItem*>& items, const TCHAR* fileName);
	bool loadList(std::list<DbItem*>& items, const TCHAR* fileName);

	// ������� ��� �������� ��������.
	// �����, �.�. ����� �����, ����� �������������
	// �������� - ������� �������� ����������� �������.
	// ��� ��������� ����� � ������� ���������������,
	// ������� ����� ������ � ���� ������-���������, 
	// �.�. ��������� ���� 
	// [](���������_�������) -> ������������_�������� { 
	//		����_������� 
	// }
	std::map<tstring, std::function<DbItem* ()>> factory{
		{ doctorsFileName, []() -> DbItem* {
			return new Doctor();
		} },
		{ scheduleFileName, []() -> DbItem* {
			return new TimeSchedule();
		} },
	};
};

