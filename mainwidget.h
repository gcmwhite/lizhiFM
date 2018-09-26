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
#include "config.h"
#include <QWidget>
#include <QGridLayout>
#include <QJsonObject>
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

    //配置信息
    Config *config;
    QJsonObject config_json;
    QString skin = ":/background/background_1.jpg";
//    QString position_ = "0";

signals:


public slots:

};

#endif // MAINWIDGET_H
