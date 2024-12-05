//*****************************************************************
// File:   toneMapping.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include <vector>
#include <cmath>
#include "utilidades.h"


void clamp(vector<float>& valores);

void equalize(vector<float>& valores, const float maxValue);

void clampAndEqualize(vector<float>& valores, const float threshold);

void gamma(vector<float>& valores, const float maxValue);

void gammaAndClamp(vector<float>& valores, const float threshold);
