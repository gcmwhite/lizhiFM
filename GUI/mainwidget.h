#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "lizhifmapi.h"
#include "medialistmodel.h"
#include <memory>
#include <atomic>
#include <QWidget>
#include <QResizeEvent>
#include <QHash>
#include <QThread>
#include <QMediaPlayer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    void initStyle();               //初始化样式
    void initPlayer();              //播放器初始化
    void init();
    void createRadioListWidget(const PageData& pageData);   //创建电台列表窗体
    void createRadioTagWidget(const PageData& pageData);    //创建电台分类窗体
    void createAudioListWidget(const PageData& pageData);   //创建声音列表窗体

public slots:
    void slotParseFinished(const PageData& pageData);           //根据解析数据创建窗体
    void slotcurrentTabChanged(int index);                      //当前选中tab改变
    void slotPlayItems(const QVector<QPair<QString,QString>>& items);       //播放当前列表
    void slotPlayCurrentItem(const QModelIndex& index);         //播放当前项
    void slotPlay(const QString& mediaUrl);                     //播放
    void slotDurationChanged(const qint64 duration);            //播放时长发生改变
    void slotPositionChanged(const qint64 position);            //播放位置发生改变
    void slotStateChanged(QMediaPlayer::State state);           //播放状态改变

private slots:
    void on_soundBtn_clicked();

    void on_soundSlider_valueChanged(int value);        //音量调节

    void on_playBtn_clicked();

    void on_preBtn_clicked();

    void on_nextBtn_clicked();

    void on_timeSlider_sliderReleased();

    void on_timeSlider_sliderPressed();

private:
    void clearChildren(QWidget* parent);                //清空子窗体

private:
    Ui::MainWidget *ui;
    LiZhiFmAPI* lizhiFmApi_;
    QString LabelHistoryUrl_;            //用于记录分类>电台列表按下的地址
    MediaListModel* mediaListModel_;     //播放列表
    std::shared_ptr<QMediaPlayer> player_;  //播放器
    QThread* playerThread_;              //播放器线程
    bool timeSilder_{true};              //时间进度条改变
    std::atomic_bool autoNext_{true};                //下一曲，解决播放死循环的问题
};

#endif // MAINWIDGET_H
