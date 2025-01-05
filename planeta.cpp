//*****************************************************************
// File:   planeta.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "planeta.h"


Planeta::Planeta(): centro(Punto()), eje(Direccion()), cref(Punto()),
                    estacion{0.0f, 0.0f}, reflectancia(RGB({0.0f,0.0f,0.0f})) {}


Planeta::Planeta(const Punto& _centro, const Direccion& _eje, const Punto& _cref,
                 const float& _inclinacion, const float& _azimut,
                 const RGB& _reflectancia): centro(_centro), eje(_eje), cref(_cref),
                                           estacion{_inclinacion, _azimut}, reflectancia(_reflectancia) {
    // Faltan comprobaciones rango de azimut e inclinación
    estacion[0] = _inclinacion;
    estacion[1] = _azimut;
                     
    radio = modulo(_cref - _centro);
    if (abs(modulo(_eje) - radio*2) > MARGEN_ERROR) {
        throw invalid_argument("Error: eje del planeta (" + to_string(modulo(_eje)) +
                                 ") no es el doble del radio (" + to_string(radio) + ").");
    }
    
    calcularCoordLocEstacion();
    
    // No sirve para nada...
    // calcularVectoresCentroPlaneta();
}


void Planeta::calcularCoordLocEstacion(){
    float sinAzim, sinIncl, cosAzim, cosIncl;

    sinAzim = static_cast<float>(sin(float(estacion[1] * GRAD_A_RAD)));
    sinIncl = static_cast<float>(sin(float(estacion[0] * GRAD_A_RAD)));
    cosAzim = static_cast<float>(cos(float(estacion[1] * GRAD_A_RAD)));
    cosIncl = static_cast<float>(cos(float(estacion[0] * GRAD_A_RAD)));

    coordLocEstac[0] = this->radio * sinIncl * cosAzim;
    coordLocEstac[1] = this->radio * sinIncl * sinAzim;
    coordLocEstac[2] = this->radio * cosIncl;
}

/*
void Planeta::calcularVectoresCentroPlaneta() {
    // Base de la ciudad de referencia
    this->normal = normalizar(this->cref - this->centro);
    this->tangLong = normalizar(cross(this->normal, this->eje));
    this->tangLat = normalizar(this->eje);
}
*/


Punto Planeta::estacionToUCS() const {
    return Punto(coordLocEstac) + this->centro;
}


Base Planeta::getBaseEstacion() {
    Punto estacionUCS = this->estacionToUCS();
    Direccion k = normalizar(estacionUCS - this->centro);
    Direccion i = normalizar(cross(k, this->eje));
    Direccion j = normalizar(cross(k, i));
    return Base(i.coord, j.coord, k.coord);
}


Direccion Planeta::getTrayectoria(const Planeta& pDestino) {
    Punto origen = this->estacionToUCS();
    Punto destino = pDestino.estacionToUCS();
    Direccion trayec = Direccion(destino - origen);
    return normalizar(trayec);
}


bool Planeta::impactoOrEscape(const Direccion& trayectoria) {
    // Miramos la componente Z ("altura" = la normal)
    return trayectoria.coord[2] > 0;
}


bool Planeta::interconexion(Planeta& pDest) {
    Direccion trayUCS = Direccion(this->getTrayectoria(pDest));
    Base baseDest = pDest.getBaseEstacion();
    Base baseOrig = this->getBaseEstacion();
    
    // DEBUG
    //cout << "Base estacion destino:\n" << baseDest << endl;
    //cout << "Base estacion origen:\n" << baseOrig << endl;
    //cout << "Estacion destino UCS:" << pDest.estacionToUCS() << endl;
    //cout << "Estacion origen UCS:" << this->estacionToUCS() << endl;
    
    Direccion trayDest = normalizar(cambioBase(trayUCS, baseDest, pDest.estacionToUCS()));
    Direccion trayOrig = normalizar(cambioBase(trayUCS, baseOrig, this->estacionToUCS()));
    
    // Cambiamos de sentido porque pDestino necesita saber de "dónde viene" el cohete (MAL)
    //trayDest = trayDest * (-1);
    bool pDestEscape = pDest.impactoOrEscape(trayDest);
    bool pOrigEscape = this->impactoOrEscape(trayOrig);
    
    // DEBUG
    //cout << "Info Planeta origen:\n" << *this << endl;
    //cout << "Info Planeta destino:\n" << pDest << endl;
    cout << "Trayectoria UCS: " << trayUCS
         << "\nTrayectoria destino: " << trayDest
         << "\nTrayectoria origen:" << trayOrig << endl;
    cout << "Planeta origen, impacto (0) o escape (1)? " << pOrigEscape << endl;
    cout << "Planeta destino, impacto (0) o escape (1)? " << pDestEscape << endl;
    
    return !pDestEscape and pOrigEscape;
}


bool interconexionPlanetaria(Planeta& pOrig, Planeta& pDest) {
    return pOrig.interconexion(pDest);
}


bool interseccionRayoEsfera(Punto& p, Direccion& d, Planeta& e, Punto& puntoInterseccion) {
    bool resVal = true;
    float a, b, c;
    
    a = modulo(d);
    a *= a;
    b = 2 * dot(d, p - e.centro);
    c = (modulo(p - e.centro) * modulo(p - e.centro)) - (e.radio * e.radio);
    
    // DEBUG
    cout << "a = " << a << "\nb = " << b << "\nc = " << c << endl;
    
    // Cálculo del discriminante
    float discriminante = b * b - 4 * a * c;

    // Caso de soluciones reales
    if (discriminante > 0) {
        float t1 = (-b + sqrt(discriminante)) / (2 * a);
        float t2 = (-b - sqrt(discriminante)) / (2 * a);
        Punto p1 = p + d * t1;
        Punto p2 = p + d * t2;
        puntoInterseccion = (modulo(p - p1) < modulo(p - p2) ? p1 : p2);
        cout << "Hay 2 puntos de interseccion: " << endl;
        cout << "t1 = " << t1 << " --> " << p1 << endl;
        cout << "t2 = " << t2 << " --> " << p2 << endl;
        cout << "Primero intersecara con: ";
        cout << puntoInterseccion << endl;
    }
    // Caso de una solución única
    else if (discriminante == 0) {
        float t = -b / (2 * a);
        puntoInterseccion = p + d * t;
        cout << "Hay 1 punto de interseccion (tngente): " << endl;
        cout << "t = " << t << " --> " << puntoInterseccion << endl;
    }
    // Caso de soluciones complejas
    else {
        cout << "No hay puntos de intersección!" << endl;
        resVal = false;
    }
    
    return resVal;
}


ostream& operator<<(ostream& os, const Planeta& r)
{
    os << "Centro: " << r.centro << ",\nEje: " << r.eje << ",\nCiudad ref: "
       << r.cref << "\nRadio: " << r.radio << endl;
       // << "\nNormal: "<< r.normal
       // << "\nTangente longitud: " << r.tangLong
       // << "\nTangete latitud: " << r.tangLat << endl;

    return os;
}
