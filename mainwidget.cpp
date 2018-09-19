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
    timer_->start(10);
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
    stackedLayout = new QStackedLayout;
    stackedLayout->setMargin(0);
    stackedLayout->addWidget(tabWidget);
    stackedLayout->setCurrentWidget(tabWidget);

    mainLayout->setMargin(10);
    mainLayout->setSpacing(5);

    mainLayout->addWidget(leftWidget,0,0);
    mainLayout->addLayout(stackedLayout,0,1);
    mainLayout->addWidget(footWidget,1,0,2,0);

}

//网络初始化
void MainWidget::init_network_()
{
    lizhiAPI = new LiZhiAPI(this);
    radioType->setRadioType(lizhiAPI->get_radio_type());                                        //获取节目类别
    hot_grid_widget->set_grid_btn_widget(lizhiAPI->get_hot_grid());                             //获取热门电台
    optimization_grid_widget->set_grid_btn_widget(lizhiAPI->get_optimization_grid());           //获取优选电台

    musicListWidget = new MusicListWidget;
    stackedLayout->addWidget(musicListWidget);
    //绑定hot_grid_widget按钮
    connect(hot_grid_widget,&GridBtnWidget::grid_btn_signal,[=](const QString &path){
        musicListWidget->set_music_list_widget(lizhiAPI->get_music_list(path));
        stackedLayout->setCurrentWidget(musicListWidget);
    });
}
