//*****************************************************************
// File:   bsdfs.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 4.3 de Informática Gráfica
//*****************************************************************

#include "bsdfs.h"
#include <map>


std::map<string, array<float, 3>> materiales{
    {"muy_difuso", {0.9f, 0.0f, 0.0f}},
    {"difuso", {0.55f, 0.0f, 0.0f}},
    {"poco_difuso", {0.3f, 0.0f, 0.0f}},
    {"cristal", {0.0f, 0.1f, 0.8f}},
    {"refractante", {0.0f, 0.0f, 0.9f}},
    {"espejo", {0.0f, 0.9f, 0.0f}},
    {"plastico", {0.7f, 0.2f, 0.0f}},
};

BSDFs::BSDFs() {
    RGB _color(1.0f, 1.0f, 1.0f);
    array<float, 3> coefs = materiales["difuso"];
    kd = _color * coefs[KD_i];
    ks = _color * coefs[KS_i];
    kt = _color * coefs[KT_i];
    sinEmision = coefs;
}


BSDFs::BSDFs(const RGB& _color, const string _material) {
    array<float, 3> coefs = materiales[_material];
    kd = _color * coefs[KD_i];
    ks = _color * coefs[KS_i];
    kt = _color * coefs[KT_i];
    sinEmision = coefs;
}

BSDFs::BSDFs(const RGB& _color, const RGB& _kd, const RGB& _ks, const RGB& _kt) {
    kd = _color * _kd;
    ks = _color * _ks;
    kt = _color * _kt;
}

BSDFs::BSDFs(const RGB& _color, const array<float, 3> _kd, const array<float, 3> _ks,
             const array<float, 3> _kt) {
    kd = _color * _kd;
    ks = _color * _ks;
    kt = _color * _kt;
}

BSDFs& BSDFs::operator=(const BSDFs& c) {
    if (this != &c) {  // Evitar auto-asignación
        this->kd = c.kd;
        this->ks = c.ks;
        this->kt = c.kt;
    }
    return *this;
}

ostream& operator<<(ostream& os, const BSDFs& c){
    os << "[ kd= " << c.kd << endl;
    os << "  ks= " << c.ks << endl;
    os << "  kt= " << c.kt << " ]" << endl;
    return os;
}
