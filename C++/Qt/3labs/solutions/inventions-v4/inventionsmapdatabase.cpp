#include "inventionsmapdatabase.h"

#include <QFile>

int InventionsMapDatabase::count() const
{
	return inventions.size();
}

int InventionsMapDatabase::append(const Invention& record)
{
	Id id = nextID++;
	inventions[id] = record;
	mModified = true;
	return id;
}

void InventionsMapDatabase::remove(Id id)
{
	inventions.remove(id);
	mModified = true;
}

void InventionsMapDatabase::update(Id id, const Invention& record)
{
	auto item = inventions.find(id);
	if(item == inventions.end())
		return;
	if(item.value() != record) {
		item.value() = record;
		mModified = true;
	}
}

bool InventionsMapDatabase::record(Id id, Invention &record) const
{
	auto item = inventions.find(id);
	if(item == inventions.end())
		return false;
	record = item.value();
	return true;
}

void InventionsMapDatabase::clear() {
	inventions.clear();
	nextID = 0;
	mModified = true;
}

bool InventionsMapDatabase::isModified() const
{
	return mModified;
}

InventionsMapDatabase::Inventions::const_iterator InventionsMapDatabase::cbegin() const
{
	return inventions.cbegin();
}

InventionsMapDatabase::Inventions::const_iterator InventionsMapDatabase::cend() const
{
	return inventions.cend();
}

bool InventionsMapDatabase::load(const QString& filename)
{
	// создаем объектное представление  файала
	QFile file(filename);
	// открываем файл только для чтения
	if(!file.open(QIODevice::ReadOnly)) {
		return false;
	}
	// удаляем текущие данные
	clear();
	// свзяываем поток данных с открытым файлом
	QDataStream stream(&file);
	// Считываем данные из файла (в том же порядке, что и записывали), пока не достигнем конца файла
	Invention record;
	while(!stream.atEnd()) {
		stream >> record;
		append(record);
	}
	// Закрываем файл
	file.close();
	// Возвращаем признак успешного завершения
	return true;
}

bool InventionsMapDatabase::save(const QString& filename) const
{
	// создаем объектное представление  файала
	QFile file(filename);
	// открываем файл только для чтения
	if(!file.open(QIODevice::WriteOnly)) {
		return false;
	}
	// свзяываем поток данных с открытым файлом
	QDataStream stream(&file);
	// Записываем данные в файл
	for(auto entry = inventions.cbegin(); entry != inventions.cend(); entry++) {
		stream << entry.value();
	}
	// Закрываем файл
	file.close();
	// Сбрасываем признак модификации, поскольку только что синхронизировались с хранилищем
	mModified = false;
	// Возвращаем признак успешного завершения
	return true;
}
