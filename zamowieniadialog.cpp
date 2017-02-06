#include "zamowieniadialog.h"
#include "ui_zamowieniadialog.h"

ZamowieniaDialog::ZamowieniaDialog(QSqlRelationalTableModel *Mod, QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::ZamowieniaDialog)
{
    Dialog = new PozycjaDialog(Mod, this);
    ui->setupUi(this);

    ui->View->setModel(new QStandardItemModel(0,4));
    dynamic_cast<QStandardItemModel*>(ui->View->model())->setHorizontalHeaderLabels(TableHeaders);
    ui->View->hideColumn(PozycjaColumns::IDProduktu);
    ui->View->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    ui->View->setEditTriggers(0);
    ui->View->horizontalHeader()->setStretchLastSection(true);

    connect(Dialog, &PozycjaDialog::accepted, this, &ZamowieniaDialog::onPozycjaAdded);
    connect(ui->View->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ZamowieniaDialog::onIndexChanged);

    ui->OrderDate->setText(QString("%1 %2").arg(StaticDataLabel, QDate::currentDate().toString("dd-MM-yyyy")));

    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

ZamowieniaDialog::~ZamowieniaDialog()
{
    delete ui;
}

void ZamowieniaDialog::updateTotalCost()
{
    double Cost = 0;
    for(int x = 0; x<ui->View->model()->rowCount(); ++x)
        Cost += ui->View->model()->index(x, PozycjaColumns::Koszt).data().toDouble()*ui->View->model()->index(x, PozycjaColumns::Ilosc).data().toDouble();

    ui->TotalCostLabel->setText(QString("%1 %2 zl").arg(StaticTotalCostLabel, QString::number(Cost)));

    TotalKoszt = Cost;

    emit discountRequest(Cost);
}

int ZamowieniaDialog::isAlredyInOrder(const QStringList &List)
{
    for(int x = 0; x<ui->View->model()->rowCount(); ++x)
    {
        if(List.at(PozycjaColumns::IDProduktu).toInt() == ui->View->model()->index(x,PozycjaColumns::IDProduktu).data().toInt())
            return x;
    }

    return -1;
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

void ZamowieniaDialog::onNewDiscount(int Rabat)
{
    TotalRabat = Rabat;
    ui->DiscountLabel->setText( QString("%1 %2 %").arg(StaticDiscountLabel, QString::number(TotalRabat)) );
    ui->DiscountCostLabel->setText( QString("%1 %2 zl").arg(StaticDiscountCostLabe, QString::number(TotalKoszt*(1-TotalRabat/100.f) )));
}

void ZamowieniaDialog::onAddPozycja()
{
    Dialog->open();
}

void ZamowieniaDialog::onDeletePozycja()
{
    auto Index = ui->View->currentIndex();
    ui->View->model()->removeRow(Index.row());

    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(ui->View->model()->rowCount());
    updateTotalCost();
}

void ZamowieniaDialog::onPozycjaAdded(QStringList List)
{
    int Index = isAlredyInOrder(List);

    if(Index>-1)
    {
        auto ModelIndex = ui->View->model()->index(Index, PozycjaColumns::Ilosc);
        int Count = ModelIndex.data().toInt()+List.at(PozycjaColumns::Ilosc).toInt();
        ui->View->model()->setData(ModelIndex, QVariant(Count));
    }
    else
    {
        QList<QStandardItem*> NewRow;
        for(auto & Element : List)
            NewRow.push_back(new QStandardItem(Element));

        dynamic_cast<QStandardItemModel*>(ui->View->model())->insertRow(ui->View->model()->rowCount(),NewRow);
    }

    ui->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(ui->View->model()->rowCount());

    updateTotalCost();
}

void ZamowieniaDialog::onIndexChanged(const QItemSelection &newSelection, const QItemSelection &oldSelection)
{
    ui->RemoveButton->setEnabled(!newSelection.isEmpty());
}

void ZamowieniaDialog::open()
{
    emit shippingOptionRequest();
    emit clientsNameRequest();    

    BaseDialog::open();
}
