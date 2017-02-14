#ifndef DETAILSDIALOG_H
#define DETAILSDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QLabel>
#include <QMap>
#include <QList>
#include <QSqlTableModel>
#include <QDebug>


namespace Ui {
class DetailsDialog;
}

class DetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DetailsDialog(QSqlQueryModel *M, QMap<QString, QVariant> Lables, QWidget *parent = 0);
    ~DetailsDialog();

private:
    Ui::DetailsDialog *ui;
};

#endif // DETAILSDIALOG_H
