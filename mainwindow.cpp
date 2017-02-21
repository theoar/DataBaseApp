#include "mainwindow.h"
#include "ui_mainwindow.h"

//const QList<MainWindow::RELATIONAL> MainWindow::Relationals =
//{
//    {
//        "produkty",
//        tr("Products"),
//        false, true,
//        {
//            { 1, QSqlRelation("kategorie", "IDKategorii", "Kategoria") }
//        },
//        {
//            0
//        },
//        {
//            tr("ID"),
//            tr("Category"),
//            tr("Name"),
//            tr("Price"),
//            tr("Count"),
//            tr("Capacity")
//        }
//    },
//    {
//        "klienci",
//        tr("Clients"),
//        false, true,
//        {},
//        {
//            0
//        },
//        {
//            tr("ID"),
//            tr("Name"),
//            tr("Phone"),
//            tr("Mail"),
//            tr("City"),
//            tr("Street"),
//            tr("Home number")
//        }
//    },

//    {
//        "pozycja",
//        tr("Order products"),
//        true, false,
//        {
//            { 2, QSqlRelation("produkty", "IDProduktu", "NazwaProduktu") }
//        },
//        {
//            0, 1
//        },
//        {
//            tr("ID Position"),
//            tr("ID Order"),
//            tr("Product Name"),
//            tr("Count"),
//            tr("Cost"),
//            tr("ID Product"),
//            tr("ID Category"),
//            tr("Product's name"),
//            tr("Price/one"),
//            tr("Magazin state"),
//            tr("Capacity")
//        }
//    },

//    {
//        "kategorie",
//        tr("Categories"),
//        false, true,
//        {
//            { 0, QSqlRelation() }
//        },
//        {
//            0
//        },
//        {
//            tr("IDKategorii"),
//            tr("Category")
//        }
//    },

//    {
//        "rabaty",
//        tr("Discounts"),
//        true, false,
//        {
//            { 0, QSqlRelation() }
//        },
//        {
//            0
//        },
//        {
//            tr("ID Dictount"),
//            tr("Discount [%]"),
//            tr("Minimal order amount [zł]")
//        }
//    },

//    {
//        "wysylka",
//        tr("Shipment"),
//        true, false,
//        {
//            { 0, QSqlRelation() }
//        },
//        {
//            0
//        },
//        {
//            tr("ID Shipment"),
//            tr("Shipment option")
//        }
//    }

//};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    Relationals(
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
            tr("Home number")
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
    },

    {
        "kategorie",
        tr("Categories"),
        false, true,
        {
            { 0, QSqlRelation() }
        },
        {
            0
        },
        {
            tr("IDKategorii"),
            tr("Category")
        }
    },

    {
        "rabaty",
        tr("Discounts"),
        true, false,
        {
            { 0, QSqlRelation() }
        },
        {
            0
        },
        {
            tr("ID Dictount"),
            tr("Discount [%]"),
            tr("Minimal order amount [zł]")
        }
    },

    {
        "wysylka",
        tr("Shipment"),
        true, false,
        {
            { 0, QSqlRelation() }
        },
        {
            0
        },
        {
            tr("ID Shipment"),
            tr("Shipment option")
        }
    }

})
{
    ui->setupUi(this);

    ui->MainTab->setVisible(false);
    connect(ui->MainTab, &QTabWidget::currentChanged, [this](int Index)->void{
        if(TabsWidgets.value("zamowienia") == ui->MainTab->widget(Index))
            ui->PrintAction->setEnabled(!TabsWidgets.value("zamowienia")->getView()->selectionModel()->selection().isEmpty());
        else
            ui->PrintAction->setDisabled(true);
    });

    DataBase = QSqlDatabase::addDatabase("QMYSQL");    

    QCoreApplication::setApplicationName("Storage");
    Settings = new QSettings(this);

    WebView = new QWebView(this);
    WebView->hide();

    Printer = new QPrinter();
    Printer->setOutputFormat(QPrinter::NativeFormat);
}

