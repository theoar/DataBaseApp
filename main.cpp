#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator Tran;
    Tran.load("hurtownia1_"+QLocale().name());
    a.installTranslator(&Tran);

    MainWindow w;
    w.show();

    return a.exec();
}
