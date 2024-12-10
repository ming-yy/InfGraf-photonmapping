//*****************************************************************
// File:   photon.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   diciembre 2024
// Coms:   Práctica 5 de Informática Gráfica
//*****************************************************************

#include "photon.h"

Photon::Photon(array<float, 3>& _coord, Direccion _wi, RGB _flujo) :
                                coord(_coord), wi(_wi), flujo(_flujo){}


float Photon::getCoord(size_t i) const {
    return coord[i];
}


Direccion Photon::getDirIncidente() const {
    return wi;
}

RGB Photon::getFlujo() const {
    return flujo;
}

float PhotonAxisPosition::operator()(const Photon& p, size_t i) const {
    return p.getCoord(i);
}

PhotonMap generarPhotonMap(vector<Photon>& fotonVec){
    list<Photon> photons(fotonVec.begin(), fotonVec.end());
    return PhotonMap(photons, PhotonAxisPosition());
}


void fotonesCercanos(PhotonMap& photonMap, array<float, 3>& coordBusqueda, float radio,
                        unsigned long numFotones, vector<const Photon*>& fotonesCercanos){
    fotonesCercanos = photonMap.nearest_neighbors(coordBusqueda,
                                                    numFotones,
                                                    radio);
}


void fotonesCercanosPorNumFotones(PhotonMap& photonMap, array<float, 3>& coordBusqueda,
                        unsigned long numFotones, vector<const Photon*>& fotonesCercanos){
    fotonesCercanos = photonMap.nearest_neighbors(coordBusqueda,
                                                    numFotones,
                                                    std::numeric_limits<float>::infinity());
}


void fotonesCercanosPorRadio(PhotonMap& photonMap, array<float, 3>& coordBusqueda,
                                float radio, vector<const Photon*>& fotonesCercanos){
    fotonesCercanos = photonMap.nearest_neighbors(coordBusqueda,
                                                std::numeric_limits<unsigned long>::max(),
                                                radio);
}
