#ifndef PAGEDATA_H
#define PAGEDATA_H

#include <QObject>
#include <QVector>
#include <tuple>

class PageData : public QObject
{
    Q_OBJECT

public:
    enum class PARSE_WAY {              //格式化方式
        ALL_RADIO_TAG,                  //所有分类
        RADIO_LIST,                     //电台列表
        AUDIO_LIST,                     //声音列表
    };
    struct Adata{                       //一行数据
        bool parse{true};
        //AUDIO_LIST
        int itemIndex{ 1 };
        qint64  dataDuration;
        QString audioTime;
        QString dataId;
        QString dataTitle;

        //RADIO_LIST
        QString dataBand;
        QString dataCover;
        QString dataUserName;

        //ALL_RADIO_TAG
        QVector<std::tuple<QString,QString>> radioTag;

        QString url;
    };
    struct PageStatus {                             //标签页状态信息
        bool status             { true  };          //格式化状态
        bool prev               { false };          //上一页
        bool next               { false };          //下一页
        int current             {   1   };          //当前页
        PARSE_WAY parseWay;                         //格式化方式
        QString title;                              //标签名
        QString requestUrl;                         //请求地址
    };
public:
    explicit PageData(QObject *parent = nullptr);
    PageData(const PageData& obj);
    ~PageData();
    void setPageStatus(const PageStatus& pageStatus);           //设置标签页状态
    PageStatus getPageStatus() const;           //获取标签页状态
    void appendData(const Adata& adata);        //添加一行数据
    QVector<Adata> getData() const;             //获取数据

private:
    QVector<Adata> dataVec_;            //格式化数据
    PageStatus pageStatus_;             //标签页状态
};
#endif // PAGEDATA_H
