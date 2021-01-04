#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "invention.h"
#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QMap>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	// Отобразить поля открытия/изобретения
	void showItem(int index);
	// Отобразить выбранного автора
	void showAuthor(int index);
	// Реакция на нажатие кнопки "Заполнить"
	void on_btnFill_clicked();
	// Реакция на нажатие кнопки "Добавить"
	void on_btnAdd_clicked();
	// Реакция на нажатие кнопки "Сохранить"
	void on_btnSave_clicked();
	// Реакция на нажатие кнопки "Удалить"
	void on_btnDelete_clicked();
	// Реакция на выбор области применения из перечня
	void on_cbxRealm_currentIndexChanged(int index);
	// Реакция изменение имени
	void on_txtName_editingFinished();
	// Реакция на изменение года
	void on_txtYear_editingFinished();
	// Реакция на изменение флага наличия нобелевской премии
	void on_chkAward_toggled(bool checked);
	// Реакция на изменение флага наличия патента
	void on_chkPatent_toggled(bool checked);
	// Реакция на изменение даты регистрации патента
	void on_datPatentReg_userDateChanged(const QDate &date);
	// Реакция на изменение автора
//	void on_txtAuthor_editingFinished();
	// Реакция на нажатие кнопки "Добавить автора"
	void on_btnAddAuthor_clicked();
	// Реакция на нажатие кнопки "Сохранить автора"
	void on_btnSaveAuthor_clicked();
	// Реакция на нажатие кнопки "Удалить автора"
	void on_btnDelAuthor_clicked();

private:
	Ui::MainWindow ui;
	// выбранное изобретение (индекс элемента списка)
	int currentItem = -1;
	// выбранный автор
	int currentAuthor = -1;
	// база данных открытий/изобретений
	QMap<int, Invention> inventions;
	// Текущий буфер изменений параметров изобретения
	Invention buffer;
	// следующий уникальный идентификатор для добавления записей
	int nextID = 0;

private:
	// Отобразить записи базы данных в браузере: isKey = true означает,
	// что current - это ключ объекта, иначе индекс списка
	void showItems(int current = 0, bool isKey = false);
	// Отобразить поля текущей записи в элементах управления
	Invention viewToObject();
	// Прочитать поля текущей записи с элементов управления
	void objectToView(const Invention& invention);
	// Найти индекс нужной записи в браузере
	int keyToIndex(int key);
	// Проверить текущие значения элементов управления
	bool verifyView();
	bool verifyRealmValue();
	bool verifyNameValue();
	bool verifyYearValue();
	bool verifyAuthor() {
		if(currentAuthor < 0) {
			ui.txtAuthor->setEnabled(false);
			ui.txtAuthor->clear();
		} else {
			ui.txtAuthor->setText(ui.lstAutors->item(currentAuthor)->text());
			ui.txtAuthor->setEnabled(true);
		}
	}
	void setPatentWidgets(bool patent);
	void setRealmWidgets(Invention::Realm);
	void putAuthors(const QString& authors);
	QString getAuthors();
	// Выделить в списке элемент, соответствующий текущему индексу
	int selectCurrent(QListWidget* lst, int& index);
	void selectCurrentInvention(int& index);
};

#endif // MAINWINDOW_H
