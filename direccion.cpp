//*****************************************************************
// File:   direccion.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "direccion.h"


Direccion::Direccion() : PuntoDireccion() {}

Direccion::Direccion(const Direccion& d) : PuntoDireccion(d.coord) {}

Direccion::Direccion(float x, float y, float z) : PuntoDireccion(x, y, z) {}

Direccion::Direccion(array<float,3> _coord) : PuntoDireccion(_coord) {}

Matriz<4,1> Direccion::getCoordHomo() const {
    array<array<float, 1>, 4> d = {coord[0], coord[1], coord[2], 0.0f};
    return d;
}

Direccion Direccion::operator+(const Direccion& d) const {
    return Direccion(coord[0] + d.coord[0], coord[1] + d.coord[1], coord[2] + d.coord[2]);
}

Direccion Direccion::operator-() const {
    return Direccion(-coord[0], -coord[1], -coord[2]);
}

Direccion Direccion::operator-(const Direccion& d) const {
    return Direccion(coord[0] - d.coord[0], coord[1] - d.coord[1], coord[2] - d.coord[2]);
}

Direccion Direccion::operator*(const float escalar) const {
    return Direccion(coord[0] * escalar, coord[1] * escalar, coord[2] * escalar);
}

Direccion Direccion::operator/(const float escalar) const {
    if (escalar == 0) {
        throw invalid_argument("Error: Division por cero no permitida.");
    }
    
    return Direccion(coord[0] / escalar, coord[1] / escalar, coord[2] / escalar);
}

float Direccion::modulo() const {
    return sqrt(coord[0] * coord[0] + coord[1] * coord[1] + coord[2] * coord[2]);
}

float modulo(const Direccion& d) {
    return d.modulo();
}

Direccion Direccion::normalizar() const {
    float longitud = this->modulo();
    if (longitud == 0) {
        throw invalid_argument(
            "Error: No se puede calcular porque direccion tiene modulo cero.");
    }
    return *this / longitud;
}

Direccion normalizar(const Direccion& d) {
    return d.normalizar();
}

Direccion Direccion::productoVectorial(const Direccion& d) const {
    return Direccion(
        coord[1] * d.coord[2] - coord[2] * d.coord[1],
        coord[2] * d.coord[0] - coord[0] * d.coord[2],
        coord[0] * d.coord[1] - coord[1] * d.coord[0]
    );
}

Direccion cross(const Direccion& d1, const Direccion& d2) {
    return d1.productoVectorial(d2);
}

Direccion& Direccion::operator=(const Direccion& d) {
    if (this != &d) {  // Evitar auto-asignación
        coord = d.coord;  // Asigna las coordenadas
    }
    return *this;
}

Direccion Direccion::absoluto() const {
    return Direccion(abs(this->coord[0]), abs(this->coord[1]),
                     abs(this->coord[2]));
}

Direccion abs(const Direccion& d) {
    return Direccion(d.absoluto());
}
