//*****************************************************************
// File:   transformaciones.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "transformaciones.h"


Punto puntoFromCoordHomo(Matriz<4,1> m){
    return Punto(m.matriz[0][0], m.matriz[1][0], m.matriz[2][0]);
}

Direccion dirFromCoordHomo(Matriz<4,1> m){
    return Direccion(m.matriz[0][0], m.matriz[1][0], m.matriz[2][0]);
}

Punto translate(const Punto& pd, float x, float y, float z) {
    Matriz<4, 4> m = Matriz<4, 4>(
            init_list<init_list<float>>{
                {1.0f, 0.0f, 0.0f, x},
                {0.0f, 1.0f, 0.0f, y},
                {0.0f, 0.0f, 1.0f, z},
                {0.0f, 0.0f, 0.0f, 1.0f}
            }
    );

    Matriz<4, 1> p = Matriz<4, 1>(pd.getCoordHomo());
    Matriz<4, 1> res = m * p;
    return puntoFromCoordHomo(res);
}


Punto scale(const Punto& pd, float x, float y, float z) {
    Matriz<4, 4> m = Matriz<4, 4>(
            init_list<init_list<float>>{
                {x, 0.0f, 0.0f, 0.0f},
                {0.0f, y, 0.0f, 0.0f},
                {0.0f, 0.0f, z, 0.0f},
                {0.0f, 0.0f, 0.0f, 1.0f}
            }
    );

    Matriz<4, 1> p = Matriz<4, 1>(pd.getCoordHomo());
    Matriz<4, 1> res = m * p;
    return puntoFromCoordHomo(res);
}


Punto rotateX(const Punto& pd, float d) {
    float dRad = d * GRAD_A_RAD;
    float cosD = static_cast<float>(cos(dRad));
    float sinD = static_cast<float>(sin(dRad));

    Matriz<4, 4> m = Matriz<4, 4>(
        init_list<init_list<float>>{
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, cosD, -sinD, 0.0f},
            {0.0f, sinD, cosD, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        }
    );

    Matriz<4, 1> p = Matriz<4, 1>(pd.getCoordHomo());
    Matriz<4, 1> res = m * p;
    return puntoFromCoordHomo(res);
}


