#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>
#include "calendar.h"

namespace Ui {
class MainWindow;
}

class Calendar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
	QSharedPointer<Calendar> m_calendar;
};

#endif // MAINWINDOW_H
