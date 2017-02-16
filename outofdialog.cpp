#include "outofdialog.h"
#include "ui_outofdialog.h"

OutOfDialog::OutOfDialog(QStringList Header, QList<QStringList> Data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OutOfDialog)
{
    ui->setupUi(this);
    QStandardItemModel *Model = new QStandardItemModel(0, Header.length(), this);
    ui->TableWidget->setModel(Model);
    ui->TableWidget->setEditTriggers(0);
    ui->TableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->TableWidget->horizontalHeader()->setStretchLastSection(true);

    Model->setHorizontalHeaderLabels(Header);

    QList<QStandardItem*> Items;
    for(auto & Row : Data)
    {
        Items.clear();
        for(auto & Element : Row)
            Items.push_back(new QStandardItem(Element));
        Model->appendRow(Items);
    }


}

OutOfDialog::OutOfDialog(QAbstractItemModel *M, QString LableHeader, QWidget *Parent) :
    QDialog(Parent),
    ui(new Ui::OutOfDialog)
{
    M->setParent(this);
    ui->setupUi(this);
    ui->TableWidget->setModel(M);
    ui->TableWidget->setEditTriggers(0);

    ui->TableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->TableWidget->horizontalHeader()->setStretchLastSection(true);

    ui->label->setText(LableHeader);
    ui->TableWidget->hideColumn(0);

    QSqlTableModel *Table = dynamic_cast<QSqlTableModel*>(M);
    if(Table)
        Table->select();
}

OutOfDialog::~OutOfDialog()
{
    delete ui;
}