MainWindow::~MainWindow()
{
    delete Printer;
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

    if( Table == "kategorie")
        return new CategoryDialog();


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
    QVariant ID;
    QSqlTableModel Model(nullptr, DataBase);
    QSqlRecord Record;

    if(Zamowienie.keys().contains("IDZamowienia"))
    {
        ID = Zamowienie["IDZamowienia"];

        QSqlQuery Query(DataBase);
        Query.exec(" DELETE pozycja"
                   " FROM zamowienia"
                   " INNER JOIN pozycja ON pozycja.IDZamowienia=zamowienia.IDZamowienia"
                   " WHERE zamowienia.IDZamowienia="+ID.toString());

        Query.prepare("UPDATE zamowienia SET IDWysylki=:wys, IDKlienta=:kli, Rabat=:rb WHERE IDZamowienia=:id");
        Query.bindValue(":wys", Zamowienie["IDWysylki"].toString());
        Query.bindValue(":kli", Zamowienie["IDKlienta"].toString());
        Query.bindValue(":id", ID.toString());
        Query.bindValue(":rb", Zamowienie["Rabat"].toString());


        qDebug() << Zamowienie;
        qDebug() << Query.lastQuery();

        Query.exec();

        qDebug() << Query.lastError();
    }
    else
    {
        Model.setTable("zamowienia");

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

       ID = Model.record(Model.rowCount()-1).value("IDZamowienia");
    }

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
    QSqlQuery Query(DataBase);
    Query.exec("SELECT DataRealizacji FROM zamowienia WHERE IDZamowienia="+Key.toString());

    if(Query.next() && Query.value("DataRealizacji").toDate().isValid())
    {
        QMessageBox::information(this, tr("Can not do that"), tr("This order is alredy completed"));
        return;
    }

    QSqlQueryModel Model;
    Model.setQuery(" SELECT P.IDProduktu, CONCAT(P.NazwaProduktu, ', ', P.Pojemnosc, ' l') AS 'Nazwa Produktu', P.Pojemnosc, P.StanMagazynu-PZ.Ilosc AS Brak"
                   " FROM produkty P INNER JOIN pozycja PZ ON PZ.IDproduktu=P.IDProduktu"
                   " WHERE PZ.IDZamowienia=" + Key.toString(), DataBase);

    qDebug() << Model.query().lastQuery();

    QList<QStringList> Lista;
    QStringList Headers = { tr("Product name"), tr("Missing") };

    for(int x = 0; x<Model.rowCount(); ++x)
    {
        auto Record = Model.record(x);
        qDebug() << Record;
        if(Record.value("Brak").toInt() < 0)
        {
            Lista.push_back(QStringList());
            Lista.last().append(Record.value("Nazwa Produktu").toString()),
                    Lista.last().append(QString::number(Record.value("Brak").toInt()*-1));
        }
    }

    if(Lista.isEmpty())
    {
        Model.setQuery("SELECT CzyOplacone FROM zamowienia WHERE IDZamowienia=" + Key.toString());
        if(Model.rowCount() > 0 && Model.record(0).value("CzyOplacone").toInt()>0)
        {
            Model.setQuery("UPDATE zamowienia SET DataRealizacji=NOW() WHERE IDZamowienia="+ Key.toString());

            QSqlQuery Query(DataBase);
            Query.prepare(" UPDATE produkty p"
                          " INNER JOIN pozycja pz ON pz.IDProduktu=p.IDProduktu"
                          " SET p.StanMagazynu=p.StanMagazynu-pz.Ilosc"
                          " WHERE pz.IDZamowienia="+Key.toString());
            Query.exec();
        }
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

    if(Model.index(0, Model.record().indexOf("CzyOplacone")).data().toBool())
        QMessageBox::information(this, "Can not do that", "Can not pay paid order");

    for(int x = 0; x<Model.rowCount(); x++)
        Model.setData(Model.index(x, Model.record().indexOf("CzyOplacone")), QVariant(1));

    emit requestRefresh();
}

void MainWindow::onDetails(QMap<QString, QVariant> Map)
{
    QSqlTableModel * ModPozycja = dynamic_cast<QSqlTableModel *>(Models["pozycje_view"]);

    if( !ModPozycja )
        return;

    ModPozycja->setFilter("IDZamowienia=" + Map["Identyfikator"].toString());
    qDebug() << ModPozycja->filter();


    QDialog *Dialog = new DetailsDialog(ModPozycja, Map, this);
    connect(Dialog, &QDialog::accepted, Dialog, &QDialog::deleteLater);
    connect(Dialog, &QDialog::rejected, Dialog, &QDialog::deleteLater);

    Dialog->show();
}

void MainWindow::onOrderDataRequest(int PK)
{
    QSqlQuery Query(DataBase);
    Query.exec(" SELECT k.IDKlienta, k.Nazwa, w.IDWysylki, w.SposobWysylki, z.DataZamowienia"
                " FROM klienci k"
                " INNER JOIN zamowienia z ON z.IDKlienta=k.IDKlienta"
                " INNER JOIN wysylka w ON z.IDWysylki=w.IDWysylki"
                " WHERE z.IDZamowienia="+QString::number(PK));

    QMap<QString, QVariant> Map;
    QList<QStringList> List;

    QSqlRecord Record = Query.record();
    if(Query.next())
        for(int x = 0; x<Query.record().count(); ++x)
            Map.insert(Record.fieldName(x), Query.value(x));

    Map.insert("IDZamowienia", QVariant(PK));

    Query.exec(" SELECT poz.IDProduktu, CONCAT(p.NazwaProduktu, ', ', p.Pojemnosc, ' l'), poz.KosztPozycji, poz.Ilosc"
               " FROM pozycja poz INNER JOIN produkty p ON poz.IDProduktu=p.IDProduktu"
               " WHERE IDZamowienia="+QString::number(PK));


    while(Query.next())
    {
        QStringList L;
        L.push_back(Query.value(0).toString());
        L.push_back(Query.value(1).toString());
        L.push_back(Query.value(2).toString());
        L.push_back(Query.value(3).toString());
        List.push_back(L);
    }

    emit orderDataReady(Map, List);

    ZamowieniaDialog* Z = dynamic_cast<ZamowieniaDialog*>(TabsWidgets["zamowienia"]->getDialog());
    if(Z)
        Z->open(List, Map);

}

void MainWindow::onEdit()
{

}

void MainWindow::onSuccesedQuery()
{
    for(QSqlQueryModel* X : Models.values())
    {
        QSqlTableModel* Casted;
        if((Casted = dynamic_cast<QSqlTableModel*>(X)))
            Casted->select();
    }
}


void MainWindow::onDataRequest()
{
    // TODO implement queries
}

void MainWindow::onLoggin(QString Server, QString User, QString Password)
{
    if(DataBase.open())
        onActionLogout();

    DataBase.setDatabaseName("hurtownia");
    DataBase.setUserName(User);
    DataBase.setPassword(Password);
    DataBase.setHostName(Server);

    if (DataBase.open())
    {
        ui->LoginAction->setEnabled(false);
        ui->CheckCountAction->setEnabled(true);
        ui->QueryAction->setEnabled(true);

        Settings->setValue("server", Server );
        Settings->setValue("login", User);        

        ui->LogoutAction->setEnabled(true);
        ui->StatusBar->showMessage(tr("Current user: %1 Server: %2").arg(User, Server));
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), DataBase.lastError().text());
        return;
    }

    // tabele relacyjne
    for (const auto& Table : Relationals)
    {
        QSqlRelationalTableModel* M = new QSqlRelationalTableModel(this, DataBase);
        M->setEditStrategy(QSqlTableModel::EditStrategy::OnFieldChange);

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

        connect(this, &MainWindow::requestRefresh, W, &TabWidget::onRefresh);        

        W->setDelegate(new QSqlRelationalDelegate(W));
        W->setReadonly(Table.Readonly);
        W->setDeleteable(Table.Deleteable);
        W->hideColumns(Table.Hidden);

        if(Table.Table!="pozycja")
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
        W->addEditButton();

        connect(this, &MainWindow::requestRefresh, W, &TabWidget::onRefresh);
        connect(this, &MainWindow::orderDataReady, W, &TabWidget::onOrderDataReady);
        connect(W, &TabWidget::pay, this, &MainWindow::onPayOrder);
        connect(W, &TabWidget::complete, this, &MainWindow::onCompleteOrder);
        connect(W, &TabWidget::details, this, &MainWindow::onDetails);
        connect(W, &TabWidget::requestOrderData, this, &MainWindow::onOrderDataRequest);
        connect(W, &TabWidget::propetSelection, ui->PrintAction, &QAction::setEnabled);


        W->setReadonly(true);
        W->setDeleteable(false);

        ui->MainTab->addTab(W, tr("Orders"));

        Models.insert("zamowienia", M);
        TabsWidgets.insert("zamowienia", W);

        M = new QSqlTableModel(this, DataBase);
        M->setTable("pozycje_view");
        Models.insert("pozycje_view", M);        

    }
    ui->StatusLable->hide();
    ui->MainTab->show();

}

