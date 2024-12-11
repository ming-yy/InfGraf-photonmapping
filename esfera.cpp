//*****************************************************************
// File:   esfera.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#include <cmath>
#include <random>
#include "esfera.h"


Esfera::Esfera(): Primitiva(), centro(Punto()), radio(0.0f) {}

Esfera::Esfera(const Punto& _centro, const float& _radio, const RGB& _reflectancia,
               const string _material, const RGB& _power) :
               Primitiva(_reflectancia, _material, _power), centro(_centro), radio(_radio)  {}

Esfera::Esfera(const Planeta& p) : Primitiva(), centro(p.centro), radio(p.radio) {}

void Esfera::interseccion(const Rayo& rayo, vector<Punto>& ptos, BSDFs& coefs) const {
    float a = modulo(rayo.d);
    a *= a;
    float b = 2 * dot(rayo.d, rayo.o - this->centro);
    float c = (modulo(rayo.o - this->centro) * modulo(rayo.o - this->centro))
                - (this->radio * this->radio);
    
    // DEBUG
    //cout << "a = " << a << "\nb = " << b << "\nc = " << c << endl;
    
    float discriminante = b * b - 4 * a * c;    // Cálculo del discriminante
    if (discriminante > 0) {    // Caso de soluciones reales
        float t1 = (-b + sqrt(discriminante)) / (2 * a);
        float t2 = (-b - sqrt(discriminante)) / (2 * a);
        Punto p1 = rayo.o + rayo.d * t1;
        Punto p2 = rayo.o + rayo.d * t2;
        if (t1 > MARGEN_ERROR && t2 > MARGEN_ERROR) {
            if (modulo(rayo.o - p1) < modulo(rayo.o - p2)) {    // Primero interseca con p1
                ptos.push_back(p1);
                ptos.push_back(p2);
            } else {             // Primero interseca con p2
                ptos.push_back(p2);
                ptos.push_back(p1);
            }
            
        } else if (t1 > MARGEN_ERROR) {
            ptos.push_back(p1);
            
        } else if (t2 > MARGEN_ERROR) {
            ptos.push_back(p2);
        }
        
        coefs = this->coeficientes;
        // DEBUG
        //cout << "Hay 2 puntos de interseccion: " << endl;
        //cout << "t1 = " << t1 << " --> " << p1 << endl;
        //cout << "t2 = " << t2 << " --> " << p2 << endl;
        //cout << "Primero intersecara con: ";
        //cout << puntoInterseccion << endl;
    } else if (discriminante == 0) {  // Caso de una solución única
        float t = -b / (2 * a);
        Punto puntoInterseccion = rayo.o + rayo.d * t;
        if (t > MARGEN_ERROR) {
            ptos.push_back(puntoInterseccion);
            coefs = this->coeficientes;
        }
        // DEBUG
        //cout << "Hay 1 punto de interseccion (tngente): " << endl;
        //cout << "t = " << t << " --> " << puntoInterseccion << endl;
    }
    //else {  // Caso no hay solución real (soluciones complejas)
        // DEBUG
        //cout << "No hay puntos de intersección!" << endl;
        //resVal = false;
    //}
}

bool Esfera::pertenece(const Punto& p0) const {
    float distancia = modulo(p0 - this->centro);
    return abs(distancia - this->radio) <= MARGEN_ERROR;
}

Direccion Esfera::getNormal(const Punto& punto) const {
    return normalizar(punto - centro);
}

bool Esfera::puntoEsFuenteDeLuz(const Punto& punto) const {
    return pertenece(punto) && soyFuenteDeLuz();
}

Punto Esfera::generarPuntoAleatorio(float& prob) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    std::uniform_real_distribution<float> distAngle(0.0f, 2.0f * M_PI);

    // Genera dos ángulos aleatorios para coordenadas esféricas
    float theta = acos(1.0f - 2.0f * dist(gen)); // Distribución uniforme en una esfera
    float phi = distAngle(gen);

    // Coordenadas esféricas --> cartesianas
    float x = radio * sin(theta) * cos(phi);
    float y = radio * sin(theta) * sin(phi);
    float z = radio * cos(theta);

    Punto puntoAleatorio(this->centro.coord[0] + x, this->centro.coord[1] + y, this->centro.coord[2] + z);
    float areaSuperficie = 4.0f * M_PI * radio * radio;
    prob = 1.0f / areaSuperficie;

    return puntoAleatorio;
}

void Esfera::diHola() const {
    cout << "Soy esfera: centro = " << this->centro << ", radio = " << this->radio << endl;
}
