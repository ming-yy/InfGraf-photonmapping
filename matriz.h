//*****************************************************************
// File:   matriz.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#pragma once
#include <array>
#include <iostream>
#include <initializer_list>
#include <cmath>
#include <iomanip>
#include "utilidades.h"


template <size_t Filas, size_t Columnas>
class Matriz {
public:
    // Array de arrays con <Columnas> x <Filas> floats a almacenar
    array<array<float, Columnas>, Filas> matriz;

    // Constructor base
    Matriz();

    // Constructor a partir de una lista de floats
    Matriz(init_list<init_list<float>> valores);

    // Constructor a partir de un array de <Filas> arrays,
    // cada uno con <Filas> valores float
    Matriz(array<array<float, Columnas>, Filas> valores);
    
    // Método para obtener el número de filas
    size_t getFilas() const { return Filas; }
    
    // Método para obtener el número de columnas
    size_t getColumnas() const { return Columnas; }

    // Método para multiplicar dos matrices: AxB * BxC = AxC
    template <size_t Columnas2>
    Matriz<Filas, Columnas2> operator*(const Matriz<Columnas, Columnas2>& otra) const;
    
    
    // Método que devuelve la matriz inversa
    Matriz<4, 4> inversa() const {
        Matriz<4, 4> inv;
        Matriz<4, 4> temp = *this;

        // Matriz identidad
        for (size_t i = 0; i < 4; ++i) {
            inv.matriz[i][i] = 1.0f;
        }

        // Implementación de Gauss-Jordan para la inversa
        for (size_t i = 0; i < 4; ++i) {
            // Buscar el pivote más grande en la columna i para intercambiar filas
            size_t maxRow = i;
            for (size_t k = i + 1; k < 4; ++k) {
                if (abs(temp.matriz[k][i]) > abs(temp.matriz[maxRow][i])) {
                    maxRow = k;
                }
            }

            // Intercambiar filas si es necesario
            if (i != maxRow) {
                swap(temp.matriz[i], temp.matriz[maxRow]);
                swap(inv.matriz[i], inv.matriz[maxRow]);
            }

            // Verificar si la matriz es singular
            float diagElem = temp.matriz[i][i];
            if (abs(diagElem) < 1e-7) {
                throw runtime_error("La matriz es singular y no tiene inversa.");
            }

            // Dividir la fila actual por el elemento diagonal
            for (size_t j = 0; j < 4; ++j) {
                temp.matriz[i][j] /= diagElem;
                inv.matriz[i][j] /= diagElem;
            }

            // Hacer ceros en la columna actual para las otras filas
            for (size_t k = 0; k < 4; ++k) {
                if (k != i) {
                    float factor = temp.matriz[k][i];
                    for (size_t j = 0; j < 4; ++j) {
                        temp.matriz[k][j] -= factor * temp.matriz[i][j];
                        inv.matriz[k][j] -= factor * inv.matriz[i][j];
                    }
                }
            }
        }

        return inv;
    }
    

    friend ostream& operator<<(ostream& os, const Matriz& m) {
        os << fixed << setprecision(3);  // Tres decimales para todos los números

        // Encuentra el ancho máximo que debe ocupar cada número
        int max_width = 0;
        for (const auto& fila : m.matriz) {
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
        for (const auto& fila : m.matriz) {
            os << "|";
            for (const auto& valor : fila) {
                os << " " << setw(max_width) << valor;  // Alinea cada valor con el ancho máximo
            }
            os << " |" << endl;
        }

        return os;
    }
};


// ----------------------------------------------------------------
//                          Implementación
// ----------------------------------------------------------------
template <size_t Filas, size_t Columnas>
Matriz<Filas, Columnas>::Matriz() {
    for (auto& fila : matriz) {
        fila.fill(0.0f);
    }
}


template <size_t Filas, size_t Columnas>
Matriz<Filas, Columnas>::Matriz(init_list<init_list<float>> valores) {
    try{
        if(Filas != valores.size() || Columnas != valores.begin()->size()){
            for (auto& fila : matriz) {
                fila.fill(0.0f);
            }
            throw invalid_argument("Error: Matriz e initializer_list no tienen las mismas filas/columnas.");
        }

        size_t i = 0;

        for (const auto& fila : valores) {
            size_t j = 0;
            for (const auto& valor : fila) {
                matriz[i][j] = valor;
                j++;
            }
            i++;
        }  
    } catch (const invalid_argument& e) {
        cerr << e.what() << endl;
    }
}


template <size_t Filas, size_t Columnas>
Matriz<Filas, Columnas>::Matriz(array<array<float, Columnas>, Filas> valores) {
    for (size_t i = 0; i < Filas; ++i) {
        copy(valores[i].begin(), valores[i].end(), matriz[i].begin());
    }
}


template <size_t Filas, size_t Columnas>
template <size_t Columnas2>
Matriz<Filas, Columnas2> Matriz<Filas, Columnas>::operator*(const Matriz<Columnas,
                                                            Columnas2>& otra) const {
    Matriz<Filas, Columnas2> resultado;

    for (size_t i = 0; i < Filas; ++i) {
        for (size_t j = 0; j < Columnas2; ++j) {
            resultado.matriz[i][j] = 0.0f; // Inicializa el valor en 0
            for (size_t k = 0; k < Columnas; ++k) {
                resultado.matriz[i][j] += this->matriz[i][k] * otra.matriz[k][j];
            }
        }
    }

    return resultado;
}
