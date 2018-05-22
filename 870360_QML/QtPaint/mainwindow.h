#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <vector>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	// узловая точка рисунка
	struct PathPoint {
		QPoint point;
		QPen pen;
	};

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	// обработчики событий
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private slots:
	void on_btnColor_clicked();

private:
	Ui::MainWindow *ui;
	QPainter painter;
	// признак нажатой кнопки мыши (т.е. необходимости отрисовывать линию)
	bool drawing;
	// рисунок (совокупность ломаных линий)
	std::vector<PathPoint> path;
};

#endif // MAINWINDOW_H
