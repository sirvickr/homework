#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMouseEvent>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
	if(event->button() == Qt::LeftButton) {
		// нажали кнопку мыши - значит, есть первая точка новой ломаной
		// значит, отсюда пойдёт новая линий, но в эту точку линия
		// входить не должна, поэтому задаём перо Qt::NoPen
		path.push_back({QPoint(event->x(), event->y()), Qt::NoPen});
		// включаем рисование (добавление новых линий)
		drawing = true;
	}
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
	if(event->button() == Qt::LeftButton) {
		// выключаем рисование (добавление новых линий)
		drawing = false;
	}
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
	if(drawing) {
		// рисetv (добавляем новые линии к рисунку)
		QColor color = ui->btnColor->palette().color(ui->btnColor->foregroundRole());
		// перо (толщину и цвет линии) задаём согласно заданным пользователем параметрам
		path.push_back({QPoint(event->x(), event->y()), QPen(color, ui->spbLineWidth->value())});
		// вызываем обработчик paintEvent() для отображения всего рисунка,
		// включая только что добавленные элементы
		this->repaint();
	}
}

void MainWindow::paintEvent(QPaintEvent *event)
{
	if(!path.empty()) {
		// отображаем весь рисунок
		painter.begin(this);
		for(size_t i = 1; i < path.size(); ++i) {
			painter.setPen(path[i].pen);
			painter.drawLine(path[i - 1].point, path[i].point);
		}
		painter.end();
	}
}

void MainWindow::on_btnColor_clicked()
{
	// текущий цвет храним в foreground-палитре кнопки btnColor
	QPalette palette = ui->btnColor->palette();
	QColor color = QColorDialog::getColor(palette.color(ui->btnColor->foregroundRole()), this);
	palette.setColor(ui->btnColor->foregroundRole(), color);
	ui->btnColor->setPalette( palette );
}
