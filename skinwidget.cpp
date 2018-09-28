#include "skinwidget.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QIcon>
#include <QFileDialog>

#define width 230
#define height 150

SkinWidget::SkinWidget(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("皮肤");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QGridLayout *gridLayout = new QGridLayout;

    QSize iconSize(width,height);
    for (int i = 0;i < 8;i++)
    {
        QString backgroundFile = QString(":/background/background_%1.jpg").arg(i+1);
        QPushButton *button = new QPushButton;
        button->setIcon(QIcon(backgroundFile));
        button->setIconSize(iconSize);
        button->setFlat(true);
        connect(button,&QPushButton::clicked,[=](){
            emit backgroundSiganl(backgroundFile);
        });
        gridLayout->addWidget(button,i/4,i%4);
    }

    QPushButton *openFileBtn = new QPushButton;

    connect(openFileBtn,&QPushButton::clicked,[=](){
        QString fileName = QFileDialog::getOpenFileName(this,"选择本地图片",
                                                        "/home/","Image Files(*.jpg *.jpeg *.png);;"
                                                            "JPEG Files(*.jpg *jpeg);;"
                                                            "PNG Files(*.png)");
        if (!fileName.isEmpty())
        {
            emit backgroundSiganl(fileName);
        }
    });

    openFileBtn->setText("打开本地图片");
    mainLayout->addLayout(gridLayout);
    mainLayout->addWidget(openFileBtn,0,Qt::AlignRight);
}
