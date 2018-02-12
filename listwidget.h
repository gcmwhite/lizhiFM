#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class ListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ListWidget(QWidget *parent = nullptr);
    QListWidget *radioListWidget;
    QPushButton *preBtn;
    QPushButton *nextBtn;
    QLabel *pageLabel;
    QPushButton *backBtn;
    QPushButton *playListBtn;
    QVBoxLayout *mainLayout;

signals:

public slots:
};

#endif // LISTWIDGET_H
