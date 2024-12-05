//*****************************************************************
// File:   base.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "base.h"
#include <iomanip>


Base::Base() {
    for (int i=0; i<3; i++) {
        base[i].fill(0.0f);
        base[i][i] = 1.0f;
    }
}


Base::Base(const Base& b) : base(b.base) {}


Base::Base(init_list<init_list<float>> valores){
    try{
        if((valores.size() != 3) || (valores.begin()->size() != 3)){
            for (auto& vector : base) {
                vector.fill(0.0f);
            }
            throw invalid_argument(
                "Error: Dimensiones de la base a crear son incorrectas.");
        }

        size_t i = 0;

        for (const auto& fila : valores) {
            size_t j = 0;
            for (const auto& valor : fila) {
                base[i][j] = valor;
                j++;
            }
            i++;
        }
    } catch (const invalid_argument& e) {
        cerr << e.what() << endl;
    }
}


Base::Base(array<array<float, 3>, 3> valores) {
    for (size_t i = 0; i < 3; ++i) {
        copy(valores[i].begin(), valores[i].end(), base[i].begin());
    }
}


Base::Base(const array<float, 3>& arr1, const array<float, 3>& arr2,
           const array<float, 3>& arr3) {
    copy(arr1.begin(), arr1.end(), base[0].begin());
    copy(arr2.begin(), arr2.end(), base[1].begin());
    copy(arr3.begin(), arr3.end(), base[2].begin());
}


Base::Base(const Direccion& arr1, const Direccion& arr2, const Direccion& arr3) {
    copy(arr1.coord.begin(), arr1.coord.end(), base[0].begin());
    copy(arr2.coord.begin(), arr2.coord.end(), base[1].begin());
    copy(arr3.coord.begin(), arr3.coord.end(), base[2].begin());
}


ostream& operator<<(ostream& os, const Base& m) {
    os << fixed << setprecision(3);  // Tres decimales para todos los números

    // Encuentra el ancho máximo que debe ocupar cada número
    int max_width = 0;
    for (const auto& fila : m.base) {
        for (const auto& valor : fila) {
            // Calcula el tamaño de cada número, incluyendo el signo negativo y los decimales
            // El ancho se calcula solo para el valor absoluto y se ajusta para el signo y el punto decimal
            int width = to_string(static_cast<int>(abs(valor))).length() +
                        (valor < 0 ? 1 : 0) + // Para el signo negativo
                        4; // Para incluir el punto decimal y los tres decimales
            max_width = max(max_width, width);
        }
    }

    // Imprime la matriz alineada
    for (const auto& fila : m.base) {
        os << "( ";
        for (const auto& valor : fila) {
            os << " " << setw(max_width) << valor;  // Alinea cada valor con el ancho máximo
        }
        os << " )" << endl;
    }

    return os;
}

void construirBaseOrtonormal(const Direccion& normal, Direccion& tangente, Direccion& bitangente) {
    if (fabs(normal.coord[0]) > fabs(normal.coord[2])) {
        tangente = Direccion(-normal.coord[1], normal.coord[0], 0);
    } else {
        tangente = Direccion(0, -normal.coord[2], normal.coord[1]);
    }
    tangente = normalizar(tangente);
    bitangente = cross(normal, tangente);
}
