#ifndef INVENTIONSMAPDATABASE_H
#define INVENTIONSMAPDATABASE_H

#include "invention.h"

#include <QMap>

class InventionsMapDatabase
{
public:
	using Id = int;
	using Inventions = QMap<Id, Invention>;

	// получить кол-во записей
	int count() const;
	// добавить запись в базу данных, возвращает уникальный идентификатор для указанной записи
	int append(const Invention& record);
	// удалить из базы данных запись с указанным идентификатором
	void remove(Id id);
	// изменить в базе данных запись с указанным идентификатором
	void update(Id id, const Invention& record);
	// получить запись (только для чтения) по заданному идентификатору; если такой записи нет, возвращает false
	bool record(Id id, Invention &record) const;
	// уничтожает все данные
	void clear();
	// показывает, имеются ли изменения БД после ее загрузки/сохранения
	bool isModified() const;
	// эти методы позволят итерироваться по коллекции извне, без необходимости получения к ней непосредственного доступа
	// или тем более полного копирования элементов (например, в методе const QVector<Invention> records() const)
	Inventions::const_iterator cbegin() const;
	Inventions::const_iterator cend() const;

	// загрузить данные из заданного файла, false, если загрузить данные не удалось
	bool load(const QString& filename);
	// сохранить данные в заданный файл, возвращает false, если сохранить данные не удалось
	bool save(const QString& filename) const;

private:
	// база данных открытий/изобретений
	Inventions inventions;
	// следующий уникальный идентификатор для добавления записей
	Id nextID = 0;
	// признак изменения базы данных (mutable, чтобы можно было изменять в константных методах)
	mutable bool mModified = false;
};

#endif // INVENTIONSMAPDATABASE_H
