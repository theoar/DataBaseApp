#include "pozycjadialog.h"
#include "ui_pozycjadialog.h"

PozycjaDialog::PozycjaDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PozycjaDialog)
{    
    TableWidget = nullptr;
    //    TableWidget = new TabWidget(nullptr, this);
    //    TableWidget->setReadonly(true);
    //    TableWidget->getView()->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    //    TableWidget->getView()->hideColumn(0);

    ui->setupUi(this);
    //    ui->verticalLayout->addWidget(TableWidget);

    //    connect(TableWidget->getView()->selectionModel(), &QItemSelectionModel::selectionChanged, this, &PozycjaDialog::onIndexChange);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

PozycjaDialog::~PozycjaDialog()
{
    delete ui;
}

void PozycjaDialog::setModel(QSqlTableModel *Mod)
{
    if(TableWidget)
        return;

    TableWidget = new TabWidget(nullptr, Mod, this);
    TableWidget->setReadonly(true);
    TableWidget->getView()->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    TableWidget->getView()->hideColumn(0);

    ui->verticalLayout->addWidget(TableWidget);

    connect(TableWidget->getView()->selectionModel(), &QItemSelectionModel::selectionChanged, this, &PozycjaDialog::onIndexChange);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
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
        double Cena = TableWidget->getModel()->index(Index.row(), TableWidget->getModel()->record().indexOf("CenaJednostkowa")).data().toDouble();
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
        QString ID = Model->index(Index.row(), Model->record().indexOf("IDProduktu")).data().toString();
        QString Nazwa = Model->index(Index.row(), Model->record().indexOf("NazwaProduktu")).data().toString();
        QString Pojemnosc = Model->index(Index.row(), Model->record().indexOf("Pojemnosc")).data().toString();
        QString Cena = Model->index(Index.row(), Model->record().indexOf("CenaJednostkowa")).data().toString();
        QString Ilosc = ui->CountBox->text();

        List.push_back(ID );
        List.push_back(QString("%1, %2 l").arg(Nazwa, Pojemnosc));
        List.push_back(Cena);
        List.push_back(Ilosc);
    }

    emit accepted(List);
    QDialog::accept();
}

void PozycjaDialog::open()
{
    QSqlTableModel* M = dynamic_cast<QSqlTableModel*>(TableWidget->getModel());
    if(M)
        M->select();

    QDialog::open();
}
