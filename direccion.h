//*****************************************************************
// File:   direccion.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#pragma once
#include "puntoDireccion.h"
#include "matriz.h"
#include "utilidades.h"
#include <cmath>
#include <stdexcept>  // Para manejar excepciones


class Direccion : public PuntoDireccion {
public:
    Direccion();
    Direccion(const Direccion& d);
    Direccion(float x, float y, float z);
    Direccion(array<float,3> _coord);
    Matriz<4,1> getCoordHomo() const override;

    // Operación de suma de dos direcciones
    Direccion operator+(const Direccion& d) const;
    
    // Operación de resta unaria (-direccion)
    Direccion operator-() const;
    
    // Operación de resta de dos direcciones
    Direccion operator-(const Direccion& d) const;
    
    // Operación de multiplicación entre un escalar y la dirección
    Direccion operator*(const float escalar) const;
    
    // Operación de división entre un escalar y la dirección
    Direccion operator/(const float escalar) const;

    // Operador de asignación
    Direccion& operator=(const Direccion& d);
    
    // Función global para calcular el módulo de la dirección
    friend float modulo(const Direccion& d);
    
    // Función global para normalizar la dirección
    friend Direccion normalizar(const Direccion& d);
    
    // Función global para producto vectorial entre dos direcciones
    friend Direccion cross(const Direccion& d1, const Direccion& d2);
    
    // Función global para devolver la dirección con componentes absolutas
    friend Direccion abs(const Direccion& d);
    
private:
    // Método privado para calcular el módulo de la dirección
    float modulo() const;
    
    // Método privado para normalizar la dirección
    Direccion normalizar() const;
    
    // Método privado para producto vectorial con otra dirección
    Direccion productoVectorial(const Direccion& d) const;
    
    // Método privado para obtener Direccion correspondiente de hacer el absoluto de
    // cada coordenada
    Direccion absoluto() const;
};
