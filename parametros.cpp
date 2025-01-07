#include "parametros.h"

Parametros::Parametros(const unsigned _numPxlsAncho,
                const unsigned _numPxlsAlto,
                const unsigned _rpp,
                const int _numRandomWalks,
                TipoVecinos _tipoVecinosGlobales,
                unsigned long _vecinosGlobalesNum,
                float _vecinosGlobalesRadio,
                TipoVecinos _tipoVecinosCausticos,
                unsigned long _vecinosCausticosNum,
                float _vecinosCausticosRadio,
                const bool _nee,
                const bool _luzIndirecta,
                const bool _printPixelesProcesados
                )

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
                vecinosCausticosRadio(_vecinosCausticosRadio),
                luzIndirecta(_luzIndirecta),
                nee(_nee)
                {}

