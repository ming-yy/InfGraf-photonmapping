//*****************************************************************
// File:   cuboide.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   enero 2025
// Coms:   Práctica 3.3 de Informática Gráfica
//*****************************************************************

#pragma once
#include <vector>
#include "punto.h"
#include "direccion.h"
#include "primitiva.h"
#include "plano.h"
#include "bsdfs.h"
#include "utilidades.h"


class Cuboide : public Primitiva {
public:
    // Vector de los 6 planos que definen el cuboide
    // En orden: derecha, izquierda, techo, suelo, fondo, frente
    vector<Plano> planos;
    
    // Constructor de un cubo dado un tamaño de lado, un color y un material
    Cuboide(const float tamano, const RGB& color = RGB(), const string& material = "muy_difuso");

    // Constructor de un cuboide dado un vector de 6 planos
    // En orden: derecha, izquierda, techo, suelo, fondo, frente
    Cuboide(const vector<Plano>& p);
    
    // Método que devuelve el coeficiente kd de la primitiva en el punto <p>.
    RGB kd(const Punto& p) const override;

    // Método para calcular la intersección entre un rayo y el cubo
    //
    // Devuelve en <ptos> un vector con los puntos de intersección en UCS del rayo <rayo>
    // con el objeto. Si hay dos puntos de intersección, el primer elemento introducido
    // en el vector será el primer punto de intersección. Si hay intersección, también
    // devuelve los BSDFs del objeto en <coefs>.
    // IMPORTANTE: si el rayo tiene origen en un punto perteneciente al objeto, no cuenta.
    void interseccion(const Rayo& rayo, vector<Punto>& ptos, BSDFs& coefs) const override;

    // Método que devuelve "True" si y solo si el punto <p0> pertecene al cubo.
    bool pertenece(const Punto& punto) const override;
    
    // Método que devuelve la normal del objeto en el punto <punto>
    Direccion getNormal(const Punto& punto) const override;
    
    // Método que devuelve "True" si y solo si <punto> pertenece al cuboide
    // y además, es un punto lumínico del cuboide.
    bool puntoEsFuenteDeLuz(const Punto& punto) const override;
    
    // Método que devuelve un punto aleatorio de la superficie del cuboide en UCS.
    // También devuelve en <prob> la probabilidad de muestrear dicho punto.
    Punto generarPuntoAleatorio(float& prob) const override;
    
    // Método que obtiene la posición del punto <pto> del cuboide en el eje U de la
    // textura correspondiente. Tenemos garantizado que <pto> pertenece al objeto.
    float getEjeTexturaU(const Punto& pto) const override;
    
    // Método que obtiene la posición del punto <pto> del cuboide en el
    // eje V de la textura correspondiente. Tenemos garantizado que <pto> pertenece al objeto.
    float getEjeTexturaV(const Punto& pto) const override;
    
    // Debug
    void diHola() const override;

};
