#ifndef GRIDBTNWIDGET_H
#define GRIDBTNWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QGridLayout>
#include <QVector>
#include <QNetworkRequest>

class GridBtnWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GridBtnWidget(QWidget *parent = nullptr);
    QPixmap pixmapToRound(const QPixmap &,int);
    void set_grid_btn_widget(const QVector<QStringList> &);

private:
    QGridLayout *mainLayout;
    QNetworkRequest request;


signals:
    void grid_btn_signal(const QString &);

public slots:
};

#endif // GRIDBTNWIDGET_H
