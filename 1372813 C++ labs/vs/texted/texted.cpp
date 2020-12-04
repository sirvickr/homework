#include "texted.h"
#include "aboutdialog.h"

#include <QTextStream>
#include <QTextCodec>
#include <QToolBar>
#include <QLineEdit>

texted::texted(QSettings& settings, QWidget *parent)
    : QMainWindow(parent), settings(settings)
{
	// загрузка пользовательского интерфейса
	ui.setupUi(this);

	// восстановление сохранённых настроек
	loadSession();

	// подключаем объекты действий к слотам
	connect(ui.actQuit, &QAction::triggered, qApp, QApplication::quit);

	// помещаем действия в меню "Файл"
	ui.menuFile->addAction(ui.actSave);
	ui.menuFile->addAction(ui.actAutosave);
	ui.menuFile->addAction(ui.actShowAll);
	ui.menuFile->addSeparator();
	ui.menuFile->addAction(ui.actQuit);

	// помещаем действия в меню "Справка"
	ui.menuHelp->addAction(ui.actAbout);

	// помещаем действия на панель инструментов
	QToolBar* tbMain = addToolBar("main toolbar");
	tbMain->addAction(ui.actQuit);
	tbMain->addSeparator();
	tbMain->addAction(ui.actSave);
	tbMain->addAction(ui.actAutosave);
	tbMain->addAction(ui.actShowAll);

	QString root = "C:/";
	// создаём модель данных для каталогов  и присоединяем к ней окно каталогов
	dirsModel = new QFileSystemModel(this);
	dirsModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
	dirsModel->setRootPath(root);
	ui.dirsView->setModel(dirsModel);

	// создаём модель данных для файлов и присоединяем к ней окно файлов
	filesModel = new QFileSystemModel(this);
	filesModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
	filesModel->setNameFilters(filter.currentValue);
	filesModel->setNameFilterDisables(showAllFiles.currentValue);
	ui.filesView->setModel(filesModel);

	syncViews(root);

	// визуализация параметров сессии
	ui.txtFilter->setText(filter.currentValue.join(";"));
	ui.actShowAll->setChecked(showAllFiles.currentValue);
	ui.actAutosave->setChecked(autosave.currentValue);
}

texted::~texted()
{
	if (autosave.currentValue)
	{
		saveSession();
	}
}

void texted::loadSession()
{
	// загружаем текущие параметры сессии
	filter.currentValue = settings.value(filter.name, filter.defaultValue).toStringList();
	showAllFiles.currentValue = settings.value(showAllFiles.name, showAllFiles.defaultValue).toBool();
	autosave.currentValue = settings.value(autosave.name, autosave.defaultValue).toBool();
}

void texted::saveSession()
{
	// сохраняем текущие параметры сессии
	settings.setValue(filter.name, filter.currentValue);
	settings.setValue(showAllFiles.name, showAllFiles.currentValue);
	settings.setValue(autosave.name, autosave.currentValue);
}

void texted::syncViews(const QString& path)
{
	ui.filesView->setRootIndex(filesModel->setRootPath(path));
}

void texted::on_dirsView_clicked(const QModelIndex &index)
{
	// при щелчке ЛКМ по узлу дерева папок, отображаем его содержимое в окне файлов
	syncViews(dirsModel->fileInfo(index).absoluteFilePath());
}

void texted::on_filesView_clicked(const QModelIndex &index)
{
	// при щелчке по имени файла, читаем его содержимое как текст в кодировке UTF-8 и отображаем
	QString fileName = filesModel->fileInfo(index).absoluteFilePath();
	file.setFileName(fileName);
	if (file.open(QFile::ReadOnly | QFile::Text)) {
		ui.textView->clear();
		QTextStream text(&file);
		text.setCodec(QTextCodec::codecForName("UTF-8"));
		while (!text.atEnd()) {
			ui.textView->append(text.readLine());
		}
		file.close();
	}
}

void texted::on_actSave_triggered()
{
	saveSession();
}

void texted::on_actAbout_triggered()
{
	// Выводим диалог "О программе"
	AboutDialog* dlg = new AboutDialog;
	dlg->exec();
	delete dlg;
}

void texted::on_actShowAll_toggled(bool arg1)
{
	showAllFiles.currentValue = arg1;
	filesModel->setNameFilterDisables(showAllFiles.currentValue);
}

void texted::on_actAutosave_toggled(bool arg1)
{
	autosave.currentValue = arg1;
}

void texted::on_txtFilter_editingFinished()
{
	filter.currentValue = ui.txtFilter->text().split(";");
	filesModel->setNameFilters(filter.currentValue);
}
