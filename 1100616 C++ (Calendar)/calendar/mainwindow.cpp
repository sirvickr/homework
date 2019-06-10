#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "calendar.h"
#include "birthday.h"
#include "meeting.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_calendar(QSharedPointer<Calendar>::create())
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}
