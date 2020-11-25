#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_dirsView_clicked(const QModelIndex &index);

	void on_filesView_clicked(const QModelIndex &index);

private:
	// объект для загрузки пользовательского графического
	// интерфейса из xml-файла mainwindow.ui
	Ui::MainWindow *ui;
	// модели данных для навигации по файловой системе
	QFileSystemModel* dirsModel;
	QFileSystemModel* filesModel;
	// объект файла для чтения содержимого текстовых файлов
	QFile file;
};
#endif // MAINWINDOW_H
