#pragma once
#include <string>
#include "Ranking.h"

class RankingArchivo
{
public:
    RankingArchivo(const char*);
    bool leerRegistrosRanking();
    bool grabarRegistroRanking(Ranking &);
    Ranking buscaPosicion(int);
    ~RankingArchivo();
    void setPRanking(FILE*);

private:
    FILE * _pRanking;
    const char* _url;
};

