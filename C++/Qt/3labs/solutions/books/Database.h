#pragma once

#include <QVector>
#include "Book.h"

// ���� ������ ����
class Database
{
public:
	// ����������� �� ���������
	Database();

	// ���-�� �������
	int count();

	// �������� ������ � ���� ������. ����� ���������� ���������� ������������� 
	// ��� ��������� ������ � �� ������������ � �������� record
	int append(Book &record);
	
	// ������� �� ���� ������ ������ � �������� ���������������
	void remove(unsigned int id);

	// �������� ������ � ���� ������. ������������ ����� ������� ������ 
	// � ������������ � �������� ����������
	int update(const Book &record);

	// ���������� ������ �� ��������� ��������������
	const Book *record(unsigned int id);

	// ���������� ������ � ������ �� ����������
	const QVector<BookItem> records();

	// ��������� ������ � �������� ����
	void save(QString filename);

	// ��������� ������ �� ��������� �����
	void load(QString filename);

	// ���������� ��� ������
	void clear();

private:
	// ������ ���� ������
	QVector <Book>	_records;

	unsigned int   globalId;	// ���������� �������������

	/** ����� ����� ������� ��� �����, ��� �� ��� �� �������� ������� ���������� ����
		\param [in] book - ����� ��� ������
		\return - ������ �������, ���� ������ ���� �������� ��������� �����, ����� ��� 
		�� �������� ������� ���������� ����
	*/
	int lowerBound(const Book &book);

	/** �������� ����� � ������ ���, ����� ��� �� �������� ������� ���������� ����
		\param [in] book - ����� ������� ��������� � ������
		\return - ������ ����������� �����
	*/
	int insertBook(const Book &book);
};