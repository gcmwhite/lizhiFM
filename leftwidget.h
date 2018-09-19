#ifndef LEFTWIDGET_H
#define LEFTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>

class LeftWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LeftWidget(QWidget *parent = nullptr);

public:
    QWidget *control_widget_;                           //控制界面
    QPushButton *previous_btn_;                         //上一曲
    QPushButton *next_btn_;                             //下一曲
    QPushButton *play_btn_;                             //播放按钮
    QListWidget *list_wigdet_;                           //音乐列表

signals:

public slots:
};

#endif // LEFTWIDGET_H
