#ifndef MEDIALISTMODEL_H
#define MEDIALISTMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include <QPair>

class MediaListModel : public QAbstractListModel
{
public:
    explicit MediaListModel(QObject* parent = nullptr);
    void updateModel(const QVector<QPair<QString,QString>>& mediaVec);      //更新数据
    void addItem(const QPair<QString,QString>& item);                       //添加数据
    void addItems(const QVector<QPair<QString,QString>>& items);            //添加数据
    void removeItem(int index);         //删除数据
    void clear();                       //清空数据
    QString getUrl(const QModelIndex& index) const;       //获取当前项的Url
    int getCurrentPlay() const;         //获取当前播放位置
    void setCurrentPlay(int index);     //设置当前播放位置
    QVariant data(int row,int role = Qt::DisplayRole);

public:
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;

private:
    QVector<QPair<QString,QString>> mediaVec_;      //曲名 URL
    int currentPlay_{0};
};

#endif // MEDIALISTMODEL_H
