//*****************************************************************
// File:   photonMapping.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   diciembre 2024
// Coms:   Práctica 5 de Informática Gráfica
//*****************************************************************

#include "photonMapping.h"
#include "base.h"
#include "gestorPPM.h"
#include <random>
#include <thread>

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

Direccion calcDirEspecular(const Direccion& wo, const Direccion& n) {
    return normalizar(wo - n * 2.0f * dot(wo, n));
}

std::optional<Direccion> calcDirRefractante(const Direccion& wo, const Direccion& normal,
                                            const float ni, const float nr) {
    Direccion woo = wo * (-1);      // woo apunta hacia la cámara
    Direccion n = normal;
    float eta = nr / ni;
    float cosThetaI = dot(n, woo);
    if (cosThetaI < 0) {    // Rayo entrando al material
        eta = 1 / eta;
        cosThetaI = - cosThetaI;
        n = n * (-1);
    }
    float sin2ThetaI = max(0.0f, 1 - cosThetaI * cosThetaI);
    float sin2ThetaT = sin2ThetaI / (eta * eta);
    if (sin2ThetaT >= 1) {      // Reflexión interna total
        return std::nullopt;
    }
    
    float cosThetaT = sqrt(max(0.0f, 1.0f - sin2ThetaT));
    Direccion wt = (woo * (-1)) / eta + n * (cosThetaI / eta - cosThetaT);
    return normalizar(wt);
    
}

Rayo obtenerRayoRuletaRusa(const TipoRayo tipoRayo, const Punto& origen, const Direccion& wo,
                           const Direccion& normal, float& probRayo) {
    Rayo wi;
    wi.o = origen;
    probRayo = 1.0f;
    if (tipoRayo == DIFUSO) {
        wi.d = generarDireccionAleatoriaHemiesfera(normal, probRayo);
    } else if (tipoRayo == ESPECULAR) {
        wi.d = calcDirEspecular(wo, normal);
    } else if (tipoRayo == REFRACTANTE){
        auto wt = calcDirRefractante(wo, normal, 1.0f, 1.5f);
        if (wt) {       // Hay refracción
            wi.d = *wt;
        } else {        // Reflexión interna total
            wi.d = calcDirEspecular(wo, normal);
        }
    }
    return wi;
}

TipoRayo dispararRuletaRusa(const BSDFs& coefs, float& probRuleta) {
    float maxKD = max(coefs.kd);
    float maxKS = max(coefs.ks);
    float maxKT = max(coefs.kt);
    float total = 1.0f;

    float probDifuso = maxKD / total;
    float probEspecular = maxKS / total;
    float probRefractante = maxKT / total;
    float probAbsorbente = 1.0f - probDifuso - probEspecular - probRefractante;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    float bala = dist(gen);     // Random float entre (0,1)

    if (bala <= probDifuso) {
        probRuleta = probDifuso;
        return DIFUSO;  // Rayo difuso
    } else if (bala <= probDifuso + probEspecular) {
        probRuleta = probEspecular;
        return ESPECULAR;  // Rayo especular
    } else if (bala <= probDifuso + probEspecular + probRefractante) {
        probRuleta = probRefractante;
        return REFRACTANTE;  // Rayo refractante
    } else {
        return ABSORBENTE;   // Absorbente
    }
}

float calcCosenoAnguloIncidencia(const Direccion& d, const Direccion& n){
    return abs(dot(n, d / modulo(d)));
}

RGB calcBrdfDifusa(const RGB& kd){
    return kd; // El M_PI que divide se anula con el de ProbDirRayo
}

