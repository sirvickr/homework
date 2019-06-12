#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>

namespace Ui {
class MainWindow;
}

class Calendar;
class Task;

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
    void on_calendarWidget_clicked(const QDate &date);

private:
    void initActions();
    void showTasks();
    void showTask(Task* task, int index);

private:
    Ui::MainWindow *ui = nullptr;
    Calendar* m_calendar = nullptr;
    // Выбранная на календаре дата:
    // в списке отображаются задачи только на этот день
    QDate m_dateFilter;
};

#endif // MAINWINDOW_H
