//*****************************************************************
// File:   luzpuntual.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 4 de Informática Gráfica
//*****************************************************************

#pragma once
#include "punto.h"
#include "rgb.h"


class LuzPuntual {
public:
    Punto c;
    RGB p;
    
    
    LuzPuntual();
    LuzPuntual(Punto _c, RGB _p = {1.0f, 1.0f, 1.0f});
};
