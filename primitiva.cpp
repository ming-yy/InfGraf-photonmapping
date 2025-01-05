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


Primitiva::Primitiva(): coeficientes(BSDFs()), power(RGB()), textura(Textura()) {}

Primitiva::Primitiva(const RGB& color, const string material, const RGB& _power, const string& rutaTextura):
                     coeficientes(color, material), power(_power), textura(Textura(rutaTextura)) {}

Primitiva::Primitiva(const RGB& color, const array<float, 3> kd, const array<float, 3> ks,
                     const array<float, 3> kt, const RGB& _power, const string& rutaTextura):
                     coeficientes(color, kd, ks, kt), power(_power), textura(Textura(rutaTextura)) {}

bool Primitiva::soyFuenteDeLuz() const {
    return !valeCero(this->power);
}

bool Primitiva::tengoTextura() const {
    return !((this->textura.alto == 0) || (this->textura.ancho == 0));
}

RGB Primitiva::kd(const Punto& p) const {
    if (this->tengoTextura()) return this->kd_Textura(p);
    return this->coeficientes.kd;
}

RGB Primitiva::kd_Textura(const Punto& p) const {
    //int x = floor(this->textura.ancho * this->getEjeTexturaU(p)) + 1;
    //int y = floor(this->textura.alto * this->getEjeTexturaV(p)) + 1;
    
    
    float y = this->getEjeTexturaU(p);
    float x = this->getEjeTexturaV(p);
    
    float angulo = 0.0f; // 0.0f * M_PI / 2.0f;    // pi/2 -> 90º, pi -> 180º, 3*pi/2 -> 270º, 0 -> 0º
    // Esto se puede optimizar
    if (angulo != 0) {
        float x_1 = x * cos(angulo) - y * sin(angulo);
        float y_1 = x * sin(angulo) + y * cos(angulo);
        return this->textura.obtenerTextura(x_1, y_1) * this->coeficientes.sinEmision[KD_i];
    } else {
        return this->textura.obtenerTextura(x, y) * this->coeficientes.sinEmision[KD_i];
    }
}
