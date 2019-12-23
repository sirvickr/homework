#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <functional>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	// корабль
	/*struct Ship {
		enum Orient { Horz, Vert } orient;
		enum { minSize = 1, maxSize = 4 };
		int size = 0; // количество занимаемых клеток (от minSize до maxSize)
		int alive = true; // корабль жив
		QPoint cells[maxSize];
	};*/

	// клетка
	struct Cell {
		//enum Status { Hidden, Miss, Injured, Killed };
		//bool visible = false; // true: открыта
		//bool occupied = false; // true: либо под кораблём, либо по соседству (тогда ship = null)
		//Ship* ship = nullptr; // указатель на объект корабля, под которым находится клетка
		QRect rect; // координаты прямоугольника клетки на форме
		MainWindow* game = nullptr;
		int value = 0;
		bool isEmpty() const {
			return 0 == value;
		}
		bool isSameTo(const Cell& cell) const {
			return value == cell.value;
		}
		void clear() {
			value = 0;
		}
		void merge(Cell& cell) {
			if(value)
				game->totalScore += (value + cell.value);
			value += cell.value;
			cell.clear();
			//hasMoved = true;
		}
	};

	// количество кораблей
	//static const int maxShips = 10;
	// количество клеток одной стороны квадрата
	static const int maxCells = 4;
	// отступ игрового поля от края окна
	static const int padding = 50;

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	// обработчик события: нажатие кнопки мыши
	void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
	// обработчик события: нажатие кнопки мыши
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	// обработчик события: перерисовка главного окна
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private slots:
	// слот: меню "Новая игра"
	void on_actRestart_triggered();

private:
	// начать игру заново
	void resetGame();
	// проверить статус открытой клетки
	//Cell::Status getStatus(const Cell& cell);
	// все ли корабли уничтожены
	bool gameOver();
	bool moveHorz(int inc, int prev, std::function<bool(int)> stop);
	bool moveVert(int inc, int prev, std::function<bool(int)> stop);

private:
	Ui::MainWindow *ui;
	// рисоватор
	QPainter painter;
	// игровое поле
	Cell field[maxCells][maxCells];
	// корабли
	//Ship ships[maxShips];
	// количество ходов
	int stepCount = 0;
	// количество очков
	int totalScore = 0;
};

#endif // MAINWINDOW_H
