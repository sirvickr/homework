#pragma once

#include "ui_texted.h"

#include <QtWidgets/QMainWindow>
#include <QFileSystemModel>
#include <QFile>
#include <QSettings>

class texted : public QMainWindow
{
    Q_OBJECT

public:
    texted(QSettings& settings, QWidget *parent = Q_NULLPTR);
	~texted();

private slots:
	// обработчики событий пользовательского интерфейса
	void on_dirsView_clicked(const QModelIndex &index);
	void on_filesView_clicked(const QModelIndex &index);
	void on_actSave_triggered();
	void on_actAbout_triggered();
	void on_actShowAll_toggled(bool arg1);
	void on_actAutosave_toggled(bool arg1);
	void on_txtFilter_editingFinished();

private:
	// загрузить настройки сессии
	void loadSession();
	// сохранить настройки сессии
	void saveSession();
	// синхронизация представлений каталогов и файлов
	void syncViews(const QString& path);

	// объект для загрузки пользовательского графического
	// интерфейса из xml-файла mainwindow.ui
	Ui::textedClass ui;
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
	// Автосохранение сессии при выходе
	StoredParam<bool> autosave{ "autosave", false };
};