void recursividadRandomWalk(vector<Photon>& vecFotones, const Escena& escena,
                            const RGB& radianciaInicial, RGB& radianciaActual, const Punto& origen,
                            const Direccion &wo_d, const BSDFs &coefsOrigen, const Direccion& normal) {
    if (vecFotones.size() >= vecFotones.max_size()) { // TERMINAL: no se pueden almacenar mas fotones
        cout << endl << "-- LIMITE DE FOTONES ALCANZADO --" << endl << endl;
        return;
    }

    if (modulo(radianciaActual) <= MARGEN_ERROR_FOTON) {    // TERMINAL: fotón ha perdido casi toda la energía
        return;
    }

    /*
    if(modulo(radianciaActual) > modulo(radianciaInicial)){
        // Paramos la recursividad cuando el foton se ilumina mas que la luz (PRUEBA)
        return;
    }
    */

    float probTipoRayo;
    TipoRayo tipoRayo = dispararRuletaRusa(coefsOrigen, probTipoRayo);
    if (tipoRayo == ABSORBENTE) {       // TERMINAL: rayo absorbente
        //cout << " -- Acaba recurisividad: Rayo absorbente" << endl;
        return;
    } else if (tipoRayo == DIFUSO) {
        radianciaActual = radianciaActual * calcBrdfDifusa(coefsOrigen.kd);
        radianciaActual = radianciaActual / probTipoRayo;
        Photon foton = Photon(origen.coord, wo_d, radianciaActual);
        //cout << "Rayo difuso, metemos foton " << foton << endl;
        vecFotones.push_back(foton);
    }

    float probDirRayo;
    Rayo wi = obtenerRayoRuletaRusa(tipoRayo, origen, wo_d, normal, probDirRayo);
    
    BSDFs coefsPtoIntersec;
    Punto ptoIntersec;
    Direccion nuevaNormal;
    bool hayIntersec = escena.interseccion(wi, coefsPtoIntersec, ptoIntersec, nuevaNormal);
    if (!hayIntersec) {     // TERMINAL: el siguiente rayo (wi) no interseca con nada
        //cout << " -- Acaba recurisividad: Rayo no interseca con nada" << endl;
        return;
    }
    
    recursividadRandomWalk(vecFotones, escena, radianciaInicial, radianciaActual,
                           ptoIntersec, wi.d, coefsPtoIntersec, nuevaNormal);
}

void comenzarRandomWalk(vector<Photon>& vecFotones, const Escena& escena, const Rayo& wi,
                        const RGB& flujoInicial, RGB& flujoRestante){
    Punto ptoIntersec;
    BSDFs coefsPtoInterseccion;
    Direccion normal;
    
    if(escena.interseccion(wi, coefsPtoInterseccion, ptoIntersec, normal)){
        // CUIDADO: si interseca con una fuente de luz !
        //RGB powerLuz;
        //if (escena.puntoPerteneceALuz(ptoIntersec, powerLuz)) {     // Para luces área
        //    cout << endl << "Choca contra luz de área, muestreamos otro camino." << endl;
        //    return;
        //}

        //cout << endl << "Comienza recursividad..." << endl;
        //cout << "Interseca en punto " << ptoIntersec << ", coefs " << coefsPtoInterseccion;
        //cout << " y normal " <<  normal << endl;
        recursividadRandomWalk(vecFotones, escena, flujoInicial, flujoRestante, ptoIntersec,
                               wi.d, coefsPtoInterseccion, normal);
    } else {
        //cout << endl << "Rayo no interseca con nada, muestreamos otro camino." << endl;
    }
}

// Optamos por almacenar todos los rebotes difusos (incluido el primero)
// y saltarnos el NextEventEstimation posteriormente
int lanzarFotonesDeUnaLuz(vector<Photon>& vecFotones, const int numFotonesALanzar,
                          const RGB& flujoPorFoton, const LuzPuntual& luz, const Escena& escena){
    int numRandomWalksRestantes = numFotonesALanzar;
    int numFotonesLanzados = 0;
    
    while(numRandomWalksRestantes > 0 && vecFotones.size() < vecFotones.max_size()){
        if(numRandomWalksRestantes % 100000 == 0){ cout << "Fotones restantes: " << numRandomWalksRestantes << endl;}
        //cout << "Nuevo random path, numRandomWalksRestantes = " << numRandomWalksRestantes;
        //cout << ", numFotonesLanzados = " << numFotonesLanzados  << endl;
        numRandomWalksRestantes--;
        numFotonesLanzados++;

        Rayo wi(generarDireccionAleatoriaEsfera(), luz.c);
        //cout << "Rayo aleatorio generado desde luz para randomWalk = " << wi << endl;
        RGB flujoRestante = flujoPorFoton;
        comenzarRandomWalk(vecFotones, escena, wi, flujoPorFoton, flujoRestante);
    }

    return numFotonesLanzados;
}

