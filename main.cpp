#include "mainwidget.h"
#include <QApplication>
#include <QFile>
#include <QDesktopWidget>

void setStyle(QString style)
{
    QFile qss(style);
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QDesktopWidget *desktopWdiget = QApplication::desktop();

    MainWidget w;
    setStyle(":/qss/style.qss");
    w.setWindowIcon(QIcon(":/imgs/lizhi_favicon.ico"));
    w.move(desktopWdiget->width()/2-w.width()/2,desktopWdiget->height()/2-w.height()/2);
    w.show();
    return a.exec();
}
