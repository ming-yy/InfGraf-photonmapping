//*****************************************************************
// File:   plano.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "plano.h"
#include "base.h"
#include <random>


Plano::Plano(): Primitiva(), n(0.0f, 0.0f, 0.0f), d(0.0f), minLimite(), maxLimite(), centro() {}

Plano::Plano(const Direccion& _n, const float _d, const RGB& _reflectancia, const string _material,
             const RGB& _power, const float _minLimite, const float _maxLimite, const Punto& _c) :
             Primitiva(_reflectancia, _material, _power), n(normalizar(_n)), d(_d), centro(_c) {
    try {
        if (!valeCero(_power) && (_minLimite >= _maxLimite)) {
            throw invalid_argument("Error: Limites incorrectos de la luz del plano [" +
                                   to_string(_n.coord[0]) + ", " + to_string(_n.coord[1]) +
                                   ", " + to_string(_n.coord[2]) + "]");
        } else {
            this->minLimite = _minLimite;
            this->maxLimite = _maxLimite;
        }
    } catch (const invalid_argument& e) {
        cerr << e.what() << endl;
    }
}

void Plano::interseccion(const Rayo& rayo, vector<Punto>& ptos, BSDFs& coefs) const {
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
}

bool Plano::pertenece(const Punto& p0) const {
    return abs(dot(this->n, p0) + this->d) < MARGEN_ERROR;
}

Direccion Plano::getNormal(const Punto& punto) const {
    return this->n;
}

bool Plano::puntoEsFuenteDeLuz(const Punto& punto) const {
    if (!this->soyFuenteDeLuz()) {
        return false;
    }
    
    if (!this->pertenece(punto)) {
        return false;
    }

    Direccion u, v;
    construirBaseOrtonormal(normalizar(this->n), u, v);

    // Proyectar el punto en la base del plano
    float coordU = dot(punto - this->centro, u);
    float coordV = dot(punto - this->centro, v);

    // Verificar si las coordenadas proyectadas están dentro de los límites
    bool dentroLimites = (coordU >= this->minLimite && coordU <= this->maxLimite) &&
                         (coordV >= this->minLimite && coordV <= this->maxLimite);
    
    //cout << "Proyección U: " << coordU << ", Proyección V: " << coordV << endl;
    //cout << "Límites U/V: [" << this->minLimite << ", " << this->maxLimite << "]" << endl;

    return dentroLimites;
}

Punto Plano::generarPuntoAleatorio(float& prob) const {
    Direccion u, v;  // Vectores ortogonales en el plano
    construirBaseOrtonormal(this->n, u, v);
    
    float limite = this->maxLimite - this->minLimite;
    float areaPlano = limite * limite;
    prob = 1.0f / areaPlano;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(this->minLimite, this->maxLimite);

    // Generamos coordenadas aleatorias en el plano usando u y v
    float randomU = dist(gen);
    float randomV = dist(gen);

    // En UCS
    Punto puntoAleatorio = this->centro + u * randomU + v * randomV;
    return puntoAleatorio;
}


ostream& operator<<(ostream& os, const Plano& r)
{
    os << "\nNormal: " << r.n << "\nDistancia: " << r.d;
    os << "\nCoeficientes: " << r.coeficientes << endl << "\nPower: " << r.power << endl;
    return os;
}

void Plano::diHola() const {
    cout << "Soy plano: normal = " << this->n << ", distancia = " << this->d << endl;
}
