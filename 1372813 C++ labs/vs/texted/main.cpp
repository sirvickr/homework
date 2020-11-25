#include "texted.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    texted w;
    w.show();
    return a.exec();
}
