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

// Clase que representa una tripleta de 3 valores Rojo, Verde y Azul
class RGB {
public:
    // Vector que contiene los 3 valores R, G y B en ese orden
    array<float,3> rgb;

    // Constructor base
    RGB();

    // Constructor a partir de 3 floats
    RGB(const float& r, const float& g, const float& b);

    // Constructor a partir de una lista de floats (coge los 3 primeros)
    RGB(init_list<float> _rgb);

    // Constructor a partir de un array de 3 floats
    RGB(const array<float, 3>& _rgb);

    // Constructor de copia
    RGB(const RGB& other) = default;
    

    // Operación de suma de dos RGB
    RGB operator+(const RGB& d) const;
    
    // Operación de resta de dos RGB
    RGB operator-(const RGB& d) const;
    
    // Operación de multiplicación entre un escalar y el RGB
    RGB operator*(const float escalar) const;

    // Operación de multiplicación entre el RGB y otro RGB
    RGB operator*(const RGB& d) const;
    
    // Operación de división entre un escalar y el RGB
    RGB operator/(const float escalar) const;

    // Operador de asignación
    RGB& operator=(const RGB& d);
    
    // Operación de asignación de array
    RGB& operator=(const init_list<float>& r);
    
    // Operación de +=
    RGB& operator+=(const RGB& d);
    
    // Función que devuelve la multiplicación entre escalar por cada componente rgb de <color>
    friend RGB operator*(const float escalar, const RGB& color);
    
    // Función para calcular el módulo del rgb
    friend float modulo(const RGB& r);
    
    // Función que devuelve el valor máximo del RGB
    friend float max(const RGB& a);
    
    // Función que devuelve "True" si y solo si el RGB <a> vale todo 0.
    friend bool valeCero(const RGB& a);
    
    // Función para mostrar por pantalla la información del RGB
    friend ostream& operator<<(ostream& os, const RGB& r);

private:
    // Método privado para calcular el módulo del rgb
    float modulo() const;
    
    // Método privado que devuelve el mayor valor de entre los 3 (R, G o B)
    float max() const;
    
    // Método privado que devuelve "True" si y solo si todas las componentes de this->rgb son 0.
    bool valeCero() const;
};
