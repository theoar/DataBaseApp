#include "printeregzample.h"
#include "ui_printeregzample.h"

PrinterEgzample::PrinterEgzample(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PrinterEgzample)
{
    ui->setupUi(this);
}

PrinterEgzample::~PrinterEgzample()
{
    delete ui;
}
