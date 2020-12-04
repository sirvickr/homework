#include "texted.h"
#include <QtWidgets/QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	QSettings settings("Dispatcher", "TextViewer");
	texted w(settings);
    w.show();
    return a.exec();
}
