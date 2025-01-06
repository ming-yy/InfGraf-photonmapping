//*****************************************************************
// File:   base.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#pragma once
#include <array>
#include <iostream>
#include <initializer_list>
#include "direccion.h"
#include "utilidades.h"

// Clase que representa una base formada por 3 vectores
class Base {
public:
    array<array<float, 3>, 3> base;

    // Constructor default
    Base();

    // Constructor a partir de otra base
    Base(const Base& b);
    
    // Constructor a partir de una lista de listas de valores float
    Base(init_list<init_list<float>> valores);

    // Constructor a partir de un array de 3 arrays, de 3 float cada uno
    Base(array<array<float, 3>, 3> valores);
    
    // Constructor a partir de de 3 arrays, de 3 float cada uno
    Base(const array<float, 3>& arr1, const array<float, 3>& arr2,
         const array<float, 3>& arr3);

    // Constructor a partir de un array de 3 direcciones
    Base(const Direccion& arr1, const Direccion& arr2, const Direccion& arr3);
    
    // Función para mostrar por pantalla la base <b>
    friend ostream& operator<<(ostream& os, const Base& b);
};


// Función que dado el vector <normal>, devuelve los vectores <tangente> y <bitangente>. Estos 3
// vectores constituyen una base ortonormal conformada por <normal>, <tangente>, <bitangente>
void construirBaseOrtonormal(const Direccion& normal, Direccion& tangente, Direccion& bitangente);
