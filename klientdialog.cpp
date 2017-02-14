#include "klientdialog.h"
#include "ui_klientdialog.h"

KlientDialog::KlientDialog(QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::KlientDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

}

KlientDialog::~KlientDialog()
{
    delete ui;
}

void KlientDialog::onTextChanged()
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled( !ui->NazwaEdit->text().isEmpty() &&
                                                             !ui->MiastoEdit->text().isEmpty() &&
                                                             !ui->UlicaEdit->text().isEmpty() &&
                                                             !ui->NumerEdit->text().isEmpty());
}

void KlientDialog::accept()
{
    QMap<QString, QVariant> Lista;
    Lista.insert("Nazwa",ui->NazwaEdit->text());
    Lista.insert("Telefon", ui->TelefonEdit->text());
    Lista.insert("Mail", ui->MailEdit->text());
    Lista.insert("Miasto", ui->MiastoEdit->text());
    Lista.insert("Ulica", ui->UlicaEdit->text());
    Lista.insert("NumerDomu", ui->NumerEdit->value());

    for(auto & val : Lista )
    {
        if(val == QVariant(""))
            val = QVariant();
    }

    emit accepted(Lista);
    QDialog::accept();
}
