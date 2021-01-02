#include "Book.h"

/** ќпераци€ "меньше". —равнение выполн€етс€ по трем пол€м "название", "автор", "год"
\param [in] other - книга, с которой выполн€етс€ сравнение
\return - результат сравнени€
*/
bool Book::operator<(const Book &other) const
{
	if (name != other.name)				return name < other.name;
	else if (author != other.author) return author < other.author;
	else										return year > other.year;
}

// ѕреобразовать книгу-запись в книгу-строку
struct BookItem Book::toItem() const
{
	struct BookItem bookItem;	// книга-строка

	bookItem.name = name;			
	bookItem.author = author;		
	bookItem.id = id;	

	return bookItem;
}

// «аписать в поток
QTextStream & operator<<(QTextStream &out, const Book &book)
{
	QString str;
	str = book.name; str.replace(' ', "_");
	out << str << "\t";
	str = book.author; str.replace(' ', "_");
	out << str << "\t";
	out << (quint16)book.genre  << "\t";
	out << (quint16)book.year  << "\t";
	out << (quint32)book.id;

	return out;
}

// —читать из потока
QTextStream & operator>>(QTextStream &in, Book &book)
{
	quint16	year;		// считанный год издани€
	quint16	genre;	// считанный жанр
	quint32	id;		// считанный идентификатор


	in >> book.name;
	in >> book.author;
	in >> genre;		book.genre = genre;
	in >> year;			book.year = year;
	in >> id;			book.id = id;

	return in;
}