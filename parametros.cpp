#include "parametros.h"

Parametros::Parametros(const unsigned _numPxlsAncho,
                       const unsigned _numPxlsAlto,
                       const unsigned _rpp,
                       const int _numRandomWalks,
                       TipoVecinos _tipoEstimacionVecinos,
                       const unsigned long _vecinos,
                       const bool _printPixelesProcesados)

                        : rpp(_rpp),
                        numRandomWalks(_numRandomWalks),
                        numPxlsAncho(_numPxlsAncho),
                        numPxlsAlto(_numPxlsAlto),
                        printPixelesProcesados(_printPixelesProcesados),
                        tipoEstimacionVecinos(_tipoEstimacionVecinos),
                        vecinos(_vecinos)
                        {}

