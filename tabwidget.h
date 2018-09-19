#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>
#include <QPaintEvent>
#include <QPixmap>
#include <QMouseEvent>

class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

public:
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

public slots:
};

#endif // TABWIDGET_H
