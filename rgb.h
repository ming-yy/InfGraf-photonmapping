//*****************************************************************
// File:   rgb.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#pragma once
#include <array>
#include <initializer_list>
#include <iostream>
#include "utilidades.h"

class RGB {
public:
    array<float,3> rgb;

    RGB();
    RGB(const float& r, const float& g, const float& b);
    RGB(init_list<float> _rgb);
    RGB(const array<float, 3>& _rgb);
    RGB(const RGB& other) = default; // constructor de copia
    

    // Operación de suma de dos RGB
    RGB operator+(const RGB& d) const;
    
    // Operación de resta de dos RGB
    RGB operator-(const RGB& d) const;
    
    // Operación de multiplicación entre un escalar y el RGB
    RGB operator*(const float escalar) const;

    RGB operator*(const RGB& d) const;
    
    // Operación de división entre un escalar y el RGB
    RGB operator/(const float escalar) const;

    // Operador de asignación
    RGB& operator=(const RGB& d);
    
    // Operación de asignación de array
    RGB& operator=(const init_list<float>& r);
    
    // Operación de +=
    RGB& operator+=(const RGB& d);
    
    // Función que devuelve el valor máximo del RGB
    friend float max(const RGB& a);
    
    // Función que devuelve "True" si y solo si el RGB <a> vale todo 0.
    friend bool valeCero(const RGB& a);
    
    friend ostream& operator<<(ostream& os, const RGB& r);

private:
    // Método privado que devuelve el mayor valor de entre los 3 (R, G o B)
    float max() const;
    
    // Método privado que devuelve "True" si y solo si todas las componentes de this->rgb son 0.
    bool valeCero() const;
};
