#include "audiolistitem.h"
#include "Common.h"
#include "ui_audiolistitem.h"

AudioListItem::AudioListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudioListItem)
{
    ui->setupUi(this);
    init();
}

AudioListItem::~AudioListItem()
{
    delete ui;
}

void AudioListItem::setIndex(int i)
{
    ui->indexLabel->setNum(i);
}

void AudioListItem::setTime(const qint64 &time)
{
    int min = time / 60;
    int sec = time % 60;
    ui->timeLabel->setText(QString("%1'%2''").arg(min,2,10,QLatin1Char('0'))
                           .arg(sec,2,10,QLatin1Char('0')));
    ui->timeLabel->setStyleSheet("font-family: SimSun-ExtB;");
}

void AudioListItem::setTitle(const QString &str)
{
    ui->titleLabel->setText(str);
}

void AudioListItem::setDate(const QString &str)
{
    ui->dateLabel->setText(str);
}

void AudioListItem::setUrl(const QString &url)
{
    url_ = url;
}

QString AudioListItem::getUrl() const
{
    return url_;
}

QString AudioListItem::getTitle() const
{
    return ui->titleLabel->text();
}

void AudioListItem::enterEvent(QEvent *event)
{
    ui->indexLabel->hide();
    ui->timeLabel->hide();
    ui->playBtn->show();
    ui->addBtn->show();
    ui->titleLabel->setStyleSheet(QStringLiteral("color: #46A3FF; font-weight: bold;"));
    QWidget::enterEvent(event);
}

void AudioListItem::leaveEvent(QEvent *event)
{
    ui->indexLabel->show();
    ui->timeLabel->show();
    ui->playBtn->hide();
    ui->addBtn->hide();
    ui->titleLabel->setStyleSheet("");

    QWidget::leaveEvent(event);
}

void AudioListItem::init()
{
    ui->playBtn->setVisible(false);
    ui->addBtn->setVisible(false);

    ui->playBtn->setFont(LiZhiFM::iconFont);
    ui->addBtn->setFont(LiZhiFM::iconFont);

    ui->dateLabel->setStyleSheet(QStringLiteral("font-size: 10px; color: gray;"));
}

void AudioListItem::on_playBtn_clicked()
{
    emit signalPlay(qMakePair(ui->titleLabel->text(),url_));
}

void AudioListItem::on_addBtn_clicked()
{
    emit signalAddItem(qMakePair(ui->titleLabel->text(),url_));
}
