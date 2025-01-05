//*****************************************************************
// File:   textura.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   diciembre 2024
// Coms:   Práctica 3.3 de Informática Gráfica
//*****************************************************************

#include <iostream>
#include <fstream>
#include "textura.h"


Textura::Textura() : ancho(0), alto(0) {}

Textura::Textura(const string& ruta): ancho(0), alto(0) {
    if (ruta != "") cargarPPM(ruta);
}

void Textura::cargarPPM(const string& ruta) {
    std::ifstream file(ruta, std::ios::binary);     // Abrimos en modo BINARIO!
    if (!file) {
        throw runtime_error("Error: No se pudo abrir la textura " + ruta);
        return;
    }

    string formato;
    file >> formato; // Leemos "P6"
    if (formato != "P6") {
        throw runtime_error("Error: Formato PPM del fichero en \"" + ruta +
                "\" no soportado (se espera P6)");
        return;
    }

    file >> this->ancho >> this->alto;
    int maxVal;
    file >> maxVal;
    this->c = static_cast<float>(maxVal);
    file.get();     // Consumimos el salto de línea

    vector<unsigned char> data(this->ancho * this->alto * 3);
    file.read(reinterpret_cast<char*>(data.data()), data.size());
    
    /* // Paso a clase RGB (array<float,3>)
    imagen.reserve(ancho * alto);
    for (size_t i = 0; i < data.size(); i += 3) {
        RGB color = {
            data[i] / 255.0f,
            data[i + 1] / 255.0f,
            data[i + 2] / 255.0f
        };
        imagen.push_back(color);
    }
    */
    
    // Verificamos que la lectura fue exitosa
    if (!file) {
        cerr << "Error: No se pudieron leer los datos de la imagen" << endl;
        return;
    }
    // Asignar el vector data al atributo imagen de la clase Textura
    this->imagen = std::move(data);
}

RGB Textura::obtenerTextura(float u, float v) const {
    //int x = static_cast<int>(u * this->ancho) % this->ancho;
    //int y = static_cast<int>(v * this->alto) % this->alto;
    int x = (static_cast<int>(u * this->ancho) % this->ancho + this->ancho) % this->ancho;
    int y = (static_cast<int>(v * this->alto) % this->alto + this->alto) % this->alto;
    int indice = (y * this->ancho + x) * 3;
    
    return RGB(static_cast<float>(this->imagen[indice]) / this->c,
               static_cast<float>(this->imagen[indice + 1]) / this->c,
               static_cast<float>(this->imagen[indice + 2]) / this->c);
}

void Textura::mostrarContenido() const {
    for (size_t i = 0; i < imagen.size(); i += 3) {
        cout << "R: " << static_cast<float>(imagen[i]) <<
                " G: " << static_cast<float>(imagen[i + 1]) <<
                " B: " << static_cast<float>(imagen[i + 2]) << endl;
    }
}

void Textura::diHola() const {
    cout << "Ancho: " << ancho << ", alto: " << alto << endl;
    cout << "Vector imagen[0]: " << imagen[0] << endl;
}
