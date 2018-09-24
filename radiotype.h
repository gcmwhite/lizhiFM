#ifndef RADIOTYPE_H
#define RADIOTYPE_H

#include <QWidget>
#include <QGridLayout>
#include <QJsonObject>

class RadioType : public QWidget
{
    Q_OBJECT
public:
    explicit RadioType(QWidget *parent = nullptr);
    void setRadioType(const QJsonObject &);

private:
    QGridLayout *mainLayout;

signals:
    void radio_type_btn_clicked_signal(const QString &);

public slots:
};

#endif // RADIOTYPE_H
