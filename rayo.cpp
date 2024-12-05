//*****************************************************************
// File:   rayo.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#include "rayo.h"
#include "base.h"
#include "transformaciones.h"


Rayo::Rayo(): d(Direccion()), o(Punto()) {};

Rayo::Rayo(const Direccion& dir, const Punto& origen) : d(dir), o(origen) {};

void Rayo::imprimir() const {
    cout << "Rayo - Direccion: " << d << ", Origen: " << o << endl;
}

void globalizarYNormalizarRayo(Rayo& rayo, const Punto& o, const Direccion& f, const Direccion& u,
                               const Direccion& l) {
    rayo.d = normalizar(rayo.d);
    Base baseLocalToGlobal = Base(abs(f / modulo(f)),
                                  abs(u / modulo(u)),
                                  abs(l / modulo(l)));
    
    rayo.d = cambioBase(rayo.d, baseLocalToGlobal, o, false);
    rayo.o = cambioBase(rayo.o, baseLocalToGlobal, o, false);
    rayo.d = normalizar(rayo.d);
}

ostream& operator<<(ostream& os, const Rayo& r)
{
    os << "Rayo - Direccion: " << r.d << ", Origen: " << r.o;
    return os;
}
