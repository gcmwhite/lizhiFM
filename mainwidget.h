#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "leftwidget.h"
#include "footwidget.h"
#include "listwidget.h"
#include "lizhifmhttp.h"
#include "mtabwidget.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMediaPlayer>
#include <QResizeEvent>
#include <QPixmap>

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();
    void mainWidgetInit();
    void addListtoListWidget(QStringList,QStringList,QWidget *);
    void resizeEvent(QResizeEvent *);
    void setBackgroundImage(QPixmap &pixmap);
    void readData();
    void writeData();
    QStringList radioNameList(QString);
    QStringList readPage(QString,QString);
    QStringList hrefList(QStringList);
    QStringList classList(QString);
    QStringList user_sList(QString);



private:
    LeftWidget *leftWidget;
    FootWidget *footWidget;
    MTabWidget *tabWidget;
    QWidget *tabWidget_1;
    QWidget *tabWidget_2;
    QWidget *tabWidget_3;
    QVBoxLayout *tabWidget_3_layout;
    QWidget *tabWidget_3_1;
    QWidget *tabWidget_3_2;
    ListWidget *listWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *centerLayout;
    LizhiFMHttp *lizhiFMHttp;
    QMediaPlayer *fmPlayer;

    QString prePageUrl;
    QString nextPageUrl;
    QStringList urlList;
    QStringList titleList;

    int listCurrentIndex;
    qint64 position;
    QStringList playList;
    QStringList playTitle;
    QString background;

    QPixmap pixmap;

    bool readFlag;


public slots:
    void htmlSlot(QString,QString);
    void imgLabelSlot(QString);
    void previousSlot();
    void nextSlot();

signals:
    void urlSignal(QString);
    void imgUrlSiganl(QString);
};

#endif // MAINWIDGET_H
