#ifndef COMMON_H
#define COMMON_H

#include <QDebug>

//日志-打印信息
#define LOG_PRINT(str) (qDebug().noquote() << (str) <<  __FILE__ << __FUNCTION__ << __LINE__)
#ifdef QT_DEBUG
#define aLog() LOG_PRINT(QStringLiteral("\033[0m\033[0m!!Log>> "))
#define aWarning() LOG_PRINT(QStringLiteral("\033[0m\033[33m!!Warning>> "))
#define aError() LOG_PRINT(QStringLiteral("\033[0m\033[35m!!Error>> "))
#define aFatal() LOG_PRINT(QStringLiteral("\033[0m\033[31m!!Fatal>> "))
#define aSucess() LOG_PRINT(QStringLiteral("\033[0m\033[32m!!Sucess>> "))

#else
#define aLog() LOG_PRINT(QStringLiteral("\033[0m\033[0m!!Log>> "))
#define aWarning() LOG_PRINT(QStringLiteral("\033[0m\033[33m!!Warning>> "))
#define aError() LOG_PRINT(QStringLiteral("\033[0m\033[35m!!Error>> "))
#define aFatal() LOG_PRINT(QStringLiteral("\033[0m\033[31m!!Fatal>> "))
#define aSucess() LOG_PRINT(QStringLiteral("\033[0m\033[32m!!Sucess>> "))
#endif

//默认配置保存位置程序路径下.config文件夹
#define CONFIG_PATH QStringLiteral("./.config")
//请求头部
#define REQUEST_HEADEAR_FILE QStringLiteral("request-header.json")

namespace LiZhiFM {
    extern QFont iconFont;       //符号字体【不可将其设置为全局字体】
}

#endif // COMMON_H
