#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "invention.h"
#include "ui_mainwindow.h"

#include <QMainWindow>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);

private slots:
	// Отобразить поля открытия/изобретения
	void showItem(int index);
	// Отобразить выбранного автора
	void showAuthor(int index);
	// Реакция на выбор области применения из перечня
	void on_cbxRealm_currentIndexChanged(int index);
	// Реакция изменение имени
	void on_txtName_editingFinished();
	// Реакция на изменение флага наличия патента
	void on_chkPatent_toggled(bool checked);
	// Реакция на изменение флага наличия нобелевской премии
	void on_chkAward_toggled(bool checked);
	// Реакция на изменение автора
	void on_txtAuthor_editingFinished();
	// Реакция на изменение даты регистрации патента
	void on_datPatentReg_userDateChanged(const QDate &date);

	void on_btnDelAuthor_clicked();

private:
	Ui::MainWindow ui;
	// выбранное изобретение
	int currentItem = -1;
	// выбранный автор
	int currentAuthor = -1;
	// база данных открытий/изобретений
	enum { MaxItemCount = 2 };
	Invention inventions[MaxItemCount];

private:
	inline bool checkIndex(int index) const {
		return index >= 0 && index < MaxItemCount;
	}
	void setPatentWidgets(bool patent);
	void setRealmWidgets(Invention::Realm);
	void putAuthors(const QString& authors);
	void delAuthor();
	QString getAuthors();
};

#endif // MAINWINDOW_H
