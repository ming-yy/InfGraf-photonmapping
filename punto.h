//*****************************************************************
// File:   punto.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#pragma once
#include <array>
#include "puntoDireccion.h"
#include "direccion.h"
#include "matriz.h"
#include "utilidades.h"

// Clase que representa un punto en el espacio, respecto a un punto
// de origen. Hereda de PuntoDireccion
class Punto : public PuntoDireccion {
public:
    // Coordenadas del punto de origen
    array<float, 3> o;

    // Constructor base
    Punto();

    // Constructor dado otro punto
    Punto(const Punto& p);

    // Constructor dadas 3 coordenadas del punto
    Punto(float x, float y, float z);

    // Constructor dadas 3 coordenadas del punto en un array
    Punto(array<float, 3> _coord);
    
    // Función que devuelve una Matriz 4x1 con las coordenadas
    // homogeneas del punto
    Matriz<4,1> getCoordHomo() const override;
    
    // Operación de suma de un punto con una dirección
    Punto operator+(const PuntoDireccion& pd) const;
    
    // Operación de resta de un punto con una dirección
    Punto operator-(const Direccion& d) const;
    
    // Operación de resta de un punto con otro punto
    Direccion operator-(const Punto& p) const;

    // Operación de multiplicación entre un escalar y el punto
    Punto operator*(const float escalar) const;
    
    // Operación de división entre un escalar y el punto
    Punto operator/(const float escalar) const;

    // Operador de asignación
    Punto& operator=(const Punto& p);

    // Función que calcula el punto que hay a la misma distancia del
    // punto y el otro punto pasado por parametro, en la linea virtual
    // que los une (es decir, el punto que hay "a medio camino" entre ellos)
    Punto puntoMedio(const Punto& p) const;
};

