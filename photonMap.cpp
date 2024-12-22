//*****************************************************************
// File:   photonMap.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   diciembre 2024
// Coms:   Práctica 5 de Informática Gráfica
//*****************************************************************

#include "photonMap.h"

float PhotonAxisPosition::operator()(const Photon& p, size_t i) const {
    return p.getCoord(i);
}

PhotonMap generarPhotonMap(vector<Photon>& vecFotones){
    list<Photon> photons(vecFotones.begin(), vecFotones.end());
    return PhotonMap(std::move(photons), PhotonAxisPosition());
}

void fotonesCercanos(const PhotonMap& photonMap, const array<float, 3>& coordBusqueda, float radio,
                        unsigned long numFotones, vector<const Photon*>& fotonesCercanos){
    fotonesCercanos = photonMap.nearest_neighbors(coordBusqueda,
                                                    numFotones,
                                                    radio);
}

void fotonesCercanosPorNumFotones(const PhotonMap& photonMap, const array<float, 3>& coordBusqueda,
                        unsigned long numFotones, vector<const Photon*>& fotonesCercanos){
    fotonesCercanos = photonMap.nearest_neighbors(coordBusqueda,
                                                    numFotones,
                                                    std::numeric_limits<float>::max());
}

void fotonesCercanosPorRadio(const PhotonMap& photonMap, const array<float, 3>& coordBusqueda,
                                float radio, vector<const Photon*>& fotonesCercanos){
    fotonesCercanos = photonMap.nearest_neighbors(coordBusqueda,
                                                std::numeric_limits<unsigned long>::max(),
                                                radio);
}
