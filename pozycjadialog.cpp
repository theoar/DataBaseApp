#include "pozycjadialog.h"
#include "ui_pozycjadialog.h"

PozycjaDialog::PozycjaDialog(QSqlRelationalTableModel *Mod, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PozycjaDialog)
{    
    TableWidget = new TabWidget(nullptr, Mod, this);
    TableWidget->setReadonly(true);
    TableWidget->getView()->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

    ui->setupUi(this);
    ui->verticalLayout->addWidget(TableWidget);

    connect(TableWidget->getView()->selectionModel(), &QItemSelectionModel::selectionChanged, this, &PozycjaDialog::onIndexChange);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

}

PozycjaDialog::~PozycjaDialog()
{
    delete ui;
}


void PozycjaDialog::onIndexChange(const QItemSelection & newSelection, const QItemSelection & oldSelection)
{
    if(!newSelection.isEmpty())
    {
        double Cena = TableWidget->getModel()->index(newSelection.indexes().first().row(), 3).data().toDouble();
        int Ilosc = ui->CountBox->value();
        ui->TotalCostEdit->setText( QString::number(Cena*Ilosc) );
    }
    else
        ui->TotalCostEdit->setText( tr("Didn't select item") );


    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!newSelection.isEmpty());
}

void PozycjaDialog::onCountChanged(int Value)
{    
    auto Index = TableWidget->getView()->currentIndex();
    if(!TableWidget->getView()->selectionModel()->selection().isEmpty())
    {
        double Cena = TableWidget->getModel()->index(Index.row(), 3).data().toDouble();
        ui->TotalCostEdit->setText( QString::number(Cena*Value) );
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(Value);
    }
}
