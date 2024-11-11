#include <iostream>
#include "RankingArchivo.h"
#include "CompatibilidadMacos.h"

RankingArchivo::RankingArchivo(const char* url)
{
  _pRanking = nullptr;
  _url = url;
}

void RankingArchivo::setPRanking(FILE * pRanking)
{
  _pRanking = pRanking;
}

bool RankingArchivo::leerRegistrosRanking()
{
//  Ranking reg;
//  setPRanking(fopen(_url,"rb"));
//  if(_pRanking==nullptr)
//  {
//    std::cerr << "Error al abrir el archivo para leer" << std::endl;
//    return false;
//  } else
//  {
//    std::cout << "--- DATOS LEIDOS CORRECTAMENTE... ---" << std::endl;
//  }
//  limpiar();
//
//  while(fread(&reg,sizeof(Ranking),1,_pRanking)==1)
//  {
//    reg.mostrar();
//    std::cout << "----------------o---------------" << std::endl;
//  }
//  pausar();
//  fclose(_pRanking);
  return true;
}

bool RankingArchivo::grabarRegistroRanking(Ranking &reg)
{
//  if ( buscaLegajo(reg.getLegajo()).getLegajo() == 0 )
//  {
//    _pRanking = fopen(_url,"ab");
//    if (_pRanking == nullptr)
//    {
//      std::cerr << "Error al abrir el archivo para escribir." << std::endl;
//      return false;
//    }
//    fwrite(&reg,sizeof(Ranking),1,_pRanking);
//    fclose(_pRanking);
//  }
//  else
//  {
//    std::cerr << std::endl <<"El legajo ya existe. No se guardaron los datos" << std::endl;
//    pausar();
//    return false;
//  }
  return true;
}

RankingArchivo::~RankingArchivo()
{
  if(_pRanking != nullptr)
    fclose(_pRanking);
}

Ranking RankingArchivo::buscaPosicion(int legajo)
{
  Ranking reg;
//  int res; // 0 si no se encontro el registro, o la posicion donde se encontro
//  _pRanking = fopen(_url, "rb");
//
//  while(fread(&reg,sizeof(Ranking),1,_pRanking)==1)
//  {
//    if(reg.getPosicion() == legajo)
//    {
//      return reg;
//    }
//  }
//  reg.setPosicion(0);
  return reg;

}