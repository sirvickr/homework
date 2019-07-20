#include "stdafx.h"
#include "Registry.h"

using namespace std;

// Строковые константы, содержащие имена файлов данных
const TCHAR* Registry::doctorsFileName = _T("db/doctors");
const TCHAR* Registry::scheduleFileName = _T("db/schedule");

Registry::Registry()
{
	loadList(_doctors, doctorsFileName);
	loadList(_schedule, scheduleFileName);
	DbItem* item = factory[doctorsFileName]();
#if 0
	Doctor* doc = dynamic_cast<Doctor*>(item);
	if (doc) {
		doc->Id(_doctors.size() + 1);
		doc->Name(_T("Иванов"));
		doc->BirthDate(_T("10.05.1979"));
		doc->CabinetNumber(111);
		doc->Specialization(_T("2"));
		_doctors.push_back(item);
	}
#endif
}

Registry::~Registry()
{
	saveList(_doctors, doctorsFileName);
	saveList(_schedule, scheduleFileName);
}

void Registry::AddVisitToSchedule(int idDoc, int day, const tstring& patient,
	const tstring& time, const tstring& span)
{
	TimeSchedule* item = dynamic_cast<TimeSchedule*>(factory[scheduleFileName]());
	if (item) {
		item->DoctorID(idDoc);
		item->Day(day);
		item->Patient(patient);
		item->Time(time);
		item->Span(span);
		_schedule.push_back(item);
	}
}

int Registry::maxId(const list<DbItem*>& items) {
	int result = 0;
	for (auto item : items) {
		if (auto unique = dynamic_cast<UniqueItem*>(item)) {
			if (result < unique->Id()) {
				result = unique->Id();
			}
		}
	}
	return result;
}

void Registry::deleteById(list<DbItem*>& items, int id) {
	items.remove_if([id](const DbItem* item) {
		if (auto unique = dynamic_cast<const UniqueItem*>(item)) {
			if (unique->Id() == id)
				return true;
		}
		return false;
	});
}

UniqueItem* Registry::findById(const list<DbItem*>& items, int id) {
	auto it = find_if(begin(items), end(items), [id](const DbItem* item) {
		if (auto unique = dynamic_cast<const UniqueItem*>(item)) {
			if (unique->Id() == id)
				return true;
		}
		return false;
	});
	if (it == end(items))
		return nullptr;
	return dynamic_cast<UniqueItem*>(*it);
}

void Registry::clearList(list<DbItem*>& items) {
	for (auto item : items) {
		if (item) {
			delete item;
			item = nullptr;
		}
	}
	items.clear();
}

int Registry::printList(tostream& stream, const list<DbItem*>& items, const TCHAR* title) {
	int result = 0;
	if (title)
		stream << title << endl;
	for (const auto& item : items) {
		item->serialize(stream);
		stream << endl;
	}
	return result;
}

void Registry::saveList(const list<DbItem*>& items, const TCHAR* fileName) {
	tofstream file(fileName);
	if (!file) {
		//cerr << "failed to open file " << fileName << endl;
		return;
	}
	printList(file, items);
}

bool Registry::loadList(list<DbItem*>& items, const TCHAR* fileName) {
	tifstream file(fileName);
	if (!file) {
		return false;
	}
	while (true) {
		DbItem* item = factory[fileName]();
		if (item->deserialize(file)) {
			items.push_back(item);
		}
		else {
			delete item;
			break;
		}
	}
	return true;
}
