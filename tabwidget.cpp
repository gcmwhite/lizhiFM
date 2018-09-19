#include "tabwidget.h"
#include <QPainter>
#include <QTabBar>
#include <QRect>
#include <QToolTip>
#include <QDebug>

#define size 32     //自定义图标大小

TabWidget::TabWidget(QWidget *parent) : QTabWidget(parent)
{
    const QSize PIX_SIZE(size,size);
    skin.load(":/imgs/skin.png");
    skin = skin.scaled(PIX_SIZE);
    github.load(":/imgs/github.png");
    github = github.scaled(PIX_SIZE);
    setting.load(":/imgs/setting.png");
    setting = setting.scaled(PIX_SIZE);
    donate.load(":/imgs/donate.png");
    donate = donate.scaled(PIX_SIZE);
    about.load(":/imgs/about.png");
    about = about.scaled(PIX_SIZE);

    this->setMouseTracking(true);
}

void TabWidget::paintEvent(QPaintEvent *event)         //绘制自定义按钮
{
    QPainter painter(this);
    painter.drawPixmap(width()-size*5-30,0,size,size,skin);
    painter.drawPixmap(width()-size*3-20,0,size,size,setting);
    painter.drawPixmap(width()-size*4-25,0,size,size,github);
    painter.drawPixmap(width()-size*2-15,0,size,size,donate);
    painter.drawPixmap(width()-size-5,0,size,size,about);
    QTabWidget::paintEvent(event);
}

void TabWidget::mousePressEvent(QMouseEvent *event)        //鼠标按下检测
{
    if (event->button() == Qt::LeftButton)
    {
        if (event->pos().y() >=  0 && event->pos().y() <= tabBar()->height() - 6)
        {
            if (event->pos().x() >= width() - size - 5
                    && event->pos().x() <= width() - 5)        //about按钮
            {
                qDebug() << "about按钮按下";
                emit aboutClicked();
            }
            if (event->pos().x() >= width() - size*2 - 10
                    && event->pos().x() <= width() - size - 10)       //donate按钮
            {
                qDebug() << "donate按钮按下";
                emit donateClicked();
            }
            if (event->pos().x() >= width() - size*3 - 15
                    && event->pos().x() <= width() - size*2 -15)      //setting按钮按下
            {
                qDebug() << "setting按钮按下";
                emit settingClicked();
            }
            if (event->pos().x() >= width() - size*4 - 20
                    && event->pos().x() <= width() - size*3 -20)      //github按钮按下
            {
                qDebug() << "github按钮按下";
                emit githubClicked();
            }
            if (event->pos().x() >= width() - size*5 - 25
                    && event->pos().x() <= width() - size*4 -25)      //skin按钮按下
            {
                qDebug() << "skin按钮按下";
                emit skinClicked();
            }
        }
    }
    QTabWidget::mousePressEvent(event);
}

void TabWidget::mouseMoveEvent(QMouseEvent *event)     //鼠标移动操作
{
    if (event->pos().y() >=  0 && event->pos().y() <= tabBar()->height() - 6)
    {
        int x = QCursor::pos().x();
        int y = QCursor::pos().y();
        if (event->pos().x() >= width() - size - 5
                && event->pos().x() <= width() - 5)       //about按钮
        {
            this->setCursor(Qt::PointingHandCursor);
            QToolTip::showText(QPoint(x,y),"关于");
        } else
        if (event->pos().x() >= width() - size*2 - 10
                    && event->pos().x() <= width() - size - 10)        //donate按钮
        {
            this->setCursor(Qt::PointingHandCursor);
            QToolTip::showText(QPoint(x,y),"捐赠");
        } else
        if (event->pos().x() >= width() - size*3 - 15
                    && event->pos().x() <= width() - size*2 -15)        //setting按钮
        {
            this->setCursor(Qt::PointingHandCursor);
            QToolTip::showText(QPoint(x,y),"设置");
        } else
        if (event->pos().x() >= width() - size*4 - 20
                    && event->pos().x() <= width() - size*3 -15)       //github按钮
        {
            this->setCursor(Qt::PointingHandCursor);
            QToolTip::showText(QPoint(x,y),"github");
        } else
        if (event->pos().x() >= width() - size*5 - 25
                    && event->pos().x() <= width() - size*4 -15)      //skin按钮按下
        {
            this->setCursor(Qt::PointingHandCursor);
            QToolTip::showText(QPoint(x,y),"背景");
        } else {
            this->setCursor(Qt::ArrowCursor);
        }
    } else {
        this->setCursor(Qt::ArrowCursor);
    }
    QTabWidget::mouseMoveEvent(event);
}
