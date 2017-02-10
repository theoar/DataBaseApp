#ifndef OUTOFDIALOG_H
#define OUTOFDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QTableWidget>
#include <QList>
#include <QStringList>

namespace Ui {
class OutOfDialog;
}

class OutOfDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OutOfDialog(QStringList Header, QList<QStringList> Data, QWidget *parent = 0);
    ~OutOfDialog();

private:
    Ui::OutOfDialog *ui;
};

#endif // OUTOFDIALOG_H
