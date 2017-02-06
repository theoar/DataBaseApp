#include "pozycjadialog.h"
#include "ui_pozycjadialog.h"

PozycjaDialog::PozycjaDialog(QSqlRelationalTableModel *Mod, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PozycjaDialog)
{    
    TableWidget = new TabWidget(nullptr, Mod, this);
    TableWidget->setReadonly(true);
    TableWidget->getView()->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    TableWidget->getView()->hideColumn(0);

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

void PozycjaDialog::accept()
{
    QStringList List;

    auto Index = TableWidget->getView()->currentIndex();
    auto Model = TableWidget->getModel();
    if(!TableWidget->getView()->selectionModel()->selection().isEmpty())
    {
        List.push_back(Model->index(Index.row(), 0).data().toString());
        List.push_back(QString("%1, %2 l").arg(Model->index(Index.row(), 2).data().toString()).arg(Model->index(Index.row(), 5).data().toString()));
        List.push_back(Model->index(Index.row(), 3).data().toString());
        List.push_back(QString::number(ui->CountBox->value()));
    }

    emit accepted(List);
    QDialog::accept();
}
