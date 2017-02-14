#include "detailsdialog.h"
#include "ui_detailsdialog.h"

DetailsDialog::DetailsDialog(QSqlQueryModel *M, QMap<QString, QVariant> Lables, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DetailsDialog)
{
    ui->setupUi(this);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    int I = 0;
    for(auto & Element : Lables.keys())
    {
        ui->gridLayout->addWidget(new QLabel(Element+":", this),I,0);
        ui->gridLayout->addWidget(new QLabel(Lables[Element].toString(), this), I, 1);
        I++;
    }

    QSqlTableModel* Mm = dynamic_cast<QSqlTableModel*>(M);

    if(!Mm)
        qDebug() << QString("error");
    else
    {
        ui->tableView->setModel(Mm);
        ui->tableView->setEditTriggers(0);
        ui->tableView->hideColumn(0);
        Mm->select();
    }
}

DetailsDialog::~DetailsDialog()
{
    delete ui;
}
