#ifndef KLIENTDIALOG_H
#define KLIENTDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QStringList>
#include "basedialog.h"

namespace Ui {
class KlientDialog;
}

class KlientDialog : public BaseDialog
{
    Q_OBJECT

public:
    explicit KlientDialog(QWidget *parent = 0);
    ~KlientDialog();

private:
    Ui::KlientDialog *ui;

public slots:
    void onTextChanged(void);
    virtual void accept();
};

#endif // KLIENTDIALOG_H
