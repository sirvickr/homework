#include "mainwindow.h"
#include "ui_mainwindow.h"

//#include <QDebug>
#include <QMouseEvent>
#include <QColorDialog>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->statusBar->addWidget(ui->lblStepCount);
	ui->statusBar->addWidget(ui->lblStepResult);

	int width = this->width() - padding * 2;
	int cellSize = width / maxCells;
	//qDebug() << width << "x" << height;

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

				if(field[i][j].rect.contains(event->pos(), true) && !field[i][j].visible) {
					// ткнули в закрытую ячейку
					stepCount++;
					ui->lblStepCount->setText(QString::number(stepCount));
					// открываем её ..
					field[i][j].visible = true;
					// и проверяем наличие корабля
					Cell::Status status = getStatus(field[i][j]);
					switch(status) {
					case Cell::Miss:
						ui->lblStepResult->setText("Промах");
						break;
					case Cell::Injured:
						ui->lblStepResult->setText("Ранил");
						break;
					case Cell::Killed:
						field[i][j].ship->alive = false;
						if(gameOver()) {
							ui->lblStepResult->setText("Победа!");
							openField();
						} else {
							ui->lblStepResult->setText("Убил");
						}
						break;
					}

				} // if(открыли клетку)

			} // for(j)
		} // for(i)
		this->repaint();
	}
}

void MainWindow::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)
	painter.begin(this);

	// рисуем текущее игровое поле
	int width = this->width() - padding * 2;
	int height = width;
	// надо проверить каждую клетку и нарисовать её,
	// причем раскрасить в зависимости от её текущего статуса
	// (закрытая или открытая, есть ли под ней корабль, ранен он или убит)
	painter.setPen(QPen(Qt::darkBlue, 1));
	for(int i = 0; i < maxCells; ++i) {
		for(int j = 0; j < maxCells; ++j) {
			Qt::GlobalColor clr = Qt::darkCyan;
			Qt::BrushStyle brushStyle = Qt::SolidPattern;
			Cell::Status status = getStatus(field[i][j]);
			switch(status) {
			case Cell::Hidden:
				brushStyle = Qt::Dense1Pattern;
				break;
			case Cell::Miss:
				clr = Qt::darkCyan;
				break;
			case Cell::Injured:
				clr = Qt::red;
				break;
			case Cell::Killed:
				clr = Qt::darkRed;
				break;
			}
			painter.setBrush(QBrush(clr, brushStyle));
			painter.drawRect(field[i][j].rect);
		}
	}
#if 1 // границы игрового поля
	painter.setPen(QPen(Qt::darkBlue, 2));
	painter.drawLine(QPoint(padding, padding), QPoint(padding, padding + height));
	painter.drawLine(QPoint(padding, padding), QPoint(padding + width, padding));
	painter.drawLine(QPoint(padding, padding + height), QPoint(padding + width, padding + height));
	painter.drawLine(QPoint(padding + width, padding), QPoint(padding + width, padding + height));
#endif

	painter.end();
}

void MainWindow::on_actRestart_triggered()
{
	resetShips();
}

void MainWindow::resetShips()
{
	// можно QDateTime::currentMSecsSinceEpoch()
	qsrand(QTime::currentTime().msec());

	stepCount = 0;
	ui->lblStepCount->setText(QString::number(stepCount));
	// закрываем поле
	for(int i = 0; i < maxCells; ++i) {
		for(int j = 0; j < maxCells; ++j) {
			field[i][j].visible = false;
			field[i][j].occupied = false;
			field[i][j].ship = nullptr;
		}
	}
	// размещаем корабли случайным образом
	int index = 0;
	for(int size = Ship::maxSize, count = Ship::minSize; size >= Ship::minSize; --size, ++count) {
		//qDebug() << "size " << size << " count " << count;
		for(int n = Ship::minSize; n <= count; ++n) {
			Ship& ship = ships[index++];
			ship.orient = static_cast<Ship::Orient>(qrand() & 1);
			ship.size = size;
			ship.alive = true;
			int rowLim = maxCells, colLim = maxCells;
			switch(ship.orient) {
			case Ship::Horz:
				colLim -= (ship.size - 1);
				break;
			case Ship::Vert:
				rowLim -= (ship.size - 1);
				break;
			}
			int row, col;
			bool placed;
			do { // попытка разместить корабль

				placed = true;
				col = qrand() % colLim;
				row = qrand() % rowLim;
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
			} while(!placed);
			// занимаем клетки под кораблём и вокруг него
			for(int i = 0; i < ship.size; ++i) {
				field[ship.cells[i].y()][ship.cells[i].x()].ship = &ship;
			}
			int colMin = qMax(ship.cells[0].x() - 1, 0);
			int rowMin = qMax(ship.cells[0].y() - 1, 0);
			int colMax = qMin(ship.cells[ship.size - 1].x() + 2, maxCells);
			int rowMax = qMin(ship.cells[ship.size - 1].y() + 2, maxCells);
			// помечаем занятыми ячейки под кораблём и вокруг него на одну клетку
			for(int i = rowMin; i < rowMax; ++i) {
				for(int j = colMin; j < colMax; ++j) {
					field[i][j].occupied = true;
				}
			}
#if 0
			QString line = ": ";
			for(int i = 0; i < ship.size; ++i) {
				line += "[";
				line += QString::number(ship.cells[i].y());
				line += ", ";
				line += QString::number(ship.cells[i].x());
				line += "]";
			}
			qDebug() << "\tship: " << ship.size << " orient " << ship.orient << "[" << row << "][" << col << "] " << " min " << rowMin << ", " << colMin << " max " << rowMax << ", " << colMax << line;
#endif
		}
	}
	this->repaint();
}

MainWindow::Cell::Status MainWindow::getStatus(const Cell& cell)
{
	Cell::Status status = Cell::Hidden;
	if(cell.visible) {
		Ship* ship = cell.ship;
		// можно как-то подсказывать игроку, что вокруг убитого корабля
		// другие корабли не могут быть ближе, чем через одну клетку,
		// но это будет совсем уж неинтересно играть
		//if(field[i][j].occupied && !field[i][j].ship) {
		//	brushStyle = Qt::Dense3Pattern;
		//}
		if(ship) { // попадание в корабль
			int k;
			for(k = 0; k < ship->size; ++k) {
				// если есть хоть одна закрытая клетка у
				// корабля, тогда он ранен, иначе - убит
				if(!field[ship->cells[k].y()][ship->cells[k].x()].visible) {
					break; //  таки ранен
				}
			}
			if(k < ship->size) {
				status = Cell::Injured;
			} else {
				status = Cell::Killed;
			}
		} else { // промах
			status = Cell::Miss;
		}
	}
	return status;
}

bool MainWindow::gameOver()
{
	int i;
	for(i = 0; i < maxShips; ++i)
		if(ships[i].alive)
			break;
	return (i == maxShips);
}

void MainWindow::openField()
{
	for(int i = 0; i < maxCells; ++i) {
		for(int j = 0; j < maxCells; ++j) {
			field[i][j].visible = true;
		}
	}
}