Punto rotateY(const Punto& pd, float d) {
    float dRad = d * GRAD_A_RAD;
    float cosD = static_cast<float>(cos(dRad));
    float sinD = static_cast<float>(sin(dRad));

    Matriz<4, 4> m = Matriz<4, 4>(
        init_list<init_list<float>>{
            {cosD, 0.0f, sinD, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {-sinD, 0.0f, cosD, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        }
    );

    Matriz<4, 1> p = Matriz<4, 1>(pd.getCoordHomo());
    Matriz<4, 1> res = m * p;
    return puntoFromCoordHomo(res);
}


Punto rotateZ(const Punto& pd, float d) {
    float dRad = d * GRAD_A_RAD;
    float cosD = static_cast<float>(cos(dRad));
    float sinD = static_cast<float>(sin(dRad));

    Matriz<4, 4> m = Matriz<4, 4>(
        init_list<init_list<float>>{
            {cosD, -sinD, 0.0f, 0.0f},
            {sinD, cosD, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        }
    );

    Matriz<4, 1> p = Matriz<4, 1>(pd.getCoordHomo());
    Matriz<4, 1> res = m * p;
    return puntoFromCoordHomo(res);
}


Direccion translate(const Direccion& pd, float x, float y, float z) {
    Matriz<4, 4> m = Matriz<4, 4>(
            init_list<init_list<float>>{
                {1.0f, 0.0f, 0.0f, x},
                {0.0f, 1.0f, 0.0f, y},
                {0.0f, 0.0f, 1.0f, z},
                {0.0f, 0.0f, 0.0f, 1.0f}
            }
    );

    Matriz<4, 1> p = Matriz<4, 1>(pd.getCoordHomo());
    Matriz<4, 1> res = m * p;
    return dirFromCoordHomo(res);
}


Direccion scale(const Direccion& pd, float x, float y, float z) {
    Matriz<4, 4> m = Matriz<4, 4>(
            init_list<init_list<float>>{
                {x, 0.0f, 0.0f, 0.0f},
                {0.0f, y, 0.0f, 0.0f},
                {0.0f, 0.0f, z, 0.0f},
                {0.0f, 0.0f, 0.0f, 1.0f}
            }
    );

    Matriz<4, 1> p = Matriz<4, 1>(pd.getCoordHomo());
    Matriz<4, 1> res = m * p;
    return dirFromCoordHomo(res);
}


Direccion rotateX(const Direccion& pd, float d) {
    float dRad = d * GRAD_A_RAD;
    float cosD = static_cast<float>(cos(dRad));
    float sinD = static_cast<float>(sin(dRad));

    Matriz<4, 4> m = Matriz<4, 4>(
        init_list<init_list<float>>{
            {1.0f, 0.0f, 0.0f, 0.0f},
            {0.0f, cosD, -sinD, 0.0f},
            {0.0f, sinD, cosD, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        }
    );

    Matriz<4, 1> p = Matriz<4, 1>(pd.getCoordHomo());
    Matriz<4, 1> res = m * p;
    return dirFromCoordHomo(res);
}


Direccion rotateY(const Direccion& pd, float d) {
    float dRad = d * GRAD_A_RAD;
    float cosD = static_cast<float>(cos(dRad));
    float sinD = static_cast<float>(sin(dRad));

    Matriz<4, 4> m = Matriz<4, 4>(
        init_list<init_list<float>>{
            {cosD, 0.0f, sinD, 0.0f},
            {0.0f, 1.0f, 0.0f, 0.0f},
            {-sinD, 0.0f, cosD, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        }
    );

    Matriz<4, 1> p = Matriz<4, 1>(pd.getCoordHomo());
    Matriz<4, 1> res = m * p;
    return dirFromCoordHomo(res);
}


Direccion rotateZ(const Direccion& pd, float d) {
    float dRad = d * GRAD_A_RAD;
    float cosD = static_cast<float>(cos(dRad));
    float sinD = static_cast<float>(sin(dRad));

    Matriz<4, 4> m = Matriz<4, 4>(
        init_list<init_list<float>>{
            {cosD, -sinD, 0.0f, 0.0f},
            {sinD, cosD, 0.0f, 0.0f},
            {0.0f, 0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 0.0f, 1.0f}
        }
    );

    Matriz<4, 1> p = Matriz<4, 1>(pd.getCoordHomo());
    Matriz<4, 1> res = m * p;
    return dirFromCoordHomo(res);
}



//Matriz<4, 1> cambioBase(const Punto& p, const Base& b, const Punto& o) {
Punto cambioBase(const Punto& p, const Base& b, const Punto& o, const bool& invertir) {
    Matriz<4, 1> res;
    Matriz<4, 4> m = Matriz<4, 4>(init_list<init_list<float>>{
            {b.base[0][0], b.base[1][0], b.base[2][0], o.coord[0]},
            {b.base[0][1], b.base[1][1], b.base[2][1], o.coord[1]},
            {b.base[0][2], b.base[1][2], b.base[2][2], o.coord[2]},
            {0.0f, 0.0f, 0.0f, 1.0f}
        }
    );
    
    if (invertir) {
        Matriz<4, 4> ucsToLocal = m.inversa();
        res = Matriz<4, 1>((ucsToLocal * p.getCoordHomo()).matriz);
    } else {
        res = Matriz<4, 1>((m * p.getCoordHomo()).matriz);
    }

    return puntoFromCoordHomo(res);
}


Direccion cambioBase(const Direccion& d, const Base& b, const Punto& o, const bool& invertir) {
    Matriz<4, 1> res;
    Matriz<4, 4> m = Matriz<4, 4>(init_list<init_list<float>>{
            {b.base[0][0], b.base[1][0], b.base[2][0], o.coord[0]},
            {b.base[0][1], b.base[1][1], b.base[2][1], o.coord[1]},
            {b.base[0][2], b.base[1][2], b.base[2][2], o.coord[2]},
            {0.0f, 0.0f, 0.0f, 1.0f}
        }
    );
    
    if (invertir) {
        Matriz<4, 4> ucsToLocal = m.inversa();
        res = Matriz<4, 1>((ucsToLocal * d.getCoordHomo()).matriz);
    } else {
        res = Matriz<4, 1>((m * d.getCoordHomo()).matriz);
    }

    // cout << m << "\n" << ucsToLocal << endl;
    return dirFromCoordHomo(res);
}
