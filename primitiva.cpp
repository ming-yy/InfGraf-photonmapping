//*****************************************************************
// File:   primitiva.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   noviembre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#include "primitiva.h"
#include "bsdfs.h"
#include <stdexcept>
#include <array>


Primitiva::Primitiva(): coeficientes(BSDFs()), power(RGB()) {}

Primitiva::Primitiva(const RGB& color, const string material, const RGB& _power) {
    this->coeficientes = BSDFs(color, material);
    this->power = _power;
}

Primitiva::Primitiva(const RGB& color, const array<float, 3> kd, const array<float, 3> ks,
                     const array<float, 3> kt, const RGB& _power) {
    this->coeficientes = BSDFs(color, kd, ks, kt);
    this->power = _power;
}

bool Primitiva::soyFuenteDeLuz() const {
    return !valeCero(this->power);
}
