#ifndef TAGWIDGET_H
#define TAGWIDGET_H

#include "gridbtnwidget.h"
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QJsonObject>
#include <QVBoxLayout>

class TagWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TagWidget(QWidget *parent = nullptr);
    void set_tag_widget(const QJsonObject &);

public:
    GridBtnWidget *gridBtnWidget;

private:
    QPushButton *previous_page_btn;
    QPushButton *next_page_btn;
    QPushButton *back_btn;
    QLabel *index_label;
    QLabel *title_label;

    QVBoxLayout *mainLayout;

signals:
    void back_btn_clicked_signal();
    void tag_page_changed(const QString &);

public slots:
};

#endif // TAGWIDGET_H
