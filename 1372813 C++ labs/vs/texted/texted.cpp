#include "texted.h"
#include "aboutdialog.h"

#include <QTextStream>
#include <QTextCodec>
#include <QToolBar>
#include <QLineEdit>

texted::texted(QSettings& settings, QWidget *parent)
    : QMainWindow(parent), settings(settings)
{
	// �������� ����������������� ����������
	ui.setupUi(this);

	// �������������� ���������� ��������
	loadSession();

	// ���������� ������� �������� � ������
	connect(ui.actQuit, &QAction::triggered, qApp, QApplication::quit);

	// �������� �������� � ���� "����"
	ui.menuFile->addAction(ui.actSave);
	ui.menuFile->addAction(ui.actAutosave);
	ui.menuFile->addAction(ui.actShowAll);
	ui.menuFile->addSeparator();
	ui.menuFile->addAction(ui.actQuit);

	// �������� �������� � ���� "�������"
	ui.menuHelp->addAction(ui.actAbout);

	// �������� �������� �� ������ ������������
	QToolBar* tbMain = addToolBar("main toolbar");
	tbMain->addAction(ui.actQuit);
	tbMain->addSeparator();
	tbMain->addAction(ui.actSave);
	tbMain->addAction(ui.actAutosave);
	tbMain->addAction(ui.actShowAll);

	QString root = "C:/";
	// ������ ������ ������ ��� ���������  � ������������ � ��� ���� ���������
	dirsModel = new QFileSystemModel(this);
	dirsModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
	dirsModel->setRootPath(root);
	ui.dirsView->setModel(dirsModel);

	// ������ ������ ������ ��� ������ � ������������ � ��� ���� ������
	filesModel = new QFileSystemModel(this);
	filesModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
	filesModel->setNameFilters(filter.currentValue);
	filesModel->setNameFilterDisables(showAllFiles.currentValue);
	ui.filesView->setModel(filesModel);

	syncViews(root);

	// ������������ ���������� ������
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
	// ��������� ������� ��������� ������
	filter.currentValue = settings.value(filter.name, filter.defaultValue).toStringList();
	showAllFiles.currentValue = settings.value(showAllFiles.name, showAllFiles.defaultValue).toBool();
	autosave.currentValue = settings.value(autosave.name, autosave.defaultValue).toBool();
}

void texted::saveSession()
{
	// ��������� ������� ��������� ������
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
	// ��� ������ ��� �� ���� ������ �����, ���������� ��� ���������� � ���� ������
	syncViews(dirsModel->fileInfo(index).absoluteFilePath());
}

void texted::on_filesView_clicked(const QModelIndex &index)
{
	// ��� ������ �� ����� �����, ������ ��� ���������� ��� ����� � ��������� UTF-8 � ����������
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
	// ������� ������ "� ���������"
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
