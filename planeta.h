//*****************************************************************
// File:   planeta.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#pragma once
#include <iostream>
#include <cmath>
#include "punto.h"
#include "direccion.h"
#include "base.h"
#include "matriz.h"
#include "transformaciones.h"
#include "utilidades.h"
#include "rgb.h"


class Planeta {
public:
    Punto centro;   // En UCS
    Direccion eje;  // Modulo debe ser doble del radio
    Punto cref;     // En UCS
    float radio;
    array<float, 2> estacion;  // [inclinacion, azimut] respecto a <cref>
                               // inclin = (0, pi)   azimut = (-pi, pi]
    array<float, 3> coordLocEstac; // Coord locales de estación respecto centro planeta
    RGB reflectancia;    // (r,g,b)
    
    // De momento NO SIRVE PARA NADA
    //Direccion normal;   // Respecto a la superficie de la estación y tiene módulo 1
    //Direccion tangLong; // Respecto a superficie planeta y 90º con eje planeta
    //Direccion tangLat;  // Respecto superficie planeta y 90º con <tangLong>
    
    
    Planeta();
    Planeta(const Punto& _centro, const Direccion& _eje, const Punto& _cref,
            const float& inclinacion, const float& azimut,
            const RGB& _reflectancia = RGB({255.0f, 255.0f, 255.0f}));
    
    // Método que devuelve las coordenadas de la estación en UCS
    Punto estacionToUCS() const;
    
    // Método que devuelve la base de la estación planetaria en UCS
    Base getBaseEstacion();
    
    // Método que dado este planeta como origen y el planeta <pDestino> como destino,
    // devuelve el vector normalizado entre sus respectivas estaciones en UCS
    Direccion getTrayectoria(const Planeta& pDestino);
    
    // Método que determina si dada la trayectoria del lanzamiento, se va a impactar
    // contra el planeta (return True) o se va a escapar del planeta (return False).
    // Consideramos que tangente implica impacto.
    //
    // PRECONDICION: <trayectoria> en base local de la estación.
    bool impactoOrEscape(const Direccion& trayectoria);
    
    // Método que dados el planeta origen, planeta destino, la base UCS y el punto
    // de origen UCS, devuelve <True> si y solo si la interconexión entre nuestro
    // planeta y el planeta destino ha sido exitosa.
    friend bool interconexionPlanetaria(Planeta& pOrig, Planeta& pDest);
    
    friend bool interseccionRayoEsfera(Punto& p, Direccion& d, Planeta& e,
                                       Punto& puntoInterseccion);
    
    // Función para mostrar por pantalla el planeta
    friend ostream& operator<<(ostream& os, const Planeta& pd);

private:
    // Método que obtiene las coordenadas cartesianas de la estación en "UCS"
    // pero respecto al centro del planeta
    void calcularCoordLocEstacion();
    
    // NO SIRVE PARA NADA
    // Obtiene los 3 vectores respecto al CENTRO DEL PLANETA
    // void calcularVectoresCentroPlaneta();
    
    // Método AUXILIAR que dados el planeta destino y el punto de origen
    // UCS, devuelve <True> si y solo si la interconexión entre nuestro planeta y el
    // planeta destino ha sido exitosa.
    bool interconexion(Planeta& pDest);

};
