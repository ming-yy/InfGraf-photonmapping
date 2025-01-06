//*****************************************************************
// File:   rayo.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#pragma once
#include "direccion.h"
#include "punto.h"
#include "utilidades.h"
#include <memory>

// Clase que representa un rayo, es decir, una direccion que comienza en un punto
class Rayo {
public:
    // Direccion del rayo
    Direccion d;

    // Punto de origen del rayo
    Punto o;

    // Constructor base
    Rayo();

    // Constructor dada una direccion y un punto de origen
    Rayo(const Direccion& dir, const Punto& origen);

    // Método para mostrar la información del rayo
    void imprimir() const;

    // Función para mostrar por pantalla el rayo
    friend ostream& operator<<(ostream& os, const Rayo& pd);
};


// Función que pasa el rayo <rayo> a las coordenadas globales (en base al origen <o>,
// y los vectores <f>, <u> y <l>. Finalmente, normaliza el rayo y lo devuelve.
void globalizarYNormalizarRayo(Rayo& rayo, const Punto& o, const Direccion& f,
                               const Direccion& u, const Direccion& l);
