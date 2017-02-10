#ifndef ZAMOWIENIADIALOG_H
#define ZAMOWIENIADIALOG_H

#include <QDialog>
#include <QDebug>
#include <QSqlRelationalTableModel>
#include <QDebug>
#include <QStandardItemModel>
#include <QDate>

#include "basedialog.h"
#include "products.h"
#include "pozycjadialog.h"

namespace Ui {
class ZamowieniaDialog;
}

class ZamowieniaDialog : public BaseDialog
{
    Q_OBJECT

public:
    explicit ZamowieniaDialog(QWidget *parent = 0);
    ~ZamowieniaDialog();

    void setPozycjaModel(QSqlTableModel *M);

private:
    Ui::ZamowieniaDialog *ui;
    PozycjaDialog *Dialog;
    QStringList TableHeaders = { tr("IDProduct"),tr("Name"), tr("Cost per one"), tr("Count") };
    QString StaticDataLabel = tr("Order date: ");
    QString StaticTotalCostLabel = tr("Total cost: ");
    QString StaticDiscountLabel = tr("Discount: ");
    QString StaticDiscountCostLabe = tr("Cost with discount: ");

    double TotalKoszt;
    int TotalRabat;

    void updateTotalCost();
    int isAlredyInOrder(const QStringList &List);

    enum PozycjaColumns
    {
        IDProduktu = 0,
        NazwaProduktu = 1,
        Koszt = 2,
        Ilosc = 3
    };

public slots:
    void onNewShippingOptions(QList<QPair<QString, int>> List);
    void onNewClientsNames(QList<QPair<QString, int>> List);
    void onNewDiscount(int Rabat);

    void onAddPozycja();
    void onDeletePozycja();
    void onPozycjaAdded(QStringList List);
    void onIndexChanged(const QItemSelection &newSelection, const QItemSelection &oldSelection);

    virtual void open();
    virtual void accept();

    void onRefreshRequest();

signals:
    void shippingOptionRequest();
    void clientsNameRequest();
    void discountRequest(double Kwota);

    void accepted(QMap<QString, QVariant> Zamowienie, QMap<QString, QList<QVariant>> Pozycje);

};

#endif // ZAMOWIENIADIALOG_H
