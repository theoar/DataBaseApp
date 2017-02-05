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

#include "logindialog.h"
#include "klientdialog.h"
#include "tabwidget.h"
#include "produktydialog.h"
#include "zamowieniadialog.h"
#include "products.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{

		Q_OBJECT

	public: struct RELATIONAL
	{
		QString Table;
		QString Name;

		bool Readonly;
		bool Deleteable;

		QList<QPair<int, QSqlRelation>> Relations;
		QList<int> Hidden;
		QStringList Headers;
	};

	public:

		static const QList<RELATIONAL> Relationals;

		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	private:

		Ui::MainWindow *ui;
		LoginDialog *Login;
		QSqlDatabase DataBase;

		QString UserName;

		QSqlTableModel *KlienciModel;

		BaseDialog* getDialogByTable(const QString& Table);

	private slots:

		void onCategoriesRequest(void);
        void onShippingOptionsRequest(void);
        void onClientsNamesRequest(void);
        void onProductsRequest(void);

        void onDataRequest(void); //TO DO

		void onLoggin(QString Server, QString User, QString Password);

		void onActionLogin(void);

	signals:

		void categoriesReady(QList<QPair<QString, int>>);
        void shippingOpitonsReady(QList<QPair<QString, int>>);
        void clientsNamesReady(QList<QPair<QString, int>>);
        void productsReady(QList<Products>);

		void dataReady(QList<QPair<QString, int>>,
					QList<QPair<QString, int>>,
					QList<QPair<QString, int>>);
};

#endif // MAINWINDOW_H
