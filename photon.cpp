//*****************************************************************
// File:   photon.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   diciembre 2024
// Coms:   Práctica 5 de Informática Gráfica
//*****************************************************************

#include "photon.h"

Photon::Photon(const array<float, 3>& _coord, const Direccion& _wi, const RGB& _flujo) :
                                coord(_coord), wi(_wi), flujo(_flujo){}

float Photon::getCoord(size_t i) const {
    if(i < 0 || i > 2) {
        cout << "Indice de coordenadas del foton fuera de rango" << endl;
    }
    return coord[i];
}

ostream& operator<<(ostream& os, const Photon& p)
{
    os << "Foton - Coord = (" << p.getCoord(0) << ", " << p.getCoord(1) << ", " << p.getCoord(2);
    os << ", Dir.Incidente = " << p.wi << ", Flujo = " << p.flujo << endl;
    return os;
}