void MainWindow::onActionLogin()
{
    LoginDialog *Login = new LoginDialog(Settings->value("login").toString(), Settings->value("server").toString(), this);

    connect(Login, &LoginDialog::accepted, this, &MainWindow::onLoggin);

    connect(Login, &LoginDialog::accepted, Login, &LoginDialog::deleteLater);
    connect(Login, &LoginDialog::rejected, Login, &LoginDialog::deleteLater);

    Login->open();
}

void MainWindow::onActionLogout()
{
    ui->MainTab->setVisible(false);
    ui->QueryAction->setEnabled(false);
    ui->LogoutAction->setEnabled(false);
    ui->CheckCountAction->setEnabled(false);

    while(ui->MainTab->count())
        ui->MainTab->removeTab(0);

    for(auto Element : Models.values())
        Element->deleteLater();

    for(auto Element : TabsWidgets.values())
        Element->deleteLater();

    Models.clear();
    TabsWidgets.clear();

    DataBase.close();

    ui->LoginAction->setEnabled(true);
    ui->StatusLable->show();
    ui->StatusBar->clearMessage();
}

void MainWindow::onActionCheckCount()
{        
        QSqlRelationalTableModel *Model = new QSqlRelationalTableModel(nullptr, DataBase);
        auto Relation = Relationals[RELATIONALMAP::PRODUCTS];
        Model->setTable(Relation.Table);
        Model->setFilter("StanMagazynu<="+QString::number(MagazynRaport));
        Model->setRelation(Relation.Relations.first().first, Relation.Relations.first().second);

        QDialog *Dialog = new OutOfDialog(Model, tr("These product are almost of out:"), this);
        connect(Dialog, &QDialog::accepted, Dialog, &QDialog::deleteLater);
        connect(Dialog, &QDialog::rejected, Dialog, &QDialog::deleteLater);

        Dialog->open();
}

