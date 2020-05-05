#include "version.h"
#include <QDebug>

quint32 Version::getVersion()
{
    quint32 version{0};
    QStringList list = VERSION.split('.');
    for (const auto x : list) {
        qDebug() << "x:" << x;
        version |= static_cast<quint8>(x.toUInt());
        version <<= 8;
    }
    return version;
}
