#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::LoginDialog)
{
	ui->setupUi(this);
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

LoginDialog::~LoginDialog()
{
	delete ui;
}

void LoginDialog::onTextChanged()
{
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(
				!ui->LoginText->text().isEmpty() &&
				!ui->PasswordText->text().isEmpty() &&
                !ui->ServerText->text().isEmpty());
}

void LoginDialog::open()
{
    if(!ui->LoginText->text().isEmpty() && !ui->ServerText->text().isEmpty())
        ui->PasswordText->setFocus();

    QDialog::open();
}

void LoginDialog::accept()
{
	emit accepted(ui->ServerText->text(), ui->LoginText->text(), ui->PasswordText->text());

	QDialog::accept();
}
