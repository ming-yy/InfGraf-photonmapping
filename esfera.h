//*****************************************************************
// File:   esfera.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************
#pragma once
#include <vector>
#include "primitiva.h"
#include "planeta.h"
#include "rgb.h"
#include "utilidades.h"

class Esfera: public Primitiva {
public:
    Punto centro;   // Generalmente en UCS
    float radio;
    
    
    Esfera();
    Esfera(const Punto& _centro, const float& _radio,
           const RGB& _reflectancia = RGB(1.0f, 1.0f, 1.0f),
           const string _material = "difuso", const RGB& _power = RGB());
    Esfera(const Planeta& p);
    
    // Método para calcular la intersección entre un rayo y la esfera
    //
    // Devuelve en <ptos> un vector con los puntos de intersección en UCS del rayo <rayo>
    // con el objeto. Si hay dos puntos de intersección, el primer elemento introducido
    // en el vector será el primer punto de intersección. Si hay intersección, también
    // devuelve los BSDFs del objeto en <coefs>.
    // IMPORTANTE: si el rayo tiene origen en un punto perteneciente a la primitiva, no cuenta.
    void interseccion(const Rayo& rayo, vector<Punto>& ptos, BSDFs& coefs) const override;
    
    // Mëtodo que devuelve "True" si y solo si el punto <p0> pertecene a la esfera.
    bool pertenece(const Punto& p0) const override;
    
    // Método que devuelve la normal de la primitiva en el punto <punto>
    Direccion getNormal(const Punto& punto) const override;
    
    // Método que devuelve "True" si y solo si <punto> pertenece a la esfera
    // y además, es un punto lumínico de la esfera.
    bool puntoEsFuenteDeLuz(const Punto& punto) const override;
    
    // Método que devuelve un punto aleatorio de la esfera en UCS.
    // También devuelve en <prob> la probabilidad de muestrear dicho punto.
    Punto generarPuntoAleatorio(float& prob) const override;
    
    // Debug
    void diHola() const override;
};
