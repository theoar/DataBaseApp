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

BaseDialog* MainWindow::getDialogByTable(const QString& Table)
{
	if (Table == "klienci") return new KlientDialog();

	if (Table == "produkty")
	{
		auto Dialog = new ProduktyDialog();

		connect(Dialog, &ProduktyDialog::categoriesRequest, this, &MainWindow::onCategoriesRequest);
		connect(this, &MainWindow::categoriesReady, Dialog, &ProduktyDialog::onNewList);

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

	if (DataBase.open()) ui->LoginAction->setEnabled(false);
	else QMessageBox::critical(this, tr("Error"), DataBase.lastError().text());

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
	}

	// tabele specjalne
	{
		static const QStringList Headers =
		{
			// TODO headers
		};

		QSqlQueryModel* M = new QSqlQueryModel(this);

		M->setQuery("", DataBase);

		for (int i = 0; i < M->columnCount() && i < Headers.size(); ++i)
		{
			M->setHeaderData(i, Qt::Horizontal, Headers[i]);
		}

		TabWidget* W = new TabWidget(getDialogByTable("zamowienia"), M, this);
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
