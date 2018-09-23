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
#include <QStackedLayout>

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);
    void init_ui_();
    void init_network_();
    void init_a_player_();

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

signals:


public slots:

};

#endif // MAINWIDGET_H
