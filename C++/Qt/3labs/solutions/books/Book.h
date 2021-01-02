#pragma once

#include <QString> 
#include <QList> 
#include <QTextStream> 

// �����, ��� ������ ��������
struct BookItem
{
	QString			name;			// ��������
	QString			author;		// �����

	unsigned int id;				// �������������
};

// �����, ����������� �����
class Book
{
public:
	// ���� (���� ������������)
	QString name;		// �������� �����
   QString author;	// ����� �����
   int genre;			// ���� �����
   int year;			// ��� �������

	unsigned int id;	// ������������� 

	/** �������� "������". ��������� ����������� �� ���� ����� "��������", "�����", "���"
		\param [in] other - �����, � ������� ����������� ���������
		\return - ��������� ���������
	*/
	bool operator<(const Book &other) const;	

	// ������������� �����-������ � �����-������
	struct BookItem toItem() const;
};

// �������� � �����
QTextStream & operator<<(QTextStream &out, const Book &book);

// ������� �� ������
QTextStream & operator>>(QTextStream &in, Book &book);