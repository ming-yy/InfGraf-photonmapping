//*****************************************************************
// File:   escena.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 3.2 de Informática Gráfica
//*****************************************************************

#pragma once
#include <vector>
#include "primitiva.h"
#include "rgb.h"
#include "luzpuntual.h"
#include "utilidades.h"

class Escena {
public:
    // Vector de los objetos de la escena
    vector<Primitiva*> primitivas;

    // Vector de luces puntuales de la escena
    vector<LuzPuntual> luces;        
    
    // Constructor base
    Escena();

    // Constructor dado un vector de objetos y un vector de luces puntuales
    Escena(vector<Primitiva*> _primitivas, vector<LuzPuntual> _luces);
    
    // Método que devuelve "True" si y solo si hay intersección entre el rayo <rayo> y algún
    // objeto de la escena. En caso de haber intersección, el primer objeto de la escena con
    // el que interseca el rayo <rayo> es devuelto en <objIntersecado>, el punto por el que
    // interseca <rayo> con <objIntersecado> es devuelto en <ptoMasCerca> y en <powerLuzArea>
    // devolverá el power de la fuente de luz si y solo si el objeto se trata de una fuente
    // de luz. También devuelve la normal del punto más cercano respecto del objeto al que
    // pertenece.
    bool interseccion(const Rayo& rayo, Punto& ptoMasCerca, Direccion& normal,
                      Primitiva** objIntersecado) const;
    
    // Función que devuelve "True" si y solo si el punto p0 pertenece a una fuente de luz. Además,
    // si devuelve "True", también devolverá en <powerLuzArea> el power de dicha luz.
    bool puntoPerteneceALuz(const Punto& p0, RGB& powerLuzArea) const;
    
    // Método que devuelve "True" si y solo si el punto <p0> es iluminado por la fuente de luz <luz>
    bool luzIluminaPunto(const Punto& p0, const LuzPuntual& luz) const;
    
    // Método que devuelve "True" si y solo si el punto <p0> es iluminado por la fuente de luz <luz>.
    // Si devuelve "True", también devuelve en <origenLuz> el punto de origen de la luz desde donde
    // ilumina al punto <p0>. También devuelve en <prob> la probabilidad de haber elegido el punto
    // <origenLuz> en caso de que exista.
    bool luzIluminaPunto(const Punto& p0, const Primitiva* luz, Punto& origenLuz, float& prob) const;
    
    // Método que devuelve "True" si y solo si al punto <p0> lo ilumina una fuente de luz.
    // En caso contrario, devuelve False.
    bool puntoIluminado(const Punto& p0) const;
};
