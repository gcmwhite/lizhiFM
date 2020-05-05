#include "pagedata.h"

PageData::PageData(QObject *parent)
    : QObject(parent)
{

}

PageData::PageData(const PageData &obj)
{
    pageStatus_ = obj.pageStatus_;
    dataVec_ = obj.dataVec_;
}

PageData::~PageData()
{

}

void PageData::setPageStatus(const PageData::PageStatus &pageStatus)
{
    pageStatus_ = pageStatus;
}

PageData::PageStatus PageData::getPageStatus() const
{
    return pageStatus_;
}

void PageData::appendData(const PageData::Adata &adata)
{
    dataVec_.append(adata);
}

QVector<PageData::Adata> PageData::getData() const
{
    return dataVec_;
}
