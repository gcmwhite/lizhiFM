#include "mainwidget.h"
#include "QGridLayout"
#include <QTimer>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    init_ui_();
    QTimer *timer_ = new QTimer(this);
    connect(timer_,&QTimer::timeout,this,[=](){
        timer_->stop();
        init_network_();
    });
    timer_->start(50);
}

//主界面初始化
void MainWidget::init_ui_()
{
    this->resize(990,590);
    this->setWindowIcon(QIcon(":/imgs/lizhi_favicon.ico"));
    this->setWindowTitle("荔枝FM 2.0 dev");

    leftWidget = new LeftWidget;
    footWidget = new FootWidget;
    tabWidget = new TabWidget;

    hot_grid_widget = new GridBtnWidget;
    optimization_grid_widget = new GridBtnWidget;
    radioType = new RadioType;

    tabWidget->addTab(hot_grid_widget,"热门电台");
    tabWidget->addTab(optimization_grid_widget,"优选电台");
    tabWidget->addTab(radioType,"节目类型");

    mainLayout = new QGridLayout(this);

    mainLayout->setMargin(10);
    mainLayout->setSpacing(5);

    mainLayout->addWidget(leftWidget,0,0);
    mainLayout->addWidget(tabWidget,0,1);
    mainLayout->addWidget(footWidget,1,0,2,0);

}

//网络初始化
void MainWidget::init_network_()
{
    lizhiAPI = new LiZhiAPI(this);
    lizhiAPI->get_radio_type();
//    radioType->setRadioType(lizhiAPI->get_radio_type());
}
