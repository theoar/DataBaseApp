#ifndef CATEGORYDIALOG_H
#define CATEGORYDIALOG_H

#include "basedialog.h"
#include <QDialogButtonBox>
#include <QPushButton>

namespace Ui {
class CategoryDialog;
}

class CategoryDialog : public BaseDialog
{
    Q_OBJECT

public:
    explicit CategoryDialog(QWidget *parent = 0);
    ~CategoryDialog();

    virtual void accept();

private:
    Ui::CategoryDialog *ui;

public slots:
    void onTextChanged(QString Text);
};

#endif // CATEGORYDIALOG_H
