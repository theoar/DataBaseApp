#include "produktydialog.h"
#include "ui_produktydialog.h"

ProduktyDialog::ProduktyDialog(QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::ProduktyDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

ProduktyDialog::~ProduktyDialog()
{
    delete ui;
}

void ProduktyDialog::onNewList(QList<QPair<QString, int> > Lista)
{
    ui->KategoriaBox->clear();
    for(auto Element : Lista)
	   ui->KategoriaBox->insertItem(-1, Element.first, Element.second );
}


void ProduktyDialog::accept(void)
{
    QMap<QString, QVariant> Map;
    Map.insert("IDKategorii", ui->KategoriaBox->currentData().toInt());
    Map.insert("NazwaProduktu", ui->NazwaEdit->text());
    Map.insert("CenaJednostkowa", ui->CenaEdit->value());
    Map.insert("StanMagazynu", ui->MagazynEdit->value());
    Map.insert("Pojemnosc", ui->PojemnoscEdit->value());

    emit accepted(Map);

    BaseDialog::accept();
}

void ProduktyDialog::onTextChanged(void)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
			 !ui->CenaEdit->text().isEmpty() &&
			 !ui->MagazynEdit->text().isEmpty() &&
			 !ui->NazwaEdit->text().isEmpty() &&
			 !ui->PojemnoscEdit->text().isEmpty() &&
			    ui->KategoriaBox->currentIndex());
}

void ProduktyDialog::open()
{
    emit categoriesRequest();
    BaseDialog::open();
}

