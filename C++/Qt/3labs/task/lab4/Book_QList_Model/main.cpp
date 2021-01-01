#include <QtGui/QApplication>
#include "DatabaseWindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DatabaseWindow w;
	w.show();
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}
