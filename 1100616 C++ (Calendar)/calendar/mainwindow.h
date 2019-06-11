#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
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

private Q_SLOTS:
    void addTask();
    void editTask();
    void deleteTask();

private:
    void initActions();

private:
    Ui::MainWindow *ui = nullptr;
    Calendar* m_calendar = nullptr;
};

#endif // MAINWINDOW_H
