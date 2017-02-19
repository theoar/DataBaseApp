#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator Tran;
    Tran.load(":/new/prefix2/translation_"+QLocale().name());
    a.installTranslator(&Tran);

    QTranslator Tran1;
    Tran1.load("qt_" + QLocale().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&Tran1);

    QTranslator Tran2;
    Tran2.load("qtbase_" + QLocale().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&Tran2);

    MainWindow w;
    w.show();

    return a.exec();
}
