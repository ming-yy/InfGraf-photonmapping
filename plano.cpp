//*****************************************************************
// File:   plano.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "plano.h"
#include "base.h"
#include <random>


Plano::Plano(): Primitiva(), n(0.0f, 0.0f, 0.0f), d(0.0f) {}

Plano::Plano(const Direccion& _n, float _d, const RGB& _reflectancia,
             const string _material, const bool _soyLuz) :
             Primitiva(_reflectancia, _material, _soyLuz), n(normalizar(_n)), d(_d){}

void Plano::interseccion(const Rayo& rayo, vector<Punto>& ptos,
                        BSDFs& coefs, bool& choqueConLuz) const {
    float denominador = dot(rayo.d, n);
    if (fabs(denominador) < MARGEN_ERROR) {    // Para evitar problemas de imprecision
        //cout << "No hay intersección, el rayo es paralelo al plano." << endl;
        return;
    }
    
    float numerador = (-1) * (d + dot(rayo.o, n));
    float t = numerador / denominador;
    if (t <= 0) {
        //cout << "No hay intersección en la dirección positiva del rayo." << endl;
        return;
    }
    
    Punto aux = rayo.o + rayo.d * t;
    ptos.push_back(aux);
    coefs = this->coeficientes;
    choqueConLuz = this->soyLuz;
    
    // DEBUG
    // cout << "Numerador: " << numerador << endl;
    // cout << "Denominador: " << denominador << endl;
    // cout << "t: " << t << endl;
    // cout << "Hay interseccion en el punto: " << aux << endl;
    
}

bool Plano::pertenece(const Punto& p0) const {
    Direccion aux(this->c.coord);
    return (dot(p0,this->n) + modulo(aux) == 0);
}

Direccion Plano::getNormal(const Punto& punto) const {
    return this->n;
}

bool Plano::soyFuenteDeLuz() const {
    return this->soyLuz;
}


Punto Plano::generarPuntoAleatorio() const {
    Direccion u, v;  // Vectores ortogonales en el plano
    construirBaseOrtonormal(this->n, u, v);
    
    // Generador de números aleatorios en el rango de la superficie del plano
    float minLimite = -0.5f;
    float maxLimite = 0.5f;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(minLimite, maxLimite);

    // Genera coordenadas aleatorias en el plano usando u y v
    float randomU = dist(gen);
    float randomV = dist(gen);

    // Calcula el punto en el sistema global
    Punto puntoAleatorio = c + u * randomU + v * randomV;
    return puntoAleatorio;
}


ostream& operator<<(ostream& os, const Plano& r)
{
    os << "\nNormal: " << r.n << "\nDistancia: " << r.d;
    os << "\nCoeficientes: " << r.coeficientes << endl << "\nSoyLuz: " << r.soyLuz << endl;
    return os;
}

void Plano::diHola() const {
    cout << "Soy plano: centro = " << this->c << ", normal = " << this->n << ", distancia = " << this->d << endl;
}
