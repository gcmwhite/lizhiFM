#ifndef MUSICLISTWIDGET_H
#define MUSICLISTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTableView>
#include <QStandardItemModel>
#include <QVector>
#include <QLabel>
#include <QPushButton>

class MusicListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicListWidget(QWidget *parent = nullptr);
    void set_music_list_widget(const QVector<QStringList> &);

public:
    QTableView *view;

private:
    QVBoxLayout *mainLayout;
    QStandardItemModel *head;
    QPushButton *back_btn;
    QLabel *title_label;
    QPushButton *previous_btn;
    QPushButton *next_btn;
    QLabel *page_label;


signals:
    void back_btn_clicked_signal();
    void page_changed_signal(const QString &);


public slots:
};

#endif // MUSICLISTWIDGET_H
