#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "lizhifmapi.h"
#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QCloseEvent>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();
    void layoutInit();
    QStringList radioNameList(QString);
    QStringList classList(QString);
    QStringList hrefList(QStringList);
    QStringList user_sList(QString);
    void closeEvent(QCloseEvent*);
    void addListtoListWidget(QStringList,QStringList,QWidget*);
    QStringList readPage(QString,QString);


private:
    Ui::MainWidget *ui;
    LiZhiFMAPI *liZhiFMAPI;
    QMediaPlayer *fmPlayer;
//    QMediaPlaylist *playList;
    QStringList playList,playTitle;
    int listCurrentIndex = 0;
    QWidget *widget;
    QWidget *tagWidget;

signals:
    void urlSignal(QString);
    void imgUrlSiganl(QString);

public slots:
    void htmlSlot(QString,QString);
    void previousSlot();
    void nextSlot();
    void imgLabelSlot(QString);
};

#endif // MAINWIDGET_H
