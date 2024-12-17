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
    return kd / M_PI;
}

void recursividadRandomWalk(vector<Photon>& vecFotones, const Escena& escena,
                            const RGB& flujoActual, const Punto& origen, const Direccion &wo_d,
                            const BSDFs &coefsOrigen, const Direccion& normal){
    if(vecFotones.size() >= vecFotones.max_size()){ // TERMINAL: no se pueden almacenar mas fotones
        cout << endl << "-- LIMITE DE FOTONES ALCANZADO --" << endl << endl;
        return;
    }

    float probRuleta;
    TipoRayo tipoRayo = dispararRuletaRusa(coefsOrigen, probRuleta);
    if (tipoRayo == ABSORBENTE) {       // TERMINAL: rayo absorbente
        //cout << " -- Acaba recurisividad: Rayo absorbente" << endl;
        return;
    } else if(tipoRayo == DIFUSO){
        // Solo luces puntuales
        RGB radianciaFoton = flujoActual * calcBrdfDifusa(coefsOrigen.kd) *
                             calcCosenoAnguloIncidencia(-wo_d, normal);
        Photon foton = Photon(origen.coord, wo_d, radianciaFoton);
        //cout << "Rayo difuso, metemos foton " << foton << endl;
        vecFotones.push_back(foton);
    } else if(tipoRayo == ESPECULAR){
        //cout << "Rayo especular, seguimos" << endl;
    } else if(tipoRayo == REFRACTANTE){
        //cout << "Rayo refractante, seguimos" << endl;
    } else {
        //cout << "Rayo ??? que" << endl;
    }

    float probRayo;     // Ojo! La probabilidad es para la siguiente llamada recursiva pq es wi, no wo
    Rayo wi = obtenerRayoRuletaRusa(tipoRayo, origen, wo_d, normal, probRayo);

    BSDFs coefsPtoIntersec;
    Punto ptoIntersec;
    Direccion nuevaNormal;
    bool hayIntersec = escena.interseccion(wi, coefsPtoIntersec, ptoIntersec, nuevaNormal);

    if (!hayIntersec) {     // TERMINAL: el siguiente rayo (wi) no interseca con nada
        //cout << " -- Acaba recurisividad: Rayo no interseca con nada" << endl;
        return;
    }
    
    recursividadRandomWalk(vecFotones, escena, flujoActual,
                           ptoIntersec, wi.d, coefsPtoIntersec, nuevaNormal);
}

void comenzarRandomWalk(vector<Photon>& vecFotones, const Escena& escena, const Rayo& wi,
                        const RGB& flujo){
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
        recursividadRandomWalk(vecFotones, escena, flujo, ptoIntersec,
                               wi.d, coefsPtoInterseccion, normal);
    } else {
        //cout << endl << "Rayo no interseca con nada, muestreamos otro camino." << endl;
    }
}

