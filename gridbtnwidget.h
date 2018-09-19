#ifndef GRIDBTNWIDGET_H
#define GRIDBTNWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QGridLayout>

class GridBtnWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GridBtnWidget(QWidget *parent = nullptr);
    QPixmap pixmapToRound(const QPixmap &,int);

private:
    QGridLayout *mainLayout;

signals:

public slots:
};

#endif // GRIDBTNWIDGET_H
