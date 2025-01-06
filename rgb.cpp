//*****************************************************************
// File:   rgb.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#include "rgb.h"
#include <stdexcept>
#include <algorithm>

RGB::RGB() : rgb({0.0f, 0.0f, 0.0f}) {}

RGB::RGB(const float& r, const float& g, const float& b): rgb({r, g, b}) {}

RGB::RGB(init_list<float> _rgb) {
    if (_rgb.size() == 3) {
        auto it_rgb = _rgb.begin();
        this->rgb = {*(it_rgb), *(it_rgb + 1), *(it_rgb + 2)};
    } else {
        throw invalid_argument("RGB no esta recibiendo un initializer_list de 3 valores.");
    }
}

RGB::RGB(const array<float, 3>& _rgb): rgb(_rgb) {}

RGB RGB::operator+(const RGB& d) const {
    return RGB(rgb[0] + d.rgb[0], rgb[1] + d.rgb[1], rgb[2] + d.rgb[2]);
}

RGB RGB::operator-(const RGB& d) const {
    return RGB(rgb[0] - d.rgb[0], rgb[1] - d.rgb[1], rgb[2] - d.rgb[2]);
}

RGB RGB::operator*(const float escalar) const {
    return RGB(rgb[0] * escalar, rgb[1] * escalar, rgb[2] * escalar);
}

RGB RGB::operator*(const RGB& d) const {
    return RGB(rgb[0] * d.rgb[0], rgb[1] * d.rgb[1], rgb[2] * d.rgb[2]);
}

RGB RGB::operator/(const float escalar) const {
    if (escalar == 0) {
        throw invalid_argument("Error: Division por cero no permitida.");
    }
    
    return RGB(rgb[0] / escalar, rgb[1] / escalar, rgb[2] / escalar);
}

RGB& RGB::operator=(const RGB& r) {
    if (this != &r) {  // Evitar auto-asignación
        this->rgb = r.rgb;  // Asigna las coordenadas
    }
    return *this;
}

RGB& RGB::operator=(const init_list<float>& r) {
    if (r.size() == 3) {
        auto it_r = r.begin();
        this->rgb = {*(it_r), *(it_r + 1), *(it_r + 2)};
    } else {
        throw invalid_argument("La tripleta RGB debe tener 3 elementos.");
    }
    return *this;
}

RGB& RGB::operator+=(const RGB& d) {
    for (int i=0; i < 3; ++i) {
        this->rgb[i] += d.rgb[i];
    }
    return *this;
}

RGB operator*(const float escalar, const RGB& color) {
    return RGB(escalar * color.rgb[0], escalar * color.rgb[1], escalar * color.rgb[2]);
}

float RGB::modulo() const {
    return sqrt(rgb[0] * rgb[0] + rgb[1] * rgb[1] + rgb[2] * rgb[2]);
}

float modulo(const RGB& r) {
    return r.modulo();
}

float RGB::max() const {
    return std::max({rgb[0], rgb[1], rgb[2]});
}

float max(const RGB& a) {
    return a.max();
}

bool RGB::valeCero() const {
    for (float componente : this->rgb) {
        if (componente != 0.0f) {
            return false;
        }
    }
    return true;
}

bool valeCero(const RGB& a) {
    return a.valeCero();
}

ostream& operator<<(ostream& os, const RGB& r){
    os << std::defaultfloat << std::setprecision(9) <<
         "[r=" << r.rgb[0] << ", g=" << r.rgb[1] << ", b=" << r.rgb[2] << "]";
    return os;
}
