#ifndef DATABASEWINDOW_H
#define DATABASEWINDOW_H

#include <QApplication>

#include "ui_DatabaseWindow.h"

#include "Database.h"


// ѕрограмма демонстрирует, как реализовать сортировку записей. ѕри этом
// расматриваетс€ эффективный подход к выполнению операции добавлени€ записи 
// при наличии сортировки: вместо добавлени€ записи в конец и упор€дочивани€ всего 
// массива записей используетс€ вставка новой записи в ту позицию, котора€ не нарушает 
// уже существующего пор€дка записей


//  ласс главного окна
class DatabaseWindow : public QMainWindow
{
	Q_OBJECT

public:
	DatabaseWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~DatabaseWindow();

private:
	Ui::DatabaseWindowClass ui;

    Database Db;		// база данных книг
    int current;			// индекс текущей книги

   // «адать стартовое состо€ние виджетов
   void setStartStatus();

	// —оздаем книгу на основании текущих значений полей ввода
	Book createCurrentBook();

private slots:
	// ƒобавить новую книгу
	void addBook();
	// ќтобразить книгу в заданной строке
	void showBook(int currentRow);
};

#endif // DATABASEWINDOW_H
