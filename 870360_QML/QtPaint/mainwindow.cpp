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
		path.push_back({QPoint(event->x(), event->y()), QColor(Qt::red), 0, false});
		drawing = true;
	}
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
	if(event->button() == Qt::LeftButton) {
		drawing = false;
	}
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
	if(drawing) {
		QColor color = ui->btnColor->palette().color(ui->btnColor->foregroundRole());
		path.push_back({QPoint(event->x(), event->y()), color, ui->spbLineWidth->value(), true});
		this->repaint();
	}
}

void MainWindow::paintEvent(QPaintEvent *event)
{
	if(!path.empty()) {
		painter.begin(this);
		PathPoint prev = path[0];
		for(size_t i = 1; i < path.size(); ++i) {
			const PathPoint& item = path[i];
			if(item.visible) {
				painter.setPen(QPen(item.color, item.width));
			} else {
				painter.setPen(Qt::NoPen);
			}
			painter.drawLine(prev.point, item.point);
			prev = item;
		}
		painter.end();
	}
}

void MainWindow::on_btnColor_clicked()
{
	QPalette palette = ui->btnColor->palette();
	QColor color = QColorDialog::getColor(palette.color(ui->btnColor->foregroundRole()), this);
	palette.setColor(ui->btnColor->foregroundRole(), color);
	ui->btnColor->setPalette( palette );
}