void MainWindow::onQueryAction()
{
    QDialog *Dialog = new QueryDialog(DataBase, this);
    connect(Dialog, &QDialog::accepted, Dialog, &QDialog::deleteLater);
    connect(Dialog, &QDialog::rejected, Dialog, &QDialog::deleteLater);
    connect(dynamic_cast<QueryDialog*>(Dialog), &QueryDialog::queryExecuted, this, &MainWindow::onSuccesedQuery);

    Dialog->show();
}

void MainWindow::onActionPrint()
{
    auto Widok = TabsWidgets["zamowienia"];

    QVariant PK = Widok->getCurrentSelectedPK();
    QSqlQuery ZamowienieQuery(" SELECT *"
                              " FROM zamowienia z"
                              " INNER JOIN klienci k ON k.IDKlienta=z.IDKlienta"
                              " INNER JOIN wysylka ON z.IDWysylki=wysylka.IDWysylki"
                              " WHERE z.IDZamowienia="+PK.toString(), DataBase);    

    if(!ZamowienieQuery.size())
        return;

    QSqlQuery Pozycje(" SELECT CONCAT(NazwaProduktu, ', ', Pojemnosc, ' l'),"
                      " Ilosc, ROUND(KosztPozycji, 2), ROUND(Ilosc*KosztPozycji, 2) FROM produkty p"
                      " INNER JOIN pozycja pz ON pz.IDProduktu=p.IDProduktu"
                      " WHERE pz.IDZamowienia="+PK.toString(), DataBase);

    QSqlQuery Kwoty(" SELECT ROUND(SUM(Ilosc*KosztPozycji), 2) AS Suma,"
                    " ROUND(SUM(Ilosc*KosztPozycji)*(1-Rabat/100),2) AS Rsuma,"
                    " ROUND(SUM(Ilosc*KosztPozycji)*Rabat/100, 2) AS Rabat"
                    " FROM pozycja p"
                    " INNER JOIN zamowienia z ON p.IDZamowienia=z.IDzamowienia"
                    " WHERE p.IDZamowienia="+PK.toString(), DataBase);

    QList<QSqlQuery *> Queries;
    Queries.append(&ZamowienieQuery);
    Queries.append(&Pozycje);
    Queries.append(&Kwoty);


    QMap<QString, QVariant> KlientMap;
    QMap<QString, QVariant> ZamowienieMap;
    QList<QStringList> PozycjeMap;

    ZamowienieQuery.next();
    Kwoty.next();
    QSqlRecord Record = ZamowienieQuery.record();
    KlientMap.insert("!--KLIENT--!", Record.value("Nazwa"));
    KlientMap.insert("!--TELEFON--!", Record.value("Telefon"));
    KlientMap.insert("!--EMAIL--!", Record.value("Mail"));
    KlientMap.insert("!--MIASTO--!", Record.value("Miasto"));
    KlientMap.insert("!--ULICA--!", Record.value("Ulica"));
    KlientMap.insert("!--DOM--!", Record.value("NumerDomu"));

    ZamowienieMap.insert("!--DATAZAMOWIENIA--!", Record.value("DataZamowienia"));
    ZamowienieMap.insert("!--DATAREALIZACJI--!", Record.value("DataRealizacji"));
    ZamowienieMap.insert("!--DOSTAWA--!", Record.value("SposobWysylki"));
    ZamowienieMap.insert("!--IDZAMOWIENIA--!", Record.value("IDZamowienia"));

    Record = Kwoty.record();
    ZamowienieMap.insert("!--SUMA--!", QString::number(Record.value("Suma").toDouble(), 'f', 2));
    ZamowienieMap.insert("!--RSUMA--!", QString::number(Record.value("Rsuma").toDouble(), 'f', 2));
    ZamowienieMap.insert("!--RABAT--!", QString::number(Record.value("Rabat").toDouble(), 'f', 2));

    while(Pozycje.next())
    {
        PozycjeMap.append(QStringList());
        for(int x = 0; x<Pozycje.record().count()/2; ++x)
            PozycjeMap.last().append(Pozycje.value(x).toString());
        for(int x = Pozycje.record().count()/2; x<Pozycje.record().count(); ++x)
            PozycjeMap.last().append(QString::number(Pozycje.value(x).toDouble(), 'f', 2));
    }

    QFile File(":/new/prefix1/print.html");

    if(!File.open(QFile::ReadOnly))
        return;

    QTextStream Stream(&File);
    HtmlPrint = Stream.readAll();

    for(auto X : KlientMap.keys())
        HtmlPrint.replace(X, KlientMap[X].toString());

    for(auto X : ZamowienieMap.keys())
        HtmlPrint.replace(X, ZamowienieMap[X].toString());

    QString Table;
    for(auto X : PozycjeMap)
    {
        Table+="<tr>";
        for(auto Y : X)
            Table += "<td>"+Y+"</td>";
        Table+="</tr>";
    }

    HtmlPrint.replace("!--POZYCJE--!", Table);

    QPrintPreviewDialog *Previev = new QPrintPreviewDialog(Printer, this);
    connect(Previev, &QPrintPreviewDialog::paintRequested, [this](void) -> void{
        WebView->setHtml(HtmlPrint);
        WebView->print(Printer);
    });

    connect(Previev, &QPrintPreviewDialog::accepted, Previev, &QPrintPreviewDialog::deleteLater);
    connect(Previev, &QPrintPreviewDialog::reject, Previev, &QPrintPreviewDialog::deleteLater);

    Previev->open();


    return;
}

void MainWindow::onTabChanged(int Index)
{
    TabWidget *Tab;
    Tab = dynamic_cast<TabWidget*>(ui->MainTab->widget(Index));
    if(Tab)
    {
        QSqlTableModel *Model = dynamic_cast<QSqlTableModel*>(Tab->getModel());
        if(Model)
            Model->select();
    }
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

