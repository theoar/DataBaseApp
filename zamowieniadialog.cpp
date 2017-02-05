#include "zamowieniadialog.h"
#include "ui_zamowieniadialog.h"

ZamowieniaDialog::ZamowieniaDialog(QSqlRelationalTableModel *Mod, QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::ZamowieniaDialog)
{
    Dialog = new PozycjaDialog(Mod, this);
    ui->setupUi(this);

}

ZamowieniaDialog::~ZamowieniaDialog()
{
    delete ui;
}

void ZamowieniaDialog::onNewShippingOptions(QList<QPair<QString, int> > List)
{
    ui->ShippingBox->clear();
    for(auto Element : List)
        ui->ShippingBox->insertItem(-1, Element.first, Element.second);
}

void ZamowieniaDialog::onNewClientsNames(QList<QPair<QString, int> > List)
{
    ui->ClientBox->clear();
    for(auto Element : List)
        ui->ClientBox->insertItem(-1, Element.first, Element.second);
}

void ZamowieniaDialog::onAddPozycja()
{
    Dialog->open();   
}

void ZamowieniaDialog::open()
{
    emit shippingOptionRequest();
    emit clientsNameRequest();    

    BaseDialog::open();
}
