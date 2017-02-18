#include "querydialog.h"
#include "ui_querydialog.h"

QueryDialog::QueryDialog(QSqlDatabase & Db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QueryDialog)
{

    ui->setupUi(this);

    Model = new QSqlQueryModel(this);
    DataBase = Db;
    Kolorowanka = new KLHighlighter(ui->EditBox->document());

    ui->View->setModel(Model);
    ui->View->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->View->horizontalHeader()->setStretchLastSection(true);
    ui->View->hide();

    ui->LastQueryButton->setEnabled(false);
    ui->EditBox->setFocus();

    qDebug() << ButtonNames.size() << ButtonNames.length();

    //qDebug() << ButtonNames.size();
    QBoxLayout * Layout = ui->horizontalLayout;
    for(int x = ButtonNames.size()-1; x>=0; --x )
    {
        QPushButton *Button = new QPushButton(ButtonNames[x],this);
        Layout->insertWidget(1, Button);
        connect(Button, &QPushButton::clicked, [this, x](void) -> void{
            ui->EditBox->clear();
            ui->EditBox->setPlainText(Hints[x]);
            ui->EditBox->setFocus();
        });
    }

}

QueryDialog::~QueryDialog()
{
    Kolorowanka->deleteLater();
    delete ui;
}

void QueryDialog::onSendQuery()
{
    if(QueryList.length()==MaxSize)
        QueryList.removeFirst();

    if(!ui->EditBox->document()->toPlainText().trimmed().isEmpty())
    {
        QueryList.push_back(ui->EditBox->document()->toPlainText());
        ui->LastQueryButton->setEnabled(true);
    }

    Model->setQuery(ui->EditBox->document()->toPlainText(), DataBase);
    if(Model->lastError().isValid())
    {
        ui->ResponseLabel->setText(LableText+Model->lastError().text());
        ui->View->hide();
    }
    else
    {
        ui->ResponseLabel->setText(LableText+tr("Affected: %1 row(s)").arg(QString::number(Model->rowCount())));
        if(Model->rowCount())
            ui->View->show();

        QRegExp RegExp("INSERT|UPDATE|DELETE");
        RegExp.setCaseSensitivity(Qt::CaseInsensitive);
        if(Model->query().lastQuery().contains(RegExp))
            emit queryExecuted();
    }

    ui->EditBox->setFocus();
}

void QueryDialog::onBackButton()
{
    if(!QueryList.isEmpty())
    {
        ui->EditBox->setPlainText(QueryList.last());
        QueryList.removeLast();
    }

    ui->LastQueryButton->setEnabled(!QueryList.isEmpty());

}
