#include "footwidget.h"

FootWidget::FootWidget(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QHBoxLayout(this);
    timeLabel = new QLabel;
    timeLabel->setText("00:00");
    remainingTimeLabel = new QLabel;
    remainingTimeLabel->setText("00:00");
    timeSlider = new QSlider;
    timeSlider->setOrientation(Qt::Horizontal);
    mainLayout->addWidget(timeLabel);
    mainLayout->addWidget(timeSlider);
    mainLayout->addWidget(remainingTimeLabel);

    mainLayout->setMargin(0);

}
