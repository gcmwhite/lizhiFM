#ifndef SKINWIDGET_H
#define SKINWIDGET_H

#include <QWidget>

class SkinWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SkinWidget(QWidget *parent = nullptr);
//    QPushButton *skin_1;
//    QPushButton *skin_2;
//    QPushButton *skin_3;

signals:
    void backgroundSiganl(const QString &);

public slots:
};

#endif // SKINWIDGET_H
