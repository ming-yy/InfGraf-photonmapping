//*****************************************************************
// File:   luzpuntual.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 4 de Informática Gráfica
//*****************************************************************

#include "luzpuntual.h"


LuzPuntual::LuzPuntual(): c(0.0f, 0.0f, 0.0f), p(1.0f, 1.0f, 1.0f) {}

LuzPuntual::LuzPuntual(Punto _c, RGB _p): c(_c), p(_p) {}
