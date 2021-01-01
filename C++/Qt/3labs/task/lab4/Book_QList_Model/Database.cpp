#include <QFile>
#include "Database.h"

// ����������� �� ���������
Database::Database()
{
	// �������������� ���������� �������������
	globalId= 0;		
}

// ���-�� �������
int Database::count()
{
	return _records.count();
}

// �������� ������ � ���� ������. ����� ���������� ���������� ������������� 
// ��� ��������� ������ � �� ������������ � �������� record
int Database::append(Book &record)
{
	int pos;		// ������� ����� ������

	// ����������� ������ ���������� ������������� 
	record.id = globalId;

	// ���������� ����� ������������� �� �������
	globalId++;

	// ��������� ����� � ������ ���, ����� ��� �� �������� ������� ���������� ����
	pos = insertBook(record);

	return pos;
}
	
// ������� �� ���� ������ ������ � �������� ���������������
void Database::remove(unsigned int id)
{
	// �� �����������
}

// �������� ������ � ���� ������. ������������ ����� ������� ������ 
// � ������������ � �������� ����������
int Database::update(const Book &record)
{
	// �� �����������
	return 0;
}

// ���������� ������ �� ��������� ��������������
const Book * Database::record(unsigned int id)
{
	QVector<Book>::const_iterator i;
	bool isFind;		// ������� ����, ��� ������ �������

	isFind	= false; i = _records.constBegin();
	while(!isFind && (i != _records.constEnd()))
	{
		if((*i).id == id)		isFind = true;
		else						++i;
	}

	// ���� ����� � �������� ��������������� ���, �� ���������� 
	// ������ �����
	if(!isFind)		i = _records.constBegin();

	return i;
}

// ���������� ������ � ������ �� ����������
const QVector<BookItem> Database::records()
{
	QVector<Book>::const_iterator i;		// ��������
	QVector<BookItem> items;				// ������ � ������ ����������

	for(i = _records.constBegin(); i != _records.constEnd(); ++i)
	{
		items.append(i->toItem());
	}

	return items;
}

// ��������� ������ � �������� ����
void Database::save(QString filename)
{
	// !!! ������� ������ ������ � ��������� ������� - � ����� ������ ��� ������ �� ����� !!!!
	
	QVector<Book>::const_iterator i;			// ��������
	QFile file(filename);						// ������� ��������� �������������  �����
	
	if(file.open(QIODevice::WriteOnly))		// ��������� ���� ������ ��� ������, �.�. ��������������
	{
		QTextStream out(&file);					// ��������� ����� � ������


		// ������ ������ ���������� ������ ���������
		for(i = _records.constBegin(); i != _records.constEnd(); ++i)
		{
			if(i != _records.constBegin())	out << "\t";
			
			out << *i;
		}

		// �� ������ ������ ��������� ����
		file.close();
	}
}

// ��������� ������ �� ��������� �����
void Database::load(QString filename)
{
	Book	book;				// ��������� �����
	
	// !!! ������� ������ ������ � ��������� ������� - � ����� ������ ��� ������ �� ����� !!!!
	
	QFile file(filename);						// ������� ��������� �������������  ������
	
	_records.clear();								// ������� ������� ������

	if(file.open(QIODevice::ReadOnly))		// ��������� ���� ������ ��� ������
	{
		QTextStream in(&file);					// ��������� ����� � ������

		// ��������� ������ �� ����� (� ��� �� �������, ��� � ����������) 
		// ���� �� ��������� ����� �����. ����� �� �������� 
		// ����� ������������ �������������� ���������� �������������
		globalId = 0;
		while(!in.atEnd())
		{
			in >> book;

			if(globalId < book.id)		globalId = book.id;	// ���������� �������������� ��������������

			_records << book;
		}

		globalId++;									// ��������� ��� �� �������������� �������������

		// �� ������ ������ ��������� ����
		file.close();
	}
}

// ���������� ��� ������
void Database::clear()
{
	// �� �����������
}

/** ����� ����� ������� ��� �����, ��� �� ��� �� �������� ������� ���������� ����
	\param [in] book - ����� ��� ������
	\return - ������ �������, ���� ������ ���� �������� ��������� �����, ����� ��� 
	�� �������� ������� ���������� ����
*/
int Database::lowerBound(const Book &book)
{
	int i;		// ������� ������

	i = 0;
	while((i < _records.count()) && (_records[i] < book)) // ���������� ��������  "������" ������ Book !!!
	{ i++; }

	return i;
}


/** �������� ����� � ������ ���, ����� ��� �� �������� ������� ���������� ����
	\param [in] book - ����� ������� ��������� � ������
	\return - ������ ����������� �����
*/
int Database::insertBook(const Book &book)
{
	int pos;	// ������� ������� �����

	// ���� ����� ������� ��� �����, ��� �� ��� �� �������� ������� ���������� ����
	pos = lowerBound(book);

	// ���������� ����� � �������� �������
	_records.insert(pos, book);

	return pos;
}
