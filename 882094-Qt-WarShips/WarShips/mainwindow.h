#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	// корабль
	struct Ship {
		enum Orient { Horz, Vert } orient;
		enum { minSize = 1, maxSize = 4 };
		//enum { minCount = 1, maxCount = 4 };
		int size = 0; // minSize ... maxSize
		int alive = true; // корабль жив
		QPoint cells[maxSize];
	};

	// клетка
	struct Cell {
		bool visible = false; // true: открыта
		bool occupied = false; // true: либо под кораблём, либо по соседству (тогда ship = null)
		Ship* ship = nullptr;
		QRect rect;
	//	QPen pen;
	};

	static const int maxShips = 10;
	static const int maxCells = 10;
	static const int padding = 50;

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	// обработчики событий
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private slots:
	void on_actRestart_triggered();

private:
	// начать игру заново
	void resetShips();
	// все ли корабли уничтожены
	bool gameOver();
	// открыть всё поле (в конце игры)
	void openField();

private:
	Ui::MainWindow *ui;
	// рисоватор
	QPainter painter;
	// игровое поле
	Cell field[maxCells][maxCells];
	// корабли
	Ship ships[maxShips];
	// количество ходов
	int stepCount = 0;
};

#endif // MAINWINDOW_H
