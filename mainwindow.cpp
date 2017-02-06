#include "mainwindow.h"
#include "ui_mainwindow.h"

const QList<MainWindow::RELATIONAL> MainWindow::Relationals =
{
    {
        "produkty",
        tr("Products"),
        false, true,
        {
            { 1, QSqlRelation("kategorie", "IDKategorii", "Kategoria") }
        },
        {
            0
        },
        {
            tr("ID"),
            tr("Category"),
            tr("Name"),
            tr("Price"),
            tr("Count"),
            tr("Capacity")
        }
    },
    {
        "klienci",
        tr("Clients"),
        false, true,
        {},
        {
            0
        },
        {
            tr("ID"),
            tr("Name"),
            tr("Phone"),
            tr("Mail"),
            tr("City"),
            tr("Street"),
            tr("Home")
        }
    }
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->MainTab->setVisible(false);

    DataBase = QSqlDatabase::addDatabase("QMYSQL");
}

MainWindow::~MainWindow()
{
    delete ui;
}

BaseDialog* MainWindow::getDialogByTable(const QString& Table, QAbstractTableModel *Model)
{
    if (Table == "klienci")
        return new KlientDialog();

    if (Table == "produkty")
    {
        auto Dialog = new ProduktyDialog();
        connect(Dialog, &ProduktyDialog::categoriesRequest, this, &MainWindow::onCategoriesRequest);
        connect(this, &MainWindow::categoriesReady, Dialog, &ProduktyDialog::onNewList);

        return Dialog;
    }

    if( Table == "zamowienia" )
    {
        auto Dialog = new ZamowieniaDialog(dynamic_cast<QSqlRelationalTableModel*>(Model));
        connect(Dialog, &ZamowieniaDialog::shippingOptionRequest, this, &MainWindow::onShippingOptionsRequest);
        connect(Dialog, &ZamowieniaDialog::clientsNameRequest, this, &MainWindow::onClientsNamesRequest);
        connect(Dialog, &ZamowieniaDialog::discountRequest, this, &MainWindow::onDiscountRequest);
        connect(Dialog, &ZamowieniaDialog::accepted,this, &MainWindow::onNewZamowienie);

        connect(this, &MainWindow::shippingOpitonsReady, Dialog, &ZamowieniaDialog::onNewShippingOptions);
        connect(this, &MainWindow::clientsNamesReady, Dialog, &ZamowieniaDialog::onNewClientsNames);
        connect(this, &MainWindow::discountReady, Dialog, &ZamowieniaDialog::onNewDiscount);


        return Dialog;
    }

    return nullptr;
}

void MainWindow::onCategoriesRequest()
{
    QList<QPair<QString, int>> List;
    QSqlQuery Query(DataBase);

    Query.prepare("SELECT IDKategorii, Kategoria FROM kategorie ORDER BY Kategoria");

    if( Query.exec() )
        while(Query.next())
            List.append(qMakePair(Query.value(1).toString(), Query.value(0).toInt()));

    emit categoriesReady(List);
}

void MainWindow::onShippingOptionsRequest()
{
    QList<QPair<QString, int>> List;
    QSqlQuery Query(DataBase);

    Query.prepare("SELECT IDWysylki, SposobWysylki FROM wysylka ORDER BY SposobWysylki");

    if( Query.exec() )
        while( Query.next() )
            List.append(qMakePair(Query.value(1).toString(), Query.value(0).toInt()));

    emit shippingOpitonsReady(List);
}

void MainWindow::onClientsNamesRequest()
{
    QList<QPair<QString, int>> List;
    QSqlQuery Query(DataBase);

    Query.prepare("SELECT IDKlienta, Nazwa FROM klienci ORDER BY Nazwa");

    if( Query.exec() )
        while( Query.next() )
            List.append(qMakePair(Query.value(1).toString(), Query.value(0).toInt()));

    emit clientsNamesReady(List);
}

void MainWindow::onDiscountRequest(double Kwota)
{
    int Rabat = 0;
    QSqlQuery Query(DataBase);

    Query.prepare("SELECT Rabat FROM rabaty WHERE :Kwota>=MinKwota ORDER BY MinKwota DESC LIMIT 1");
    Query.bindValue(":Kwota", Kwota);
    if( Query.exec() )
        while( Query.next() )
            Rabat = Query.value(0).toInt();

    emit discountReady(Rabat);
}

