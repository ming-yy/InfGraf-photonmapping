//*****************************************************************
// File:   photonMapping.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   diciembre 2024
// Coms:   Práctica 5 de Informática Gráfica
//*****************************************************************
#pragma once

#include "utilidades.h"
#include "camara.h"
#include "escena.h"
#include "photonMap.h"
#include "photonMapping.h"
#include "base.h"
#include <random>
#include <optional>
#include "parametros.h"

enum TipoRayo {
    ABSORBENTE = -1,
    DIFUSO = 0,
    ESPECULAR = 1,
    REFRACTANTE = 2
};

// Método que devuelve las coordenadas cartesianas correspondientes de (azimut, inclinacion)
void getCoordenadasCartesianas(const float azimut, const float inclinacion,
                                float& x, float& y, float& z);

// Método que devuelve un valor aleatorio para azimut y otro para inclinación
// para muestreo uniforme de ángulo sólido sobre la hemiesfera norte
void generarAzimutInclinacionHemiesfera(float& azimut, float& inclinacion);

// Función que devuelve una dirección generada aleatoriamente empleando muestreo por importancia
// basado en el coseno. La dirección es aleatoria pero está contenida en el hipotético hemisferio
// superior que tiene como centro al punto por el que sale la normal <normal> y como altura a la
// dirección <normal> (|normal| == radio hemisferio). Devuelve en <prob> la probabilidad de que
// salga el rayo generado.
// Disclaimer: solo debería usarse para rayos difusos.
Direccion generarDireccionAleatoriaHemiesfera(const Direccion& normal, float& prob);

// Método que devuelve un valor aleatorio para azimut y otro para inclinación
// para muestreo uniforme de ángulo sólido sobre la esfera
void generarAzimutInclinacionEsfera(float& azimut, float& inclinacion);

// Función que devuelve una dirección generada aleatoriamente contenida en la hipotética esfera (1,1,1)
// Disclaimer: usada en luces puntuales.
Direccion generarDireccionAleatoriaEsfera();

// Función que calcula la dirección incidente (especular perfecta, hacia donde proviene la luz
// reflejada).
// <wo>: dirección luz entrante. Rayo que ha chocado contra un elemento de la escena
//       en un punto
// <n>:  normal del objeto por el punto donde ha chocado wo.
Direccion calcDirEspecular(const Direccion& wo, const Direccion& n);

// Función que calcula la dirección incidente del rayo refractado empleando la Ley de Snell.
// Devuelve std::nullopt si y solo si ocurre Reflexión Interna Total. En caso contrario,
// devuelve la dirección refractada.
// <wo>: dirección entrante (que parte de la cámara o rebote anterior)
// <normal>: normal de la superficie por el punto en el que ha intersecado <wo>
// <ni>: índice de refracción del medio incidente
// <nr>: índice de refracción del medio de transmisión
std::optional<Direccion> calcDirRefractante(const Direccion& wo, const Direccion& normal,
                                            const float ni, const float nr);

// Función que dado el tipo de rayo y los parámetros de entrada, devuelve la dirección
// incidente correspondiente (la que se "aleja" de la cámara). Devuelve en <probRayo> la
// probabilidad de que salga el rayo que se ha decidido que salga.
Rayo obtenerRayoRuletaRusa(const TipoRayo tipoRayo, const Punto& origen, const Direccion& wo,
                           const Direccion& normal, float& probRayo);

// Función que realiza una selección probabilística del tipo de rayo que
// será disparado (difuso, especular o refractante) basándose en los coeficientes
// de la superficie (kd, ks, kt) proporcionados por <coefs>.
TipoRayo dispararRuletaRusa(const BSDFs& coefs, float& probRuleta);

// Función que calcula el coseno del ángulo de incidencia, es decir, el ángulo formado
// por <n> y <d>. En general, <n> será la normal y <d> la otra dirección.
float calcCosenoAnguloIncidencia(const Direccion& d, const Direccion& n);

// Función que calcula la reflectancia difusa de Lambert.
RGB calcBrdfDifusa(const RGB& kd);

// Método que, si caben más fotones en <vecFotones>, dado un punto <origen>, 
// una direccion de incidencia <wo_d>, un flujo, unos coeficientes del punto
// origen y una normal, guarda el fotón correspondiente en <vecFotones> si la
// superficie es difusa. Luego vuelve a llamarse recursivamente con los parámetros
// de la siguiente intersección, según el rayo devuelto tras la ruleta rusa.
void recursividadRandomWalk(vector<Photon>& vecFotones, const Escena& escena,
                            const RGB& radianciaInicial, RGB& radianciaActual, const Punto& origen, const Direccion &wo_d,
                            const BSDFs &coefsOrigen, const Direccion& normal);

