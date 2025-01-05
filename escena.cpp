//*****************************************************************
// File:   escena.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#include "escena.h"
#include <memory>

Escena::Escena(): primitivas(vector<Primitiva*>()) {}

Escena::Escena(vector<Primitiva*> _primitivas, vector<LuzPuntual> _luces):
               primitivas(_primitivas), luces(_luces) {}


bool Escena::interseccion(const Rayo& rayo, Punto& ptoMasCerca, Direccion& normal,
                          Primitiva** objIntersecado) const {
    bool resVal = false;
    bool primerIntersec = true;  // Flag: la primera intersección encontrada

    for (Primitiva* objeto : this->primitivas) {
        vector<Punto> interseciones;
        BSDFs coefsAux;

        objeto->interseccion(rayo, interseciones, coefsAux);
        if (!interseciones.empty()) {
            resVal = true;
            // El intersec[0] es el punto más cercano al origen del rayo en este objeto
            if (primerIntersec || (modulo(rayo.o - interseciones[0]) < modulo(rayo.o - ptoMasCerca))) {
                *objIntersecado = objeto;
                ptoMasCerca = interseciones[0];
                normal = objeto->getNormal(ptoMasCerca);
                primerIntersec = false;
            }
        }
    }

    return resVal;
}

bool Escena::puntoPerteneceALuz(const Punto& p0, RGB& powerLuzArea) const {
    bool resVal = false;
    for (const Primitiva* primitiva : this->primitivas) {
        if (primitiva->puntoEsFuenteDeLuz(p0)) {
            resVal = true;
            powerLuzArea = primitiva->power;
            break;
        }
    }
    return resVal;
}

bool Escena::luzIluminaPunto(const Punto& p0, const LuzPuntual& luz) const {
    bool iluminar = true;
    Direccion d = normalizar(luz.c - p0);
    Punto ptoMasCerca;
    Direccion normal;
    Primitiva* primitiva = nullptr;
    bool chocaObjeto = this->interseccion(Rayo(d, p0), ptoMasCerca, normal, &primitiva);
    
    if (chocaObjeto) {
        iluminar = modulo(luz.c - p0) <= modulo(ptoMasCerca - p0);
    }
    
    return iluminar;
}

bool Escena::luzIluminaPunto(const Punto& p0, const Primitiva* luz, Punto& origenLuz, float& prob) const {
    bool iluminar = false;
    int numIters = NUM_MUESTRAS_LUZ_AREA;     // Tiene que ir en función del tamaño del plano
    for (int i = 0; i < numIters && !iluminar; ++i) {
        Punto ptoRandom = luz->generarPuntoAleatorio(prob);
        Direccion d = normalizar(ptoRandom - p0);
        Punto ptoMasCerca;
        Direccion normal;
        Primitiva* primitiva = nullptr;
        // Rayo desde punto a iluminar (p0) --> ptoRandom de la luz
        bool chocaObjeto = this->interseccion(Rayo(d, p0), ptoMasCerca, normal, &primitiva);
        
        if (chocaObjeto) {  // Miramos si el objeto más cercano contra el que choca es esa luz en <ptoRandom>
            iluminar = modulo(ptoRandom - p0) <= modulo(ptoMasCerca - p0);
            origenLuz = ptoRandom;
        }
    }
    return iluminar;
}

bool Escena::puntoIluminado(const Punto& p0) const {
    for(const LuzPuntual& luz : this->luces) {
        if (luzIluminaPunto(p0, luz)) return true;
    }
    
    Punto origenLuz;
    float prob;
    for(auto objeto : this->primitivas) {
        if (luzIluminaPunto(p0, objeto, origenLuz, prob)) return true;
    }
    
    return false;
}
