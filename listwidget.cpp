#include "listwidget.h"

#include <QHBoxLayout>

ListWidget::ListWidget(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QVBoxLayout(this);
    radioListWidget = new QListWidget;
    preBtn = new QPushButton;
    preBtn->setText("上一页");
    pageLabel = new QLabel;
    pageLabel->setText(" 第 1 页 ");
    nextBtn = new QPushButton;
    nextBtn->setText("下一页");
    backBtn = new QPushButton;
    backBtn->setText("返回");
    playListBtn = new QPushButton;
    playListBtn->setText("播放列表");

    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    hBoxLayout->addStretch();
    hBoxLayout->addWidget(preBtn);
    hBoxLayout->addWidget(pageLabel);
    hBoxLayout->addWidget(nextBtn);
    hBoxLayout->addStretch();

    QHBoxLayout *hBoxLayout_2 = new QHBoxLayout;
    hBoxLayout_2->addWidget(backBtn);
    hBoxLayout_2->addWidget(playListBtn);
    mainLayout->addLayout(hBoxLayout_2);
    mainLayout->addWidget(radioListWidget,10);
    mainLayout->addLayout(hBoxLayout);
}
