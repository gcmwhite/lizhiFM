#ifndef MUSICLISTWIDGET_H
#define MUSICLISTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTableView>
#include <QStandardItemModel>
#include <QLabel>
#include <QPushButton>
#include <QContextMenuEvent>
#include <QMenu>
#include <QJsonObject>

class MusicListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicListWidget(QWidget *parent = nullptr);
    void set_music_list_widget(const QJsonObject &);

private:
    void contextMenuEvent(QContextMenuEvent *);

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
    QMenu *menu;
    QMenu *add_play_list;
    QAction *play_current_music;
    QAction *add_play_list_not_clear;
    QAction *add_play_list_clear;
    QAction *add_play_list_selection;



signals:
    void back_btn_clicked_signal();
    void page_changed_signal(const QString &);
    void add_play_list_signal(const bool,const QJsonObject &);
    void play_current_music_signal(const QStringList &);


public slots:

};

#endif // MUSICLISTWIDGET_H
