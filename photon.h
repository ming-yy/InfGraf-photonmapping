//*****************************************************************
// File:   photon.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   diciembre 2024
// Coms:   Práctica 5 de Informática Gráfica
//*****************************************************************

#include "utilidades.h"
#include "direccion.h"
#include "rgb.h"

// Clase que representa un foton
// <coord> representan las coordenadas espaciales (x,y,z) en las que se encuentra
// <wi> representa la direccion incidente de donde viene la luz
// <flujo> representa la cantidad de energia luminosa que viene de la direccion incidente
class Photon {
    array<float, 3> coord;
    Direccion wi;
    RGB flujo;
    
    public:
        // Constructor de Photon
        Photon(array<float, 3>& _coord, Direccion _wi, RGB _flujo);

        // Getters de Photon
        float getCoord(size_t i) const;
        Direccion getDirIncidente() const;
        RGB getFlujo() const;
};
