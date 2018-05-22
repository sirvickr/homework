#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <vector>

namespace Ui {
	class MainWindow;
}

struct PathPoint {
	QPoint point;
	QColor color;
	int width;
	bool visible;
};

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private slots:
	void on_btnColor_clicked();

private:
	Ui::MainWindow *ui;
	bool drawing;
	QPainter painter;
	std::vector<PathPoint> path;
};

#endif // MAINWINDOW_H