void MainWindow::onNewZamowienie(QList<QVariant> Zamowienie, QList<QList<QVariant> > Pozycje)
{
    QSqlQuery ZamowienieQuery(DataBase);

    ZamowienieQuery.prepare("INSERT INTO zamowienia(IDKlienta, DataZamowienia, IDWysylki, Rabat) VALUES(:klient, now(), :wysylka, :rabat)");
    ZamowienieQuery.bindValue(":klient", Zamowienie.at(ZamowienieData::IDKlienta));
    ZamowienieQuery.bindValue(":wysylka", Zamowienie.at(ZamowienieData::IDWysylki));
    ZamowienieQuery.bindValue(":rabat", Zamowienie.at(ZamowienieData::Rabat));

    if( ZamowienieQuery.exec() )
    {
        QVariant NewId = ZamowienieQuery.lastInsertId();

        QSqlQuery PozycjaQuery(DataBase);
        PozycjaQuery.prepare("INSERT INTO pozycja(IDZamowienia, IDProduktu, Ilosc, KosztPozycji) VALUES(:zamowienie, :produkt, :ilosc, :koszt)");
        PozycjaQuery.bindValue(":zamowienie", NewId );
        for(auto & Element : Pozycje)
        {
            PozycjaQuery.bindValue(":produkt", Element.at(PozycjeData::IDProduktu));
            PozycjaQuery.bindValue(":ilosc", Element.at(PozycjeData::Ilosc));
            PozycjaQuery.bindValue(":koszt", Element.at(PozycjeData::KosztPozycji));

            qDebug() << PozycjaQuery.lastQuery();
            if( !PozycjaQuery.exec() )
                qDebug() << PozycjaQuery.lastError();
        }
    }
}


void MainWindow::onDataRequest()
{
    // TODO implement queries
}

void MainWindow::onLoggin(QString Server, QString User, QString Password)
{
    DataBase.setDatabaseName("hurtownia");
    DataBase.setUserName(User);
    DataBase.setPassword(Password);
    DataBase.setHostName(Server);

    if (DataBase.open())
        ui->LoginAction->setEnabled(false);
    else
    {
        QMessageBox::critical(this, tr("Error"), DataBase.lastError().text());
        return;
    }

    QSqlRelationalTableModel *DoubleModel;
    // tabele relacyjne
    for (const auto& Table : Relationals)
    {
        QSqlRelationalTableModel* M = new QSqlRelationalTableModel(this, DataBase);

        M->setTable(Table.Table);

        for (const auto& Relation : Table.Relations)
        {
            M->setRelation(Relation.first, Relation.second);
        }

        M->select();

        for (int i = 0; i < M->columnCount() && i < Table.Headers.size(); ++i)
        {
            M->setHeaderData(i, Qt::Horizontal, Table.Headers[i]);
        }

        TabWidget* W = new TabWidget(getDialogByTable(Table.Table, M), M, this);

        W->setDelegate(new QSqlRelationalDelegate(W));
        W->setReadonly(Table.Readonly);
        W->setDeleteable(Table.Deleteable);
        W->hideColumns(Table.Hidden);

        ui->MainTab->addTab(W, Table.Name);

        if(Table.Table=="produkty")
            DoubleModel = M;
    }

    // tabele specjalne
    {
        static const QStringList Headers =
        {
            // TODO headers
        };

        QSqlQueryModel* M = new QSqlQueryModel(this);

        M->setQuery("SELECT Z.IDZamowienia, K.Nazwa, Z.DataZamowienia, Z.DataRealizacji,Z.CzyOplacone, Z.Rabat, W.SposobWysylki, Pr.NazwaProduktu, Pr.Pojemnosc, P.Ilosc, P.KosztPozycji"
                    " FROM zamowienia Z"
                    " INNER JOIN klienci K ON K.IDKlienta=Z.IDKlienta"
                    " INNER JOIN wysylka W ON Z.IDWysylki=W.IDWysylki"
                    " INNER JOIN pozycja P ON P.IDZamowienia=Z.IDZamowienia"
                    " INNER JOIN produkty Pr ON Pr.IDProduktu=P.IDProduktu", DataBase);

        qDebug() << M->rowCount();

        for (int i = 0; i < M->columnCount() && i < Headers.size(); ++i)
        {
            M->setHeaderData(i, Qt::Horizontal, Headers[i]);
        }

        TabWidget* W = new TabWidget(getDialogByTable("zamowienia", DoubleModel), M, this);
        QPushButton* Complete = new QPushButton(tr("Complete order"), W);
        QPushButton* Pay = new QPushButton(tr("Pay"), W);

        // TODO connect pay and complete

        W->setReadonly(true);
        W->setDeleteable(false);

        W->insertWidget(Complete);
        W->insertWidget(Pay);

        ui->MainTab->addTab(W, tr("Orders"));
    }

    ui->StatusLable->hide();
    ui->MainTab->show();
}

void MainWindow::onActionLogin()
{
    LoginDialog *Login = new LoginDialog(this);

    connect(Login, &LoginDialog::accepted, this, &MainWindow::onLoggin);

    connect(Login, &LoginDialog::accepted, Login, &LoginDialog::deleteLater);
    connect(Login, &LoginDialog::rejected, Login, &LoginDialog::deleteLater);

    Login->open();
}

void MainWindow::onActionCheckCount()
{
//    QSqlQuery Query(DataBase);

//    Query.prepare("SELECT NazwaProduktu, Pojemnosc, StanMagazynu FROM produkty WHERE StanMagazynu<500");

//    if(Query.exec() && Query.size()>0)
//    {
//        QStringList List;

//        while(Query.next())
//        {
//            QStringList L;
//            qDebug() << Query.record().count();
//            for(int x; x<Query.record().count(); ++x)
//                L.push_back(Query.record().field(x).value().toString().append(' '));

//            List.push_back(L.join(','));
//        }

//        QMessageBox::information(this, tr("Out of..."), List.join('\n'));

//    }
//    else
//        qDebug() << Query.lastError();
}
