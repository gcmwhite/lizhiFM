#-------------------------------------------------
#
# Project created by QtCreator 2018-01-27T20:36:36
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lizhiFM_7
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwidget.cpp \
    leftwidget.cpp \
    footwidget.cpp \
    listwidget.cpp \
    lizhifmhttp.cpp \
    mtabwidget.cpp \
    aboutwidget.cpp \
    donatewidget.cpp \
    skinwidget.cpp

HEADERS += \
        mainwidget.h \
    leftwidget.h \
    footwidget.h \
    listwidget.h \
    lizhifmhttp.h \
    mtabwidget.h \
    aboutwidget.h \
    donatewidget.h \
    skinwidget.h

RESOURCES += \
    imgs.qrc

DISTFILES +=
