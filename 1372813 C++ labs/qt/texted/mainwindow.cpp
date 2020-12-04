#include "aboutdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextStream>
#include <QTextCodec>
#include <QToolBar>
#include <QLineEdit>

MainWindow::MainWindow(QSettings& settings, QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, settings(settings)
{
	// загрузка пользовательского интерфейса
	ui->setupUi(this);

	// восстановление настроек
	showAllFiles.currentValue = settings.value(showAllFiles.name, showAllFiles.defaultValue).toBool();
	filter.currentValue = settings.value(filter.name, filter.defaultValue).toStringList();

	// подключаем объекты действий к слотам
	connect(ui->actQuit, &QAction::triggered, qApp, QApplication::quit);

	// помещаем действия в меню "Файл"
	QMenu* fileMenu = menuBar()->addMenu("Файл");
	fileMenu->addAction(ui->actShowAll);
	fileMenu->addAction(ui->actSave);
	fileMenu->addSeparator();
	fileMenu->addAction(ui->actQuit);

	// помещаем действия в меню "Справка"
	QMenu* helpMenu = menuBar()->addMenu("Справка");
	helpMenu->addAction(ui->actAbout);

	// помещаем действия на панель инструментов
	QToolBar* tbMain = addToolBar("main toolbar");
	tbMain->addAction(ui->actQuit);
	tbMain->addAction(ui->actSave);
	tbMain->addAction(ui->actShowAll);

	QString root = "C:/";
	// создаём модель данных для каталогов и присоединяем к ней окно каталогов
	dirsModel = new QFileSystemModel(this);
	dirsModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
	dirsModel->setRootPath(root);
	ui->dirsView->setModel(dirsModel);

	// создаём модель данных для файлов и присоединяем к ней окно файлов
	filesModel = new QFileSystemModel(this);
	filesModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
	filesModel->setNameFilters(filter.currentValue);
	filesModel->setRootPath(root);
	filesModel->setNameFilterDisables(showAllFiles.currentValue);
	ui->filesView->setModel(filesModel);

	// визуализация параметров сессии
	ui->txtFilter->setText(filter.currentValue.join(";"));
	ui->actShowAll->setChecked(showAllFiles.currentValue);
}

MainWindow::~MainWindow()
{
	// сохранить параметры
	settings.setValue(filter.name, filter.currentValue);
	settings.setValue(showAllFiles.name, showAllFiles.currentValue);
	// удалить пользовательский интерфейс
	delete ui;
}

void MainWindow::on_dirsView_clicked(const QModelIndex &index)
{
	// при щелчке ЛКМ по узлу дерева папок, отображаем его содержимое в окне файлов
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

void MainWindow::on_actSave_triggered()
{
	// сохраняем текущие параметры сессии в настройках приложения
	;
}

void MainWindow::on_actAbout_triggered()
{
	// Выводим диалог "О программе"
	AboutDialog* dlg = new AboutDialog;
	dlg->exec();
	delete dlg;
}

void MainWindow::on_actShowAll_toggled(bool arg1)
{
	showAllFiles.currentValue = arg1;
	filesModel->setNameFilterDisables(showAllFiles.currentValue);
}

void MainWindow::on_txtFilter_editingFinished()
{
	filter.currentValue = ui->txtFilter->text().split(";");
	filesModel->setNameFilters(filter.currentValue);
}