float calcularPotenciaTotal(const vector<LuzPuntual>& luces){
    float total;
    for(auto& luz : luces){
        total += modulo(luz.p);
    }

    return total;
}

void paso1GenerarPhotonMap(PhotonMap& mapaFotones, size_t& numFotones, const int totalFotonesALanzar,
                           const Escena& escena){
    vector<Photon> vecFotones;
    //cout << vecFotones.max_size() << endl;
    //cout << "Generando " << totalFotonesALanzar << " fotones en total..." << endl;
    float potenciaTotal = calcularPotenciaTotal(escena.luces);
    //cout << "Potencia total: " << potenciaTotal << endl;


    for(auto& luz : escena.luces) {  // Cada luz lanza num fotones proporcional a su potencia
        int numFotonesALanzar = totalFotonesALanzar * (modulo(luz.p) / potenciaTotal);
        int numFotonesYaAlmacenados = static_cast<int>(vecFotones.size());
        RGB flujoFoton = 4 * M_PI * luz.p; // Se dividirá por S (numFotonesLanzados) posteriormente

        //cout << endl << "Generando " << numFotonesALanzar << " fotones para luz..." << endl;
        //cout << "Flujo inicial de cada foton: " << flujoFoton << endl;
        //cout << flujoFoton << endl;
        int numFotonesLanzados = lanzarFotonesDeUnaLuz(vecFotones, numFotonesALanzar,  
                                                        flujoFoton, luz, escena);

        // Ajustamos la S después de lanzar los randomWalks (ahora que sabemos cuántos fotones
        // se han lanzado realmente, teniendo en cuenta el límite del tamaño del vector)
        int offset = numFotonesYaAlmacenados;
        for(int i = 0; i < numFotonesLanzados; i++) {
            // Medida de seguridad, nunca debería saltar porque lanzarFotonesDeUnaLuz() ya lo detecta.
            if(i + offset >= vecFotones.max_size()) {
                cout << "Se ha alcanzado el máximo de fotones almacenables: " << 
                                                        vecFotones.max_size() << endl;
                return;
            }
            
            vecFotones[i + offset].flujo = vecFotones[i + offset].flujo/numFotonesLanzados;
        }
    }

    //printVectorFotones(vecFotones);
    mapaFotones = std::move(generarPhotonMap(vecFotones));
    numFotones = vecFotones.size();
    cout << "Numero total de fotones guardados: " << numFotones << endl;
}

void printVectorFotones(const vector<Photon>& vecFotones){
    for (auto& foton : vecFotones){
        cout << foton << endl;
    }
}

RGB radianciaKernelConstante(const Photon* photon, const float radio){
    return photon->flujo/(M_PI * pow(radio, 2.0f));
}

float distanciaEntreFotonYPunto(const Photon* photon, const Punto& centro){
    Punto puntoFoton = Punto(photon->coord);
    return modulo(puntoFoton - centro);
}

RGB radianciaKernelGaussiano(const Photon* photon, const float radioAFoton, 
                                const float radioMaximo, const Punto& centro){
    float raizDosPi = sqrt(2*M_PI);
    float cteNormalizacion = 1/(radioMaximo*raizDosPi);
    Punto puntoFoton = Punto(photon->coord);
    float numeradorExp = pow(radioAFoton, 2);
    float denominadorExp = 2*pow(radioMaximo, 2);
    float exponente = -numeradorExp/denominadorExp;
    float kernel = cteNormalizacion*pow(M_E, exponente);
    return photon->flujo*kernel;
}

float maximoRadio(const Punto& ptoIntersec, const vector<const Photon*> fotonesCercanos){
    float maximoRadio = 0.0f;
    for(auto& foton : fotonesCercanos){
        float distanciaFoton = distanciaEntreFotonYPunto(foton, ptoIntersec);
        if(distanciaFoton > maximoRadio){
            maximoRadio = distanciaFoton;
        }
    }
    return maximoRadio;
}

