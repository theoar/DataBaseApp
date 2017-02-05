#include "products.h"

Products::Products(int IDP, int IDK, QString N, double C, int S, double P) :
    IDproduktu(IDP), IDkategorii(IDK), Nazwa(N), Cena(C), Stan(S), Pojemnosc(P)
{

}

int Products::getIDproduktu() const
{
    return IDproduktu;
}

void Products::setIDproduktu(int value)
{
    IDproduktu = value;
}
int Products::getIDkategorii() const
{
    return IDkategorii;
}

void Products::setIDkategorii(int value)
{
    IDkategorii = value;
}
QString Products::getNazwa() const
{
    return Nazwa;
}

void Products::setNazwa(const QString &value)
{
    Nazwa = value;
}
double Products::getCena() const
{
    return Cena;
}

void Products::setCena(double value)
{
    Cena = value;
}
int Products::getStan() const
{
    return Stan;
}

void Products::setStan(int value)
{
    Stan = value;
}
double Products::getPojemnosc() const
{
    return Pojemnosc;
}

void Products::setPojemnosc(double value)
{
    Pojemnosc = value;
}






