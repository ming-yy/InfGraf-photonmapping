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

// Clase que representa un objeto triangular (plano)
// Hereda de la clase Primitiva
class Triangulo : public Primitiva {
public:
    Punto p0, p1, p2;
    Direccion n0, n1, n2;
    float u0, u1, u2;
    float v0, v1, v2;

    Triangulo();
    Triangulo(const Punto& _p0, const Punto& _p1, const Punto& _p2, 
              const RGB& _reflectancia = RGB(1.0f, 1.0f, 1.0f),
              const string _material = "difuso", const string rutaTextura = "",
              const RGB& _power = RGB());

    Triangulo(const Punto& _p0, const Punto& _p1, const Punto& _p2,
              const float _u, const float _v, const RGB& _reflectancia = RGB(1.0f, 1.0f, 1.0f),
              const string _material = "difuso", const string rutaTextura = "",
              const RGB& _power = RGB());

    Triangulo(const Punto& _p0, const Punto& _p1, const Punto& _p2,
                     const float _u0, const float _u1, const float _u2,
                     const float _v0, const float _v1, const float _v2,
                     const Direccion& _n0, const Direccion& _n1, const Direccion& _n2,
                     const RGB& _reflectancia = RGB(1.0f, 1.0f, 1.0f),
                    const string _material = "difuso", const string rutaTextura = "",
                    const RGB& _power = RGB());
    
    // Método para calcular la intersección entre un rayo y el triángulo
    // Algoritmo usado: Möller–Trumbore
    //
    // Devuelve en <ptos> un vector con los puntos de intersección en UCS del rayo <rayo>
    // con el objeto. Si hay dos puntos de intersección, el primer elemento introducido
    // en el vector será el primer punto de intersección. Si hay intersección, también
    // devuelve los BSDFs del objeto en <coefs>.
    // IMPORTANTE: si el rayo tiene origen en un punto perteneciente a la primitiva, no cuenta.
    void interseccion(const Rayo& rayo, vector<Punto>& ptos, BSDFs& coefs) const override;
    
    // Funcion que, dado un punto, devuelve valor true y las coordenadas baricentricas
    // por los parametros por referencia <u> y <v>, o false si no se han podido calcular
    bool getCoordBaricentricas(const Punto& punto, float& u, float& v) const;

    // Funcion que devuelve "True" si y solo si el punto <p0> pertecene al triángulo.
    bool pertenece(const Punto& punto) const override;

    Direccion getNormal() const;
    
    // Funcion que devuelve la normal de la primitiva en el punto <punto>
    Direccion getNormal(const Punto& punto) const override;

    // Funcion que devuelve la normal interpolada por las normales de los 3 vertices
    // de la primitiva en el punto <punto>
    Direccion getNormalInterpolada(const Punto& punto) const;
    
    // Funcion que devuelve "True" si y solo si <punto> pertenece al triángulo
    // y además, es un punto lumínico del triángulo.
    bool puntoEsFuenteDeLuz(const Punto& punto) const override;
    
    // Funcion que devuelve un punto aleatorio del triángulo en UCS.
    // También devuelve en <prob> la probabilidad de muestrear dicho punto.
    Punto generarPuntoAleatorio(float& prob) const override;
    
    // Funcion que obtiene la posición del punto <pto> del triángulo en el eje U de la
    // textura correspondiente. Tenemos garantizado que <pto> pertenece al objeto.
    float getEjeTexturaU(const Punto& pto) const override;
    
    // Funcion que obtiene la posición del punto <pto> del triángulo en el eje V de la
    // textura correspondiente. Tenemos garantizado que <pto> pertenece al objeto.
    float getEjeTexturaV(const Punto& pto) const override;

    // Funcion que devuelve la distancia mas cercana al triangulo dado otro punto
    float distanciaPunto(const Punto& pto) const;
    
    // Debug
    void diHola() const override;

};
