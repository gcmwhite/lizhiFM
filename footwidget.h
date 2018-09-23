#ifndef FOOTWIDGET_H
#define FOOTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QPushButton>

class FootWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FootWidget(QWidget *parent = nullptr);

public:
    QLabel *current_time_label_;                           //起始时间Label
    QLabel *remaining_time_label_;                       //剩余时间Label
    QSlider *time_slider_;                               //时间进度条
    QSlider *sound_slider_;                              //音量进度条
    QPushButton *mute_btn_;                              //静音按钮

signals:

public slots:
};

#endif // FOOTWIDGET_H
