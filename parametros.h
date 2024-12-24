//*****************************************************************
// File:   parametros.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   diciembre 2024
// Coms:   Práctica 5 de Informática Gráfica
//*****************************************************************
#pragma once

enum TipoVecinos {
    RADIO = 0,
    PORCENTAJE = 1,
    NUMERO = 2,
    RADIONUMERO = 3
};

class Parametros {
public:
    unsigned numPxlsAncho;
    unsigned numPxlsAlto;
    unsigned rpp;
    int numRandomWalks;
    TipoVecinos tipoVecinosGlobales;
    unsigned long vecinosGlobalesNum;
    unsigned long vecinosGlobalesRadio;
    TipoVecinos tipoVecinosCausticos;
    unsigned long vecinosCausticosNum;
    unsigned long vecinosCausticosRadio;
    bool luzIndirecta;
    bool printPixelesProcesados;

    Parametros(const unsigned _numPxlsAncho,
                const unsigned _numPxlsAlto,
                const unsigned _rpp,
                const int _numRandomWalks,
                TipoVecinos _tipoVecinosGlobales,
                unsigned long _vecinosGlobalesNum,
                unsigned long _vecinosGlobalesRadio,
                TipoVecinos _tipoVecinosCausticos,
                unsigned long _vecinosCausticosNum,
                unsigned long _vecinosCausticosRadio,
                bool luzIndirecta,
                const bool _printPixelesProcesados);
};