#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QPushButton>
#include "basedialog.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QString LastPasswd, QString LastServer, QWidget *parent = 0);
    ~LoginDialog();

private:
    Ui::LoginDialog *ui;
signals:
    void accepted(QString, QString, QString);

public slots:
    virtual void accept();
    virtual void onTextChanged();
    virtual void open();
};

#endif // LOGINDIALOG_H
