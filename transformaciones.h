//*****************************************************************
// File:   transformaciones.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#pragma once
#include <array>
#include <memory>
#include <cmath>
#include <numbers>
#include "matriz.h"
#include "punto.h"
#include "base.h"
#include "direccion.h"

// Funcion que aplica una traslacion al punto <pd>, sumando los valores
// <x>, <y> y <z> a sus coordenadas correspondientes
Punto translate(const Punto& pd, float x, float y, float z);

// Funcion que aplica una escala al punto <pd>, multiplicando los valores
// <x>, <y> y <z> a sus coordenadas correspondientes
Punto scale(const Punto& pd, float x, float y, float z);

// Funcion que aplica una rotacion al punto <pd> en el eje X, en GRADOS <d>
Punto rotateX(const Punto& pd, float d);

// Funcion que aplica una rotacion al punto <pd> en el eje Y, en GRADOS <d>
Punto rotateY(const Punto& pd, float d);

// Funcion que aplica una rotacion al punto <pd> en el eje Z, en GRADOS <d>
Punto rotateZ(const Punto& pd, float d);

// Funcion que aplica una traslacion a la direccion <pd>, sumando los valores
// <x>, <y> y <z> a sus coordenadas correspondientes
Direccion translate(const Direccion& pd, float x, float y, float z);

// Funcion que aplica una escala a la direccion <pd>, multiplicando los valores
// <x>, <y> y <z> a sus coordenadas correspondientes
Direccion scale(const Direccion& pd, float x, float y, float z);

// Funcion que aplica una rotacion a la direccion <pd> en el eje X, en GRADOS <d>
Direccion rotateX(const Direccion& pd, float d);

// Funcion que aplica una rotacion a la direccion <pd> en el eje Y, en GRADOS <d>
Direccion rotateY(const Direccion& pd, float d);

// Funcion que aplica una rotacion a la direccion <pd> en el eje Z, en GRADOS <d>
Direccion rotateZ(const Direccion& pd, float d);

// Función que cambia el punto <p> a la base <b> con origen <o> y devuelve el nuevo punto
Punto cambioBase(const Punto& p, const Base& b, const Punto& o, const bool& invertir = true);

// Función que cambia la direccion <d> a la base <b> con origen <o> y devuelve el nuevo punto
Direccion cambioBase(const Direccion& d, const Base& b, const Punto& o, const bool& invertir = true);

// Funcion que, dada una matriz de coordenada homogeneas, devuelve el punto correspondiente 
Punto puntoFromCoordHomo(Matriz<4,1> m);

// Funcion que, dada una matriz de coordenada homogeneas, devuelve la direccion correspondiente 
Direccion dirFromCoordHomo(Matriz<4,1> m);
