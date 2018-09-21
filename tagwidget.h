#ifndef TAGWIDGET_H
#define TAGWIDGET_H

#include "gridbtnwidget.h"
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class TagWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TagWidget(QWidget *parent = nullptr);
    void set_tag_widget(const QVector<QStringList> &);

public:
    GridBtnWidget *gridBtnWidget;

private:
    QPushButton *previous_page_btn;
    QPushButton *next_page_btn;
    QPushButton *back_btn;
    QLabel *index_label;
    QLabel *title_label;

    QVBoxLayout *mainLayout;

    int index = 1;

signals:
    void back_btn_clicked_signal();

public slots:
};

#endif // TAGWIDGET_H
