#include "leftwidget.h"

LeftWidget::LeftWidget(QWidget *parent) : QWidget(parent)
{
    titleLabel = new QLabel;
    titleLabel->setText("荔枝FM");
    controlWidget = new QWidget;
    controlWidget->setObjectName("controlWidget");
//    controlWidget->setStyleSheet("QWidget#controlWidget{border: 1px solid #BFBFBF; border-radius: 5px;}");
    preBtn = new QPushButton;
    preBtn->setFixedSize(46,46);
    preBtn->setIcon(QIcon(":/imgs/pre.ico"));
    preBtn->setIconSize(QSize(30,30));
    preBtn->setFlat(true);
    preBtn->setToolTip("上一曲");
    pauseBtn = new QPushButton;
    pauseBtn->setFixedSize(46,46);
    pauseBtn->setIcon(QIcon(":/imgs/play.ico"));
    pauseBtn->setIconSize(QSize(40,40));
    pauseBtn->setFlat(true);
    pauseBtn->setToolTip("暂停");
    nextBtn = new QPushButton;
    nextBtn->setFixedSize(46,46);
    nextBtn->setIcon(QIcon(":/imgs/next.ico"));
    nextBtn->setIconSize(QSize(30,30));
    nextBtn->setFlat(true);
    nextBtn->setToolTip("下一曲");
    listWidget = new QListWidget;
    listWidget->addItem("列表为空！");

    mainLayout = new QVBoxLayout(this);
    controlLayout = new QHBoxLayout(controlWidget);

    mainLayout->setMargin(0);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(controlWidget);
    mainLayout->addWidget(listWidget);

    controlLayout->addWidget(preBtn);
    controlLayout->addWidget(pauseBtn);
    controlLayout->addWidget(nextBtn);
}
