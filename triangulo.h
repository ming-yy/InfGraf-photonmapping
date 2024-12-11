//*****************************************************************
// File:   triangulo.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#pragma once
#include "direccion.h"
#include <iostream>
#include <cmath>
#include "primitiva.h"
#include "utilidades.h"


class Triangulo : public Primitiva {
public:
    Punto v0, v1, v2;

    Triangulo();
    Triangulo(const Punto& _v0, const Punto& _v1, const Punto& _v2, 
              const RGB& _reflectancia = RGB(1.0f, 1.0f, 1.0f),
              const string _material = "difuso", const RGB& _power = RGB());

    // Método para calcular la intersección entre un rayo y el triángulo
    // Algoritmo usado: Möller–Trumbore
    //
    // Devuelve en <ptos> un vector con los puntos de intersección en UCS del rayo <rayo>
    // con el objeto. Si hay dos puntos de intersección, el primer elemento introducido
    // en el vector será el primer punto de intersección. Si hay intersección, también
    // devuelve los BSDFs del objeto en <coefs>.
    // IMPORTANTE: si el rayo tiene origen en un punto perteneciente a la primitiva, no cuenta.
    void interseccion(const Rayo& rayo, vector<Punto>& ptos, BSDFs& coefs) const override;
    
    // Mëtodo que devuelve "True" si y solo si el punto <p0> pertecene al triángulo.
    bool pertenece(const Punto& p0) const override;
    
    // Método que devuelve la normal de la primitiva en el punto <punto>
    Direccion getNormal(const Punto& punto) const override;
    
    // Método que devuelve "True" si y solo si <punto> pertenece al triángulo
    // y además, es un punto lumínico del triángulo.
    bool puntoEsFuenteDeLuz(const Punto& punto) const override;
    
    // Método que devuelve un punto aleatorio del triángulo en UCS.
    // También devuelve en <prob> la probabilidad de muestrear dicho punto.
    Punto generarPuntoAleatorio(float& prob) const override;
    
    // Debug
    void diHola() const override;
};
