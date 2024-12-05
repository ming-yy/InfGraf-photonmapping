//*****************************************************************
// File:   utilidades.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   noviembre 2024
// Coms:   Práctica 3.3 de Informática Gráfica
//*****************************************************************

#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <initializer_list>
#include <iomanip>
#include <cmath>        // ya contiene todos los métodos matemáticos con namespace std
#include <stdexcept>
#include <memory>        // Para std::shared_ptr


// Constantes
constexpr float MARGEN_ERROR = 1e-6f;          // Para calcular intersecciones
constexpr float GRAD_A_RAD = 3.1415926535898f / 180;
//const double M_PI = 3.14159265358979323846;

// Tipos o abreviaturas
template<typename T>
using init_list = std::initializer_list<T>;

template<typename T>
using sh_ptr = std::shared_ptr<T>;

// using std::abs;    //por si no funciona <cmath>
using std::array;
using std::cerr;
using std::copy;
using std::cout;
using std::endl;
using std::fixed;
using std::ifstream;
using std::invalid_argument;
using std::istringstream;
using std::max;
using std::ostream;
using std::ofstream;
// using std::pow;    //por si no funciona <cmath>
using std::runtime_error;
using std::setprecision;
using std::size_t;
using std::stack;
using std::string;
using std::to_string;
using std::vector;
using std::setw;


#endif // UTILIDADES_H
