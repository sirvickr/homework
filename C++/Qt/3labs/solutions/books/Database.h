#pragma once

#include <QVector>
#include "Book.h"

// Ѕаза данных книг
class Database
{
public:
	//  онструктор по умолчанию
	Database();

	//  ол-во записей
	int count();

	// ƒобавить запись в базу данных. ћетод генерирует уникальный идентификатор 
	// дл€ указанной записи и он записываетс€ в параметр record
	int append(Book &record);
	
	// ”далить из базы данных запись с заданным идентификатором
	void remove(unsigned int id);

	// »зменить запись в базе данных. ¬озвращаетс€ нова€ позици€ записи 
	// в соответствии с пор€дком сортировки
	int update(const Book &record);

	// ¬озвращает запись по заданному идентификатору
	const Book *record(unsigned int id);

	// ¬озвратить записи с учетом их сортировки
	const QVector<BookItem> records();

	// —охранить данные в заданный файл
	void save(QString filename);

	// «агрузить данные из заданного файла
	void load(QString filename);

	// ”ничтожить все данные
	void clear();

private:
	// «аписи базы данных
	QVector <Book>	_records;

	unsigned int   globalId;	// глобальный идентификатор

	/** Ќайти такую позицию дл€ книги, что бы она не нарушала пор€док следовани€ книг
		\param [in] book - книга вне списка
		\return - индекс массива, куда должна быть помещена указанна€ книга, чтобы она 
		не нарушала пор€док следовани€ книг
	*/
	int lowerBound(const Book &book);

	/** ¬ставить книгу в список так, чтобы она не нарушала пор€док следовани€ книг
		\param [in] book - книга которую добавл€ем в список
		\return - индекс добавленной книги
	*/
	int insertBook(const Book &book);
};