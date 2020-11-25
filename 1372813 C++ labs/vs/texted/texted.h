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
	// объект для загрузки пользовательского графического
	// интерфейса из xml-файла mainwindow.ui
	Ui::textedClass ui;
	// модели данных для навигации по файловой системе
	QFileSystemModel* dirsModel;
	QFileSystemModel* filesModel;
	// объект файла для чтения содержимого текстовых файлов
	QFile file;
};
