#ifndef QUERYDIALOG_H
#define QUERYDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlQuery>

#include "klhighlighter.hpp"

namespace Ui {
class QueryDialog;
}

class QueryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QueryDialog(QSqlDatabase &Db, QWidget *parent = 0);
    ~QueryDialog();

    KLHighlighter *Kolorowanka;
    QSqlQueryModel *Model;
    QSqlDatabase DataBase;
    QString LableText = tr("Sql server response: ");
    QStringList QueryList;
    int MaxSize = 10;

private:
    Ui::QueryDialog *ui;

public slots:
    void onSendQuery(void);
    void onBackButton(void);
};

#endif // QUERYDIALOG_H
