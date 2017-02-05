#ifndef ZAMOWIENIADIALOG_H
#define ZAMOWIENIADIALOG_H

#include <QDialog>
#include <QDebug>
#include <QSqlRelationalTableModel>
#include <QDebug>

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
    explicit ZamowieniaDialog(QSqlRelationalTableModel *Mod, QWidget *parent = 0);
    ~ZamowieniaDialog();

private:
    Ui::ZamowieniaDialog *ui;
    PozycjaDialog *Dialog;

public slots:
    void onNewShippingOptions(QList<QPair<QString, int>> List);
    void onNewClientsNames(QList<QPair<QString, int>> List);    

    void onAddPozycja();

    virtual void open();

signals:
    void shippingOptionRequest();
    void clientsNameRequest();
};

#endif // ZAMOWIENIADIALOG_H
