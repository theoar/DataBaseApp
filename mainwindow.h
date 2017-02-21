#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSqlRelationalDelegate>
#include <QSqlRelationalTableModel>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QString>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QStringList>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMap>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlRelation>
#include <QSettings>
#include <QtWebKitWidgets/QWebView>
#include <QPrinter>
#include <QPrintPreviewDialog>

#include "logindialog.h"
#include "klientdialog.h"
#include "tabwidget.h"
#include "produktydialog.h"
#include "zamowieniadialog.h"
#include "querydialog.h"
#include "outofdialog.h"
#include "detailsdialog.h"
#include "categorydialog.h"

#define PRODUKTY 1

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    struct RELATIONAL
    {
        QString Table;
        QString Name;

        bool Readonly;
        bool Deleteable;

        QList<QPair<int, QSqlRelation>> Relations;
        QList<int> Hidden;
        QStringList Headers;
    };

    enum RELATIONALMAP
    {
        PRODUCTS = 0,
        CLIENTS = 1,
        POSITION = 2,
    };

public:

    const QList<RELATIONAL> Relationals;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    Ui::MainWindow *ui;
    LoginDialog *Login;
    QSqlDatabase DataBase;
    QPrinter *Printer;
    QWebView *WebView;

    QString UserName;

    BaseDialog* getDialogByTable(const QString& Table);
    QSqlTableModel* createPozycjaModel();


    QMap<QString, TabWidget*> TabsWidgets;
    QMap<QString, QSqlQueryModel*> Models;
    int MagazynRaport = 500;

    QSettings *Settings;

    QString HtmlPrint;

private slots:

    void onCategoriesRequest(void);
    void onShippingOptionsRequest(void);
    void onClientsNamesRequest(void);
    void onDiscountRequest(double Kwota);
    void onNewZamowienie(QMap<QString, QVariant> Zamowienie, QMap<QString, QList<QVariant> > Pozycje);
    void onCompleteOrder(QVariant Key);
    void onPayOrder(QVariant Key);
    void onDetails(QMap<QString, QVariant> Map);
    void onOrderDataRequest(int PK);
    void onEdit();
    void onSuccesedQuery(void);

    void onDataRequest(void); //TO DO

    void onLoggin(QString Server, QString User, QString Password);    

    void onActionLogin(void);
    void onActionLogout(void);
    void onActionCheckCount(void);
    void onQueryAction(void);
    void onActionPrint(void);

    void onTabChanged(int Index);

signals:

    void categoriesReady(QList<QPair<QString, int>>);
    void shippingOpitonsReady(QList<QPair<QString, int>>);
    void clientsNamesReady(QList<QPair<QString, int>>);
    void discountReady(int);
    void orderDataReady(QMap<QString, QVariant> Map, QList<QStringList> List);

    void dataReady(QList<QPair<QString, int>>,
                   QList<QPair<QString, int>>,
                   QList<QPair<QString, int>>);

    void requestRefresh();


};

#endif // MAINWINDOW_H
