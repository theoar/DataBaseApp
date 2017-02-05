#include "zamowieniadialog.h"
#include "ui_zamowieniadialog.h"

ZamowieniaDialog::ZamowieniaDialog(QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::ZamowieniaDialog),
    Dialog(new PozycjaDialog(this))
{
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

void ZamowieniaDialog::onNewProducts(QList<Products> List)
{
    Dialog->setProducts(List);
}

void ZamowieniaDialog::onAddPozycja()
{
    Dialog->open();
    emit productsRequest();
}

void ZamowieniaDialog::open()
{
    emit shippingOptionRequest();
    emit clientsNameRequest();    

    BaseDialog::open();
}
