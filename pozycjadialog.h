#ifndef POZYCJADIALOG_H
#define POZYCJADIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlRelationalTableModel>
#include <QTableView>

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
    TabWidget *TableWidget;

signals:
    void accepted(QStringList List);    

public slots:    
    void onIndexChange(const QItemSelection &newSelection, const QItemSelection &oldSelection);
    void onCountChanged(int Value);    

    virtual void accept();
    virtual void open();
};

#endif // POZYCJADIALOG_H
