#ifndef AUDIOLISTWIDGET_H
#define AUDIOLISTWIDGET_H

#include "audiolistitem.h"
#include <QWidget>
#include <QVector>

namespace Ui {
class AudioListWidget;
}

class AudioListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AudioListWidget(QWidget *parent = nullptr);
    ~AudioListWidget();
    void setTitleLabel(const QString& str);      //设置标题
    void setCurrentPage(int current);            //设置当前页
    void setPrevBtnEnable(bool enable);            //设置上一页可用
    void setNextBtnEnable(bool enable);            //设置下一页可用
    int getItemsSize() const;               //获取列表个数
    void addItem(AudioListItem *item);      //添加列表
    void addItems(const QVector<AudioListItem *>& items);       //添加列表
    void updateItems(const QVector<AudioListItem *> &items);    //更新列表项
    QVector<AudioListItem *> getItems() const;                  //获取列表项
    AudioListItem* getItem(int index) const;                    //获取列表项
    void setBackBtnPropertyVar(const char* name,const QVariant& var);          //设置返回按钮属性
    void setPrecvBtnPropertyVar(const char* name,const QVariant& var);         //设置上一页按钮属性
    void setNextBtnPropertyVar(const char* name,const QVariant& var);          //设置下一页按钮属性
    QVariant getBackPropertyVar(const char* name);                             //获取返回按钮属性
    QVariant getPrevPropertyVar(const char* name);                             //获取上一页按钮属性
    QVariant getNextPropertyVar(const char* name);                             //获取下一页按钮属性

private:
    void init();
    inline void addToList(const QVector<AudioListItem *> items);      //添加到布局

signals:
    void signalBackBtnClicked();
    void signalPrecvBtnClicked();
    void signalNextBtnClicked();
    void signalAddItems(const QVector<QPair<QString,QString>>& items);
    void signalPlayItems(const QVector<QPair<QString,QString>>& items);
    void signalPlay(const QPair<QString,QString>& item);
    void signalAddItem(const QPair<QString,QString>& item);

private slots:
    void on_addAllBtn_clicked();        //添加到播放列表

    void on_playAllBtn_clicked();       //播放当前列表

private:
    Ui::AudioListWidget *ui;
    QVector<AudioListItem *> itemVec_;      //列表项
};

#endif // AUDIOLISTWIDGET_H
