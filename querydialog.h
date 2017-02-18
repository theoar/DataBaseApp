#ifndef QUERYDIALOG_H
#define QUERYDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QRegExp>
#include <QTextCursor>

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
    QStringList ButtonNames = { "SELECT", "INSERT", "UPDATE", "DELETE", tr("Clear") };
    QStringList Hints = { "SELECT * FROM <TABLE_NAME> WHERE 1",
                          "INSERT INTO <TABLE_NAME>([COL1], [COL2], [...]) VALUES([VAl1], [VAL2], [...])",
                          "UPDATE <TABLE_NAME> SET [COL1]=[VAL1], [COL2]=[VAL2], [...]=[...] WHERE 1",
                          "DELETE FROM <TABLE_NAME> WHERE 1",
                          ""
                        };
public slots:
    void onSendQuery(void);
    void onBackButton(void);

signals:
    void queryExecuted(void);
};

#endif // QUERYDIALOG_H
