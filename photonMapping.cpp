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

RGB nextEventEstimation(const Punto& p0, const Direccion& normal, const Escena& escena,
                        const RGB& kd, const LuzPuntual& luz) {

    if (!escena.luzIluminaPunto(p0, luz)) {
        return RGB(0.0f, 0.0f, 0.0f);  // Si el punto no está iluminado
    }

    // Ecuación de render
    Direccion dirIncidente = luz.c - p0;
    float cosAnguloIncidencia = calcCosenoAnguloIncidencia(dirIncidente, normal);
    RGB reflectanciaBrdfDifusa = calcBrdfDifusa(kd);
    RGB radianciaIncidente = luz.p / (modulo(dirIncidente) * modulo(dirIncidente));
    return radianciaIncidente * (reflectanciaBrdfDifusa * cosAnguloIncidencia);
}

void recursividadRandomWalk(vector<Photon>& vecFotones, const Escena& escena, const LuzPuntual& luz,
                            int &fotonesRestantes, int &rebotesRestantes, const RGB& flujoInicidente,
                            const Punto& origen, const Direccion &wo,
                            const BSDFs &coefsOrigen, const Direccion& normal){
    if(rebotesRestantes <= 0 || fotonesRestantes <= 0){
        return; // TERMINAL: se han terminado los fotones totales o
                //           se ha llegado al max de fotones en este randomWalk
    }

    float probRuleta;
    TipoRayo tipoRayo = dispararRuletaRusa(coefsOrigen, probRuleta);

    if (tipoRayo == ABSORBENTE) {
        return; // TERMINAL: rayo absorbente

    } else if(tipoRayo == DIFUSO){
        // Solo luces puntuales
        RGB flujoNEE = nextEventEstimation(origen, normal, escena, coefsOrigen.kd, luz);

        // TODO: si es difuso, calcular flujo (radiancia) del punto de origen y guardar foton
        //          restar contadores rebotesRestantes y fotonesRestantes
    }

    float probRayo;     // Ojo! La probabilidad es para la siguiente llamada recursiva pq es wi, no wo
    Rayo wi = obtenerRayoRuletaRusa(tipoRayo, origen, wo, normal, probRayo);


        //  TODO: FALTA TERMINAR

}

void comenzarRandomWalk(vector<Photon>&vecFotones, const Escena& escena, const LuzPuntual& luz,
                const Rayo& wi, int &fotonesRestantes, 
                int &rebotesRestantes, const RGB& flujo){
    
    Punto ptoIntersec;
    BSDFs coefsPtoInterseccion;
    Direccion normal;

    if(escena.interseccion(wi, coefsPtoInterseccion, ptoIntersec, normal)){
        recursividadRandomWalk(vecFotones, escena, luz, fotonesRestantes, rebotesRestantes, 
                                flujo, ptoIntersec, wi.d, coefsPtoInterseccion, normal);
    }
}

// Optamos por almacenar todos los rebotes difusos (incluido el primero)
// y saltarnos el NextEventEstimation posteriormente
void anadirFotonesDeLuz(vector<Photon>& vecFotones, const unsigned totalFotones, 
                        const LuzPuntual& luz, const Escena& escena, unsigned fotonesPorRandomWalk){
    RGB flujoPorRandomWalk = (luz.p * 4 * M_PI) / totalFotones;
    int fotonesRestantes = static_cast<int>(totalFotones);
    while(fotonesRestantes > 0){
        int rebotesRestantes = static_cast<int>(fotonesPorRandomWalk);
        Rayo wi(generarDireccionAleatoriaEsfera(), luz.c);
        comenzarRandomWalk(vecFotones, escena, luz, wi, fotonesRestantes, rebotesRestantes, flujoPorRandomWalk);
    }
}

RGB calcularPotenciaTotal(const vector<LuzPuntual>& luces){
    RGB total;

    for(auto& luz : luces){
        total += luz.p;
    }

    return total;
}

void generarFotones(vector<Photon>& vecFotones, const unsigned totalFotones, const unsigned fotonesPorRandomWalk, const Escena& escena){
    RGB potenciaTotal = calcularPotenciaTotal(escena.luces);

    for(auto& luz : escena.luces){
        unsigned numFotonesProporcionales = totalFotones * (max(luz.p) / max(potenciaTotal));
        anadirFotonesDeLuz(vecFotones, numFotonesProporcionales, luz, escena, fotonesPorRandomWalk);
    }
}


                      
void renderizarEscena(Camara& camara, unsigned numPxlsAncho, unsigned numPxlsAlto,
                      const Escena& escena, const string& nombreEscena, const unsigned rpp,
                      const unsigned totalFotones, const unsigned fotonesPorRandomWalk,
                      const bool printPixelesProcesados) {
    float anchoPorPixel = camara.calcularAnchoPixel(numPxlsAncho);
    float altoPorPixel = camara.calcularAltoPixel(numPxlsAlto);
   
    unsigned totalPixeles = numPxlsAlto * numPxlsAncho;
    //if (printPixelesProcesados) cout << "Procesando pixeles..." << endl << "0 pixeles de " << totalPixeles << endl;

    vector<Photon> vecFotones;

    generarFotones(vecFotones, totalFotones, fotonesPorRandomWalk, escena);

    PhotonMap mapaFotones = std::move(generarPhotonMap(vecFotones));


    // Inicializado todo a color negro
    vector<vector<RGB>> coloresEscena(numPxlsAlto, vector<RGB>(numPxlsAncho, {0.0f, 0.0f, 0.0f}));

    

    //  TODO: FALTA GENERAR MAPA DE COLORES (PASO 2 DE PHOTONMAPPING)


    /*
    if(rpp == 1){
        renderizarEscena1RPP(camara, numPxlsAncho, numPxlsAlto, escena, anchoPorPixel,
                             altoPorPixel, fotonesPorRandomWalk, numRayosMontecarlo, coloresEscena,
                             totalPixeles, printPixelesProcesados);
    } else {
        renderizarEscenaConAntialiasing(camara, numPxlsAncho, numPxlsAlto, escena, anchoPorPixel,
                                        altoPorPixel, fotonesPorRandomWalk, numRayosMontecarlo, coloresEscena,
                                        printPixelesProcesados, totalPixeles, rpp);
    }
    */
    
    //pintarEscenaEnPPM(nombreEscena, coloresEscena);
}
