//*****************************************************************
// File:   photonMapping.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   diciembre 2024
// Coms:   Práctica 5 de Informática Gráfica
//*****************************************************************

#include "utilidades.h"
#include "camara.h"
#include "escena.h"
#include "photonMap.h"
#include <optional>


enum TipoRayo {
    ABSORBENTE = -1,
    DIFUSO = 0,
    ESPECULAR = 1,
    REFRACTANTE = 2
};

void renderizarEscena(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                      const Escena& escena, const string& nombreEscena, const unsigned rpp,
                      const unsigned maxRebotes, const unsigned numRayosMontecarlo,
                      const bool printPixelesProcesados);

