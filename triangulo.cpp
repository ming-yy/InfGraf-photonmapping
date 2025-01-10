//*****************************************************************
// File:   triangulo.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include <random>
#include "triangulo.h"
#include "utilidades.h"

Triangulo::Triangulo() : Primitiva(), p0(Punto()), p1(Punto()), p2(Punto()),
                                        n0(Direccion()), n1(Direccion()), n2(Direccion()),
                                        u0(0.0f), u1(0.0f), u2(0.0f),
                                        v0(0.0f), v1(0.0f), v2(0.0f) {}

Triangulo::Triangulo(const Punto& _p0, const Punto& _p1, const Punto& _p2,
                     const RGB& _reflectancia, const string _material,
                     const string rutaTextura, const RGB& _power):
                     Primitiva(_reflectancia, _material, _power, rutaTextura),
                     p0(_p0), p1(_p1), p2(_p2), u0(0.0f), u1(0.0f), u2(0.0f), v0(0.0f), v1(0.0f), v2(0.0f),
                     n0(getNormal(_p0)), n1(getNormal(_p0)), n2(getNormal(_p0)) {}

Triangulo::Triangulo(const Punto& _p0, const Punto& _p1, const Punto& _p2,
                     const float _u0, const float _u1, const float _u2,
                     const float _v0, const float _v1, const float _v2,
                     const Direccion& _n0, const Direccion& _n1, const Direccion& _n2,
                     const RGB& _reflectancia, const string _material,
                     const string rutaTextura, const RGB& _power):
                     Primitiva(_reflectancia, _material, _power, rutaTextura), 
                     p0(_p0), p1(_p1), p2(_p2), u0(_u0), u1(_u1), u2(_u2), v0(_v0), v1(_v1), v2(_v2),
                     n0(_n0), n1(_n1), n2(_n2) {}

void Triangulo::interseccion(const Rayo& rayo, vector<Punto>& ptos, BSDFs& coefs) const {
    Direccion edge1 = p1 - p0;
    Direccion edge2 = p2 - p0;
    Direccion h = cross(rayo.d, edge2);
    float a = dot(edge1, h);

    if (fabs(a) < MARGEN_ERROR) {
        // cout << "No hay intersección, el rayo es paralelo al triángulo." << endl;
        return;
    }

    float f = 1.0f / a;
    Direccion s = rayo.o - p0;
    float u = f * dot(s, h);

    if (u < 0.0f || u > 1.0f) {
        return; // La intersección está fuera del triángulo
    }

    Direccion q = cross(s, edge1);
    float v = f * dot(rayo.d, q);

    if (v < 0.0f || u + v > 1.0f) {
        return; // La intersección está fuera del triángulo
    }

    float t = f * dot(edge2, q);
    if (t > MARGEN_ERROR) {
        ptos.push_back(Punto(rayo.o + rayo.d * t));
        coefs = this->coeficientes;
    } //else {
        //return ; // No hay intersección en la dirección del rayo
    //}
}

bool Triangulo::getCoordBaricentricas(const Punto& punto, float& u, float& v) const {
     // Vectores de los lados del triángulo
    Direccion p0p1 = p1 - p0;
    Direccion p0p2 = p2 - p0;
    Direccion p0p = punto - p0;

    // Calculamos los productos escalares necesarios
    float dotp1p1 = dot(p0p1, p0p1);
    float dotp1p2 = dot(p0p1, p0p2);
    float dotp2p2 = dot(p0p2, p0p2);
    float dotVPp1 = dot(p0p, p0p1);
    float dotVPp2 = dot(p0p, p0p2);

    // Determinante
    float denom = dotp1p1 * dotp2p2 - dotp1p2 * dotp1p2;
    if (denom == 0.0f) {
        u = 0.0f;
        v = 0.0f;
        return false; // Vértices colineales
    }

    // Coordenadas baricéntricas
    u = (dotp2p2 * dotVPp1 - dotp1p2 * dotVPp2) / denom;
    v = (dotp1p1 * dotVPp2 - dotp1p2 * dotVPp1) / denom;

    return true;
}


bool Triangulo::pertenece(const Punto& punto) const {
    float u, v;
    bool esValido = getCoordBaricentricas(punto, u, v);
    
    if(!esValido) return false;

    // Comprobamos si están dentro del rango [0, 1] y que u + v <= 1
    return (u >= 0.0f && v >= 0.0f && u + v <= 1.0f);
}

