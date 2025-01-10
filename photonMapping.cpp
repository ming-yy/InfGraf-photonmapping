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
#include <atomic>

std::atomic<unsigned> pixelesProcesados{0};

void printTiempo(auto inicio, auto fin) {
    auto duracion_total = std::chrono::duration_cast<std::chrono::seconds>(fin - inicio);
    auto mins = std::chrono::duration_cast<std::chrono::minutes>(duracion_total);
    auto segs = duracion_total - mins;

    cout << endl << "=========================================" << endl;
    cout << "TIEMPO DE EJECUCION: " << mins.count() << "min " << segs.count() << "s" << endl;
    cout << "=========================================" << endl << endl;
}

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

TipoRayo dispararRuletaRusa(const BSDFs& coefs, float& probRuleta, const bool hayAbsorbente) {
    float maxKD = max(coefs.kd);
    float maxKS = max(coefs.ks);
    float maxKT = max(coefs.kt);
    float total;
    
    if (hayAbsorbente) {
        total = 1.0f;
    } else {
        total = maxKD + maxKS + maxKT;
    }

    float probDifuso = maxKD / total;
    float probEspecular = maxKS / total;
    float probRefractante = maxKT / total;
    
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
        if (hayAbsorbente) {    // No deberíamos necesitarlo
            probRuleta = 1.0f - probDifuso - probEspecular - probRefractante;
        }
        return ABSORBENTE;   // Absorbente
    }
}

float calcCosenoAnguloIncidencia(const Direccion& d, const Direccion& n){
    return abs(dot(n, d / modulo(d)));
}

RGB calcBrdfDifusa(const RGB& kd){
    return kd; // El M_PI que divide se anula con el de ProbDirRayo
}

void recursividadRandomWalk(vector<Photon>& vecFotonesGlobales, vector<Photon>& vecFotonesCausticos,
                            bool& fotonCaustico, const Escena& escena, const RGB& radianciaInicial,
                            RGB& radianciaActual, const Punto& origen, const Direccion &wo_d,
                            const BSDFs &coefsOrigen, const Direccion& normal, bool& primerFoton, const bool luzIndirecta) {
    if (vecFotonesGlobales.size() >= vecFotonesGlobales.max_size() ||
        vecFotonesCausticos.size() >= vecFotonesCausticos.max_size()) { // TERMINAL: no se pueden almacenar mas fotones
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
        if(!primerFoton){
            radianciaActual = radianciaActual * calcBrdfDifusa(coefsOrigen.kd);
            radianciaActual = radianciaActual / probTipoRayo;
            Photon foton = Photon(origen.coord, wo_d, radianciaActual);
            //cout << "Rayo difuso, metemos foton " << foton << endl;

            if(fotonCaustico){
                vecFotonesCausticos.push_back(foton);
            } else {
                vecFotonesGlobales.push_back(foton);
            }
        } else {
            // Si se ha activado "nee", el primer foton no se almacena
            primerFoton = false;
        }
        

        if(!luzIndirecta) return;

    } else { // ESPECULAR o REFRACTANTE
        if(!fotonCaustico) fotonCaustico = true;
    }

    float probDirRayo;
    Rayo wi = obtenerRayoRuletaRusa(tipoRayo, origen, wo_d, normal, probDirRayo);
    Primitiva* objIntersecado = nullptr;
    Punto ptoIntersec;
    Direccion nuevaNormal;
    bool hayIntersec = escena.interseccion(wi, ptoIntersec, nuevaNormal, &objIntersecado);
    if (!hayIntersec) {     // TERMINAL: el siguiente rayo (wi) no interseca con nada
        //cout << " -- Acaba recurisividad: Rayo no interseca con nada" << endl;
        return;
    }
    
    recursividadRandomWalk(vecFotonesGlobales, vecFotonesCausticos, fotonCaustico, 
                            escena, radianciaInicial, radianciaActual,
                            ptoIntersec, wi.d, objIntersecado->coeficientes, nuevaNormal, primerFoton, luzIndirecta);
}

