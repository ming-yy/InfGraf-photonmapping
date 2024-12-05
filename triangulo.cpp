//*****************************************************************
// File:   triangulo.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "triangulo.h"
#include "utilidades.h"


Triangulo::Triangulo() : Primitiva(), v0(Punto()), v1(Punto()), v2(Punto()) {}

Triangulo::Triangulo(const Punto& _v0, const Punto& _v1, const Punto& _v2, const RGB& _reflectancia,
                     const string _material, const bool _soyLuz):
                     Primitiva(_reflectancia, _material, _soyLuz), v0(_v0), v1(_v1), v2(_v2){}

void Triangulo::interseccion(const Rayo& rayo, vector<Punto>& ptos,
                             BSDFs& coefs, bool& choqueConLuz) const {
    Direccion edge1 = v1 - v0;
    Direccion edge2 = v2 - v0;
    Direccion h = cross(rayo.d, edge2);
    float a = dot(edge1, h);

    if (fabs(a) < MARGEN_ERROR) {
        // cout << "No hay intersección, el rayo es paralelo al triángulo." << endl;
        return;
    }

    float f = 1.0f / a;
    Direccion s = rayo.o - v0;
    float u = f * dot(s, h);

    if (u < 0.0f || u > 1.0f) {
        return; // La intersección está fuera del triángulo
    }

    Direccion q = cross(s, edge1);
    float v = f * dot(rayo.d, q);

    if (v < 0.0f || u + v > 1.0f) {
        return; // La intersección está fuera del triángulo
    }

    float t = f * dot(edge2, q);
    if (t > MARGEN_ERROR) {
        ptos.push_back(Punto(rayo.o + rayo.d * t));
        coefs = this->coeficientes;
        choqueConLuz = soyLuz;
        return;
    } else {
        return ; // No hay intersección en la dirección del rayo
    }
}

bool Triangulo::pertenece(const Punto& p0) const {
    return false;
}

Direccion Triangulo::getNormal(const Punto& punto) const {
    Direccion d1 = v0 - v1;
    Direccion d2 = v1 - v2;
    return normalizar(cross(d1, d2));
}

bool Triangulo::soyFuenteDeLuz() const {
    return this->soyLuz;
}

Punto Triangulo::generarPuntoAleatorio() const {
    return Punto();
}

void Triangulo::diHola() const {
    cout << "Soy triangulo: p1 = " << this->v0 << endl;
}