// Optamos por almacenar todos los rebotes difusos (incluido el primero)
// y saltarnos el NextEventEstimation posteriormente
int lanzarFotonesDeUnaLuz(vector<Photon>& vecFotones, const int numFotonesALanzar,
                         const RGB& flujoFoton, const LuzPuntual& luz, const Escena& escena){

    int numRandomWalksRestantes = numFotonesALanzar;
    int numFotonesLanzados = 0;
    
    while(numRandomWalksRestantes > 0 && vecFotones.size() < vecFotones.max_size()){
        //cout << "Nuevo random path, numRandomWalksRestantes = " << numRandomWalksRestantes;
        //cout << ", numFotonesLanzados = " << numFotonesLanzados  << endl;
        numRandomWalksRestantes--;
        numFotonesLanzados++;

        Rayo wi(generarDireccionAleatoriaEsfera(), luz.c);
        //cout << "Rayo aleatorio generado desde luz para randomWalk = " << wi << endl;

        comenzarRandomWalk(vecFotones, escena, wi, flujoFoton);
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

void paso1GenerarPhotonMap(PhotonMap& mapaFotones, const int totalFotonesALanzar,
                            const Escena& escena){

    vector<Photon> vecFotones; 
    //cout << "Generando " << totalFotonesALanzar << " fotones en total..." << endl;
    float potenciaTotal = calcularPotenciaTotal(escena.luces);
    //cout << "Potencia total: " << potenciaTotal << endl;


    for(auto& luz : escena.luces){
        // Cada luz lanza un número de fotones proporcional a su potencia
        int numFotonesALanzar = totalFotonesALanzar * (modulo(luz.p) / potenciaTotal);
        int numFotonesYaAlmacenados = vecFotones.size();
        RGB flujoFoton = 4 * M_PI * luz.p; // Se dividirá por S (numFotonesLanzados) posteriormente

        //cout << endl << "Generando " << numFotonesALanzar << " fotones para luz..." << endl;
        //cout << "Flujo inicial de cada foton: " << flujoFoton << endl;
        
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
}

void printVectorFotones(const vector<Photon>& vecFotones){
    for (auto& foton : vecFotones){
        cout << foton << endl;
    }
}

RGB estimarEcuacionRender(const Escena& escena, const PhotonMap& mapaFotones, 
                        const Punto& ptoIntersec, const Direccion& dirIncidente,
                        const Direccion& normal, const BSDFs& coefsPtoInterseccion){
    
}

RGB obtenerRadianciaPixel(const Rayo& rayoIncidente, const Escena& escena, const PhotonMap& mapaFotones){
    
    Punto ptoIntersec;
    Direccion normal;
    RGB radiancia(0.0f, 0.0f, 0.0f);
    BSDFs coefsPtoInterseccion;
   
    if (escena.interseccion(rayoIncidente, coefsPtoInterseccion, ptoIntersec, normal)) {

        // TODO: FALTAN REFRACCION Y ESPECULAR: OBTENER UN RAYO CON RULETA RUSA, 
        // Y SI NO ES DIFUSO, SEGUIR EL CAMINO HASTA ENCONTRAR UNA SUPERFICIE DIFUSA
        
        radiancia = estimarEcuacionRender(escena, mapaFotones, ptoIntersec,
                                            rayoIncidente.d, normal, coefsPtoInterseccion);
    }

    return radiancia;
}

void paso2LeerPhotonMap1RPP(const Camara& camara, const Escena& escena, const unsigned numPxlsAncho, 
                    const unsigned numPxlsAlto, const float anchoPorPixel, const float altoPorPixel,
                    vector<vector<RGB>>& colorPixeles, const PhotonMap& mapaFotones, 
                    const bool printPixelesProcesados, const int totalPixeles){

    for (unsigned ancho = 0; ancho < numPxlsAncho; ++ancho) {
        for (unsigned alto = 0; alto < numPxlsAlto; ++alto) {
            //if (printPixelesProcesados) printPixelActual(totalPixeles, numPxlsAncho, ancho, alto);
            
            Rayo rayo(Direccion(0.0f, 0.0f, 0.0f), Punto());
            rayo = camara.obtenerRayoCentroPixel(ancho, anchoPorPixel, alto, altoPorPixel);
            globalizarYNormalizarRayo(rayo, camara.o, camara.f, camara.u, camara.l);
            colorPixeles[alto][ancho] = obtenerRadianciaPixel(rayo, escena, mapaFotones);
        }
    }
}

                      
void renderizarEscena(const Camara& camara, const unsigned numPxlsAncho, const unsigned numPxlsAlto,
                      const Escena& escena, const string& nombreEscena, const unsigned rpp,
                      const int totalFotones, const bool printPixelesProcesados) {

    float anchoPorPixel = camara.calcularAnchoPixel(numPxlsAncho);
    float altoPorPixel = camara.calcularAltoPixel(numPxlsAlto);
    unsigned totalPixeles = numPxlsAlto * numPxlsAncho;
    
    //if (printPixelesProcesados) cout << "Procesando pixeles..." << endl << "0 pixeles de " << totalPixeles << endl;

   
    PhotonMap mapaFotones;
    paso1GenerarPhotonMap(mapaFotones, totalFotones, escena);
    
    // Inicializado todo a color negro
    vector<vector<RGB>> colorPixeles(numPxlsAlto, vector<RGB>(numPxlsAncho, {0.0f, 0.0f, 0.0f}));

    paso2LeerPhotonMap1RPP(camara,  escena, numPxlsAncho, numPxlsAlto,
                        anchoPorPixel, altoPorPixel, colorPixeles, 
                        mapaFotones, printPixelesProcesados, totalPixeles);


    /*
    if(rpp == 1){
        renderizarEscena1RPP(camara, numPxlsAncho, numPxlsAlto, escena, anchoPorPixel,
                             altoPorPixel, numRayosMontecarlo, coloresPixeles,
                             totalPixeles, printPixelesProcesados);
    } else {
        renderizarEscenaConAntialiasing(camara, numPxlsAncho, numPxlsAlto, escena, anchoPorPixel,
                                        altoPorPixel, rpp, numRayosMontecarlo, coloresPixeles,
                                        printPixelesProcesados, totalPixeles, rpp);
    }
    */
    
    //pintarEscenaEnPPM(nombreEscena, coloresPixeles);
}