RGB estimarEcuacionRender(const Escena& escena, const PhotonMap& mapaFotones, const size_t numFotones,
                          const Punto& ptoIntersec, const Direccion& dirIncidente,
                          const Direccion& normal, const BSDFs& coefsPtoInterseccion){
    vector<const Photon*> fotonesCercanos;
    //float radio = 0.01f;
    //fotonesCercanosPorRadio(mapaFotones, ptoIntersec.coord, radio, fotonesCercanos);
    float porcentaje = 0.01;
    unsigned long porcentajeFotones = static_cast<unsigned long>(numFotones*porcentaje);
    fotonesCercanosPorNumFotones(mapaFotones, ptoIntersec.coord, static_cast<unsigned long>(porcentajeFotones), fotonesCercanos);
    //cout << "Num fotones cercanos: " << fotonesCercanos.size() << endl;
    float radioMaximo = maximoRadio(ptoIntersec, fotonesCercanos);

    RGB radiancia(0.0f, 0.0f, 0.0f);

    for (const Photon* photon : fotonesCercanos) {
        if (photon) {
            //radiancia += radianciaKernelConstante(photon, radio);
            radiancia += radianciaKernelGaussiano(photon, 
                                                    distanciaEntreFotonYPunto(photon, ptoIntersec), 
                                                    radioMaximo, ptoIntersec);
        }
    }

    return radiancia;
}

RGB obtenerRadianciaPixel(const Rayo& rayoIncidente, const Escena& escena, 
                            const PhotonMap& mapaFotones, const size_t numFotones) {
    Punto ptoIntersec;
    Direccion normal;
    RGB radiancia(0.0f, 0.0f, 0.0f);
    BSDFs coefsPtoInterseccion;
    Rayo wi = rayoIncidente;
    bool choqueContraDifuso = false;
    bool hayInterseccion = false;
    bool choqueContraAbsorbente = false;
    
    while (!choqueContraAbsorbente && !choqueContraDifuso) {
    //while (!choqueContraDifuso) {
        hayInterseccion = escena.interseccion(wi, coefsPtoInterseccion, ptoIntersec, normal);
        if (!hayInterseccion) { // No es muy elegante tbh
            break;
        } else {
            float probTipoRayo;
              // Opcion 1: puede salir absorbente
            TipoRayo tipoRayo = dispararRuletaRusa(coefsPtoInterseccion, probTipoRayo);
            if (tipoRayo == ABSORBENTE) {       // TERMINAL: rayo absorbente
                //cout << " -- Acaba recurisividad: Rayo absorbente" << endl;
                choqueContraAbsorbente = true;
            } else if (tipoRayo == DIFUSO) {
                choqueContraDifuso = true;
            } else if (tipoRayo == ESPECULAR || tipoRayo == REFRACTANTE) {
                float probDirRayo;
                wi = obtenerRayoRuletaRusa(tipoRayo, ptoIntersec, wi.d, normal, probDirRayo);
            }
            
            
            /*
            // Opción 2: NO puede salir absorbente --> teóricamente está mal
            TipoRayo tipoRayo = ABSORBENTE;
            while (tipoRayo == ABSORBENTE) {
                tipoRayo = dispararRuletaRusa(coefsPtoInterseccion, probTipoRayo);
            }
            
            if (tipoRayo == DIFUSO) {
                choqueContraDifuso = true;
            } else if (tipoRayo == ESPECULAR || tipoRayo == REFRACTANTE) {
                float probDirRayo;
                wi = obtenerRayoRuletaRusa(tipoRayo, ptoIntersec, wi.d, normal, probDirRayo);
            }
            */
        }
    }
    
    if (choqueContraDifuso && hayInterseccion) {
        radiancia = estimarEcuacionRender(escena, mapaFotones, numFotones, ptoIntersec, wi.d,
                                          normal, coefsPtoInterseccion);
    }

    return radiancia;
}

void printPixelActual(unsigned totalPixeles, unsigned numPxlsAncho, unsigned ancho, unsigned alto){
    unsigned pixelActual = numPxlsAncho * ancho + alto + 1;
    if (pixelActual % 100 == 0 || pixelActual == totalPixeles) {
        cout << pixelActual << " pixeles de " << totalPixeles << endl;
    }
}

