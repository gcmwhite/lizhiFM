#ifndef MUSICLISTWIDGET_H
#define MUSICLISTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTableView>
#include <QStandardItemModel>
#include <QVector>

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
    int index_page = 1;

signals:

public slots:
};

#endif // MUSICLISTWIDGET_H
