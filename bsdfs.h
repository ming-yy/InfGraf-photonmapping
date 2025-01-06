//*****************************************************************
// File:   bsdfs.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 4.3 de Informática Gráfica
//*****************************************************************
#pragma once
#include <string>
#include <array>
#include "rgb.h"
#include "utilidades.h"


constexpr int KD_i = 0;  // coef difuso
constexpr int KS_i = 1;  // coef especular (reflexión)
constexpr int KT_i = 2;  // coef transmitancia (refracción)

// Clase que representa los coeficientes de reflexión difusa,
// reflexión especular y transmisión
class BSDFs {
public:
    // Coeficiente de reflexión difusa
    RGB kd;
    
    // Coeficiente de reflexión especular
    RGB ks;

    // Coeficiente de transmisión
    RGB kt;

    // Guarda los coeficientes de reflexión difusa, reflexión especular y transmisión
    // (en ese orden) dado el material, en crudo (sin color)
    array<float, 3> sinEmision;

    // Constructor base
    BSDFs();

    // Constructor dado un color y un material
    BSDFs(const RGB& _color, const string _material);

    // Constructor dado un color y los coeficientes explícitos en RGB
    BSDFs(const RGB& _color, const RGB& _kd, const RGB& _ks, const RGB& _kt);

    // Constructor dado un color y los coeficientes explícitos en arrays de 3 floats
    BSDFs(const RGB& _color, const array<float, 3> _kd, const array<float, 3> _ks,
          const array<float, 3> _kt);

    // Destructor
    ~BSDFs() = default;

    // Operador de asignación por copia de objeto de la misma clase
    BSDFs& operator=(const BSDFs& d);
    
    // Operador de salida por pantalla
    friend ostream& operator<<(ostream& os, const BSDFs& r);
};
