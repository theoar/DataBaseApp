#include "categorydialog.h"
#include "ui_categorydialog.h"

CategoryDialog::CategoryDialog(QWidget *parent) :
    BaseDialog(parent),
    ui(new Ui::CategoryDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

CategoryDialog::~CategoryDialog()
{
    delete ui;
}

void CategoryDialog::accept()
{
    QMap<QString, QVariant> Map;

    Map.insert("Kategoria", ui->EditBox->text());
    emit accepted(Map);

    BaseDialog::accept();
}

void CategoryDialog::onTextChanged(QString Text)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!Text.isEmpty() && !Text.isNull());
}
