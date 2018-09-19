#include "mainwidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QFile>

void setStyle(const QString &style)
{
    QFile qss(style);
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();
}

int main(int argc,char **argv)
{
    QApplication app(argc,argv);
    QDesktopWidget *desktop = QApplication::desktop();
    MainWidget mw;
    int x = (desktop->width() - mw.width()) / 2;
    int y = (desktop->height() - mw.height()) / 2;
    mw.move(x,y);
    setStyle(":/qss/style.qss");
    mw.show();

    return app.exec();
}
