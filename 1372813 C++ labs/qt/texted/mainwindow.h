#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QFile>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QSettings& settings, QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_dirsView_clicked(const QModelIndex &index);

	void on_filesView_clicked(const QModelIndex &index);

	void on_actSave_triggered();

	void on_actAbout_triggered();

	void on_actShowAll_toggled(bool arg1);

	void on_txtFilter_editingFinished();

private:
	// объект для загрузки пользовательского графического
	// интерфейса из xml-файла mainwindow.ui
	Ui::MainWindow *ui;
	// модели данных для навигации по файловой системе
	QFileSystemModel* dirsModel;
	QFileSystemModel* filesModel;
	// объект файла для чтения содержимого текстовых файлов
	QFile file;

	// параметры сессии

	// Структура для хранения имени, значения по умолчанию
	// и текущего значения параметра
	template<class T> struct StoredParam
	{
		QString name;
		const T defaultValue;
		T currentValue;
		StoredParam(const QString &name, const T defVal)
			: name(name), defaultValue(defVal), currentValue(defVal)
		{
		}
	};

	// ссылка на объект для сохранения/восстановления
	QSettings& settings;

	// Фильтр по имени файлов
	StoredParam<QStringList> filter{ "filter", {"*.txt", "*.log"} };
	// Показовать только отфильтрованные файлы
	StoredParam<bool> showAllFiles{ "showAllFiles", false };
};

#endif // MAINWINDOW_H
