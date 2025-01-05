//*****************************************************************
// File:   camara.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#pragma once
#include "direccion.h"
#include "punto.h"
#include "rayo.h"
#include "primitiva.h"
#include "utilidades.h"
#include <initializer_list>


class Camara {
public:
      // Punto de origen
      Punto o;

      // Vector (dirección) frontal
      Direccion f;

      // Vector (dirección) hacia arriba
      Direccion u;

      // Vector (dirección) hacia la izquierda
      Direccion l;

      // Constructor base
      Camara();

      // Constructor dado un punto de origen y 3 direcciones ortonormales
      Camara(Punto& _o, Direccion& _f, Direccion& _l, Direccion& _u);

      // Constructor dado un punto de origen y 3 direcciones ortonormales
      // (con listas de 3 floats)
      Camara(init_list<float> _o, init_list<float> _f, init_list<float> _u,
            init_list<float> _l);

      // Método que calcula el ancho que debería tener un pixel teniendo en cuenta
      // el vector l y el numero de pixeles de ancho pasado por parametro
      float calcularAnchoPixel(unsigned numPixeles) const;
      
      // Método que calcula el alto que debería tener un pixel teniendo en cuenta
      // el vector u y el numero de pixeles de alto pasado por parametro
      float calcularAltoPixel(unsigned numPixeles) const;

      // Método que devuelve la direccion que hay hacia la esquina superior izquierda
      // en el plano de proyeccion del pixel (ancho, alto)
      Direccion obtenerDireccionEsquinaPixel(unsigned coordAncho, float anchoPorPixel,
                                          unsigned coordAlto, float altoPorPixel) const;

      // Método que devuelve el rayo que va desde la cámara hasta
      // la esquina superior izquierda del pixel (ancho, alto)
      Rayo obtenerRayoEsquinaPixel(unsigned coordAncho, float anchoPorPixel,
                                    unsigned coordAlto, float altoPorPixel) const;

      // Método que devuelve el rayo que va desde la cámara hasta
      // el centro del pixel (ancho, alto)
      Rayo obtenerRayoCentroPixel(unsigned coordAncho, float anchoPorPixel,
                                    unsigned coordAlto, float altoPorPixel) const;
      // Método que devuelve el rayo que va desde la cámara hasta
      // un punto aleatorio del pixel (ancho, alto)
      Rayo obtenerRayoAleatorioPixel(unsigned coordAncho, float anchoPorPixel,
                                    unsigned coordAlto, float altoPorPixel) const;
};
