#include "audiolistwidget.h"
#include "ui_audiolistwidget.h"
#include "Common.h"

#include <QListWidgetItem>

AudioListWidget::AudioListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudioListWidget)
{
    ui->setupUi(this);
    init();
}

AudioListWidget::~AudioListWidget()
{
    delete ui;
}

//设置当前页
void AudioListWidget::setCurrentPage(int current)
{
    ui->pageLabel->setText(QString("第 %1 页").arg(current));
}

//设置上一页可用
void AudioListWidget::setPrevBtnEnable(bool enable)
{
    ui->precvBtn->setEnabled(enable);
}

//设置下一页可用
void AudioListWidget::setNextBtnEnable(bool enable)
{
    ui->nextBtn->setEnabled(enable);
}

void AudioListWidget::setTitleLabel(const QString &str)
{
    ui->titleLabel->setText(str);
}

void AudioListWidget::init()
{
    ui->backBtn->setFont(LiZhiFM::iconFont);
    ui->precvBtn->setFont(LiZhiFM::iconFont);
    ui->nextBtn->setFont(LiZhiFM::iconFont);
    ui->playAllBtn->setFont(LiZhiFM::iconFont);
    ui->addAllBtn->setFont(LiZhiFM::iconFont);

    connect(ui->backBtn,&QPushButton::clicked,this,&AudioListWidget::signalBackBtnClicked);
    connect(ui->precvBtn,&QPushButton::clicked,this,&AudioListWidget::signalPrecvBtnClicked);
    connect(ui->nextBtn,&QPushButton::clicked,this,&AudioListWidget::signalNextBtnClicked);
}

//添加到列表
void AudioListWidget::addToList(const QVector<AudioListItem *> items)
{
    for (const auto item : items) {
        ui->listWidget->setItemWidget(new QListWidgetItem(ui->listWidget),item);
        connect(item,&AudioListItem::signalPlay,this,&AudioListWidget::signalPlay);
        connect(item,&AudioListItem::signalAddItem,this,&AudioListWidget::signalAddItem);
    }
}

//获取列表个数
int AudioListWidget::getItemsSize() const
{
    return itemVec_.size();
}

//添加列表
void AudioListWidget::addItem(AudioListItem *item)
{
    itemVec_.append(item);
    ui->listWidget->setItemWidget(new QListWidgetItem(ui->listWidget),item);
}

void AudioListWidget::addItems(const QVector<AudioListItem *> &items)
{
    itemVec_ += items;
    addToList(items);
}

//更新列表项
void AudioListWidget::updateItems(const QVector<AudioListItem *>& items)
{
    itemVec_ = items;
    addToList(itemVec_);
}

//获取列表项
QVector<AudioListItem *> AudioListWidget::getItems() const
{
    return itemVec_;
}

AudioListItem *AudioListWidget::getItem(int index) const
{
    return itemVec_.at(index);
}

//设置返回按钮属性
void AudioListWidget::setBackBtnPropertyVar(const char *name, const QVariant &var)
{
    ui->backBtn->setProperty(name,var);
}

//设置上一页按钮属性
void AudioListWidget::setPrecvBtnPropertyVar(const char *name, const QVariant &var)
{
    ui->precvBtn->setProperty(name,var);
}

//设置下一页按钮属性
void AudioListWidget::setNextBtnPropertyVar(const char *name, const QVariant &var)
{
    ui->nextBtn->setProperty(name,var);
}

//获取返回按钮属性
QVariant AudioListWidget::getBackPropertyVar(const char *name)
{
    return ui->backBtn->property(name);
}

//获取上一页按钮属性
QVariant AudioListWidget::getPrevPropertyVar(const char *name)
{
    return ui->precvBtn->property(name);
}

//获取下一页按钮属性
QVariant AudioListWidget::getNextPropertyVar(const char *name)
{
    return ui->nextBtn->property(name);
}

void AudioListWidget::on_addAllBtn_clicked()
{
    QVector<QPair<QString,QString>> items;
    for (const auto& item : itemVec_) {
        items.append(qMakePair(item->getTitle(),item->getUrl()));
    }
    emit signalAddItems(items);
}

void AudioListWidget::on_playAllBtn_clicked()
{
    QVector<QPair<QString,QString>> items;
    for (const auto& item : itemVec_) {
        items.append(qMakePair(item->getTitle(),item->getUrl()));
    }
    emit signalPlayItems(items);
}
