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
	// ����������� ������� ����������������� ����������
	void on_dirsView_clicked(const QModelIndex &index);
	void on_filesView_clicked(const QModelIndex &index);
	void on_actSave_triggered();
	void on_actAbout_triggered();
	void on_actShowAll_toggled(bool arg1);
	void on_actAutosave_toggled(bool arg1);
	void on_txtFilter_editingFinished();

private:
	// ��������� ��������� ������
	void loadSession();
	// ��������� ��������� ������
	void saveSession();
	// ������������� ������������� ��������� � ������
	void syncViews(const QString& path);

	// ������ ��� �������� ����������������� ������������
	// ���������� �� xml-����� mainwindow.ui
	Ui::textedClass ui;
	// ������ ������ ��� ��������� �� �������� �������
	QFileSystemModel* dirsModel;
	QFileSystemModel* filesModel;
	// ������ ����� ��� ������ ����������� ��������� ������
	QFile file;

	// ��������� ������

	// ��������� ��� �������� �����, �������� �� ���������
	// � �������� �������� ���������
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

	// ������ �� ������ ��� ����������/��������������
	QSettings& settings;

	// ������ �� ����� ������
	StoredParam<QStringList> filter{ "filter", {"*.txt", "*.log"} };
	// ���������� ������ ��������������� �����
	StoredParam<bool> showAllFiles{ "showAllFiles", false };
	// �������������� ������ ��� ������
	StoredParam<bool> autosave{ "autosave", false };
};
