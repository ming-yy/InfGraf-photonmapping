//*****************************************************************
// File:   plano.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#pragma once
#include <iostream>
#include <vector>
#include "punto.h"
#include "direccion.h"
#include "primitiva.h"
#include "utilidades.h"
#include "rgb.h"


class Plano : public Primitiva {
public:
    Punto c;        // Centro del plano
    Direccion n;    // Normal respecto al plano (generalmente en UCS)
    float d;        // Distancia máxima admitida

    
    Plano();
    Plano(const Direccion& _n, float _d, const RGB& _reflectancia = RGB(1.0f, 1.0f, 1.0f),
                     const string _material = "difuso", const bool _soyLuz = false);

    // Método para calcular la intersección entre un rayo y el plano
    //
    // Devuelve en <ptos> un vector con los puntos de intersección en UCS del rayo <rayo>
    // con el objeto. Si hay dos puntos de intersección, el primer elemento introducido
    // en el vector será el primer punto de intersección. Si hay intersección, también
    // devuelve los BSDFs del objeto en <coefs> y si el objeto intersecado es luz o no en
    // <choqueConLuz>.
    // IMPORTANTE: si el rayo tiene origen en un punto perteneciente a la primitiva, no cuenta.
    void interseccion(const Rayo& rayo, vector<Punto>& ptos,
                      BSDFs& coefs, bool& choqueConLuz) const override;
    
    // Mëtodo que devuelve "True" si y solo si el punto <p0> pertecene al plano.
    bool pertenece(const Punto& p0) const override;
    
    // Método que devuelve la normal de la primitiva en el punto <punto>
    Direccion getNormal(const Punto& punto) const override;
    
    // Método que devuelve <True> si y solo si esta primitiva es una fuente de luz.
    bool soyFuenteDeLuz() const override;
    
    // Método que devuelve un punto aleatorio de la superficie del plano en UCS.
    Punto generarPuntoAleatorio() const override;

    // Función para mostrar por pantalla la información del plano
    friend ostream& operator<<(ostream& os, const Plano& p);
    
    // Debug
    void diHola() const override;
};
