//*****************************************************************
// File:   mesh.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   diciembre 2024
// Coms:   Práctica 4 de Informática Gráfica
//*****************************************************************

#include "mesh.h"
#include "gestorPLY.h"

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
   return;
}

bool Mesh::pertenece(const Punto& p0) const {
    return false;
}

Direccion Mesh::getNormal(const Punto& punto) const {
    return Direccion();
}

bool Mesh::puntoEsFuenteDeLuz(const Punto& punto) const {
    return false;
}

Punto Mesh::generarPuntoAleatorio(float& prob) const {
    return Punto();
}

float Mesh::getEjeTexturaU(const Punto& pto) const {
    return 0.0f;
}

float Mesh::getEjeTexturaV(const Punto& pto) const {
    return 0.0f;
}

void Mesh::diHola() const {
    cout << "Soy Mesh" << endl;
}
