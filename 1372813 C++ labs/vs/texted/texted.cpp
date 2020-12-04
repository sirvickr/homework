#include "texted.h"

#include <QTextStream>
#include <QTextCodec>

texted::texted(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	QString path = "C:/";
	// ������ ������ ������ ��� ���������
	// � ������������ � ��� ���� ���������
	dirsModel = new QFileSystemModel(this);
	dirsModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
	dirsModel->setRootPath(path);
	ui.dirsView->setModel(dirsModel);

	// ������ ������ ������ ��� ������
	// � ������������ � ��� ���� ������
	filesModel = new QFileSystemModel(this);
	filesModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
	filesModel->setNameFilters({ "*.txt", "*.log" });
	filesModel->setRootPath(path);
	filesModel->setNameFilterDisables(false);
	ui.filesView->setModel(filesModel);
}

void texted::on_dirsView_clicked(const QModelIndex &index)
{
	// ��� ������ ��� �� ���� ������ �����,
	// ���������� ��� ���������� � ���� ������
	QString path = dirsModel->fileInfo(index).absoluteFilePath();
	ui.filesView->setRootIndex(filesModel->setRootPath(path));
}

void texted::on_filesView_clicked(const QModelIndex &index)
{
	// ��� ������ �� ����� �����, ������ ��� ����������
	// � ���� ������ � ��������� UTF-8 � ����������
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
