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

	resetGame();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
	/*if(event->button() == Qt::LeftButton) {
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
	}*/
}

bool MainWindow::moveHorz(int inc, int k0, std::function<bool(int)> progress) {
	//Left:	 inc = 1;  // слева напрово start = 1              stop(int k) { return k < maxCells; }
	//Right: int = -1; // справа налево start = maxCells - 2   stop(int k) { return k >= 0; }
	bool hasMoved = false;
	int start = k0 + inc;
	for(int i = 0; i < maxCells; ++i) { // для каждой строки
		//Left:	 int k0 = 0; // крайняя левая клетка в строке
		//Right: int k0 = maxCells - 1; // крайняя правая клетка в строке
		int k = k0;
		for(int j = start; progress(j); j += inc) {
			auto& next = field[i][j];
			if(!next.isEmpty()) {
				if(next.isSameTo(field[i][k])) {
					// merge
					//field[i][k].value += next.value;
					//next.clear();
					//totalScore += field[i][k].value;
					field[i][k].merge(next);
					k += inc;
					hasMoved = true;
				} else {
					if(!field[i][k].isEmpty())
						k += inc;
					if(j != k) {
						//field[i][k].value += next.value;
						//next.clear();
						field[i][k].merge(next);
						hasMoved = true;
					}
				}
			}
		}
	}
	return hasMoved;
}

