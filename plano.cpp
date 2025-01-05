//*****************************************************************
// File:   plano.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "plano.h"
#include "base.h"
#include <random>


Plano::Plano(): Primitiva(), n(0.0f, 0.0f, 0.0f), u(0.0f, 0.0f, 0.0f), v(0.0f, 0.0f, 0.0f),
                d(0.0f), minLimite(), maxLimite(), centro(), centroSinDistancia(), escalaTexturaX(1),
                escalaTexturaY(1) {}

Plano::Plano(const Direccion& _n, const float _d, const RGB& _reflectancia, const string _material,
             const RGB& _power, const float _minLimite, const float _maxLimite, const Punto& _c,
             const string rutaTextura, const float _escalaX, const float _escalaY) :
             Primitiva(_reflectancia, _material, _power, rutaTextura), n(normalizar(_n)),
             d(_d), centroSinDistancia(_c), escalaTexturaX(_escalaX),
            escalaTexturaY(_escalaY == -1.0f ? _escalaX : _escalaY) {
    try {
        if (!valeCero(_power) && (_minLimite >= _maxLimite)) {
            throw invalid_argument("Error: Limites incorrectos de la luz del plano [" +
                                   to_string(_n.coord[0]) + ", " + to_string(_n.coord[1]) +
                                   ", " + to_string(_n.coord[2]) + "]");
        } else {
            this->minLimite = _minLimite;
            this->maxLimite = _maxLimite;
            this->centro = _c - _n * d;
            
            construirBaseOrtonormal(normalizar(this->n), this->u, this->v);
        }
    } catch (const invalid_argument& e) {
        cerr << e.what() << endl;
    }
}

void Plano::interseccion(const Rayo& rayo, vector<Punto>& ptos, BSDFs& coefs) const {
    float denominador = dot(rayo.d, n);
    if (fabs(denominador) < MARGEN_ERROR_INTERSEC_PLANO) {    // Para evitar problemas de imprecision
        //cout << "No hay intersección, el rayo es paralelo al plano." << endl;
        //cout << coefs << endl;
        return;
    }
    
    float numerador = (-1) * (d + dot(rayo.o, n));
    float t = numerador / denominador;
    if (t <= MARGEN_ERROR_INTERSEC_PLANO) {
        //cout << "No hay intersección en la dirección positiva del rayo." << endl;
        //cout << coefs << endl;
        return;
    }
    
    Punto aux = rayo.o + rayo.d * t;
    ptos.push_back(aux);
    coefs = this->coeficientes;
}

bool Plano::pertenece(const Punto& p0) const {
    //cout << "Pertenece: " << abs(dot(this->n, p0) + this->d) << endl;
    return abs(dot(this->n, p0) + this->d) <= MARGEN_ERROR_PERTENECE_PLANO;
}

float Plano::distancia(const Punto& p0) const {
    return dot(this->n, p0) + this->d;
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

    // Proyectamos el punto en la base del plano
    float coordU = dot(punto - this->centro, this->u);
    float coordV = dot(punto - this->centro, this->v);

    // Verificamos que las coordenadas proyectadas están dentro de los límites
    bool dentroLimites = (coordU >= this->minLimite - MARGEN_ERROR && coordU <= this->maxLimite + MARGEN_ERROR) &&
                         (coordV >= this->minLimite - MARGEN_ERROR && coordV <= this->maxLimite + MARGEN_ERROR);

    return dentroLimites;
}

Punto Plano::generarPuntoAleatorio(float& prob) const {
    float limite = abs(this->maxLimite - this->minLimite);
    float areaPlano = limite * limite;
    prob = 1.0f / areaPlano;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(this->minLimite, this->maxLimite);

    // Generamos coordenadas aleatorias en el plano usando u y v
    float randomU = dist(gen);
    float randomV = dist(gen);

    // En UCS
    Punto puntoAleatorio = this->centro + this->u * randomU + this->v * randomV;
    
    return puntoAleatorio;
}


float Plano::getEjeTexturaU(const Punto& pto) const {
    float u_prima = (-dot(pto - this->centro, this->u)) / this->escalaTexturaY;
    float u_textura = u_prima - static_cast<int>(u_prima);
    return (u_textura < 0) ? u_textura + 1 : u_textura;
}

float Plano::getEjeTexturaV(const Punto& pto) const {
    float v_prima = dot(pto - this->centro, this->v) / this->escalaTexturaX;
    float v_textura = v_prima - static_cast<int>(v_prima);
    return (v_textura < 0) ? v_textura + 1 : v_textura;
}


ostream& operator<<(ostream& os, const Plano& r) {
    os << "\nNormal: " << r.n << "\nDistancia: " << r.d;
    os << "\nCoeficientes: " << r.coeficientes << endl << "\nPower: " << r.power << endl;
    return os;
}

void Plano::diHola() const {
    cout << "Soy plano: normal = " << this->n << ", distancia = " << this->coeficientes << endl;
}


