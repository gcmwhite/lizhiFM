#include "gridbtnwidget.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QBitmap>
#include <QPainter>

GridBtnWidget::GridBtnWidget(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QGridLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(5);
    QNetworkRequest r;
    r.setRawHeader("Accept-Language","zh-CN,zh;q=0.9");
    r.setRawHeader("Connection","keep-alive");
    r.setHeader(QNetworkRequest::UserAgentHeader,"Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/68.0.3440.84 Safari/537.36");

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