void comenzarRandomWalk(vector<Photon>& vecFotonesGlobales, vector<Photon>& vecFotonesCausticos,
                        const Escena& escena, const Rayo& wi, const RGB& flujoInicial, RGB& flujoRestante,
                        const bool nee, const bool luzIndirecta){
    Punto ptoIntersec;
    Direccion normal;
    Primitiva* objIntersecado = nullptr;

    if(escena.interseccion(wi, ptoIntersec, normal, &objIntersecado)){
        // CUIDADO: si interseca con una fuente de luz !
        //RGB powerLuz;
        //if (escena.puntoPerteneceALuz(ptoIntersec, powerLuz)) {     // Para luces área
        //    cout << endl << "Choca contra luz de área, muestreamos otro camino." << endl;
        //    return;
        //}

        //cout << endl << "Comienza recursividad..." << endl;
        //cout << "Interseca en punto " << ptoIntersec << ", coefs " << coefsPtoInterseccion;
        //cout << " y normal " <<  normal << endl;
        bool fotonCaustico = false;
        bool primerFoton = nee;
        recursividadRandomWalk(vecFotonesGlobales, vecFotonesCausticos, fotonCaustico, escena,
                                flujoInicial, flujoRestante, ptoIntersec,
                                wi.d, objIntersecado->coeficientes, normal, primerFoton, luzIndirecta);
    } else {
        //cout << endl << "Rayo no interseca con nada, muestreamos otro camino." << endl;
    }
}

