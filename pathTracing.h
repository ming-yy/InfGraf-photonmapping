//*****************************************************************
// File:   pathTracing.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   noviembre 2024
// Coms:   Práctica 4 de Informática Gráfica
//*****************************************************************

#pragma once
#include "direccion.h"
#include "rayo.h"
#include "transformaciones.h"
#include "escena.h"
#include "rgb.h"
#include "camara.h"
#include "utilidades.h"
#include <optional>
#include <thread>


enum TipoRayo {
    ABSORBENTE = -1,
    DIFUSO = 0,
    ESPECULAR = 1,
    REFRACTANTE = 2
};



// Función que devuelve un valor aleatorio para azimut y otro para inclinación
// para muestreo uniforme de ángulo sólido.
void generarAzimutInclinacion(float& azimut, float& inclinacion);

// Función que devuelve las coordenadas cartesianas correspondientes de (azimut, inclinacion)
void getCoordenadasCartesianas(const float azimut, const float inclinacion,
                                float& x, float& y, float& z);

// Función que devuelve una dirección generada aleatoriamente empleando muestreo por importancia
// basado en el coseno. La dirección es aleatoria pero está contenida en el hipotético hemisferio
// superior que tiene como centro al punto por el que sale la normal <normal> y como altura a la
// dirección <normal> (|normal| == radio hemisferio). Devuelve en <prob> la probabilidad de que
// salga el rayo generado.
// Disclaimer: solo debería usarse para rayos difusos.
Direccion generarDireccionAleatoria(const Direccion& normal, float& prob);

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

// Función que calcula la reflectancia difusa de Lambert.
RGB calcBrdfDifusa(const RGB &kd);

// Función que calcula la reflectancia especular perfecta. Devuelve <ks> porque si se invoca
// esta función, es porque se ha decidido que el rayo va a ser especular y no hay pérdidas.
// <cosenoAnguloIncidencia>: coseno del ángulo de incidencia. Se usa para anular el coseno
//                  que se multiplica fuera por la ecuación de render. El sentido que tiene
//                  es que un rayo especular no se distribuye por todo el hemisferio (coseno)
//                  y por eso lo dividimos para que "solo haya un rayo" (el especular perfecto).
RGB calcBrdfEspecular(const RGB& ks);

// Función que calcula la refracción perfecta. Devuelve <kt> porque si se invoca esta
// función, es porque se ha decidido que el rayo va a ser refractante y no hay pérdidas.
RGB calcBtdf(const RGB& kt);

// Función que calcula BSDF dado todos los coeficientes y el tipo de rayo que es.
RGB calcBsdf(const BSDFs& coefs, const TipoRayo tipoRayo);

// Función que calcula el coseno del ángulo de incidencia, es decir, el ángulo formado
// por <n> y <d>. En general, <n> será la normal y <d> la otra dirección.
float calcCosenoAnguloIncidencia(const Direccion& d, const Direccion& n);

// Función que devuelve False si y solo si no hay ninguna fuente de luz que incide sobre el punto <p0>.
// Además, si devuelve False, la radiancia devuelta será 0.
// En caso contrario, devuelve True. Si devuelve True, también devolverá la radiancia correspondiente
// al punto <p0>.
// <wi>: dirección luz saliente inversa. Es un rayo hijo del rayo que sale de la cámara, y que
//       interseca con una primita en el punto <p0> y tiene la normal <normal> en ese punto.
//
//
//           CAMBIAR CAMBAIR CAMBIAR
//
// Disclaimer: solo debería usarse para rayos difusos. Si no lo es, debe devolver RGB(0,0,0).
RGB nextEventEstimation(const Punto& p0, const Direccion& normal, const Escena& escena,
                        const BSDFs& coefs);

// Función recursiva que calcula la radiancia del punto <origen> y las radiancias de los puntos
// intersectados tras <rebotesRestantes> rebotes
RGB recursividadRadianciaIndirecta(const Punto& origen, const Direccion &wo, const BSDFs &coefsPtoInterseccion, 
                                    const Direccion& normal, const Escena& escena,
                                    const unsigned rebotesRestantes);

// Función que, especificaciones contenidas por los parámetros pasados, devuelve la emisión indirecta
// para el punto <ptoIntersec> que tiene la normal <normal> respecto al objeto con el que ha intersecado.
RGB obtenerRadianciaSalienteIndirecta(const Escena& escena, const unsigned maxRebotes, 
                                        const unsigned numRayosMontecarlo, const Punto& ptoIntersec,
                                        const Direccion& wo, const BSDFs &coefsPtoInterseccion,
                                        const Direccion& normal);

// Función que devuelve la radiancia saliente total (directa + indirecta) dado un rayo y una escena,
// rebotando un máximo de <maxRebotes> veces y calculada a través de la media de <numRayosMontecarlo> rayos
// Cabe aclarar que el primer punto de intersección es siempre el mismo, los rayos "Montecarlo" comienzan
// aleatoriamente siempre desde este primer punto.
RGB obtenerRadianciaSalienteTotal(const Rayo &rayo, const Escena &escena, const unsigned maxRebotes,
                                  const unsigned numRayosMontecarlo);

// Método que muestra por pantalla el número de píxeles procesados (cada 100 píxeles)
void printPixelActual(unsigned totalPixeles, unsigned numPxlsAncho, unsigned ancho, unsigned alto);

// Función que calcula todas las emisiones y sus radiancias para la escena con la especificación pasada
// por los parámetros y las devuelve dentro de la matriz 2D <coloresEscena>. Los cálculos de emisiones
// son de 1 rayo por pixel.
void renderizarEscena1RPP(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                          const Escena& escena, float anchoPorPixel, float altoPorPixel,
                          const unsigned maxRebotes, const unsigned numRayosMontecarlo,
                          vector<vector<RGB>>& coloresEscena, const unsigned totalPixeles,
                          const bool printPixelesProcesados);

// Método que calcula todas las emisiones y sus radiancias para la escena con la especificación pasada
// por los parámetros y las devuelve dentro de la matriz 2D <coloresEscena>. Los cálculos de emisiones
// son de <rpp> rayos por pixel (rpp > 1).
void renderizarEscenaConAntialiasing(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                          const Escena& escena, float anchoPorPixel, float altoPorPixel,
                          const unsigned maxRebotes, const unsigned numRayosMontecarlo,
                          vector<vector<RGB>>& coloresEscena, const bool printPixelesProcesados,
                          const unsigned totalPixeles, const unsigned rpp);

// Función que ...
void renderizarEscena(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                      const Escena& escena, const string& nombreEscena, const unsigned rpp,
                      const unsigned maxRebotes, const unsigned numRayosMontecarlo, const bool printPixelesProcesados);

// Función auxiliar para renderizar un rango específico de filas
void renderizarRangoFilas(Camara& camara, unsigned inicioFila, unsigned finFila,
                          unsigned numPxlsAncho, const Escena& escena, float anchoPorPixel, float altoPorPixel,
                          const unsigned maxRebotes, const unsigned numRayosMontecarlo,
                          vector<vector<RGB>>& coloresEscena, const unsigned rpp, const unsigned thread);

// Función principal de renderizado con threads. Si no se indica el número de threads, se usará el máximo
// número disponible.
// Disclaimer: no poner un número de threads que sobrepase los núcleos lógicos del pc; no mejorará rendimiento
void renderizarEscenaConThreads(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                                const Escena& escena, const string& nombreEscena, const unsigned rpp,
                                const unsigned maxRebotes, const unsigned numRayosMontecarlo,
                                const bool printPixelesProcesados,
                                unsigned numThreads = std::thread::hardware_concurrency());
