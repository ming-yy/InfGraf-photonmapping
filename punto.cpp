//*****************************************************************
// File:   punto.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "punto.h"
#include <cmath>


Punto::Punto() : PuntoDireccion(), o{0.0f, 0.0f, 0.0f} {}

Punto::Punto(const Punto& p) : PuntoDireccion(p.coord), o{0.0f, 0.0f, 0.0f} {}

Punto::Punto(float x, float y, float z) : PuntoDireccion(x, y, z), o{0.0f, 0.0f, 0.0f}{}

Punto::Punto(array<float, 3> _coord) : PuntoDireccion(_coord), o{0.0f, 0.0f, 0.0f}{}

Matriz<4,1> Punto::getCoordHomo() const {
    array<array<float, 1>, 4> p = {coord[0], coord[1], coord[2], 1.0f};

    return p;
}

Punto Punto::operator+(const PuntoDireccion& pd) const {
    return Punto(coord[0] + pd.coord[0], coord[1] + pd.coord[1], coord[2] + pd.coord[2]);
}

Punto Punto::operator-(const Direccion& d) const {
    return Punto(coord[0] - d.coord[0], coord[1] - d.coord[1], coord[2] - d.coord[2]);
}

Direccion Punto::operator-(const Punto& p) const {
    return Direccion(coord[0] - p.coord[0], coord[1] - p.coord[1], coord[2] - p.coord[2]);
}

Punto Punto::operator*(const float escalar) const {
    return Punto(coord[0] * escalar, coord[1] * escalar, coord[2] * escalar);
}

Punto Punto::operator/(const float escalar) const {
    if (escalar == 0) {
        throw invalid_argument("Error: Division por cero no permitida.");
    }
    
    return Punto(coord[0] / escalar, coord[1] / escalar, coord[2] / escalar);
}

Punto& Punto::operator=(const Punto& p) {
    if (this != &p) {  // Evitar auto-asignación
        coord = p.coord;  // Asigna las coordenadas
        o = p.o;          // Asigna el otro atributo
    }
    return *this;  // Devuelve la referencia al objeto actual
}
