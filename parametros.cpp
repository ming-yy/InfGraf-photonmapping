#include "parametros.h"

Parametros::Parametros(const unsigned _numPxlsAncho,
                const unsigned _numPxlsAlto,
                const unsigned _rpp,
                const int _numRandomWalks,
                TipoVecinos _tipoVecinosGlobales,
                unsigned long _vecinosGlobalesNum,
                unsigned long _vecinosGlobalesRadio,
                TipoVecinos _tipoVecinosCausticos,
                unsigned long _vecinosCausticosNum,
                unsigned long _vecinosCausticosRadio,
                const bool _printPixelesProcesados)

                : rpp(_rpp),
                numRandomWalks(_numRandomWalks),
                numPxlsAncho(_numPxlsAncho),
                numPxlsAlto(_numPxlsAlto),
                printPixelesProcesados(_printPixelesProcesados),
                tipoVecinosGlobales(_tipoVecinosGlobales),
                vecinosGlobalesNum(_vecinosGlobalesNum),
                vecinosGlobalesRadio(_vecinosGlobalesRadio),
                tipoVecinosCausticos(_tipoVecinosCausticos),
                vecinosCausticosNum(_vecinosCausticosNum),
                vecinosCausticosRadio(_vecinosCausticosRadio)
                {}

