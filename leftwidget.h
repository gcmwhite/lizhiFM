#ifndef LEFTWIDGET_H
#define LEFTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

class LeftWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LeftWidget(QWidget *parent = nullptr);

    QLabel *titleLabel;
    QWidget *controlWidget;
    QPushButton *preBtn;
    QPushButton *pauseBtn;
    QPushButton *nextBtn;
    QListWidget *listWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *controlLayout;

signals:

public slots:
};

#endif // LEFTWIDGET_H
