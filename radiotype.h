#ifndef RADIOTYPE_H
#define RADIOTYPE_H

#include <QWidget>
#include <QGridLayout>

class RadioType : public QWidget
{
    Q_OBJECT
public:
    explicit RadioType(QWidget *parent = nullptr);
    void setRadioType(const QStringList &);

private:
    QGridLayout *mainLayout;

signals:

public slots:
};

#endif // RADIOTYPE_H