// Método que, dada una luz, lanza un foton desde esa luz guarda los fotones que rebotan 
// en las superficies difusas en <vecFotones> (hasta que el randomWalk termine por absorción, 
// por no-intersección o por llegar al límite de vecFotones)
void comenzarRandomWalk(vector<Photon>& vecFotones, const Escena& escena, const Rayo& wi,
                        const RGB& flujoInicial, RGB& flujoRestante);

// Optamos por almacenar todos los rebotes difusos (incluido el primero)
// y saltarnos el NextEventEstimation posteriormente
int lanzarFotonesDeUnaLuz(vector<Photon>& vecFotones, const int numFotonesALanzar,
                         const RGB& flujoPorFoton, const LuzPuntual& luz, const Escena& escena);

// Función que devuelve la suma de los componentes maximos de las potencias de las <luces>
float calcularPotenciaTotal(const vector<LuzPuntual>& luces);

// Método que lanza max(<totalFotones>, vecFotones.max_size()) fotones en la escena 
// y los guarda en <vecFotones>
void paso1GenerarPhotonMap(PhotonMap& mapaFotones, size_t& numFotones, const int totalFotonesALanzar,
                            const Escena& escena);


// Método que imprime por pantalla un vector de fotones
void printVectorFotones(const vector<Photon>& vecFotones);

// Función que...
RGB radianciaKernelConstante(const Photon* photon, const float radio);

// Función que...
float distanciaEntreFotonYPunto(const Photon* photon, const Punto& centro);

// Función que...
RGB radianciaKernelGaussiano(const Photon* photon, const float radioMaximo, 
                                const Punto& centro);
                                
// Función que...
float maximoRadio(const Punto& ptoIntersec, const vector<const Photon*> fotonesCercanos);

// Función que...
RGB estimarEcuacionRender(const Escena& escena, const PhotonMap& mapaFotones, const size_t numFotones,
                          const Punto& ptoIntersec, const Direccion& dirIncidente,
                          const Direccion& normal, const BSDFs& coefsPtoInterseccion, const Parametros& parametros);

// Función que, dado un rayo (que proviene de la cámara y atraviesa un pixel), una escena y
// un mapa de fotones (producido por las luces de la escena), devuelve la radiancia del punto
// de intersección entre el rayo y la escena, usando la estimación de densidad del kernel con los
// fotones cercanos al punto intersecado para aproximar la ecuación de render. Dicha radiancia será
// el color que deberá tomar el pixel intersecado por el rayo
RGB obtenerRadianciaPixel(const Rayo& rayoIncidente, const Escena& escena, 
                            const PhotonMap& mapaFotones, const size_t numFotones, const Parametros& parametros);

// ...
void printPixelActual(unsigned totalPixeles, unsigned numPxlsAncho, unsigned ancho, unsigned alto);

// Método que lee los fotones dispersados por <mapaFotones> vistos desde <camara>
// y "colorea" los píxeles que forman la imagen usando la estimación de densidad de Kernel
void paso2LeerPhotonMap1RPP(const Camara& camara, const Escena& escena, const float anchoPorPixel, 
                    const float altoPorPixel, vector<vector<RGB>>& colorPixeles, const PhotonMap& mapaFotones, 
                    const size_t numFotones, const int totalPixeles, const Parametros& parametros);

// ...
void paso2LeerPhotonMapAntialiasing(const Camara& camara, const Escena& escena, const float anchoPorPixel, 
                    const float altoPorPixel, vector<vector<RGB>>& colorPixeles, const PhotonMap& mapaFotones, 
                    const size_t numFotones, const int totalPixeles, const Parametros& parametros);

// Método que genera una imagen utilizando el photonMapping                 
void renderizarEscena(const Camara& camara, const Escena& escena,
                    const string& nombreEscena, const Parametros& parametros);


//////// Parelelización

void renderizarRangoFilasPhotonMap1RPP(const Camara& camara, unsigned inicioFila, unsigned finFila,
                                       const Escena& escena, float anchoPorPixel,
                                       float altoPorPixel, vector<vector<RGB>>& colorPixeles,
                                       const PhotonMap& mapaFotones, size_t numFotones,
                                       const int totalPixeles, const Parametros& parametros);

void renderizarRangoFilasPhotonMapAntialiasing(const Camara& camara, unsigned inicioFila, unsigned finFila,
                                                const Escena& escena, float anchoPorPixel,
                                                float altoPorPixel, vector<vector<RGB>>& colorPixeles,
                                                const PhotonMap& mapaFotones, size_t numFotones,
                                                const int totalPixeles, const Parametros& parametros);

void renderizarEscenaConThreads(const Camara& camara, const Escena& escena, const string& nombreEscena, 
                                const Parametros& parametros, unsigned numThreads = thread::hardware_concurrency());