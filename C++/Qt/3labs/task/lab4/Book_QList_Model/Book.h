#pragma once

#include <QString> 
#include <QList> 
#include <QTextStream> 

// Книга, как строка браузера
struct BookItem
{
	QString			name;			// название
	QString			author;		// автор

	unsigned int id;				// идентификатор
};

// Класс, описывающий книгу
class Book
{
public:
	// Поля (пока общедоступны)
	QString name;		// название книги
   QString author;	// автор книги
   int genre;			// жанр книги
   int year;			// год издания

	unsigned int id;	// идентификатор 

	/** Операция "меньше". Сравнение выполняется по трем полям "название", "автор", "год"
		\param [in] other - книга, с которой выполняется сравнение
		\return - результат сравнения
	*/
	bool operator<(const Book &other) const;	

	// Преобразовать книгу-запись в книгу-строку
	struct BookItem toItem() const;
};

// Записать в поток
QTextStream & operator<<(QTextStream &out, const Book &book);

// Считать из потока
QTextStream & operator>>(QTextStream &in, Book &book);