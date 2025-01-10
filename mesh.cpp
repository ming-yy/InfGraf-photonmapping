//*****************************************************************
// File:   mesh.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   diciembre 2024
// Coms:   Práctica 4 de Informática Gráfica
//*****************************************************************

#include "mesh.h"
#include "gestorPLY.h"
#include <random>

Mesh::Mesh() : Primitiva(), triangulos(vector<Triangulo>()) {}

Mesh::Mesh(const string rutaModelo,   const string rutaTextura, const float escala, const Punto& centro, 
            const float rotacionX, const bool invertirX, 
            const float rotacionY, const bool invertirY, 
            const float rotacionZ, const bool invertirZ, 
            const RGB& _reflectancia,  const string _material, const RGB& _power):
                                        Primitiva(_reflectancia, _material, _power, rutaTextura) {

                                        

    triangulos = generarModeloPLY(rutaModelo, rutaTextura, esferaLimite, escala, centro,
                                    rotacionX, invertirX, rotacionY, invertirY, rotacionZ, invertirZ);
}

void Mesh::interseccion(const Rayo& rayo, vector<Punto>& ptos, BSDFs& coefs) const {
    if(!interseccionEsferaLimite(rayo)){
        return;
    }

    Punto puntoCercano;
    bool intersec = false;
    for(auto& t : triangulos){
        BSDFs bsdfsAux;
        vector<Punto> puntosAux;
        t.interseccion(rayo, puntosAux, bsdfsAux);
        if(puntosAux.size() > 0){
            if(!intersec){ // primer triangulo intersecado
                intersec = true;
                puntoCercano = puntosAux[0];
                coefs = bsdfsAux;
            } else if(modulo(rayo.o - puntosAux[0]) < modulo(rayo.o - puntoCercano)){ // otra interseccion mas cercana
                puntoCercano = puntosAux[0];
                coefs = bsdfsAux;
            }
        }
    }

    if(intersec){
        ptos.push_back(puntoCercano);
    }
    return;
}

bool Mesh::pertenece(const Punto& p0) const {
    for(auto& t : triangulos){
        if(t.pertenece(p0)){
            return true;
        }
    }

    return false;
}

Direccion Mesh::getNormal(const Punto& punto) const {
    int tMasCercano = this->trianguloMasCercano(punto);
    return triangulos[tMasCercano].getNormal(punto);
}

bool Mesh::puntoEsFuenteDeLuz(const Punto& punto) const {
    int tMasCercano = this->trianguloMasCercano(punto);
    return triangulos[tMasCercano].soyFuenteDeLuz();
}

Punto Mesh::generarPuntoAleatorio(float& prob) const {
    // Obtiene un triangulo aleatorio de la malla
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, triangulos.size() - 1);
    int randomIndex = distrib(gen);
    Triangulo t = triangulos[randomIndex];

    return t.generarPuntoAleatorio(prob);
}

float Mesh::getEjeTexturaU(const Punto& pto) const {
    int tMasCercano = this->trianguloMasCercano(pto);
    return triangulos[tMasCercano].getEjeTexturaU(pto);
}

float Mesh::getEjeTexturaV(const Punto& pto) const {
    int tMasCercano = this->trianguloMasCercano(pto);
    return triangulos[tMasCercano].getEjeTexturaV(pto);
}

bool Mesh::interseccionEsferaLimite(const Rayo& r) const{
    BSDFs bsdfsAux;
    vector<Punto> puntosAux;
    this->esferaLimite.interseccion(r, puntosAux, bsdfsAux);

    return puntosAux.size() > 0;
}

int Mesh::trianguloMasCercano(const Punto& p) const {
    int masCercano = 0;
    bool primero = false;
    for(int i = 0; i < triangulos.size(); i++){
        if(triangulos[i].pertenece(p)) {
            if (primero){
                masCercano = i;
            } else if (triangulos[i].distanciaPunto(p) < triangulos[masCercano].distanciaPunto(p)){
                masCercano = i;
            }
        }
    }

    return masCercano;
}

void Mesh::diHola() const {
    cout << "Soy Mesh" << endl;
}
