//*****************************************************************
// File:   mesh.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   diciembre 2024
// Coms:   Práctica 4 de Informática Gráfica
//*****************************************************************

#pragma once
#include "primitiva.h"
#include "utilidades.h"
#include "triangulo.h"
#include "esfera.h"

class Mesh : public Primitiva {
public:
    // Vector de triangulos que representan las caras de la malla 
    vector<Triangulo> triangulos;

    // Vector de puntos que representan los vertices de la malla  
    vector<Punto> vertices;

    // Esfera que envuelve a todos los puntos, y representa una especie
    // de "hitbox" que ayuda en optimizacion (si un rayo no interseca con
    // <esferaLimite>, no hace falta ver si interseca con cada uno de los triangulos)
    Esfera esferaLimite;

    // Constructor base
    Mesh();

    // Constructor a partir de la ruta del modelo (archivo .ply), con opcionales de una ruta
    // al archivo .ppm de su textura, la escala, el punto donde quiere centrarse la malla, 
    // las rotaciones o inversiones de los ejes X, Y y Z, un RGB que representa el color 
    // de la malla, el material y una potencia (emision) si emite luz (luz de area)
    Mesh(const string rutaModelo, const string rutaTextura = "", const float escala = 1.0f, const Punto& centro = Punto(0.0f, 0.0f, 0.0f),
        const float rotacionX = 0.0f, const bool invertirX = false, 
        const float rotacionY = 0.0f, const bool invertirY = false, 
        const float rotacionZ = 0.0f, const bool invertirZ = false,
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
    
    // Funcion que devuelve "True" si y solo si el punto <p0> pertecene al triángulo.
    bool pertenece(const Punto& p0) const override;
    
    // Funcion que devuelve la normal de la primitiva en el punto <punto>
    Direccion getNormal(const Punto& punto) const override;
    
    // Funcion que devuelve "True" si y solo si <punto> pertenece al triángulo
    // y además, es un punto lumínico del triángulo.
    bool puntoEsFuenteDeLuz(const Punto& punto) const override;
    
    // Funcion que devuelve un punto aleatorio del triángulo en UCS.
    // También devuelve en <prob> la probabilidad de muestrear dicho punto.
    Punto generarPuntoAleatorio(float& prob) const override;
    
    // Funcion que obtiene la posición del punto <pto> del triángulo en el
    // eje U de la textura correspondiente.
    float getEjeTexturaU(const Punto& pto) const override;
    
    // Funcion que obtiene la posición del punto <pto> del triángulo en el
    // eje V de la textura correspondiente.
    float getEjeTexturaV(const Punto& pto) const override;

    // Funcion que
    Esfera minimumBoundingSphere();

    
    // Debug
    void diHola() const override;
};
