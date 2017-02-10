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
    },

    {
        "pozycja",
        tr("Order products"),
        true, false,
        {
            { 2, QSqlRelation("produkty", "IDProduktu", "NazwaProduktu") }
        },
        {
            0, 1
        },
        {
            tr("ID Position"),
            tr("ID Order"),
            tr("Product Name"),
            tr("Count"),
            tr("Cost"),
            tr("ID Product"),
            tr("ID Category"),
            tr("Product's name"),
            tr("Price/one"),
            tr("Magazin state"),
            tr("Capacity")
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

BaseDialog* MainWindow::getDialogByTable(const QString& Table)
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
        auto Dialog = new ZamowieniaDialog();
        connect(Dialog, &ZamowieniaDialog::shippingOptionRequest, this, &MainWindow::onShippingOptionsRequest);
        connect(Dialog, &ZamowieniaDialog::clientsNameRequest, this, &MainWindow::onClientsNamesRequest);
        connect(Dialog, &ZamowieniaDialog::discountRequest, this, &MainWindow::onDiscountRequest);
        connect(Dialog, &ZamowieniaDialog::accepted,this, &MainWindow::onNewZamowienie);

        connect(this, &MainWindow::shippingOpitonsReady, Dialog, &ZamowieniaDialog::onNewShippingOptions);
        connect(this, &MainWindow::clientsNamesReady, Dialog, &ZamowieniaDialog::onNewClientsNames);
        connect(this, &MainWindow::discountReady, Dialog, &ZamowieniaDialog::onNewDiscount);

        Dialog->setPozycjaModel(createPozycjaModel());

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

void MainWindow::onNewZamowienie(QMap<QString, QVariant> Zamowienie, QMap<QString, QList<QVariant>> Pozycje)
{
    QSqlTableModel Model(nullptr, DataBase);
    Model.setTable("zamowienia");

    QSqlRecord Record;
    for(auto i = Zamowienie.constBegin(); i != Zamowienie.constEnd(); ++i)
    {
        Record.append(QSqlField(i.key(), i.value().type()));
        Record.setValue(i.key(), i.value());
    }
    QVariant V = QDate().currentDate();
    Record.append(QSqlField("DataZamowienia", V.type()));
    Record.setValue("DataZamowienia", V);
    qDebug() << Record;
    qDebug() << Model.record();

    Model.insertRecord(-1, Record);

    QVariant ID = Model.record(Model.rowCount()-1).value("IDZamowienia");

    Model.setTable("pozycja");
    Record = Model.record();
    Record.setValue("IDZamowienia", ID);

    qDebug() << Model.record(Model.rowCount()-1).value("IDZamowienia");

    for(int x = 0; x<Pozycje.begin().value().length(); ++x)
    {
        for(auto i = Pozycje.constBegin(); i != Pozycje.constEnd(); ++i)
            Record.setValue(i.key(), i.value().at(x));
        Model.insertRecord(-1, Record);
    }

    emit requestRefresh();
}

void MainWindow::onCompleteOrder(QVariant Key)
{
    //    QSqlRelationalTableModel Model(nullptr, DataBase);

    //    auto & Table = Relationals[RELATIONALMAP::POSITION];

    //    Model->setTable(Table.Table);
    //    Model->setFilter(QString("produkty.IDZamowienia=%1 AND StanMagazynu>").arg(Key.toString()));


    //    for (const auto& Relation : Table.Relations)
    //        Model->setRelation(Relation.first, Relation.second);

    //    for (int i = 0; i < Model->columnCount() && i < Table.Headers.size(); ++i)
    //        Model->setHeaderData(i, Qt::Horizontal, Table.Headers[i]);

    //    Model->select();


    QSqlQueryModel Model;
    Model.setQuery(" SELECT P.IDProduktu, P.NazwaProduktu AS 'Nazwa Produktu', P.Pojemnosc,P.StanMagazynu-PZ.Ilosc AS Brak"
                   " FROM produkty P INNER JOIN pozycja PZ ON PZ.IDproduktu=P.IDProduktu"
                   " WHERE PZ.IDZamowienia=" + Key.toString(), DataBase);

    qDebug() << Model.query().lastQuery();

    QList<QStringList> Lista;
    QStringList Headers = { tr("Product name"), tr("Capacity"), tr("Missing") };

    for(int x = 0; x<Model.rowCount(); ++x)
    {
        auto Record = Model.record(x);
        qDebug() << Record;
        if(Record.value("Brak").toInt() < 0)
        {
            Lista.push_back(QStringList());
            Lista.last().append(Record.value("Nazwa Produktu").toString()),
            Lista.last().append(Record.value("Pojemnosc").toString()),
            Lista.last().append(QString::number(Record.value("Brak").toInt()*-1));
        }
    }

    if(Lista.isEmpty())
    {
        Model.setQuery("SELECT CzyOplacone FROM zamowienia WHERE IDZamowienia=" + Key.toString());
        if(Model.rowCount() > 0 && Model.record(0).value("CzyOplacone").toInt()>0)
            Model.setQuery("UPDATE zamowienia SET DataRealizacji=NOW() WHERE IDZamowienia="+ Key.toString());
        else
            QMessageBox::information(this, tr("Information"), tr("Order number: %1 not payed").arg(Key.toString()));

    }
    else
    {
        OutOfDialog *Dialog = new OutOfDialog(Headers, Lista, this);
        connect(Dialog, &OutOfDialog::accepted, Dialog, &OutOfDialog::deleteLater);
        connect(Dialog, &OutOfDialog::rejected, Dialog, &OutOfDialog::deleteLater);

        Dialog->open();
    }


    requestRefresh();

    qDebug() << Key;
}

void MainWindow::onPayOrder(QVariant Key)
{
    QSqlTableModel Model(nullptr, DataBase);
    Model.setEditStrategy(QSqlTableModel::OnFieldChange);
    Model.setTable("zamowienia");
    Model.setFilter("IDZamowienia= "+ Key.toString());
    Model.select();

    for(int x = 0; x<Model.rowCount(); x++)
        Model.setData(Model.index(x, Model.record().indexOf("CzyOplacone")), QVariant(1));

    emit requestRefresh();
}

void MainWindow::onDetails(QVariant Key)
{
    QSqlRelationalTableModel *M = dynamic_cast<QSqlRelationalTableModel*>(Models.value("pozycja"));
    ui->MainTab->setCurrentWidget(TabsWidgets.value("pozycja"));
    if(M)
    {
        M->setFilter("IDZamowienia="+Key.toString());
        ui->MainTab->setTabEnabled(ui->MainTab->indexOf(TabsWidgets.value("pozycja")), true);
        qDebug() << M->query().lastQuery();
        qDebug() << M->record();
        qDebug() << M->rowCount();
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

        TabWidget* W = new TabWidget(getDialogByTable(Table.Table), M, this);

        W->setDelegate(new QSqlRelationalDelegate(W));
        W->setReadonly(Table.Readonly);
        W->setDeleteable(Table.Deleteable);
        W->hideColumns(Table.Hidden);

        ui->MainTab->addTab(W, Table.Name);

        Models.insert(Table.Table, M);
        TabsWidgets.insert(Table.Table, W);
    }

    // tabele specjalne
    {

        QSqlTableModel* M = new QSqlTableModel(this, DataBase);
        M->setTable("zamowienia_view");

        TabWidget* W = new TabWidget(getDialogByTable("zamowienia"), M, this);

        W->addPayButton();
        W->addCompleteButton();
        W->addDetailsButton();

        connect(this, &MainWindow::requestRefresh, W, &TabWidget::onRefresh);
        connect(W, &TabWidget::pay, this, &MainWindow::onPayOrder);
        connect(W, &TabWidget::complete, this, &MainWindow::onCompleteOrder);
        connect(W, &TabWidget::details, this, &MainWindow::onDetails);

        W->setReadonly(true);
        W->setDeleteable(false);


        ui->MainTab->addTab(W, tr("Orders"));

        Models.insert("zamowienia", M);
        TabsWidgets.insert("zamowienia", W);
    }

    ui->StatusLable->hide();
    ui->MainTab->show();

    ui->MainTab->setTabEnabled(ui->MainTab->indexOf(TabsWidgets.value("pozycja")), false);
    configurePozycje();
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

QSqlTableModel* MainWindow::createPozycjaModel()
{
    QSqlRelationalTableModel* M = new QSqlRelationalTableModel(this, DataBase);

    auto &Table = Relationals[0];

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

    return M;
}

void MainWindow::configurePozycje()
{
    TabsWidgets.value("pozycja")->hideStandardButtons();
}