bool MainWindow::moveVert(int inc, int k0, std::function<bool(int)> progress) {
	//Uo:	inc = 1;  // start = 1              stop(int k) { return k < maxCells; }
	//Down: int = -1; // start = maxCells - 2   stop(int k) { return k >= 0; }
	bool hasMoved = false;
	int start = k0 + inc;
	for(int j = 0; j < maxCells; ++j) { // для каждого столбца
		//Up:	 k0 = 0; // крайняя левая клетка в строке
		//Down:  k0 = maxCells - 1; // крайняя правая клетка в строке
		int k = k0;
		for(int i = start; progress(i); i += inc) {
			auto& next = field[i][j];
			if(!next.isEmpty()) {
				if(next.isSameTo(field[k][j])) {
					//field[k][j].value += next.value;
					//next.clear();
					//totalScore += field[k][j].value;
					field[k][j].merge(next);
					k += inc;
					hasMoved = true;
				} else {
					if(!field[k][j].isEmpty())
						k += inc;
					if(i != k) {
						//field[k][j].value += next.value;
						//next.clear();
						field[k][j].merge(next);
						hasMoved = true;
					}
				}
			}
		}
	}
	return hasMoved;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
	bool hasMoved = false;
	switch(event->key()) {
	case Qt::Key_Left:
		hasMoved = moveHorz(1,  0, [](int k){ return k < maxCells; });
		/*for(int i = 0; i < maxCells; ++i) { // для каждой строки
			int k = 0; // крайняя левая клетка в строке
			for(int j = 1; j < maxCells; ++j) {
				if(field[i][j].value > 0) {
					if(field[i][j].value == field[i][k].value) {
						field[i][j].value = 0;
						field[i][k].value <<= 1;
						totalScore += field[i][k].value;
						k++;
					} else {
						if(field[i][k].value > 0)
							field[i][++k].value = field[i][j].value;
						else
							field[i][k].value = field[i][j].value;
						if(j != k) {
							field[i][j].value = 0;
							hasMoved = true;
						}
					}
				}
			}
		}*/
		break;
	case Qt::Key_Right:
		hasMoved = moveHorz(-1, maxCells - 1, [](int k){ return k >= 0; });
		/*for(int i = 0; i < maxCells; ++i) { // для каждой строки
			int k = maxCells - 1; // крайняя правая клетка в строке
			for(int j = maxCells - 2; j >= 0; --j) {
				if(field[i][j].value > 0) {
					if(field[i][j].value == field[i][k].value) {
						field[i][j].value = 0;
						field[i][k].value <<= 1;
						totalScore += field[i][k].value;
						k--;
					} else {
						if(field[i][k].value > 0)
							field[i][--k].value = field[i][j].value;
						else
							field[i][k].value = field[i][j].value;
						if(j != k) {
							field[i][j].value = 0;
							hasMoved = true;
						}
					}
				}
			}
		}*/
		break;
	case Qt::Key_Up:
		hasMoved = moveVert(1,  0, [](int k){ return k < maxCells; });
		/*for(int j = 0; j < maxCells; ++j) { // для каждого столбца
			int k = 0; // крайняя верхняя клетка в строке
			for(int i = 1; i < maxCells; ++i) {
				if(field[i][j].value > 0) {
					if(field[i][j].value == field[k][j].value) {
						field[i][j].value = 0;
						field[k][j].value <<= 1;
						qDebug() << "+" << field[k][j].value;
						totalScore += field[k][j].value;
						k++;
					} else {
						if(field[k][j].value > 0)
							field[++k][j].value = field[i][j].value;
						else
							field[k][j].value = field[i][j].value;
						if(i != k) {
							field[i][j].value = 0;
							hasMoved = true;
						}
					}
				}
			}
		}*/
		break;
	case Qt::Key_Down:
		hasMoved = moveVert(-1, maxCells - 1, [](int k){ return k >= 0; });
		/*for(int j = 0; j < maxCells; ++j) { // для каждого столбца
			int k = maxCells - 1; // крайняя нижняя клетка в строке
			for(int i = maxCells - 2; i >= 0; --i) {
				if(field[i][j].value > 0) {
					if(field[i][j].value == field[k][j].value) {
						field[i][j].value = 0;
						field[k][j].value <<= 1;
						qDebug() << "+" << field[k][j].value;
						totalScore += field[k][j].value;
						k--;
					} else {
						if(field[k][j].value > 0)
							field[--k][j].value = field[i][j].value;
						else
							field[k][j].value = field[i][j].value;
						if(i != k) {
							field[i][j].value = 0;
							hasMoved = true;
						}
					}
				}
			}
		}*/
		break;
	}

	//totalScore += score;
	//if(score > 0) {
	//	hasMoved = true;
	//}
	if(hasMoved) {
		++stepCount;
		int i, j;
		int freeCount = 0;
		for(i = 0; i < maxCells; ++i) {
			for(j = 0; j < maxCells; ++j) {
				if(field[i][j].value == 0) {
					++freeCount;
				}
			}
		}
		int target = qrand() % freeCount;
		qDebug() << "freeCount " << freeCount << " target " << target;
		freeCount = 0;
		for(i = 0; i < maxCells; ++i) {
			for(j = 0; j < maxCells; ++j) {
				if(field[i][j].value == 0) {
					if(target == freeCount++) {
						field[i][j].value = 2;
						break;
					}
				}
			}
		}
	}

	ui->lblStepCount->setText(QString::number(stepCount));
	ui->lblStepResult->setText(QString::number(totalScore));

	this->repaint();
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
	QFont font = painter.font();
	font.setBold(true);
	font.setPixelSize(32);
	painter.setFont(font);
	for(int i = 0; i < maxCells; ++i) {
		for(int j = 0; j < maxCells; ++j) {
			Qt::GlobalColor clr = Qt::darkCyan;
			Qt::BrushStyle brushStyle = Qt::SolidPattern;
			/*Cell::Status status = getStatus(field[i][j]);
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
			}*/
			painter.setBrush(QBrush(clr, brushStyle));
			painter.drawRect(field[i][j].rect);
			if(field[i][j].value > 0) {
				painter.drawText(field[i][j].rect, Qt::AlignCenter, QString::number(field[i][j].value));
			}
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
	resetGame();
}

void MainWindow::resetGame()
{
	// можно QDateTime::currentMSecsSinceEpoch()
	qsrand(QTime::currentTime().msec());
	totalScore = 0;
	stepCount = 0;
	ui->lblStepCount->setText(QString::number(stepCount));
	// закрываем поле
	for(int i = 0; i < maxCells; ++i) {
		for(int j = 0; j < maxCells; ++j) {
			field[i][j].game = this;
			field[i][j].value = 0;
		}
	}
#if 0
	field[0][0].value = 2;
	field[0][1].value = 4;
	field[0][2].value = 2;
	field[0][3].value = 2;
#else
	int x0 = qrand() % maxCells;
	int y0 = qrand() % maxCells;
	int x1, y1;
	do {
		x1 = qrand() % maxCells;
		y1 = qrand() % maxCells;
	} while(x1 == x0 && y1 == y0);
	field[x0][y0].value = 2;
	field[x1][y1].value = 4;
#endif
	this->repaint();
}

/*MainWindow::Cell::Status MainWindow::getStatus(const Cell& cell)
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
}*/

bool MainWindow::gameOver()
{
	/*
	int i;
	for(i = 0; i < maxShips; ++i)
		if(ships[i].alive)
			break;
	return (i == maxShips);
	*/
	return false; // TODO
}
