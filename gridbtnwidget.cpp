#include "gridbtnwidget.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QBitmap>
#include <QPainter>
#include <QVBoxLayout>
#include <QObjectList>
#include <QDebug>

GridBtnWidget::GridBtnWidget(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QGridLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(20);
    request.setRawHeader("Accept-Language","zh-CN,zh;q=0.9");
    request.setRawHeader("Connection","keep-alive");
    request.setHeader(QNetworkRequest::UserAgentHeader,"Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/68.0.3440.84 Safari/537.36");

}

//剪切图片为圆形
QPixmap GridBtnWidget::pixmapToRound(const QPixmap &pix, int radius)
{
    if (pix.isNull())
        return QPixmap();
    QSize size(2*radius,2*radius);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0,0,size.width(),size.height(),Qt::white);
    painter.setBrush(QColor(0,0,0));
    painter.drawRoundedRect(0,0,size.width(),size.height(),99,99);
    painter.end();
    QPixmap image = pix.scaled(size);
    image.setMask(mask);
    return image;
}

//按钮设置
void GridBtnWidget::set_grid_btn_widget(const QVector<QStringList> &vec_list)
{
    int size = vec_list.size();
    for (int i = 0;i < size;i++)
    {
        QStringList list = vec_list.at(i);
        if (list.isEmpty())
            continue;
        QPushButton *btn = new QPushButton;
        QLabel *img_label = new QLabel;
        QLabel *text_label = new QLabel;
        QVBoxLayout *vboxLayout = new QVBoxLayout(btn);
        vboxLayout->setMargin(0);
        vboxLayout->addWidget(img_label);
        vboxLayout->addWidget(text_label);

        btn->setFlat(true);
        btn->setMinimumSize(100,100);
        QString tool_tip = list.at(2) + "\n" + list.at(3);
        btn->setToolTip(tool_tip);
        btn->setCursor(Qt::PointingHandCursor);
        img_label->setAlignment(Qt::AlignCenter);
        text_label->setAlignment(Qt::AlignCenter);
        text_label->setText(QString(list.at(1)));

        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        QString url = list.at(4);
        request.setUrl(QUrl(url));
        connect(manager,&QNetworkAccessManager::finished,[=](QNetworkReply *reply){
            QPixmap pix;
            pix.loadFromData(reply->readAll());
            pix = pix.scaled(750,750,Qt::KeepAspectRatio);
            pix = pixmapToRound(pix,pix.width()/22);
            img_label->setPixmap(pix);
            reply->deleteLater();
        });
        manager->get(request);
        mainLayout->addWidget(btn,i/5,i%5);

        connect(btn,&QPushButton::clicked,[=](){
            grid_btn_signal(QString(list.at(0)));
            qDebug() << list.at(0);
        });
    }

}

//删除子控件
void GridBtnWidget::clear_children()
{
    while (mainLayout->count()) {
        QWidget *p = mainLayout->itemAt(0)->widget();
        p->setParent(nullptr);
        mainLayout->removeWidget(p);
        delete p;
    }
}
