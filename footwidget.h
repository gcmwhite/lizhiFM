#ifndef FOOTWIDGET_H
#define FOOTWIDGET_H

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>

class FootWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FootWidget(QWidget *parent = nullptr);

    QSlider *timeSlider;
    QLabel *timeLabel;
    QLabel *remainingTimeLabel;
    QHBoxLayout *mainLayout;

signals:

public slots:
};

#endif // FOOTWIDGET_H
