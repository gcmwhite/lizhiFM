#include "mainwidget.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;
    w.setWindowIcon(QIcon(":/ico/lizhi_favicon.ico"));
    w.show();

    return a.exec();
}
