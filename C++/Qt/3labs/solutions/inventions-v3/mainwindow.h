#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "invention.h"
#include "ui_mainwindow.h"

#include <QMainWindow>

// Биты для маски активности (enabled) элементов управления
enum EnabledBits {
	bitCreateInvention = 0, // Добавить
	bitReadInvention   = 1, // -
	bitUpdateInvention = 2, // Сохранить
	bitDeleteInvention = 3, // Удалить
	bitCreateAuthor    = 4,
	bitReadAuthor      = 5,
	bitUpdateAuthor    = 6,
	bitDeleteAuthor    = 7,

	bitMax
};

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
	// Реакция на нажатие кнопки "Добавить автора"
	void on_btnAddAuthor_clicked();
	// Реакция на нажатие кнопки "Сохранить автора"
	void on_btnSaveAuthor_clicked();
	// Реакция на нажатие кнопки "Удалить автора"
	void on_btnDelAuthor_clicked();

private:
	using Inventions = Invention*;

	Ui::MainWindow ui;
	// выбранное изобретение (индекс элемента списка)
	int currentItem = -1;
	// выбранный автор
	int currentAuthor = -1;
	// база данных открытий/изобретений
	Inventions inventions = nullptr;
	int inventionCount = 0;
	int inventionCapacity = 0;
	// Текущий буфер изменений параметров изобретения
	Invention buffer;

private:
	// Убедиться, что памяти достаточно для размещения n новых элементов
	bool ensureCapacity(int n);
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
	void verifyView();
	// Проверить текущее значение области применения
	int verifyRealmValue(int mask);
	// Проверить текущее название
	int verifyNameValue(int mask);
	// Проверить текущее значение года
	int verifyYearValue(int mask);
	// Проверить наличие не сохранённых изменений
	int verifyBuffer(int mask);
	// Установить свойства элементов управления в зависимости от наличия патента
	void setPatentWidgets(bool patent);
	// Отключить флаг нобелевской премии для математики
	void setRealmWidgets(Invention::Realm);
	// Выделить авторов из строки и поместить их в список
	void putAuthors(const QString& authors);
	// Скомпоновать строку из списка авторов
	QString getAuthors();
	// Выделить в списке элемент, соответствующий текущему индексу
	void selectCurrent(QListWidget* lst, int& index);
	// Сбросить один бит в маске
	inline int resetBit(int mask, EnabledBits bit) {
		return mask & ~(1 << bit);
	}

	// Наследуем QListWidgetItem, чтобы перегрузить оператор сравнения для своей сортировки
	class InventionItem : public QListWidgetItem
	{
	public:
		InventionItem(const Invention::Rank& rank)
			: QListWidgetItem(), rank(rank)
		{
		}
		explicit InventionItem(const Invention::Rank& rank, const QString &text, QListWidget *listview = nullptr)
			: QListWidgetItem(text, listview), rank(rank)
		{
		}
		bool operator<(const QListWidgetItem &other) const override
		{
			return this->rank < static_cast<const InventionItem&>(other).rank;
		}
	private:
		Invention::Rank rank;
	};
};

#endif // MAINWINDOW_H
