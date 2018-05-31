#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMouseEvent>
#include <QColorDialog>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	int width = this->width() - padding * 2;
	int height = width;
	int cellSize = width / maxCells;
	qDebug() << width << "x" << height;

	for(int i = 0; i < maxCells; ++i) {
		int y = padding + i * cellSize;
		for(int j = 0; j < maxCells; ++j) {
			int x = padding + j * cellSize;
			field[i][j].rect.setRect(x, y, cellSize, cellSize);
		}
	}

	resetShips();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
	if(event->button() == Qt::LeftButton) {
		for(int i = 0; i < maxCells; ++i) {
			for(int j = 0; j < maxCells; ++j) {

				if(field[i][j].rect.contains(event->pos(), true)) {
					field[i][j].visible = !field[i][j].visible;
				}

			}
		}
		this->repaint();
	}
}

void MainWindow::paintEvent(QPaintEvent *event)
{
	painter.begin(this);

	int width = this->width() - padding * 2;
	int height = width;

	painter.setPen(QPen(Qt::darkBlue, 1));
#if 1
	for(int i = 0; i < maxCells; ++i) {
		for(int j = 0; j < maxCells; ++j) {
			Qt::BrushStyle brushStyle = Qt::SolidPattern;
			Qt::GlobalColor clr = Qt::yellow;
			if(field[i][j].occupied && !field[i][j].ship) {
				brushStyle = Qt::Dense3Pattern;
			}
			if(field[i][j].ship) {
				clr = Qt::red;
			} else {
				clr = Qt::green;
			}
			painter.setBrush(QBrush(clr, brushStyle));
			painter.drawRect(field[i][j].rect);
		}
	}
#else // то же, но должно быть чуть быстрее
	int cellSize = width / maxCells;
	for(int i = 1, j = padding + cellSize; i < maxCells; ++i, j+= cellSize) {
		painter.drawLine(QPoint(j, padding), QPoint(j, padding + height));
		painter.drawLine(QPoint(padding, j), QPoint(padding + width, j));
	}
#endif
#if 1
	painter.setPen(QPen(Qt::darkGreen, 1));
	painter.drawLine(QPoint(padding, padding), QPoint(padding, padding + height));
	painter.drawLine(QPoint(padding, padding), QPoint(padding + width, padding));
	painter.drawLine(QPoint(padding, padding + height), QPoint(padding + width, padding + height));
	painter.drawLine(QPoint(padding + width, padding), QPoint(padding + width, padding + height));
#endif

	painter.end();
}

void MainWindow::resetShips()
{
	// можно QDateTime::currentMSecsSinceEpoch()
	qsrand(QTime::currentTime().msec());

	for(int i = 0; i < maxCells; ++i) {
		for(int j = 0; j < maxCells; ++j) {
			field[i][j].visible = false;
			field[i][j].occupied = false;
			field[i][j].ship = nullptr;
		}
	}

	int index = 0;
	for(int size = Ship::maxSize, count = Ship::minSize; size >= Ship::minSize; --size, ++count) {
		qDebug() << "size " << size << " count " << count;
		for(int n = Ship::minSize; n <= count; ++n) {
			Ship& ship = ships[index++];
			ship.size = size;
			ship.orient = static_cast<Ship::Orient>(qrand() & 1);
			int rowLim = maxCells, colLim = maxCells;
			switch(ship.orient) {
			case Ship::Horz:
				colLim -= (ship.size - 1);
				break;
			case Ship::Vert:
				rowLim -= (ship.size - 1);
				break;
			}
			int attemptCount = 0;
			int row, col;
			bool placed;
			do { // попытка разметить корабль

				placed = true;
				col = qrand() % colLim;
				row = qrand() % rowLim;
				int x, y;
				switch(ship.orient) {
				case Ship::Horz:
					for(int i = 0; i < ship.size; ++i) {
						if(field[row][col + i].occupied) {
							placed = false;
							break;
						}
						ship.cells[i].setX(col + i);
						ship.cells[i].setY(row);
					}
					break;
				case Ship::Vert:
					for(int i = 0; i < ship.size; ++i) {
						if(field[row + i][col].occupied) {
							placed = false;
							break;
						}
						ship.cells[i].setX(col);
						ship.cells[i].setY(row + i);
					}
					break;
				}
				attemptCount++;
				if(attemptCount == 1000) {
					qDebug() << "!!!!!!!!!!!! EMERFTNCY EXIT !!!!!!!!!!!!";
					break;
				}
			} while(!placed);
			// занимаем клетки под кораблём и вокруг него
			for(int i = 0; i < ship.size; ++i) {
				Cell& cell = field[ship.cells[i].y()][ship.cells[i].x()];
				cell.ship = &ship;
			}
			int colMin = qMax(ship.cells[0].x() - 1, 0);
			int rowMin = qMax(ship.cells[0].y() - 1, 0);
			int colMax = qMin(ship.cells[ship.size - 1].x() + 2, maxCells);
			int rowMax = qMin(ship.cells[ship.size - 1].y() + 2, maxCells);
			//
			for(int i = rowMin; i < rowMax; ++i) {
				for(int j = colMin; j < colMax; ++j) {
					field[i][j].occupied = true;
				}
			}
			char buffer[100];
			QString line = ": ";
			for(int i = 0; i < ship.size; ++i) {
				line += "[";
				line += QString(itoa(ship.cells[i].y(), buffer, 10));
				line += ", ";
				line += QString(itoa(ship.cells[i].x(), buffer, 10));
				line += "]";
			}

			qDebug() << "\tship: " << ship.size << " orient " << ship.orient << "[" << row << "][" << col << "] attemps " << attemptCount << " min " << rowMin << ", " << colMin << " max " << rowMax << ", " << colMax << line;

		}
	}
}
