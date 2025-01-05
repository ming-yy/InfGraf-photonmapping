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
    // Centro del plano teniendo en cuenta la distancia <d>
    Punto centro;

    // Centro del plano sin tener en cuenta la distancia <d>
    Punto centroSinDistancia;

    // Normal del plano respecto a su CENTRO SIN DISTANCIA! (todo en UCS)
    Direccion n;    

    // Direcciones U y V. Forman una base ortonormal junto con la normal <n>.
    // Sirven para calcular texturas.
    Direccion u, v;
    
    // Distancia respecto del centro (0,0,0)
    float d;

    // Límites para la luz de área
    float minLimite, maxLimite;

    // Valores de escala de la textura (si la tiene)
    float escalaTexturaX, escalaTexturaY;
    
    // Constructor base
    Plano();

    // Constructor dado una direccion normal y una distancia al centro obligatorios,
    // y como opcionales: un valor RGB que representa el color del plano, un material, una potencia
    // (emision, sera luz de area), valores limite minimo y maximo para la luz de area, el punto
    // central del plano, la ruta a la textura (si la tiene), la escala en el eje X y en el eje Y 
    // de la textura (si la tiene)
    Plano(const Direccion& _n, const float _d, const RGB& _reflectancia = RGB(1.0f, 1.0f, 1.0f),
          const string _material = "difuso", const RGB& _power = RGB(),
          const float _minLimite = -0.5, const float _maxLimite = 0.5,
          const Punto& _c = {0.0f,0.0f,0.0f}, const string rutaTextura = "",
          const float _escalaX = 1.0f, const float _escalaY = -1.0f);

    // Método para calcular la intersección entre un rayo y el plano
    //
    // Devuelve en <ptos> un vector con los puntos de intersección en UCS del rayo <rayo>
    // con el objeto. Si hay dos puntos de intersección, el primer elemento introducido
    // en el vector será el primer punto de intersección. Si hay intersección, también
    // devuelve los BSDFs del objeto en <coefs>.
    // IMPORTANTE: si el rayo tiene origen en un punto perteneciente a la primitiva, no cuenta.
    void interseccion(const Rayo& rayo, vector<Punto>& ptos, BSDFs& coefs) const override;
    
    // Método que devuelve "True" si y solo si el punto <p0> pertecene al plano.
    bool pertenece(const Punto& p0) const override;
    
    // Método que devuelve la distancia del punto <p0> al plano. 
    float distancia(const Punto& p0) const;
    
    // Método que devuelve la normal de la primitiva en el punto <punto>. En UCS!
    Direccion getNormal(const Punto& punto) const override;
    
    // Método que devuelve "True" si y solo si <punto> pertenece al plano
    // y además, es un punto lumínico del plano.
    bool puntoEsFuenteDeLuz(const Punto& punto) const override;
    
    // Método que devuelve un punto aleatorio de la superficie del plano en UCS.
    // También devuelve en <prob> la probabilidad de muestrear dicho punto.
    Punto generarPuntoAleatorio(float& prob) const override;
    
    // Método que obtiene la posición del punto <pto> del plano en el eje U de la
    // textura correspondiente. Tenemos garantizado que <pto> pertenece al objeto.
    float getEjeTexturaU(const Punto& pto) const override;
    
    // Método que obtiene la posición del punto <pto> del plano en el eje V de la
    // textura correspondiente. Tenemos garantizado que <pto> pertenece al objeto.
    float getEjeTexturaV(const Punto& pto) const override;

    // Función para mostrar por pantalla la información del plano
    friend ostream& operator<<(ostream& os, const Plano& p);
    
    // Debug
    void diHola() const override;
};
