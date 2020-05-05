#ifndef AUDIOLISTITEM_H
#define AUDIOLISTITEM_H

#include <QWidget>
#include <QEvent>

namespace Ui {
class AudioListItem;
}

class AudioListItem : public QWidget
{
    Q_OBJECT

public:
    explicit AudioListItem(QWidget *parent = nullptr);
    ~AudioListItem();
    void setIndex(int i);
    void setTime(const qint64& time);
    void setTitle(const QString& str);
    void setDate(const QString& str);
    void setUrl(const QString& url);
    QString getUrl() const;
    QString getTitle() const;

protected:
    virtual void enterEvent(QEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;

private:
    void init();

signals:
    void signalPlay(const QPair<QString,QString>& item);
    void signalAddItem(const QPair<QString,QString>& item);

private slots:
    void on_playBtn_clicked();

    void on_addBtn_clicked();

private:
    Ui::AudioListItem *ui;
    QString url_;
    bool isEntered_{false};
};

#endif // AUDIOLISTITEM_H
