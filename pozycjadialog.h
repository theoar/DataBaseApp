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
    explicit PozycjaDialog(QSqlRelationalTableModel *Mod, QWidget *parent = 0);
    ~PozycjaDialog();

    void setModel(QSqlRelationalTableModel *M);   


private:
    Ui::PozycjaDialog *ui;
    QList<Products> List;
    TabWidget *TableWidget;

signals:    

public slots:    
    void onIndexChange(const QItemSelection &newSelection, const QItemSelection &oldSelection);
    void onCountChanged(int Value);
};

#endif // POZYCJADIALOG_H
