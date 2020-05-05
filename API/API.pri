HEADERS += \
    $$PWD/apiwork.h \
    $$PWD/lizhifmapi.h \
    $$PWD/networkcookiejar.h \
    $$PWD/pagedata.h

SOURCES += \
    $$PWD/apiwork.cpp \
    $$PWD/lizhifmapi.cpp \
    $$PWD/networkcookiejar.cpp \
    $$PWD/pagedata.cpp


include($$PWD/../GumboQuery/GumboQuery.pri)

INCLUDEPATH += \
    $$PWD/../GumboQuery \