void paso2LeerPhotonMap1RPP(const Camara& camara, const Escena& escena, const unsigned numPxlsAncho, 
                    const unsigned numPxlsAlto, const float anchoPorPixel, const float altoPorPixel,
                    vector<vector<RGB>>& colorPixeles, const PhotonMap& mapaFotones, const size_t numFotones,
                    const bool printPixelesProcesados, const int totalPixeles){

    for (unsigned ancho = 0; ancho < numPxlsAncho; ++ancho) {
        for (unsigned alto = 0; alto < numPxlsAlto; ++alto) {
            if (printPixelesProcesados) printPixelActual(totalPixeles, numPxlsAncho, ancho, alto);
            
            Rayo rayo(Direccion(0.0f, 0.0f, 0.0f), Punto());
            rayo = camara.obtenerRayoCentroPixel(ancho, anchoPorPixel, alto, altoPorPixel);
            globalizarYNormalizarRayo(rayo, camara.o, camara.f, camara.u, camara.l);
            colorPixeles[alto][ancho] = obtenerRadianciaPixel(rayo, escena, mapaFotones, numFotones);
        }
    }
}


void paso2LeerPhotonMapAntialiasing(const Camara& camara, const Escena& escena, const unsigned numPxlsAncho, 
                    const unsigned numPxlsAlto, const float anchoPorPixel, const float altoPorPixel,
                    vector<vector<RGB>>& colorPixeles, const PhotonMap& mapaFotones, const size_t numFotones,
                    const bool printPixelesProcesados, const int totalPixeles, const unsigned rpp){

    for (unsigned ancho = 0; ancho < numPxlsAncho; ++ancho) {
        for (unsigned alto = 0; alto < numPxlsAlto; ++alto) {
            if (printPixelesProcesados) printPixelActual(totalPixeles, numPxlsAncho, ancho, alto);

            Rayo rayo(Direccion(0.0f, 0.0f, 0.0f), Punto());
            RGB radianciaTotal;
            for (unsigned i = 0; i < rpp; i++){
                rayo = camara.obtenerRayoAleatorioPixel(ancho, anchoPorPixel, alto, altoPorPixel);
                globalizarYNormalizarRayo(rayo, camara.o, camara.f, camara.u, camara.l);
                radianciaTotal += obtenerRadianciaPixel(rayo, escena, mapaFotones, numFotones);
            }

            colorPixeles[alto][ancho] = radianciaTotal / rpp;
        }
    }
}
                      
void renderizarEscena(const Camara& camara, const unsigned numPxlsAncho, const unsigned numPxlsAlto,
                      const Escena& escena, const string& nombreEscena, const unsigned rpp,
                      const int totalFotones, const bool printPixelesProcesados) {

    float anchoPorPixel = camara.calcularAnchoPixel(numPxlsAncho);
    float altoPorPixel = camara.calcularAltoPixel(numPxlsAlto);
    unsigned totalPixeles = numPxlsAlto * numPxlsAncho;
    
    if (printPixelesProcesados) cout << "Procesando pixeles..." << endl << "0 pixeles de " << totalPixeles << endl;

   
    PhotonMap mapaFotones;
    size_t numFotones;

    paso1GenerarPhotonMap(mapaFotones, numFotones, totalFotones, escena);
    
    // Inicializado todo a color negro
    vector<vector<RGB>> colorPixeles(numPxlsAlto, vector<RGB>(numPxlsAncho, {0.0f, 0.0f, 0.0f}));

    if(rpp == 1){
        paso2LeerPhotonMap1RPP(camara,  escena, numPxlsAncho, numPxlsAlto,
                            anchoPorPixel, altoPorPixel, colorPixeles,
                            mapaFotones, numFotones, printPixelesProcesados, totalPixeles);
    } else {
        paso2LeerPhotonMapAntialiasing(camara,  escena, numPxlsAncho, numPxlsAlto,
                            anchoPorPixel, altoPorPixel, colorPixeles,
                            mapaFotones, numFotones, printPixelesProcesados, totalPixeles, rpp);
    }
    
    pintarEscenaEnPPM(nombreEscena, colorPixeles);
}

