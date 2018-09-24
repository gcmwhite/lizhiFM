#ifndef GRIDBTNWIDGET_H
#define GRIDBTNWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QGridLayout>
#include <QJsonObject>
#include <QNetworkRequest>

class GridBtnWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GridBtnWidget(QWidget *parent = nullptr);
    QPixmap pixmapToRound(const QPixmap &,int);
    void set_grid_btn_widget(const QJsonObject &);
    void clear_children();

private:
    QGridLayout *mainLayout;
    QNetworkRequest request;


signals:
    void grid_btn_signal(const QString &);

public slots:
};

#endif // GRIDBTNWIDGET_H
