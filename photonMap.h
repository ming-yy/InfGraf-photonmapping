//*****************************************************************
// File:   photonMap.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   diciembre 2024
// Coms:   Práctica 5 de Informática Gráfica
//*****************************************************************

#include "kdtree.h"
#include "photon.h"

// Estructura auxiliar que permite al KDTree acceder a la posicion del Photon
struct PhotonAxisPosition {
    float operator()(const Photon& p, size_t i) const;
};

// Un KDTree de fotones en 3 dimensiones
using PhotonMap = nn::KDTree<Photon,3,PhotonAxisPosition>;

// Función que devuelve un PhotonMap dada una lista de fotones
PhotonMap generarPhotonMap(vector<Photon>& fotonVec);

// Método que devuelve por referencia en <fotonesCercanos> los fotos más cercanos
// a la posición <coordBusqueda>, dado un radio de busqueda maximo <radio> y un
// numero maximo de fotones a encontrar <numFotones>
void fotonesCercanos(PhotonMap& photonMap, array<float, 3>& coordBusqueda, float radio,
                        unsigned long numFotones, vector<const Photon*>& fotonesCercanos);

// Método que devuelve por referencia en <fotonesCercanos> los fotos más cercanos
// a la posición <coordBusqueda>, dado un numero maximo de fotones a encontrar <numFotones>
void fotonesCercanosPorNumFotones(PhotonMap& photonMap, array<float, 3>& coordBusqueda,
                        unsigned long numFotones, vector<const Photon*>& fotonesCercanos);

// Método que devuelve por referencia en <fotonesCercanos> los fotos más cercanos
// a la posición <coordBusqueda>, dado un radio de busqueda maximo <radio>
void fotonesCercanosPorRadio(PhotonMap& photonMap, array<float, 3>& coordBusqueda,
                                float radio, vector<const Photon*>& fotonesCercanos);