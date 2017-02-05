#ifndef PRODUCTS_H
#define PRODUCTS_H

#include <QString>

class Products
{
public:
    Products(int IDP, int IDK, QString N, double C, int S, double P);

    int getIDproduktu() const;
    void setIDproduktu(int value);

    int getIDkategorii() const;
    void setIDkategorii(int value);

    QString getNazwa() const;
    void setNazwa(const QString &value);

    double getCena() const;
    void setCena(double value);

    int getStan() const;
    void setStan(int value);

    double getPojemnosc() const;
    void setPojemnosc(double value);

private:
    int IDproduktu;
    int IDkategorii;
    QString Nazwa;
    double Cena;
    int Stan;
    double Pojemnosc;
};

#endif // PRODUCTS_H
