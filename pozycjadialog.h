#ifndef POZYCJADIALOG_H
#define POZYCJADIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlRelationalTableModel>
#include <QTableView>

#include "products.h"
#include "tabwidget.h"


namespace Ui {
class PozycjaDialog;
}

class PozycjaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PozycjaDialog(QWidget *parent = 0);
    ~PozycjaDialog();

    void setModel(QSqlTableModel *Mod);


private:
    Ui::PozycjaDialog *ui;
    QList<Products> List;
    TabWidget *TableWidget;

signals:
    void accepted(QStringList List);    

public slots:    
    void onIndexChange(const QItemSelection &newSelection, const QItemSelection &oldSelection);
    void onCountChanged(int Value);    

    virtual void accept();
};

#endif // POZYCJADIALOG_H
