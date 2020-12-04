#include "mainwindow.h"

#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QSettings settings("Dispatcher", "TextViewer");
	MainWindow w(settings);
	w.show();
	return a.exec();
}
