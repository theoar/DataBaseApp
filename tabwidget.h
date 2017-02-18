#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QSqlRelationalTableModel>
#include <QWidget>
#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlQueryModel>
#include <QItemDelegate>
#include <QDebug>
#include <QTableView>
#include <QSqlQuery>
#include <QPushButton>

#include "basedialog.h"

namespace Ui
{
class TabWidget;
}

class TabWidget : public QWidget
{

    Q_OBJECT

public:

    explicit TabWidget(BaseDialog *Dial, QSqlQueryModel * M , QWidget *parent = 0);
    ~TabWidget();

    void setReadonly(bool Readonly);
    void setDeleteable(bool Deleteable);
    void setDelegate(QItemDelegate* Delegate);

    void insertWidget(QWidget* W, int Pos = 0);

    void hideColumns(QList<int> Hidden);

    QSqlQueryModel *getModel() const;
    void setModel(QSqlQueryModel *value);
    QTableView *getView();

    void addPayButton();
    void addCompleteButton();
    void addDetailsButton();
    void addEditButton();    

    void hideSearchBar();
    void hideStandardButtons();
    int getCurrentSelectedPK();

    BaseDialog *getDialog();
    void setDialog(BaseDialog *value);

private:

    Ui::TabWidget *ui;
    BaseDialog *Dialog;
    QSqlQueryModel *Model;
    QList<QAbstractButton*> ButtonDistabler;

    void onSelectionChagned(const QItemSelection &newSelection, const QItemSelection &oldSelection);

public slots:
    void onRefresh();
    void onOrderDataReady(QMap<QString, QVariant> Map, QList<QStringList> ProductList);

private slots:

    void onAddButton();
    void onDeleteButton();
    void onDialogAccepted(QMap<QString, QVariant> Vect);
    void onSearchChanged();    
    void onPayClicked();
    void onCompleteClicked();
    void onDetailsClicked();
    void onEditClicked();    

signals:
    void pay(QVariant Key);
    void complete(QVariant Key);
    void details(QMap<QString, QVariant>);
    void requestOrderData(int PK);
    void edit(int PK);
    void propetSelection(bool);

};

#endif // TABWIDGET_H
