//*****************************************************************
// File:   textura.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   diciembre 2024
// Coms:   Práctica 3.3 de Informática Gráfica
//*****************************************************************

#pragma once
#include "utilidades.h"
#include "rgb.h"
#include <vector>


class Textura {
public:
    // Pixeles de ancho y alto que tiene la textura
    int ancho, alto;

    // Valor máximo de las tripletas RGB del ppm de la textura
    float c;

    // Vector de chars que almacena los valores del ppm de la textura
    // 8 bits, más eficiente que float
    vector<unsigned char> imagen;
    
    // Constuctor base
    Textura();

    // Constructor dada la ruta al ppm de la textura
    Textura(const string& ruta);
    
    // Método que, dada la ruta de una imagen .ppm (en formato P6),
    // obtiene el atributo <ancho>, <alto> y guarda la imagen como un
    // en el atributo <imagen>.
    // Nota: esperamos tener solo valores de hasta 255 (8 bits)
    void cargarPPM(const string& ruta);
    
    // Método que devuelve el RGB (valores de 0.0f a 1.0f que tiene la textura en
    // la posición (u,v). Por default, efecto infinito.
    RGB obtenerTextura(float u, float v) const;
    
    // Método que muestra los valores almacenados de la textura.
    void mostrarContenido() const;
    
    // Debug
    void diHola() const;
};
