#include "medialistmodel.h"

MediaListModel::MediaListModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

void MediaListModel::updateModel(const QVector<QPair<QString, QString> > &mediaVec)
{
    beginResetModel();
    mediaVec_ = mediaVec;
    endResetModel();
}

void MediaListModel::addItem(const QPair<QString, QString> &item)
{
    int size = mediaVec_.size();
    beginInsertRows(QModelIndex(),size,size + 1);
    mediaVec_.append(item);
    endInsertRows();
}

void MediaListModel::addItems(const QVector<QPair<QString, QString> > &items)
{
    int size = mediaVec_.size();
    beginInsertRows(QModelIndex(),size,size + items.size());
    mediaVec_ += items;
    endInsertRows();
}

void MediaListModel::removeItem(int index)
{
    beginRemoveRows(QModelIndex(),index,index + 1);
    mediaVec_.remove(index);
    endRemoveRows();
}

void MediaListModel::clear()
{
    beginResetModel();
    mediaVec_.clear();
    endResetModel();
    currentPlay_ = 0;
}

QString MediaListModel::getUrl(const QModelIndex &index) const
{
    return mediaVec_[index.row()].second;
}

int MediaListModel::getCurrentPlay() const
{
    return currentPlay_;
}

void MediaListModel::setCurrentPlay(int index)
{
    currentPlay_ = index;
}

QVariant MediaListModel::data(int row, int role)
{
    return data(index(row),role);
}

bool MediaListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
    case Qt::ToolTipRole:
    case Qt::UserRole: {
        mediaVec_[index.row()] = value.value<QPair<QString,QString>>();
        return true;
    }
    default: break ;
    }
    return QAbstractListModel::setData(index,value,role);
}

QVariant MediaListModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
    case Qt::ToolTipRole: {
        return mediaVec_[index.row()].first;
    }
    case Qt::UserRole: {
        return QVariant::fromValue(mediaVec_[index.row()]);
    }
    default: break ;
    }
//    return QAbstractListModel::data(index,role);
    return QVariant();
}

int MediaListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mediaVec_.size();
}
