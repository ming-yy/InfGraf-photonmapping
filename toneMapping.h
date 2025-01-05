//*****************************************************************
// File:   toneMapping.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include <vector>
#include <cmath>
#include "utilidades.h"

// Metodo que aplica una funcion de Clamping a <valores>
//
// Los valores que sobrepasan 1 se convierten en 1
// Los que quedan por debajo de 1, se normalizan
void clamp(vector<float>& valores);

// Metodo que aplica una funcion de Equalization a <valores>
//
// Los valores que sobrepasan <maxValue> se convierten en 1
// Los que quedan por debajo de <maxValue>, se normalizan
void equalize(vector<float>& valores, const float maxValue);


// Metodo que aplica una funcion de Clamping+Equalization
void clampAndEqualize(vector<float>& valores, const float threshold);

// Metodo que aplica una funcion de Gamma+Equalization a <valores>
//
// Los valores que sobrepasan <maxValue> se convierten en 1 y se elevan a 1/GAMMA
// Los que quedan por debajo de <maxValue>, se normalizan y se elevan a 1/GAMMA
void gamma(vector<float>& valores, const float maxValue);

// Metodo que aplica una funcion de Gamma+Equalization+Clamping a <valores>
void gammaAndClamp(vector<float>& valores, const float threshold);

// Metodo que aplica una funcion de Reinhard a <valores>
//
// Primero aplica equalization con un valor lmax/2.5,
// luego, aplica la formula (c * (1 + (c / pow(lmax, 2)))) / (1 + c)
// al cada valor <c> de <valores>
void reinhard(vector<float>& valores, const float lmax);
