#ifndef PRINTEREGZAMPLE_H
#define PRINTEREGZAMPLE_H

#include <QMainWindow>

namespace Ui {
class PrinterEgzample;
}

class PrinterEgzample : public QMainWindow
{
    Q_OBJECT

public:
    explicit PrinterEgzample(QWidget *parent = 0);
    ~PrinterEgzample();

private:
    Ui::PrinterEgzample *ui;
public slots:
    void onPrint();
};

#endif // PRINTEREGZAMPLE_H