Direccion Triangulo::getNormal() const {
    Direccion AB = p1 - p0;
    Direccion AC = p2 - p0;
    return cross(AB, AC);
}

Direccion Triangulo::getNormal(const Punto& punto) const {
    return getNormalInterpolada(punto);
}

Direccion Triangulo::getNormalInterpolada(const Punto& punto) const {
    float u, v;
    bool esValido = getCoordBaricentricas(punto, u, v);

    if(!esValido) cout << "ERROR: normal interpolada invalida" << endl; return Direccion(1.0f, 1.0f, 1.0f);

    float w = 1.0f - u - v;

    // Interpolación de la normal
    Direccion normalInterpolada = (n0 * u) + (n1 * v) + (n2 * w);
    if(modulo(normalInterpolada) != 0.0f) normalizar(normalInterpolada);
    return normalInterpolada;
}

bool Triangulo::puntoEsFuenteDeLuz(const Punto& punto) const {
    return soyFuenteDeLuz() && pertenece(punto);
}

Punto Triangulo::generarPuntoAleatorio(float& prob) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    float r1 = dist(gen);
    float r2 = dist(gen);

    // r1 + r2 <= 1.0 (si no, reflejamos el punto en el triángulo)
    if (r1 + r2 > 1.0f) {
        r1 = 1.0f - r1;
        r2 = 1.0f - r2;
    }

    // Coordenadas baricéntricas
    float a = 1.0f - r1 - r2;
    float b = r1;
    float c = r2;

    // Coordenadas baricéntricas --> coordenadas cartesianas
    Punto puntoAleatorio = p0 * a + p1 * b + p2 * c;

    Direccion crossProd = cross(p1 - p0, p2 - p0);
    float areaTriangulo = modulo(crossProd) * 0.5f;
    prob = 1.0f / areaTriangulo;

    return puntoAleatorio;
}

float Triangulo::getEjeTexturaU(const Punto& pto) const {
    float u, v;
    bool esValido = getCoordBaricentricas(pto, u, v);

    if(!esValido) {
        cout << "ERROR: eje U invalido" << endl;
        return 0.0f;
    }
    
    float w = 1.0f - u - v;
    return u0 * u + u1 * v + u2 * w;
}

float Triangulo::getEjeTexturaV(const Punto& pto) const {
    float u, v;
    bool esValido = getCoordBaricentricas(pto, u, v);
    
    if(!esValido) {
        cout << "ERROR: eje U invalido" << endl;
        return 0.0f;
    }
    
    float w = 1.0f - u - v;
    return v0 * u + v1 * v + v2 * w;
}

float Triangulo::distanciaPunto(const Punto& pto) const {
    // Paso 1: Proyectar el punto sobre el plano del triángulo
    Direccion normal = getNormal();
    float distanciaPlano = dot((pto - p0), normal);
    Punto pProyectado = pto - normal * distanciaPlano;

    // Paso 2: Verificar si el punto proyectado está dentro del triángulo
    float u, v;
    bool esValido = getCoordBaricentricas(pProyectado, u, v);

    if (esValido) {
        // Si está dentro del triángulo, la distancia es la perpendicular al plano
        return std::abs(distanciaPlano);
    }

    // Paso 3: Calcular la distancia mínima a los bordes del triángulo
    auto distanciaSegmento = [](const Punto& p, const Punto& a, const Punto& b) -> float {
        Direccion ab = b - a;
        float t = std::max(0.0f, std::min(1.0f, dot(p - a, ab) / dot(ab, ab)));
        Punto proyeccion = a + ab * t;
        return modulo(p - proyeccion);
    };

    float distLado0 = distanciaSegmento(pto, p0, p1);
    float distLado1 = distanciaSegmento(pto, p1, p2);
    float distLado2 = distanciaSegmento(pto, p2, p0);

    // Retornar la distancia mínima a los lados del triángulo
    float aux = std::min(distLado0, distLado1);
    return std::min(aux, distLado2);
}

void Triangulo::diHola() const {
    cout << "Soy triangulo: p1 = " << this->p0 << endl;
}

ostream& operator<<(ostream& os, const Triangulo& t) {
    os << t.p0 << ", " << t.p1 << ", " << t.p2;
    return os;
}
