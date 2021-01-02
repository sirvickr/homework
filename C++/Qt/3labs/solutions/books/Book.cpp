#include "Book.h"

/** �������� "������". ��������� ����������� �� ���� ����� "��������", "�����", "���"
\param [in] other - �����, � ������� ����������� ���������
\return - ��������� ���������
*/
bool Book::operator<(const Book &other) const
{
	if (name != other.name)				return name < other.name;
	else if (author != other.author) return author < other.author;
	else										return year > other.year;
}

// ������������� �����-������ � �����-������
struct BookItem Book::toItem() const
{
	struct BookItem bookItem;	// �����-������

	bookItem.name = name;			
	bookItem.author = author;		
	bookItem.id = id;	

	return bookItem;
}

// �������� � �����
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

// ������� �� ������
QTextStream & operator>>(QTextStream &in, Book &book)
{
	quint16	year;		// ��������� ��� �������
	quint16	genre;	// ��������� ����
	quint32	id;		// ��������� �������������


	in >> book.name;
	in >> book.author;
	in >> genre;		book.genre = genre;
	in >> year;			book.year = year;
	in >> id;			book.id = id;

	return in;
}