#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QJsonDocument>

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = nullptr);
    void create_config(const QJsonDocument &);
    QJsonDocument read_config();
    void create_list(const QJsonDocument &);
    void read_list();

signals:

public slots:
};

#endif // CONFIG_H
