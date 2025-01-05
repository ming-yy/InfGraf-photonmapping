//*****************************************************************
// File:   puntoDireccion.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#pragma once
#include <iostream>
#include <array>
#include "matriz.h"
#include "utilidades.h"


class PuntoDireccion {
public:
    // Coordenadas x, y, z
    array<float, 3> coord;
    
    // Constructor base
    PuntoDireccion();

    // Constructor dadas 3 coordenadas x, y, z
    PuntoDireccion(float x, float y, float z);

    // Constructor dadas 3 coordenadas x, y, z en un array de floats
    PuntoDireccion(array<float, 3> _coord);
    
    // Método que devuelve las coordenadas homogéneas
    virtual Matriz<4,1> getCoordHomo() const = 0;
    
    // Función para calcular producto escalar entre d1 y d2
    friend float dot(const PuntoDireccion& d1, const PuntoDireccion& d2);

    // Función global para calcular el módulo
    friend float modulo(const PuntoDireccion& d);

    // Función para mostrar por pantalla el contenido del punto o dirección
    friend ostream& operator<<(ostream& os, const PuntoDireccion& pd);

private:
    // Método privado para calcular el módulo de la dirección
    float modulo() const;
    
    // Método privado para calcular el producto escalar
    float productoEscalar(const PuntoDireccion& d) const;
};
