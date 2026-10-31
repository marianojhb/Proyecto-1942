// ConfiguracionArchivo.cpp

#include <iostream>
#include "ConfiguracionArchivo.h"

ConfiguracionArchivo::ConfiguracionArchivo()
{
  _pConfiguracion = nullptr;
  _url = "assets/config/configuracion.dat";
}

ConfiguracionArchivo::ConfiguracionArchivo(const char* url)
{
  _pConfiguracion = nullptr;
  _url = url;
}

void ConfiguracionArchivo::setPConfiguracion(FILE * pRanking)
{
  _pConfiguracion = pRanking;
}

bool ConfiguracionArchivo::grabarConfiguracion(Configuracion &reg)
{

  _pConfiguracion = fopen(_url,"wb+");
  if (_pConfiguracion == nullptr)
  {
    std::cerr << "Error al abrir el archivo para escribir." << std::endl;
    return false;
  }
  fwrite(&reg,sizeof(Configuracion),1,_pConfiguracion);
  fclose(_pConfiguracion);
  _pConfiguracion = nullptr;
  std::cout << "Datos guardados correctamente en configuracion.dat" << std::endl;
  return true;
}

ConfiguracionArchivo::~ConfiguracionArchivo()
{
  if(_pConfiguracion != nullptr)
    fclose(_pConfiguracion);
}

Configuracion* ConfiguracionArchivo::obtenerConfiguracion()
{
  auto *reg = new Configuracion[1];
  reg[0].cargarConfiguracion(true, true);
  _pConfiguracion = fopen(_url, "rb");
  if (_pConfiguracion == nullptr) {
    std::cerr << "No se pudo abrir la configuracion; se usan valores por defecto." << std::endl;
    return reg;
  }
  if (fread(&reg[0], sizeof(Configuracion), 1, _pConfiguracion) != 1) {
    reg[0].cargarConfiguracion(true, true);
  }
  fclose(_pConfiguracion);
  _pConfiguracion = nullptr;

  return reg;
}
