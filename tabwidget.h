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

private:

    Ui::TabWidget *ui;
    BaseDialog *Dialog;
    QSqlQueryModel *Model;

public slots:
    void refresh();

private slots:

    void onAddButton();
    void onDeleteButton();
    void onDialogAccepted(QMap<QString, QVariant> Vect);
    void onSearchChanged();    

};

#endif // TABWIDGET_H
