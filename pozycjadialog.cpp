#include "pozycjadialog.h"
#include "ui_pozycjadialog.h"

PozycjaDialog::PozycjaDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PozycjaDialog)
{
    ui->setupUi(this);
}

PozycjaDialog::~PozycjaDialog()
{
    delete ui;
}

void PozycjaDialog::onIndexChange(int Index)
{
    auto &P = List.at(Index);

    if(List.at(Index).getStan() < 0)    
        QMessageBox::information(this, tr("Error"), tr("Out of this product: ") + P.getNazwa(), QMessageBox::Ok);

    ui->CountBox->setMaximum(P.getStan());
    ui->CountBox->setValue(0);
    ui->CountBox->valueChanged(0);
}

void PozycjaDialog::onCountChanged(int Value)
{
    ui->TotalCostEdit->setText( QString::number(List.at(ui->ProductComboBox->currentIndex()).getCena()*Value) );
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(Value);
}

void PozycjaDialog::setProducts(QList<Products> L)
{
    List = L;

    ui->ProductComboBox->clear();
    ui->CountBox->clear();
    ui->TotalCostEdit->clear();

    if(!List.isEmpty())
    {
        for(auto Element : List)
        {
            QString Str = QString("%1, %2 l, %3 zł").arg(Element.getNazwa(), QString::number(Element.getPojemnosc()), QString::number(Element.getCena()));
            ui->ProductComboBox->insertItem(-1, Str);
        }

        if(List[0].getStan() < 0)
            QMessageBox::information(this, tr("Error"), tr("Out of this product: ") + List[0].getNazwa(), QMessageBox::Ok);

        ui->CountBox->setMaximum(List[0].getStan());
        ui->CountBox->setValue(0);
    }

}
