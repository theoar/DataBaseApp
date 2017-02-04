#-------------------------------------------------
#
# Project created by QtCreator 2016-12-29T11:52:45
#
#-------------------------------------------------

QT       += core gui sql
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hurtownia1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    klientdialog.cpp \
    tabwidget.cpp \
    basedialog.cpp \
    produktydialog.cpp \
    productdelegate.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    klientdialog.h \
    tabwidget.h \
    basedialog.h \
    produktydialog.h \
    productdelegate.hpp

FORMS    += mainwindow.ui \
    logindialog.ui \
    klientdialog.ui \
    tabwidget.ui \
    produktydialog.ui

RESOURCES += \
    resources.qrc
