#ifndef DATABASEWINDOW_H
#define DATABASEWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_DatabaseWindow.h"

#include "Database.h"


// ��������� �������������, ��� ����������� ���������� �������. ��� ����
// �������������� ����������� ������ � ���������� �������� ���������� ������ 
// ��� ������� ����������: ������ ���������� ������ � ����� � �������������� ����� 
// ������� ������� ������������ ������� ����� ������ � �� �������, ������� �� �������� 
// ��� ������������� ������� �������


// ����� �������� ����
class DatabaseWindow : public QMainWindow
{
	Q_OBJECT

public:
	DatabaseWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~DatabaseWindow();

private:
	Ui::DatabaseWindowClass ui;

    Database Db;		// ���� ������ ����
    int current;			// ������ ������� �����

   // ������ ��������� ��������� ��������
   void setStartStatus();

	// ������� ����� �� ��������� ������� �������� ����� �����
	Book createCurrentBook();

private slots:
	// �������� ����� �����
	void addBook();
	// ���������� ����� � �������� ������
	void showBook(int currentRow);
};

#endif // DATABASEWINDOW_H
