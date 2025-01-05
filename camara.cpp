//*****************************************************************
// File:   camara.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#include "camara.h"
#include <iostream>
#include <random>
#include <cmath>


Camara::Camara() : o(Punto(0.0f, 0.0f, -3.5f)),
                   f(Direccion(0.0f, 0.0f, 3.0f)),
                   u(Direccion(0.0f, 1.0f, 0.0f)),
                   l(Direccion(-1.0f, 0.0f, 0.0f))
                   {}


Camara::Camara(init_list<float> _o, init_list<float> _f, init_list<float> _u,
               init_list<float> _l) {
    // Lista contiene 3 elementos (x, y, z)
    if (_o.size() == 3 && _l.size() == 3 && _u.size() == 3 && _f.size() == 3) {
        auto it_o = _o.begin();
        o = Punto(*(it_o), *(it_o + 1), *(it_o + 2));
        
        auto it_f = _f.begin();
        f = Direccion(*(it_f), *(it_f + 1), *(it_f + 2));

        auto it_u = _u.begin();
        u = Direccion(*(it_u), *(it_u + 1), *(it_u + 2));
        
        auto it_l = _l.begin();
        l = Direccion(*(it_l), *(it_l + 1), *(it_l + 2));
        
    } else {
        throw invalid_argument("Inicializadores deben contener 3 elementos cada uno.");
    }
}


Camara::Camara(Punto& _o, Direccion& _f, Direccion& _u, Direccion& _l)
    : o(_o), f(_f), u(_u), l(_l) {}


Direccion Camara::obtenerDireccionEsquinaPixel(unsigned coordAncho, float anchoPorPixel, 
                                    unsigned coordAlto, float altoPorPixel) const {
    float x = modulo(this->f);
    float y = modulo(this->u) - coordAlto * altoPorPixel;
    float z = -modulo(this->l) + (coordAncho * anchoPorPixel);
    return Direccion(x, y, z);
}


Rayo Camara::obtenerRayoEsquinaPixel(unsigned coordAncho, float anchoPorPixel, 
                                    unsigned coordAlto, float altoPorPixel) const {
    Direccion dirEsquina = obtenerDireccionEsquinaPixel(coordAncho, anchoPorPixel, coordAlto, altoPorPixel);
    return Rayo(dirEsquina, Punto(0.0f, 0.0f, 0.0f));
}


Rayo Camara::obtenerRayoCentroPixel(unsigned coordAncho, float anchoPorPixel, 
                                    unsigned coordAlto, float altoPorPixel) const {
    Direccion dirEsquina = obtenerDireccionEsquinaPixel(coordAncho, anchoPorPixel, coordAlto, altoPorPixel);
    Direccion dirCentro = dirEsquina + Direccion(0,  - altoPorPixel/2, anchoPorPixel/2);
    return Rayo(dirCentro, Punto(0.0f, 0.0f, 0.0f));
}

Rayo Camara::obtenerRayoAleatorioPixel(unsigned coordAncho, float anchoPorPixel, 
                                    unsigned coordAlto, float altoPorPixel) const {
    std::random_device rd;  // Dispositivo para obtener una semilla (aleatoriedad real del hardware)
    std::mt19937 gen(rd()); // Motor de generación Mersenne Twister con semilla
    std::uniform_real_distribution<> distribucion(0.0, 1.0);  // Rango entre 0.0 y 1.0

    Direccion dirEsquina = obtenerDireccionEsquinaPixel(coordAncho, anchoPorPixel, coordAlto, altoPorPixel);
    
    // Multiplicamos el float aleatorio (0, 1) generado por el ancho/alto del pixel
    // y se lo sumamos a la esquina para obtener las nuevas coordenadas aleatorias
    float anchoRand = distribucion(gen) * anchoPorPixel;
    float altoRand = distribucion(gen) * altoPorPixel;

    Direccion dirRand = dirEsquina + Direccion(0, anchoRand, - altoRand);
    return Rayo(dirRand, Punto(0.0f, 0.0f, 0.0f));
}

float Camara::calcularAnchoPixel(unsigned numPixeles) const {
    return (modulo(this->l) * 2) / numPixeles;
}

float Camara::calcularAltoPixel(unsigned numPixeles) const{
    return (modulo(this->u) * 2) / numPixeles;
}


