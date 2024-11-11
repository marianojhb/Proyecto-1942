#pragma once
#include <string>

class Fecha
{
public:
    Fecha(int d = 0, int m = 0, int a = 0);
    void cargar();
    void mostrar();
    void validarFecha();
    bool validarDia();
    bool validarMes();
    bool validarAnio();
    bool esBisiesto(int);
    std::string toString();
    void agregarDia();
    void agregarDias(int);
    void restarDias(int);
    void setDefaultFecha();
    void restarDia();

    ///Setters
    void setDia(int);
    void setMes(const int);
    void setAnio(int);

    ///Getters
    int getDia();
    int getMes();
    int getAnio();

private:
    int _dia, _mes, _anio;
};
