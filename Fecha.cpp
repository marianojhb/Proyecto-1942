#include <iostream>
#include <string>
#include <ctime>
#include "Fecha.h"

using namespace std;

Fecha::Fecha(int dia, int mes, int anio)
{
  setDia(dia);
  setMes(mes);
  setAnio(anio);
  validarFecha();
}
bool Fecha::validarDia()
{
  return this->_dia >= 1 && this->_dia <= 31;
}
bool Fecha::validarMes()
{
  return this->_mes >= 1 && this->_mes <=12;
}
bool Fecha::validarAnio()
{
  return this->_anio > 0;
}
void Fecha::validarFecha()
{
  if ( !validarDia()
       || !validarMes()
       || !validarAnio()
          ) {
    setDefaultFecha();
  }
  // anio bisiesto
  if (this->getDia()==29 && !esBisiesto(this->getAnio()))
  {
    setDefaultFecha();
  }
}
void Fecha::setDefaultFecha()
{
  std::time_t t = std::time(nullptr);
  std::tm* now = std::localtime(&t);

  setDia(now->tm_mday);
  setMes(now->tm_mon + 1);
  setAnio(now->tm_year + 1900);
}
int Fecha::getDia() { return _dia; }
int Fecha::getMes() { return _mes; }
int Fecha::getAnio() { return _anio; }
void Fecha::setDia(int dia)  { _dia = dia; }
void Fecha::setMes(int mes) { _mes = mes; }
void Fecha::setAnio(int anio) { _anio = anio; }
void Fecha::agregarDia()
{
  _dia++;

  if(_mes==1 || _mes == 3 || _mes == 5 || _mes==7 ||_mes== 8 || _mes == 10 || _mes == 12)
  {
    if (_dia == 32) {
      _dia = 1;

      _mes++;
      if (_mes == 13)
      {
        _mes=1;
        _anio++;
      }
    }
  }
  else if (_mes == 4 || _mes== 6 || _mes == 9 || _mes == 11)
  {
    if (_dia == 31) {
      _dia = 1;

      _mes++;
      if (_mes == 13)
      {
        _mes=1;
        _anio++;
      }
    }
  }
  else if (_mes == 2)
  {
    if(esBisiesto(_anio)==1)
    {
      if (_dia == 30) {
        _dia = 1;

        _mes++;
        if (_mes == 13)
        {
          _mes=1;
          _anio++;
        }
      }
    }
    else
    {
      if (_dia == 29) {
        _dia = 1;

        _mes++;
        if (_mes == 13)
        {
          _mes=1;
          _anio++;
        }
      }
    }
  }
}
void Fecha::restarDia()
{
  _dia--;

  if(_mes==1 || _mes == 2 || _mes == 4 || _mes==6 ||_mes== 8 || _mes == 9 || _mes == 11)
  {
    if (_dia == 0) {
      _dia = 31;

      _mes--;
      if (_mes == 0)
      {
        _mes=12;
        _anio--;
      }
    }
  }
  else if (_mes == 5 || _mes== 7 || _mes == 10 || _mes == 12)
  {
    if (_dia == 0) {
      _dia = 30;

      _mes--;
      if (_mes == 0)
      {
        _mes=12;
        _anio--;
      }
    }
  }
  else if (_mes == 3)
  {
    if(esBisiesto(_anio)==1)
    {
      if (_dia == 0) {
        _dia = 29;
        _mes--;
      }
    }
    else
    {
      if (_dia == 0) {
        _dia = 28;
        _mes--;
      }
    }
  }
}
string Fecha::toString()
{
  return to_string(this->getDia()) + "/" + to_string(getMes())  + "/" + to_string(this->getAnio());
}
bool Fecha::esBisiesto(int a)
{
  bool res;
  if (a % 4 == 0) {
    res = true;
    if (a % 100 == 0) {
      res = false;
      if (a % 400 == 0) {
        res = true;
      }
    }
  }
  return res;
}
void Fecha::agregarDias(int dias)
{
  for(int i = 0; i <dias; i++)
  {
    agregarDia();
  }
}
void Fecha::restarDias(int dias)
{
  for(int i = 0; i <dias; i++)
  {
    restarDia();
  }
}