void renderizarRangoFilasPhotonMap1RPP(const Camara& camara, unsigned inicioFila, unsigned finFila,
                                       unsigned numPxlsAncho, const Escena& escena, float anchoPorPixel,
                                       float altoPorPixel, vector<vector<RGB>>& colorPixeles,
                                       const PhotonMap& mapaFotones, size_t numFotones,
                                       const bool printPixelesProcesados, const int totalPixeles) {
    for (unsigned alto = inicioFila; alto < finFila; ++alto) {
        for (unsigned ancho = 0; ancho < numPxlsAncho; ++ancho) {
            //if (printPixelesProcesados) printPixelActual(totalPixeles, numPxlsAncho, ancho, alto);

            Rayo rayo(Direccion(0.0f, 0.0f, 0.0f), Punto());
            rayo = camara.obtenerRayoCentroPixel(ancho, anchoPorPixel, alto, altoPorPixel);
            globalizarYNormalizarRayo(rayo, camara.o, camara.f, camara.u, camara.l);
            colorPixeles[alto][ancho] = obtenerRadianciaPixel(rayo, escena, mapaFotones, numFotones);
        }
    }
}

void renderizarRangoFilasPhotonMapAntialiasing(const Camara& camara, unsigned inicioFila, unsigned finFila,
                                               unsigned numPxlsAncho, const Escena& escena, float anchoPorPixel,
                                               float altoPorPixel, vector<vector<RGB>>& colorPixeles,
                                               const PhotonMap& mapaFotones, size_t numFotones,
                                               const bool printPixelesProcesados, const int totalPixeles,
                                               const unsigned rpp) {
    for (unsigned alto = inicioFila; alto < finFila; ++alto) {
        for (unsigned ancho = 0; ancho < numPxlsAncho; ++ancho) {
            //if (printPixelesProcesados) printPixelActual(totalPixeles, numPxlsAncho, ancho, alto);

            Rayo rayo(Direccion(0.0f, 0.0f, 0.0f), Punto());
            RGB radianciaTotal;
            for (unsigned i = 0; i < rpp; ++i) {
                rayo = camara.obtenerRayoAleatorioPixel(ancho, anchoPorPixel, alto, altoPorPixel);
                globalizarYNormalizarRayo(rayo, camara.o, camara.f, camara.u, camara.l);
                radianciaTotal += obtenerRadianciaPixel(rayo, escena, mapaFotones, numFotones);
            }

            colorPixeles[alto][ancho] = radianciaTotal / rpp;
        }
    }
}

void renderizarEscenaConThreads(const Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                                const Escena& escena, const string& nombreEscena, const unsigned rpp,
                                const int totalFotones, const bool printPixelesProcesados, unsigned numThreads) {
    float anchoPorPixel = camara.calcularAnchoPixel(numPxlsAncho);
    float altoPorPixel = camara.calcularAltoPixel(numPxlsAlto);
    unsigned totalPixeles = numPxlsAncho * numPxlsAlto;

    if (printPixelesProcesados) cout << "Procesando pixeles..." << endl << "0 pixeles de " << totalPixeles << endl;

    PhotonMap mapaFotones;
    size_t numFotones;
    paso1GenerarPhotonMap(mapaFotones, numFotones, totalFotones, escena);

    vector<vector<RGB>> colorPixeles(numPxlsAlto, vector<RGB>(numPxlsAncho, {0.0f, 0.0f, 0.0f}));

    unsigned filasPorThread = numPxlsAlto / numThreads;
    unsigned filasRestantes = numPxlsAlto % numThreads;

    vector<thread> threads;
    unsigned inicioFila = 0;

    for (unsigned t = 0; t < numThreads; ++t) {
        unsigned finFila = inicioFila + filasPorThread + (t < filasRestantes ? 1 : 0);

        if (rpp == 1) {
            threads.emplace_back(renderizarRangoFilasPhotonMap1RPP, std::ref(camara), inicioFila, finFila,
                                 numPxlsAncho, std::ref(escena), anchoPorPixel, altoPorPixel,
                                 std::ref(colorPixeles), std::ref(mapaFotones), numFotones,
                                 printPixelesProcesados, totalPixeles);
        } else {
            threads.emplace_back(renderizarRangoFilasPhotonMapAntialiasing, std::ref(camara), inicioFila, finFila,
                                 numPxlsAncho, std::ref(escena), anchoPorPixel, altoPorPixel,
                                 std::ref(colorPixeles), std::ref(mapaFotones), numFotones,
                                 printPixelesProcesados, totalPixeles, rpp);
        }
        inicioFila = finFila;
    }

    for (auto& thread : threads) {
        thread.join();
    }

    pintarEscenaEnPPM(nombreEscena, colorPixeles);
}
