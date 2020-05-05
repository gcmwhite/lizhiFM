#ifndef VERSION_H
#define VERSION_H

#include <QObject>

#define APPNAME QStringLiteral("荔枝Fm")
#define VERSION QStringLiteral("3.0.0")

class Version
{
public:
    static quint32 getVersion();
};

#endif // VERSION_H
