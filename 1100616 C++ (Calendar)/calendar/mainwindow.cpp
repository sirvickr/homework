#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "taskdlg.h"
#include "task.h"
#include "birthday.h"
#include "meeting.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
    m_calendar = new Calendar();

    QStringList List;

    initActions();
    QString message = tr("");
    statusBar()->showMessage(message);
    setWindowTitle(tr("Личный календарь"));
    setMinimumSize(500, 350);
    //resize(600, 450);
}

MainWindow::~MainWindow()
{
	delete ui;
}

// реализация слотов

void MainWindow::addTask()
{
    TaskDlg dlg;
    dlg.setModal(true);
    if(dlg.exec() && dlg.task()) {
        // добавляем задачу в календарь
        m_calendar->addTask(dlg.task());
        // создаём элемент визуального списка
        auto item = new QListWidgetItem(dlg.task()->toString());
        // сохраняем в этом элементе индекс задачи
        item->setData(Qt::UserRole, m_calendar->taskCount() - 1);
        // добавляем элемент в список
        ui->lstTasks->addItem(item);
    }
}

void MainWindow::editTask()
{
    QListWidgetItem* item = ui->lstTasks->currentItem();
    if(!item) {
        return;
    }
    int index = item->data(Qt::UserRole).toInt();
    const Task* task = m_calendar->task(index);
    if(!task) {
        qDebug() << "inconsistent program state: index = " << index;
        return;
    }
    TaskDlg* dlg = 0;
    if(auto meeting = dynamic_cast<const Meeting*>(task)) {
        dlg = new TaskDlg(meeting->when(), meeting->name(),
                          meeting->who(), meeting->where());
    } else if(auto birthday = dynamic_cast<const Birthday*>(task)) {
        dlg = new TaskDlg(birthday->when(), birthday->name(),
                          birthday->who(), birthday->presents());
    } else if(auto call = dynamic_cast<const Interaction*>(task)) {
        dlg = new TaskDlg(call->when(), call->name(),
                          call->who());
    } else { // базовый класс Task
        dlg = new TaskDlg(task->when(), task->name());
    }
    if(dlg) {
        dlg->setModal(true);

        if(dlg->exec() && dlg->task()) {
            // обновлем параметры задачи в календаре
            m_calendar->setTask(index, dlg->task());
            // сохраняем в этом элементе индекс задачи
            item->setData(Qt::UserRole, m_calendar->taskCount() - 1);
            // добавляем элемент в список
            item->setText(dlg->task()->toString());
        }

        delete dlg;
        dlg = 0;
    }
}

void MainWindow::deleteTask()
{
    QListWidgetItem* item = ui->lstTasks->currentItem();
    if(!item) {
        return;
    }
    int index = item->data(Qt::UserRole).toInt();
    const Task* task = m_calendar->task(index);
    if(!task) {
        qDebug() << "inconsistent program state: index = " << index;
        return;
    }
    m_calendar->delTask(index);

    ui->lstTasks->removeItemWidget(item);
    delete item;
}

// реализация методов

void MainWindow::initActions()
{
    // зададим стандартные горячие клавиши для действий

    ui->actNew->setShortcuts(QKeySequence::New);
    ui->actNew->setStatusTip("Создать новую задачу");
    connect(ui->actNew, &QAction::triggered, this, &MainWindow::addTask);

    ui->actEdit->setStatusTip("Редактировать выбранную задачу");
    connect(ui->actEdit, &QAction::triggered, this, &MainWindow::editTask);

    ui->actDelete->setShortcuts(QKeySequence::Delete);
    ui->actDelete->setStatusTip("Удалить выбранную задачу");
    connect(ui->actDelete, &QAction::triggered, this, &MainWindow::deleteTask);

    ui->actExit->setShortcuts(QKeySequence::Quit);
    ui->actExit->setStatusTip("Завершение работы");
    connect(ui->actExit, &QAction::triggered, this, &MainWindow::close);
}
