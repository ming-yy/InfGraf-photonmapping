//*****************************************************************
// File:   photonMapping.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   diciembre 2024
// Coms:   Práctica 5 de Informática Gráfica
//*****************************************************************

#include "photonMapping.h"
#include "base.h"
#include <random>


void getCoordenadasCartesianas(const float azimut, const float inclinacion,
                                float& x, float& y, float& z) {
    float sinAzim = static_cast<float>(sin(float(azimut)));
    float sinIncl = static_cast<float>(sin(float(inclinacion)));
    float cosAzim = static_cast<float>(cos(float(azimut)));
    float cosIncl = static_cast<float>(cos(float(inclinacion)));
    x = sinIncl * cosAzim;
    y = sinIncl * sinAzim;
    z = cosIncl;
}

void generarAzimutInclinacionHemiesfera(float& azimut, float& inclinacion) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    inclinacion = acos(sqrt(1-dis(gen)));
    azimut = 2 * M_PI * dis(gen);
}

Direccion generarDireccionAleatoriaHemiesfera(const Direccion& normal, float& prob) {
    float inclinacion, azimut;
    float x, y, z;

    generarAzimutInclinacionHemiesfera(azimut, inclinacion);
    getCoordenadasCartesianas(azimut, inclinacion, x, y, z);
    Direccion wi_local = normalizar(Direccion(x, y, z));   // Inclinacion positiva, hemisferio norte asegurado
    Direccion tangente;
    Direccion bitangente;
    construirBaseOrtonormal(normal, tangente, bitangente);
    
    // Cambio de base manual de <wi_local> de coord. locales a coord. globales
    Direccion nuevaDir = normalizar(tangente * wi_local.coord[0] +
                                    bitangente * wi_local.coord[1] +
                                    normal * wi_local.coord[2]);
    
    // Probabilidad de un rayo es proporcional al coseno del ángulo de inclinación
    prob = cos(inclinacion) / M_PI;
    return nuevaDir;
}

void generarAzimutInclinacionEsfera(float& azimut, float& inclinacion) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    inclinacion = acos((2.0f * dis(gen)) - 1.0f);
    azimut = 2 * M_PI * dis(gen);
}

Direccion generarDireccionAleatoriaEsfera() {
    float inclinacion, azimut;
    generarAzimutInclinacionEsfera(azimut, inclinacion);

    float x, y, z;
    getCoordenadasCartesianas(azimut, inclinacion, x, y, z);
   
    // Se devuelve en base global
    return normalizar(Direccion(x, y, z));
}

void randomWalk(vector<Photon>&vecFotones, const Escena& escena, const Rayo& wi, const RGB& flujo){
    // TODO: Intersecar, calcular radiancia, guardar foton(radiancia, wi, flujo)
    //       repetir recursivamente como en la indirecta
}

// Optamos por almacenar todos los rebotes difusos (incluido el primero)
// y saltarnos el NextEventEstimation posteriormente
void anadirFotonesDeLuz(vector<Photon>& vecFotones, const unsigned numFotones, const LuzPuntual& luz, const Escena& escena){
    RGB flujoPorFoton = (luz.p * 4 * M_PI) / numFotones;

    for(unsigned i = 0; i < numFotones; i++){
        Rayo wi(generarDireccionAleatoriaEsfera(), luz.c);
        randomWalk(vecFotones, escena, wi, flujoPorFoton);
    }
}

RGB calcularPotenciaTotal(const vector<LuzPuntual>& luces){
    RGB total;

    for(auto& luz : luces){
        total += luz.p;
    }

    return total;
}

void generarFotones(vector<Photon>& vecFotones, const unsigned totalFotones, const Escena& escena){
    RGB potenciaTotal = calcularPotenciaTotal(escena.luces);

    for(auto& luz : escena.luces){
        unsigned numFotonesProporcionales = totalFotones * (max(luz.p) / max(potenciaTotal));
        anadirFotonesDeLuz(vecFotones, numFotonesProporcionales, luz, escena);
    }
}


                      
void renderizarEscena(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                      const Escena& escena, const string& nombreEscena, const unsigned rpp,
                      const unsigned maxRebotes, const unsigned numRayosMontecarlo, const unsigned numFotones,
                      const bool printPixelesProcesados) {
    float anchoPorPixel = camara.calcularAnchoPixel(numPxlsAncho);
    float altoPorPixel = camara.calcularAltoPixel(numPxlsAlto);
   
    unsigned totalPixeles = numPxlsAlto * numPxlsAncho;
    if (printPixelesProcesados) cout << "Procesando pixeles..." << endl << "0 pixeles de " << totalPixeles << endl;


    vector<Photon> vecFotones;

    generarFotones(vecFotones, numFotones, escena);

    PhotonMap mapaFotones = std::move(generarPhotonMap(vecFotones));





    // Inicializado todo a color negro
    vector<vector<RGB>> coloresEscena(numPxlsAlto, vector<RGB>(numPxlsAncho, {0.0f, 0.0f, 0.0f}));


    /*
    if(rpp == 1){
        renderizarEscena1RPP(camara, numPxlsAncho, numPxlsAlto, escena, anchoPorPixel,
                             altoPorPixel, maxRebotes, numRayosMontecarlo, coloresEscena,
                             totalPixeles, printPixelesProcesados);
    } else {
        renderizarEscenaConAntialiasing(camara, numPxlsAncho, numPxlsAlto, escena, anchoPorPixel,
                                        altoPorPixel, maxRebotes, numRayosMontecarlo, coloresEscena,
                                        printPixelesProcesados, totalPixeles, rpp);
    }
    */
    
    //pintarEscenaEnPPM(nombreEscena, coloresEscena);
}
