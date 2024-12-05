//*****************************************************************
// File:   gestorPPM.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "rgb.h"
#include "utilidades.h"


// Función que itera por cada valor del RGB de un ppm y lo convierte al
// nuevo estándar en función de la resolución de color <c> y el valor
// máximo real como un número real <max>
ifstream abrir_fichero(const string& nombreFichero);

// Función que quita los saltos de línea y espacios del final de <str>
void quitarEspaciosSaltos(string &str);

// Función para validar el formato del fichero
bool validar_formato(ifstream& fichero);

// Función para leer dimensiones (ancho y alto)
void leer_dimensiones(int& ancho, int& alto, string& linea);

// Función para leer el valor máximo de color
float leer_resolucion(ifstream& fichero, float& c);

// Función para leer comentarios y extraer el max
void leer_cabecera(ifstream& fichero, float& maxColorRes, int& ancho, int& alto, float& c);

// Función para leer los valores de los píxeles
void leer_valores(ifstream& fichero, float maxColorRes, float c, vector<float>& valores);

// Función para imprimir los resultados
void imprimir_resultados(const vector<float>& valores, float maxColorRes, int alto, int ancho, float c);

// Función que extrae los metadatos y los valores del fichero
bool leerFicheroPPM(const string&nombreFich, vector<float>& valores, 
                            float& max, size_t& ancho, size_t& alto, float& c);

// Escribe los metadatos del fichero PPM resultado
void escribirCabeceraPPM(ofstream& fichero, const string nombreFichero, 
        const float maxColorRes, const size_t ancho, const size_t alto, const float c);

// Escribe los valores de los pixeles (r,g,b) en el fichero PPM resultado
void escribirValoresPPM(ofstream& fichero, const vector<float>& valores, 
                                const float maxColorRes, const size_t ancho, const size_t alto, const float c);

// Recorta la ruta "nnn/mmm/fichero.ppm" para quedarse solo con fichero.ppm
string encontrarNombreFinalFichero(const string&ruta);

// Escribe todo el contenido del fichero PPM resultado
void escribirFicheroPPM(const string&nombreFich, const vector<float>& valores, 
                        const float maxColorRes, const size_t ancho, const size_t alto, const float c);

string transformarValores(vector<float>& valores, const int tipoTransform, const float maxValue);

// Función principal que coordina el proceso
int transformarFicheroPPM(const string& nombreFichero, const int idFuncion);

// Función que dada una matriz de valores RGB, devuelve el componente máximo
float maximoValorRGB(const vector<vector<RGB>>& matrizRGB);

// Función que dado una matriz de RGB's, genera el PPM correspondiente.
void pintarEscenaEnPPM(const string& nombreArchivo,
                        const vector<vector<RGB>>& imagen = {});
