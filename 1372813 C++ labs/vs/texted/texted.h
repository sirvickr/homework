#pragma once

#include <QtWidgets/QMainWindow>
#include <QFileSystemModel>
#include <QFile>
#include "ui_texted.h"

class texted : public QMainWindow
{
    Q_OBJECT

public:
    texted(QWidget *parent = Q_NULLPTR);

private slots:
	void on_dirsView_clicked(const QModelIndex &index);
	void on_filesView_clicked(const QModelIndex &index);

private:
	// ������ ��� �������� ����������������� ������������
	// ���������� �� xml-����� mainwindow.ui
	Ui::textedClass ui;
	// ������ ������ ��� ��������� �� �������� �������
	QFileSystemModel* dirsModel;
	QFileSystemModel* filesModel;
	// ������ ����� ��� ������ ����������� ��������� ������
	QFile file;
};
