//*****************************************************************
// File:   puntoDireccion.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "puntoDireccion.h"


PuntoDireccion::PuntoDireccion(): coord({0.0f, 0.0f, 0.0f}) {}

PuntoDireccion::PuntoDireccion(float x, float y, float z) : coord({x, y, z}) {}

PuntoDireccion::PuntoDireccion(array<float,3> _coord) : coord(_coord) {}

float PuntoDireccion::productoEscalar(const PuntoDireccion& d) const {
    return coord[0] * d.coord[0] + coord[1] * d.coord[1] + coord[2] * d.coord[2];
}

float dot(const PuntoDireccion& d1, const PuntoDireccion& d2) {
    return d1.productoEscalar(d2);
}

ostream& operator<<(ostream& os, const PuntoDireccion& r)
{
    os << "[" << r.coord[0] << ", " << r.coord[1] << ", " << r.coord[2] << "]";

    return os;
}
