#-------------------------------------------------
#
# Project created by QtCreator 2016-12-29T11:52:45
#
#-------------------------------------------------

QT       += core gui sql printsupport webkitwidgets
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hurtownia1
TEMPLATE = app

TRANSLATIONS += hurtownia1_pl.ts


SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    klientdialog.cpp \
    tabwidget.cpp \
    basedialog.cpp \
    produktydialog.cpp \
    productdelegate.cpp \
    zamowieniadialog.cpp \
    products.cpp \
    pozycjadialog.cpp \
    outofdialog.cpp \
    detailsdialog.cpp \
    querydialog.cpp \
    klhighlighter.cpp \
    categorydialog.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    klientdialog.h \
    tabwidget.h \
    basedialog.h \
    produktydialog.h \
    productdelegate.hpp \
    zamowieniadialog.h \
    products.h \
    pozycjadialog.h \
    outofdialog.h \
    detailsdialog.h \
    querydialog.h \
    klhighlighter.hpp \
    categorydialog.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    klientdialog.ui \
    tabwidget.ui \
    produktydialog.ui \
    zamowieniadialog.ui \
    pozycjadialog.ui \
    outofdialog.ui \
    detailsdialog.ui \
    querydialog.ui \
    categorydialog.ui

RESOURCES += \
    resources.qrc
