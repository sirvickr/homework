#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "taskdlg.h"
#include "task.h"
#include "birthday.h"
#include "meeting.h"
#include "calendar.h"

#include <QDebug>
#include <QImage>
//#include <QToolButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
    m_calendar = new Calendar();
    m_dateFilter = QDate::currentDate();

    QStringList List;

    initActions();

    setWindowTitle(tr("Личный календарь"));
    setMinimumSize(500, 350);
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
        showTask(dlg.task(), m_calendar->taskCount() - 1);
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
    int row = ui->lstTasks->currentRow();
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
    showTasks();
    if(ui->lstTasks->count()) {
        if(ui->lstTasks->count() == row)
            --row;
        ui->lstTasks->setCurrentRow(row);
    }
}

void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    m_dateFilter = date;
    showTasks();
}

// реализация методов

void MainWindow::initActions()
{
    // зададим стандартные горячие клавиши для действий

    ui->actNew->setShortcuts(QKeySequence::New);
    ui->actNew->setStatusTip("Создать новую задачу");
    //const QImage image("add.png");
    //ui->actNew->setIcon(QPixmap::fromImage(image));
    ui->actNew->setIcon(QIcon("img/add.png"));
    connect(ui->actNew, &QAction::triggered, this, &MainWindow::addTask);

    ui->actEdit->setStatusTip("Редактировать выбранную задачу");
    ui->actEdit->setIcon(QIcon("img/edit.png"));
    connect(ui->actEdit, &QAction::triggered, this, &MainWindow::editTask);

    ui->actDelete->setShortcuts(QKeySequence::Delete);
    ui->actDelete->setStatusTip("Удалить выбранную задачу");
    ui->actDelete->setIcon(QIcon("img/del.png"));
    connect(ui->actDelete, &QAction::triggered, this, &MainWindow::deleteTask);

    ui->actExit->setShortcuts(QKeySequence::Quit);
    ui->actExit->setStatusTip("Завершение работы");
    connect(ui->actExit, &QAction::triggered, this, &MainWindow::close);
    // Toolbar
    //QToolButton* btnNew = new QToolButton;
    //btnNew->setDefaultAction(ui->actNew);
    //ui->mainToolBar->addWidget(btnNew);
    ui->mainToolBar->addAction(ui->actNew);
    ui->mainToolBar->addAction(ui->actEdit);
    ui->mainToolBar->addAction(ui->actDelete);
}

void MainWindow::showTasks()
{
    ui->lstTasks->clear();
    int taskCount = m_calendar->taskCount();
    for(int i = 0; i < taskCount; ++i) {
        Task* task = m_calendar->task(i);
        if(task->when().date() == m_dateFilter) {
            showTask(task, i);
        }
    }
}

void MainWindow::showTask(Task* task, int index)
{
    const char* imgFileName = "";
    // создаём элемент визуального списка
    if(dynamic_cast<const Meeting*>(task)) {
        imgFileName = "img/share.png";
    } else if(dynamic_cast<const Birthday*>(task)) {
        imgFileName = "img/shopping-cart.png";
    } else if(dynamic_cast<const Interaction*>(task)) {
        imgFileName = "img/phone-call.png";
    } else { // базовый класс Task
        imgFileName = "img/star.png";
    }
    auto item = new QListWidgetItem(QIcon(imgFileName), task->toString());
    // сохраняем в этом элементе индекс задачи
    item->setData(Qt::UserRole, index);
    // добавляем элемент в список
    ui->lstTasks->addItem(item);
}
