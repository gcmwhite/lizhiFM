#include "radiopushbutton.h"
#include <QVBoxLayout>
#include <QPixmap>
#include <QRect>
#include <QPainter>
#include <QPainterPath>
#include <QtConcurrent>
#include <QStyleOption>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSvgRenderer>
#include <QFontMetrics>
#include <QDebug>

RadioPushButton::RadioPushButton(QWidget *parent)
    : QPushButton(parent)
{
    init();
}

void RadioPushButton::setStr(const QString &str)
{
    strLabel_->setText(elideStr(str));
    setToolTip(str);
}

void RadioPushButton::setPix(const QString &url)
{
    if (url.isEmpty()) {
        setDefaultPix();
        return ;
    }

    QNetworkAccessManager* m = new QNetworkAccessManager(this);
    connect(m,&QNetworkAccessManager::finished,this,[this](QNetworkReply* reply){
        QByteArray bytes = reply->readAll();
        int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (200 == status) {
            QPixmap pix;
            pix.loadFromData(bytes);
            if (pix.isNull()) {
                setDefaultPix();
            }else {
                RoundPixmap(pix);
            }
        }else {
            setDefaultPix();
        }
    });
    m->get(QNetworkRequest(url));
}

void RadioPushButton::enterEvent(QEvent *event)
{
    isEnter_ = true;
    strLabel_->setStyleSheet("color: #46A3FF;");
    QPushButton::enterEvent(event);
}

void RadioPushButton::leaveEvent(QEvent *event)
{
    isEnter_ = false;
    strLabel_->setStyleSheet("");
    QPushButton::leaveEvent(event);
}

void RadioPushButton::mousePressEvent(QMouseEvent *event)
{
    strLabel_->setStyleSheet("color: #0072E3;");
    QPushButton::mousePressEvent(event);
}

void RadioPushButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (isEnter_) {
        strLabel_->setStyleSheet("color: #46A3FF;");
    }else {
        strLabel_->setStyleSheet("");
    }
    QPushButton::mouseReleaseEvent(event);
}

void RadioPushButton::init()
{
    pixLabel_ = new QLabel(this);
    strLabel_ = new QLabel(this);
    pixLabel_->installEventFilter(this);
    strLabel_->installEventFilter(this);
    pixLabel_->setMinimumSize(pixW_,pixH_);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(pixLabel_,0,Qt::AlignCenter);
    mainLayout->addWidget(strLabel_,0,Qt::AlignCenter);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(10);
    setMinimumSize(pixW_ + 20 ,pixH_ + 20);

    strLabel_->setStyleSheet(styleSheet());
}

void RadioPushButton::RoundPixmap(const QPixmap &pix)
{
    QPixmap dest(pixW_,pixH_);
    dest.fill(Qt::transparent);
    QPainter p(&dest);
    p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPainterPath path;
    path.addEllipse(0,0,pixW_,pixH_);
    p.setClipPath(path);
    p.drawPixmap(0,0,pix.scaled(pixW_,pixH_,Qt::KeepAspectRatio));

    pixLabel_->setPixmap(dest);
}

void RadioPushButton::setDefaultPix()
{
    QPixmap pix(pixW_,pixH_);
    pix.fill(Qt::transparent);
    QSvgRenderer svg(QStringLiteral(":/Resources/svg/loadingFailed.svg"));
    QPainter p(&pix);
    p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    svg.render(&p);
    pixLabel_->setPixmap(pix);
}

QString RadioPushButton::elideStr(const QString &str)
{
    QFontMetrics font(strLabel_->font());
    if (font.width(str) > width()) {
        QString res;
        res = font.elidedText(str,Qt::ElideRight,width());
        return res;
    }
    return str;
}


