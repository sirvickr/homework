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
	// Строковые константы, содержащие имена файлов данных
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

	// Фабрика для создания объектов.
	// Ключу, т.е. имени файла, здесь соответствует
	// значение - функция создания конкретного объекта.
	// Для экононмии места и большей выразительности,
	// функции здесь заданы в виде лямбда-выражений, 
	// т.е. выражений вида 
	// [](параметры_функции) -> возвращаемое_значение { 
	//		тело_функции 
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

