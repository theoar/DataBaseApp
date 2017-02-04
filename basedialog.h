#ifndef BASEDIALOG_H
#define BASEDIALOG_H

#include <QDialog>
#include <QMap>
#include <QVariant>

class BaseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit BaseDialog(QWidget *parent = 0);

signals:
    void accepted(QMap<QString, QVariant> vect);
public slots:

};

#endif // BASEDIALOG_H
