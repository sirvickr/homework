#include <QFile>
#include "Database.h"

// Конструктор по умолчанию
Database::Database()
{
	// Инициализируем глобальный идентификатор
	globalId= 0;		
}

// Кол-во записей
int Database::count()
{
	return _records.count();
}

// Добавить запись в базу данных. Метод генерирует уникальный идентификатор 
// для указанной записи и он записывается в параметр record
int Database::append(Book &record)
{
	int pos;		// позиция новой записи

	// Присваиваем записи уникальный идентификатор 
	record.id = globalId;

	// Генерируем новый идентификатор на будущее
	globalId++;

	// Добавляем книгу в список так, чтобы она не нарушала порядок следования книг
	pos = insertBook(record);

	return pos;
}
	
// Удалить из базы данных запись с заданным идентификатором
void Database::remove(unsigned int id)
{
	// не реализована
}

// Изменить запись в базе данных. Возвращается новая позиция записи 
// в соответствии с порядком сортировки
int Database::update(const Book &record)
{
	// не реализована
	return 0;
}

// Возвращает запись по заданному идентификатору
const Book * Database::record(unsigned int id)
{
	QVector<Book>::const_iterator i;
	bool isFind;		// признак того, что запись найдена

	isFind	= false; i = _records.constBegin();
	while(!isFind && (i != _records.constEnd()))
	{
		if((*i).id == id)		isFind = true;
		else						++i;
	}

	// Если книги с заданным идентификатором нет, то возвращаем 
	// первую книгу
	if(!isFind)		i = _records.constBegin();

	return i;
}

// Возвратить записи с учетом их сортировки
const QVector<BookItem> Database::records()
{
	QVector<Book>::const_iterator i;		// итератор
	QVector<BookItem> items;				// записи с учетом сортировки

	for(i = _records.constBegin(); i != _records.constEnd(); ++i)
	{
		items.append(i->toItem());
	}

	return items;
}

// Сохранить данные в заданный файл
void Database::save(QString filename)
{
	// !!! Показан пример работы с текстовым потоком - в своей работе так делать не нужно !!!!
	
	QVector<Book>::const_iterator i;			// итератор
	QFile file(filename);						// создаем объектное представление  файла
	
	if(file.open(QIODevice::WriteOnly))		// открываем файл только для записи, т.е. перезаписываем
	{
		QTextStream out(&file);					// свзяываем поток с файлом


		// Каждая запись отделяется знаком табуляции
		for(i = _records.constBegin(); i != _records.constEnd(); ++i)
		{
			if(i != _records.constBegin())	out << "\t";
			
			out << *i;
		}

		// На всякий случай закрываем файл
		file.close();
	}
}

// Загрузить данные из заданного файла
void Database::load(QString filename)
{
	Book	book;				// считанная книга
	
	// !!! Показан пример работы с текстовым потоком - в своей работе так делать не нужно !!!!
	
	QFile file(filename);						// создаем объектное представление  файала
	
	_records.clear();								// удаляем текущие данные

	if(file.open(QIODevice::ReadOnly))		// открываем файл только для чтения
	{
		QTextStream in(&file);					// свзяываем поток с файлом

		// Считываем данные из файла (в том же порядке, что и записывали) 
		// пока не достигнем конца файла. Также не забываем 
		// найти максимальный использованный глобальный идентификатор
		globalId = 0;
		while(!in.atEnd())
		{
			in >> book;

			if(globalId < book.id)		globalId = book.id;	// определяем использованные идентификаторы

			_records << book;
		}

		globalId++;									// порождаем еще не использованный идентификатор

		// На всякий случай закрываем файл
		file.close();
	}
}

// Уничтожить все данные
void Database::clear()
{
	// не реализована
}

/** Найти такую позицию для книги, что бы она не нарушала порядок следования книг
	\param [in] book - книга вне списка
	\return - индекс массива, куда должна быть помещена указанная книга, чтобы она 
	не нарушала порядок следования книг
*/
int Database::lowerBound(const Book &book)
{
	int i;		// искомый индекс

	i = 0;
	while((i < _records.count()) && (_records[i] < book)) // используем операцию  "меньше" класса Book !!!
	{ i++; }

	return i;
}


/** Вставить книгу в список так, чтобы она не нарушала порядок следования книг
	\param [in] book - книга которую добавляем в список
	\return - индекс добавленной книги
*/
int Database::insertBook(const Book &book)
{
	int pos;	// позиция вставки книги

	// Ищем такую позицию для книги, что бы она не нарушала порядок следования книг
	pos = lowerBound(book);

	// Записываем книгу в заданную позицию
	_records.insert(pos, book);

	return pos;
}
