#include "radiotype.h"
#include <QPushButton>

RadioType::RadioType(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QGridLayout(this);
}

void RadioType::setRadioType(const QStringList &list)
{
    int size = list.size();
    for (int i = 0;i < size;i++)
    {
        QPushButton *btn = new QPushButton(QString(list.at(i)));
        mainLayout->addWidget(btn,i/5,i%5);
    }
}
