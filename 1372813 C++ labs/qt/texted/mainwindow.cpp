#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextStream>
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QString path = "C:/";
	// создаём модель данных для каталогов
	// и присоединяем к ней окно каталогов
	dirsModel = new QFileSystemModel(this);
	dirsModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
	dirsModel->setRootPath(path);
	ui->dirsView->setModel(dirsModel);

	// создаём модель данных для файлов
	// и присоединяем к ней окно файлов
	filesModel = new QFileSystemModel(this);
	filesModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
	filesModel->setNameFilters({"*.txt", "*.log"});
	filesModel->setRootPath(path);
	filesModel->setNameFilterDisables(false);
	ui->filesView->setModel(filesModel);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_dirsView_clicked(const QModelIndex &index)
{
	// при щелчке ЛКМ по узлу дерева папок,
	// отображаем его содержимое в окне файлов
	QString path = dirsModel->fileInfo(index).absoluteFilePath();
	ui->filesView->setRootIndex(filesModel->setRootPath(path));
}

void MainWindow::on_filesView_clicked(const QModelIndex &index)
{
	// при щелчке по имени файла, читаем его содержимое
	// в виде текста в кодировке UTF-8 и отображаем
	QString fileName = filesModel->fileInfo(index).absoluteFilePath();
	file.setFileName(fileName);
	if(file.open(QFile::ReadOnly | QFile::Text)) {
		ui->textView->clear();
		QTextStream text(&file);
		text.setCodec(QTextCodec::codecForName("UTF-8"));
		while(!text.atEnd()) {
			ui->textView->append(text.readLine());
		}
		file.close();
	}
}
