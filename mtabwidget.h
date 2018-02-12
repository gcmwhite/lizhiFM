#ifndef MTABWIDGET_H
#define MTABWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QPaintEvent>
#include <QPixmap>
#include <QMouseEvent>

class MTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit MTabWidget(QWidget *parent=nullptr);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

private:
    QPixmap skin;
    QPixmap github;
    QPixmap setting;
    QPixmap donate;
    QPixmap about;

signals:
    void skinClicked();
    void githubClicked();
    void settingClicked();
    void donateClicked();
    void aboutClicked();
};

#endif // MTABWIDGET_H
