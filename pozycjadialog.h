#ifndef POZYCJADIALOG_H
#define POZYCJADIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QPushButton>

#include "products.h"


namespace Ui {
class PozycjaDialog;
}

class PozycjaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PozycjaDialog(QWidget *parent = 0);
    ~PozycjaDialog();

    void setProducts(QList<Products> L);

private:
    Ui::PozycjaDialog *ui;
    QList<Products> List;

signals:
    void requestProducts(QList<Products> L);

public slots:    
    void onIndexChange(int Index);
    void onCountChanged(int Value);
};

#endif // POZYCJADIALOG_H
