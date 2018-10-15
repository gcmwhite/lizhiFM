#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "leftwidget.h"
#include "footwidget.h"
#include "tabwidget.h"
#include "gridbtnwidget.h"
#include "radiotype.h"
#include "lizhiapi.h"
#include "musiclistwidget.h"
#include "tagwidget.h"
#include "aplayer.h"
#include <QWidget>
#include <QGridLayout>
#include <QJsonObject>
#include <QJsonArray>
#include <QStackedLayout>
#include <QPixmap>
#include <QResizeEvent>

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    void init_ui_();
    void init_network_();
    void init_a_player_();
    void set_background_image(const QString &);

private:
    void resizeEvent(QResizeEvent *);

private:
    LeftWidget *leftWidget;
    FootWidget *footWidget;
    TabWidget *tabWidget;
    QGridLayout *mainLayout;
    GridBtnWidget *hot_grid_widget;
    GridBtnWidget *optimization_grid_widget;
    RadioType *radioType;
    LiZhiAPI *lizhiAPI;
    MusicListWidget *musicListWidget;
    QStackedLayout *stackedLayout;
    TagWidget *tagWidget;
    Aplayer *aplayer;
    QVector<QStringList> vec_play_list;

    //版本信息
    QString version = "2.0-release";                                //版本

    //配置信息
    QString skin_ = ":/background/background_1.jpg";             //皮肤
    QString position_ ;                                     //播放位置
    QString currentMusic;                                       //当前播放曲目
    QJsonArray playListJsonArray_;                                        //播放列表


signals:


public slots:

};

#endif // MAINWIDGET_H