// Optamos por almacenar todos los rebotes difusos (incluido el primero)
// y saltarnos el NextEventEstimation posteriormente
int lanzarFotonesDeUnaLuz(vector<Photon>& vecFotonesGlobales, vector<Photon>& vecFotonesCausticos,
                          const int numFotonesALanzar, const RGB& flujoPorFoton, const LuzPuntual& luz,
                          const Escena& escena, const bool nee, const bool luzIndirecta){
    int numRandomWalksRestantes = numFotonesALanzar;
    int numFotonesLanzados = 0;
    
    while(numRandomWalksRestantes > 0 
            && vecFotonesGlobales.size() < vecFotonesGlobales.max_size()
            && vecFotonesGlobales.size() < vecFotonesGlobales.max_size()){
        if(numRandomWalksRestantes % 100000 == 0){ cout << "Fotones restantes: " << numRandomWalksRestantes << endl;}
        //cout << "Nuevo random path, numRandomWalksRestantes = " << numRandomWalksRestantes;
        //cout << ", numFotonesLanzados = " << numFotonesLanzados  << endl;
        numRandomWalksRestantes--;
        numFotonesLanzados++;

        Rayo wi(generarDireccionAleatoriaEsfera(), luz.c);
        //cout << "Rayo aleatorio generado desde luz para randomWalk = " << wi << endl;
        RGB flujoRestante = flujoPorFoton;
        comenzarRandomWalk(vecFotonesGlobales, vecFotonesCausticos, escena, wi, flujoPorFoton, flujoRestante, nee, luzIndirecta);
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

void paso1GenerarPhotonMap(PhotonMap& mapaFotonesGlobales, PhotonMap& mapaFotonesCausticos,
                            size_t& numFotonesGlobales, size_t& numFotonesCausticos, 
                            const int totalFotonesALanzar, const Escena& escena, const bool nee, const bool luzIndirecta){
    vector<Photon> vecFotonesGlobales;
    vector<Photon> vecFotonesCausticos;
    //cout << vecFotones.max_size() << endl;
    //cout << "Generando " << totalFotonesALanzar << " fotones en total..." << endl;
    float potenciaTotal = calcularPotenciaTotal(escena.luces);
    //cout << "Potencia total: " << potenciaTotal << endl;


    for(auto& luz : escena.luces) {  // Cada luz lanza num fotones proporcional a su potencia
        int numFotonesALanzar = totalFotonesALanzar * (modulo(luz.p) / potenciaTotal);
        RGB flujoFoton = (4 * M_PI * luz.p)/numFotonesALanzar;

        //cout << endl << "Generando " << numFotonesALanzar << " fotones para luz..." << endl;
        //cout << "Flujo inicial de cada foton: " << flujoFoton << endl;
        //cout << flujoFoton << endl;
        
        // No se ajusta la S posteriormente ya que ralentiza mucho la ejecución
        // y  realmente solo se pasa del límite del vector con totalFotonesALanzar
        // ridículamente altos
        lanzarFotonesDeUnaLuz(vecFotonesGlobales, vecFotonesCausticos, numFotonesALanzar,  
                                                        flujoFoton, luz, escena, nee, luzIndirecta);
    }

    //printVectorFotones(vecFotones);
    mapaFotonesGlobales = std::move(generarPhotonMap(vecFotonesGlobales));
    mapaFotonesCausticos = std::move(generarPhotonMap(vecFotonesCausticos));
    
    numFotonesGlobales = vecFotonesGlobales.size();
    numFotonesCausticos = vecFotonesCausticos.size();

    cout << "Numero total de fotones DIFUSOS guardados: " << numFotonesGlobales << endl;
    cout << "Numero total de fotones CAUSTICOS guardados: " << numFotonesCausticos << endl;
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

RGB radianciaKernelGaussiano(const Photon* photon, const float radioMaximo, 
                                const Punto& centro){
    float radioAFoton = distanciaEntreFotonYPunto(photon, centro);
    float raizDosPi = sqrt(2*M_PI);
    float cteNormalizacion = 1/(radioMaximo*raizDosPi);
    float numeradorExp = pow(radioAFoton, 2);
    float denominadorExp = 2*pow(radioMaximo, 2);
    float exponente = -numeradorExp/denominadorExp;
    float kernel = cteNormalizacion*pow(M_E, exponente);
    return photon->flujo*kernel;
}

RGB radianciaKernelConico(const Photon* photon, const float radioMaximo, 
                                const Punto& centro){
    float radioAFoton = distanciaEntreFotonYPunto(photon, centro);
    float kernel = 1 - (radioAFoton/radioMaximo);
    return photon->flujo*kernel;
}

RGB radianciaKernelEpanechnikov(const Photon* photon, const float radioMaximo, 
                                const Punto& centro){
    float radioAFoton = distanciaEntreFotonYPunto(photon, centro);
    float divisionAlCuadrado = pow((radioAFoton/radioMaximo), 2);
    float kernel = (3.0f/4.0f) * (1 - divisionAlCuadrado);
    return photon->flujo*kernel;
}

RGB radianciaKernelBipeso(const Photon* photon, const float radioMaximo, 
                                const Punto& centro){
    float radioAFoton = distanciaEntreFotonYPunto(photon, centro);
    float divisionAlCuadrado = pow((radioAFoton/radioMaximo), 2);
    float kernel = (15.0f/16.0f) * pow((1 - divisionAlCuadrado), 2);
    return photon->flujo*kernel;
}

RGB radianciaKernelLogistico(const Photon* photon, const float radioMaximo, 
                                const Punto& centro){
    float radioAFoton = distanciaEntreFotonYPunto(photon, centro);
    float division = radioAFoton/radioMaximo;
    float denominador = pow(M_E, division) + 2 + pow(M_E, -division);
    float kernel = 1/denominador;
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


RGB estimarEcuacionRender(const Escena& escena, const PhotonMap& mapaFotonesGlobales, const PhotonMap& mapaFotonesCausticos,
                            const size_t numFotonesGlobales, const size_t numFotonesCausticos, const Punto& ptoIntersec, const Direccion& dirIncidente,
                            const Direccion& normal, const BSDFs& coefsPtoInterseccion, const Parametros& parametros){
    vector<const Photon*> fotonesCercanosGlobales;
    if(parametros.tipoVecinosGlobales == RADIO) {
        fotonesCercanosPorRadio(mapaFotonesGlobales, ptoIntersec.coord, 
                                    static_cast<float>(parametros.vecinosGlobalesRadio), fotonesCercanosGlobales);
    } else if (parametros.tipoVecinosGlobales == PORCENTAJE){
        fotonesCercanosPorNumFotones(mapaFotonesGlobales, ptoIntersec.coord, 
                                    static_cast<unsigned long>(numFotonesGlobales * parametros.vecinosGlobalesNum), fotonesCercanosGlobales);
    } else if (parametros.tipoVecinosGlobales == NUMERO){
        fotonesCercanosPorNumFotones(mapaFotonesGlobales, ptoIntersec.coord, 
                                    static_cast<unsigned long>(parametros.vecinosGlobalesNum), fotonesCercanosGlobales);
    } else { // RADIONUMERO
        fotonesCercanos(mapaFotonesGlobales, ptoIntersec.coord, static_cast<float>(parametros.vecinosGlobalesRadio),
                        parametros.vecinosGlobalesNum, fotonesCercanosGlobales);
    }

    vector<const Photon*> fotonesCercanosCausticos;
    if(parametros.tipoVecinosCausticos == RADIO) {
        fotonesCercanosPorRadio(mapaFotonesCausticos, ptoIntersec.coord, 
                                    static_cast<float>(parametros.vecinosCausticosRadio), fotonesCercanosCausticos);
    } else if (parametros.tipoVecinosCausticos == PORCENTAJE){
        fotonesCercanosPorNumFotones(mapaFotonesCausticos, ptoIntersec.coord, 
                                    static_cast<unsigned long>(numFotonesCausticos * parametros.vecinosCausticosNum), fotonesCercanosCausticos);
    } else if (parametros.tipoVecinosCausticos == NUMERO){
        fotonesCercanosPorNumFotones(mapaFotonesCausticos, ptoIntersec.coord, 
                                    static_cast<unsigned long>(parametros.vecinosCausticosNum), fotonesCercanosCausticos);
    } else { // RADIONUMERO
        fotonesCercanos(mapaFotonesCausticos, ptoIntersec.coord, static_cast<float>(parametros.vecinosCausticosRadio),
                        parametros.vecinosCausticosNum, fotonesCercanosCausticos);
    }

    /*
    // Juntamos todos los fotones, Globales + Causticos
    vector<const Photon*> fotonesCercanos = fotonesCercanosGlobales;
    fotonesCercanos.insert(fotonesCercanos.end(), 
                         fotonesCercanosCausticos.begin(), 
                         fotonesCercanosCausticos.end());

    */

    RGB radiancia(0.0f, 0.0f, 0.0f);
    float radioMaximoCausticos = maximoRadio(ptoIntersec, fotonesCercanosCausticos);
    float radioMaximoGlobales = maximoRadio(ptoIntersec, fotonesCercanosGlobales);

    for (const Photon* photon : fotonesCercanosCausticos) {
        if (photon) {
            //radiancia += radianciaKernelConstante(photon, parametros.vecinosGlobalesRadio);
            //radiancia += radianciaKernelGaussiano(photon, radioMaximoCausticos, ptoIntersec);
            radiancia += radianciaKernelEpanechnikov(photon, radioMaximoCausticos, ptoIntersec);
            //radiancia += radianciaKernelBipeso(photon, radioMaximoCausticos, ptoIntersec);
            //radiancia += radianciaKernelLogistico(photon, radioMaximoCausticos, ptoIntersec);
            //radiancia += radianciaKernelConico(photon, radioMaximoCausticos, ptoIntersec);
        }
    }

    for (const Photon* photon : fotonesCercanosGlobales) {
        if (photon) {
            //radiancia += radianciaKernelConstante(photon, parametros.vecinosGlobalesRadio);
            //radiancia += radianciaKernelGaussiano(photon, radioMaximoGlobales, ptoIntersec);
            radiancia += radianciaKernelEpanechnikov(photon, radioMaximoGlobales, ptoIntersec);
            //radiancia += radianciaKernelBipeso(photon, radioMaximoGlobales, ptoIntersec);
            //radiancia += radianciaKernelLogistico(photon, radioMaximoGlobales, ptoIntersec);
            //radiancia += radianciaKernelConico(photon, radioMaximoGlobales, ptoIntersec);
        }
    }

    return radiancia;
}



RGB nextEventEstimation(const Punto& p0, const Direccion& normal, const Escena& escena,
                        const Primitiva* objOrigen) {
    RGB radianciaSaliente(0.0f, 0.0f, 0.0f);
    for (LuzPuntual luz : escena.luces) {
        if (!escena.luzIluminaPunto(p0, luz)) {
            continue;     // Si el punto no está iluminado, nos saltamos la iteración
        }

        Direccion dirIncidente = luz.c - p0;
        float cosAnguloIncidencia = calcCosenoAnguloIncidencia(normalizar(dirIncidente), normal);
        RGB reflectanciaBrdfDifusa = calcBrdfDifusa(objOrigen->kd(p0));
        RGB radianciaIncidente = luz.p / (modulo(dirIncidente) * modulo(dirIncidente));
        radianciaIncidente = radianciaIncidente * (reflectanciaBrdfDifusa * cosAnguloIncidencia);
        
        radianciaSaliente += radianciaIncidente;
    }
    
    int num_luces = 0;
    
    num_luces = max(num_luces, 1);
    return radianciaSaliente / num_luces;
}

RGB obtenerRadianciaPixel(const Rayo& rayoIncidente, const Escena& escena, 
                          const PhotonMap& mapaFotonesGlobales, const PhotonMap& mapaFotonesCausticos,
                          const size_t numFotonesGlobales, const size_t numFotonesCausticos,
                          const Parametros& parametros) {
    Punto ptoIntersec;
    Direccion normal;
    RGB radianciaDirecta(0.0f, 0.0f, 0.0f);
    RGB radianciaIndirecta(0.0f, 0.0f, 0.0f);
    BSDFs coefsPtoInterseccion;
    Rayo wi = rayoIncidente;
    bool choqueContraDifuso = false;
    bool hayInterseccion = false;
    Primitiva* objIntersecado = nullptr;
    float probTipoRayo;
    
    while (!choqueContraDifuso) {
        hayInterseccion = escena.interseccion(wi, ptoIntersec, normal, &objIntersecado);
        if (!hayInterseccion) {
            break;
        } else {
            TipoRayo tipoRayo = dispararRuletaRusa(objIntersecado->coeficientes, probTipoRayo, false);
            if (tipoRayo == DIFUSO) {
                choqueContraDifuso = true;
            } else if (tipoRayo == ESPECULAR || tipoRayo == REFRACTANTE) {
                float probDirRayo;
                wi = obtenerRayoRuletaRusa(tipoRayo, ptoIntersec, wi.d, normal, probDirRayo);
            } else {    // No debería pasar nunca
                cerr << "ERROR: rayo absorbente en paso 2" << endl;
                std::exit(EXIT_FAILURE);
            }
            
            
        }
    }
    
    if (choqueContraDifuso && hayInterseccion){
        if(parametros.nee){
            radianciaDirecta = nextEventEstimation(ptoIntersec, normal, escena, objIntersecado);
        }
        
        radianciaIndirecta = estimarEcuacionRender(escena, mapaFotonesGlobales, mapaFotonesCausticos,
                                                   numFotonesGlobales, numFotonesCausticos, ptoIntersec, wi.d,
                                                   normal, coefsPtoInterseccion, parametros);
        return (radianciaDirecta + radianciaIndirecta) / probTipoRayo;
    } else {
        return RGB({0.0f, 0.0f, 0.0f});
    }
}

void printPixelActual(unsigned totalPixeles, unsigned numPxlsAncho, unsigned ancho, unsigned alto){
    unsigned pixelActual = numPxlsAncho * ancho + alto + 1;
    if (pixelActual % 100 == 0 || pixelActual == totalPixeles) {
        cout << pixelActual << " pixeles de " << totalPixeles << endl;
    }
}

void paso2LeerPhotonMap1RPP(const Camara& camara, const Escena& escena, const float anchoPorPixel, 
                    const float altoPorPixel, vector<vector<RGB>>& colorPixeles, const PhotonMap& mapaFotonesGlobales,
                    const PhotonMap& mapaFotonesCausticos, const size_t numFotonesGlobales, 
                    const size_t numFotonesCausticos, const int totalPixeles, const Parametros& parametros){

    for (unsigned ancho = 0; ancho < parametros.numPxlsAncho; ++ancho) {
        for (unsigned alto = 0; alto < parametros.numPxlsAlto; ++alto) {
            if (parametros.printPixelesProcesados) printPixelActual(totalPixeles, parametros.numPxlsAncho, ancho, alto);
            
            Rayo rayo(Direccion(0.0f, 0.0f, 0.0f), Punto());
            rayo = camara.obtenerRayoCentroPixel(ancho, anchoPorPixel, alto, altoPorPixel);
            globalizarYNormalizarRayo(rayo, camara.o, camara.f, camara.u, camara.l);
            colorPixeles[alto][ancho] = obtenerRadianciaPixel(rayo, escena, mapaFotonesGlobales, mapaFotonesCausticos,
                                                                numFotonesGlobales, numFotonesCausticos, parametros);
        }
    }
}


void paso2LeerPhotonMapAntialiasing(const Camara& camara, const Escena& escena, const float anchoPorPixel, 
                    const float altoPorPixel, vector<vector<RGB>>& colorPixeles, const PhotonMap& mapaFotonesGlobales,
                    const PhotonMap& mapaFotonesCausticos, const size_t numFotonesGlobales, 
                    const size_t numFotonesCausticos, const int totalPixeles, const Parametros& parametros){

    for (unsigned ancho = 0; ancho < parametros.numPxlsAncho; ++ancho) {
        for (unsigned alto = 0; alto < parametros.numPxlsAlto; ++alto) {
            if (parametros.printPixelesProcesados) printPixelActual(totalPixeles, parametros.numPxlsAncho, ancho, alto);

            Rayo rayo(Direccion(0.0f, 0.0f, 0.0f), Punto());
            RGB radianciaTotal;
            for (unsigned i = 0; i < parametros.rpp; i++){
                rayo = camara.obtenerRayoAleatorioPixel(ancho, anchoPorPixel, alto, altoPorPixel);
                globalizarYNormalizarRayo(rayo, camara.o, camara.f, camara.u, camara.l);
                radianciaTotal += obtenerRadianciaPixel(rayo, escena, mapaFotonesGlobales, mapaFotonesCausticos,
                                                        numFotonesGlobales, numFotonesCausticos, parametros);
            }

            colorPixeles[alto][ancho] = radianciaTotal / parametros.rpp;
        }
    }
}
                      
void renderizarEscena(const Camara& camara, const Escena& escena,
                    const string& nombreEscena, const Parametros& parametros) {
    pixelesProcesados = 0;
    float tamanoPorPixel = std::min(camara.calcularAnchoPixel(parametros.numPxlsAncho), camara.calcularAltoPixel(parametros.numPxlsAlto));
    unsigned totalPixeles = parametros.numPxlsAlto * parametros.numPxlsAncho;
       
    PhotonMap mapaFotonesGlobales;    
    PhotonMap mapaFotonesCausticos;
    size_t numFotonesGlobales;
    size_t numFotonesCausticos;
    
    paso1GenerarPhotonMap(mapaFotonesGlobales, mapaFotonesCausticos, numFotonesGlobales, 
                            numFotonesCausticos, parametros.numRandomWalks, escena, parametros.nee, parametros.luzIndirecta);
    
    // Inicializado todo a color negro
    vector<vector<RGB>> colorPixeles(parametros.numPxlsAlto, vector<RGB>(parametros.numPxlsAncho, {0.0f, 0.0f, 0.0f}));

    if(parametros.rpp == 1){
        paso2LeerPhotonMap1RPP(camara, escena, tamanoPorPixel, tamanoPorPixel, colorPixeles,
                            mapaFotonesGlobales, mapaFotonesCausticos, numFotonesGlobales, 
                            numFotonesCausticos, totalPixeles, parametros);
    } else {
        paso2LeerPhotonMapAntialiasing(camara, escena, tamanoPorPixel, tamanoPorPixel, colorPixeles,
                            mapaFotonesGlobales, mapaFotonesCausticos, numFotonesGlobales, 
                            numFotonesCausticos, totalPixeles, parametros);
    }
    
    pintarEscenaEnPPM(nombreEscena, colorPixeles);
}


void renderizarRangoFilasPhotonMap1RPP(const Camara& camara, unsigned inicioFila, unsigned finFila,
                                       const Escena& escena, float anchoPorPixel,
                                       float altoPorPixel, vector<vector<RGB>>& colorPixeles,
                                       const PhotonMap& mapaFotonesGlobales, const PhotonMap& mapaFotonesCausticos,
                                       const size_t numFotonesGlobales, const size_t numFotonesCausticos, 
                                       const int totalPixeles, const Parametros& parametros) {
    for (unsigned alto = inicioFila; alto < finFila; ++alto) {
        for (unsigned ancho = 0; ancho < parametros.numPxlsAncho; ++ancho) {
            //if (printPixelesProcesados) printPixelActual(totalPixeles, numPxlsAncho, ancho, alto);

            Rayo rayo(Direccion(0.0f, 0.0f, 0.0f), Punto());
            rayo = camara.obtenerRayoCentroPixel(ancho, anchoPorPixel, alto, altoPorPixel);
            globalizarYNormalizarRayo(rayo, camara.o, camara.f, camara.u, camara.l);
            colorPixeles[alto][ancho] = obtenerRadianciaPixel(rayo, escena, mapaFotonesGlobales, mapaFotonesCausticos, 
                                                                numFotonesGlobales, numFotonesCausticos, parametros);
        }

        pixelesProcesados += parametros.numPxlsAncho;
        if(parametros.printPixelesProcesados) cout << "Progreso: " << pixelesProcesados
                            << " / " << totalPixeles << " pixeles procesados." << endl;
    }
}

void renderizarRangoFilasPhotonMapAntialiasing(const Camara& camara, unsigned inicioFila, unsigned finFila,
                                       const Escena& escena, float anchoPorPixel,
                                       float altoPorPixel, vector<vector<RGB>>& colorPixeles,
                                       const PhotonMap& mapaFotonesGlobales, const PhotonMap& mapaFotonesCausticos,
                                       const size_t numFotonesGlobales, const size_t numFotonesCausticos, 
                                       const int totalPixeles, const Parametros& parametros) {
    for (unsigned alto = inicioFila; alto < finFila; ++alto) {
        for (unsigned ancho = 0; ancho < parametros.numPxlsAncho; ++ancho) {
            //if (printPixelesProcesados) printPixelActual(totalPixeles, numPxlsAncho, ancho, alto);

            Rayo rayo(Direccion(0.0f, 0.0f, 0.0f), Punto());
            RGB radianciaTotal;
            for (unsigned i = 0; i < parametros.rpp; ++i) {
                rayo = camara.obtenerRayoAleatorioPixel(ancho, anchoPorPixel, alto, altoPorPixel);
                globalizarYNormalizarRayo(rayo, camara.o, camara.f, camara.u, camara.l);
                radianciaTotal += obtenerRadianciaPixel(rayo, escena, mapaFotonesGlobales, mapaFotonesCausticos, 
                                                                numFotonesGlobales, numFotonesCausticos, parametros);
            }

            colorPixeles[alto][ancho] = radianciaTotal / parametros.rpp;
        }
        
        pixelesProcesados += parametros.numPxlsAncho;
        if(parametros.printPixelesProcesados) cout << "Progreso: " << pixelesProcesados 
                            << " / " << totalPixeles << " pixeles procesados." << endl;
    }
}


void renderizarEscenaConThreads(const Camara& camara, const Escena& escena, const string& nombreEscena, 
                                const Parametros& parametros, unsigned numThreads) {
    pixelesProcesados = 0;
    auto inicio = std::chrono::high_resolution_clock::now();

    float tamanoPorPixel = std::min(camara.calcularAnchoPixel(parametros.numPxlsAncho), camara.calcularAltoPixel(parametros.numPxlsAlto));
    unsigned totalPixeles = parametros.numPxlsAncho * parametros.numPxlsAlto;

    PhotonMap mapaFotonesGlobales;
    PhotonMap mapaFotonesCausticos;
    size_t numFotonesGlobales;
    size_t numFotonesCausticos;
    paso1GenerarPhotonMap(mapaFotonesGlobales, mapaFotonesCausticos, numFotonesGlobales,
                            numFotonesCausticos, parametros.numRandomWalks, escena, parametros.nee, parametros.luzIndirecta);

    vector<vector<RGB>> colorPixeles(parametros.numPxlsAlto, vector<RGB>(parametros.numPxlsAncho, {0.0f, 0.0f, 0.0f}));

    unsigned filasPorThread = parametros.numPxlsAlto / numThreads;
    unsigned filasRestantes = parametros.numPxlsAlto % numThreads;

    vector<thread> threads;
    unsigned inicioFila = 0;

    if(parametros.printPixelesProcesados) cout << "Progreso: 0 / " << totalPixeles << " pixeles procesados." << endl;
    for (unsigned t = 0; t < numThreads; ++t) {
        unsigned finFila = inicioFila + filasPorThread + (t < filasRestantes ? 1 : 0);

        if (parametros.rpp == 1) {
            threads.emplace_back([&](unsigned inicio, unsigned fin) {
                renderizarRangoFilasPhotonMap1RPP(camara, inicio, fin, escena, tamanoPorPixel, 
                                                    tamanoPorPixel, colorPixeles, mapaFotonesGlobales, mapaFotonesCausticos, 
                                                    numFotonesGlobales, numFotonesCausticos, totalPixeles, parametros);
            }, inicioFila, finFila);
        } else {
            threads.emplace_back([&](unsigned inicio, unsigned fin) {
                renderizarRangoFilasPhotonMapAntialiasing(camara, inicio, fin, escena, tamanoPorPixel, 
                                                            tamanoPorPixel, colorPixeles, mapaFotonesGlobales, mapaFotonesCausticos, 
                                                    numFotonesGlobales, numFotonesCausticos, totalPixeles, parametros);
            }, inicioFila, finFila);
        }

        inicioFila = finFila;
    }

    for (auto& thread : threads) {
        thread.join();
    }

    string nombreArchivo = "./" + nombreEscena + ".ppm";
    pintarEscenaEnPPM(nombreArchivo, colorPixeles);
    transformarFicheroPPM(nombreArchivo, 5);
          
    auto fin = std::chrono::high_resolution_clock::now();
    printTiempo(inicio, fin);
}
