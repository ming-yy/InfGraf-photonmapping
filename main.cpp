//*****************************************************************
// File:   main.cpp
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   septiembre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include <iostream>
#include <memory>       // para los shared_pointers
#include <string>
#include <chrono>
#include "base.h"
#include "punto.h"
#include "direccion.h"
#include "planeta.h"
#include "matriz.h"
#include "transformaciones.h"
#include "utilidades.h"
#include "gestorPPM.h"
#include "camara.h"
#include "rayo.h"
#include "triangulo.h"
#include "plano.h"
#include "esfera.h"
#include "luzpuntual.h"
#include "pathTracing.h"



void printTiempo(auto inicio, auto fin) {
    auto duracion_total = std::chrono::duration_cast<std::chrono::seconds>(fin - inicio);
    auto mins = std::chrono::duration_cast<std::chrono::minutes>(duracion_total);
    auto segs = duracion_total - mins;

    cout << endl << "=========================================" << endl;
    cout << "TIEMPO DE EJECUCION: " << mins.count() << "min " << segs.count() << "s" << endl;
    cout << "=========================================" << endl << endl;
}

void liberarMemoriaDePrimitivas(vector<Primitiva*>& objetos) {
    for (Primitiva* objeto : objetos) {
        delete objeto; // Elimina cada objeto, llama al destructor correcto
    }
    objetos.clear(); // Limpia el vector para evitar punteros colgantes
}

void cajaDeCornell(){
    vector<Primitiva*> objetos;
    objetos.push_back(new Plano({1.0f, 0.0f, 0.0f}, 1.0f, RGB({1.0f, 0.0f, 0.0f}), "muy_difuso")); // plano izquierdo, rojo
    objetos.push_back(new Plano({-1.0f, 0.0f, 0.0f}, 1.0f, RGB({0.0f, 1.0f, 0.0f}), "muy_difuso")); // plano derecho, verde
    objetos.push_back(new Plano({0.0f, 1.0f, 0.0f}, 1.0f, RGB({1.0f, 1.0f, 1.0f}), "muy_difuso")); // plano suelo, blanco
    objetos.push_back(new Plano({0.0f, -1.0f, 0.0f}, 1.0f, RGB({1.0f, 1.0f, 1.0f}), "muy_difuso")); // plano techo, blanco
    objetos.push_back(new Plano({0.0f, -1.0f, 0.0f}, 1.0f, RGB({1.0f, 1.0f, 1.0f}), "muy_difuso",
                                  {1,1,1}, -0.5, 0.5, {0.0f, 0.0f, 0.25f})); // plano techo, blanco
    objetos.push_back(new Plano({0.0f, 0.0f, -1.0f}, 1.0f, RGB({1.0f, 1.0f, 1.0f}), "muy_difuso")); // plano fondo, blanco
    //objetos.push_back(new Esfera({-0.5f, -0.7f, 0.25f}, 0.3f, RGB({0.89f, 0.45f, 0.82f}), "plastico")); // esfera izquierda, rosa
    objetos.push_back(new Esfera({-0.5f, -0.7f, 0.25f}, 0.3f, RGB({0.7f, 1.0f, 1.0f}), "plastico")); // esfera izquierda, azul
    //objetos.push_back(new Esfera({0.5f, -0.7f, -0.25f}, 0.3f, RGB({0.7f, 1.0f, 1.0f}), "dielectrico")); // esfera derecha, azul
    objetos.push_back(new Esfera({0.5f, -0.7f, -0.25f}, 0.3f, RGB({1.0f, 1.0f, 1.0f}), "cristal")); // esfera derecha, azul
    vector<LuzPuntual> luces;

    RGB potencia(1.0f, 1.0f, 1.0f);
    luces.push_back(LuzPuntual({0.0f, 0.5f, 0.0f}, potencia));
    //luces.push_back(LuzPuntual({0.0f, 0.0f, -1.0f}, potencia));
    Escena cornell = Escena(objetos, luces);
    
    Camara cam = Camara({0.0f, 0.0f, -3.5f},
                        {0.0f, 0.0f, 3.0f},
                        {0.0f, 1.0f, 0.0f},
                        {-1.0f, 0.0f, 0.0f});
    
    // Zoom esfera izquierda
    Camara cam2 = Camara({-0.5f, -0.5f, -0.5f},
                        {0.0f, 0.0f, 3.0f},
                        {0.0f, 1.0f, 0.0f},
                        {-1.0f, 0.0f, 0.0f});

    // Zoom esfera izquierda parte derecha
    Camara cam3 = Camara({0.0f, 1.0f, -1.0f},
                        {0.0f, 0.0f, 3.0f},
                        {0.0f, 1.0f, 0.0f},
                        {-1.0f, 0.0f, 0.0f});

    const unsigned maxRebotes = 4;
    const unsigned rpp = 32;
    const unsigned numRayosMontecarlo = 1;
    const bool printPixelesProcesados = true;
    
    auto inicio = std::chrono::high_resolution_clock::now();
    //renderizarEscena(cam, 256, 256, cornell, "cornell", rpp, maxRebotes, numRayosMontecarlo, printPixelesProcesados);
    renderizarEscenaConThreads(cam, 256, 256, cornell, "cornell", rpp, maxRebotes, numRayosMontecarlo, printPixelesProcesados);
    auto fin = std::chrono::high_resolution_clock::now();
    printTiempo(inicio, fin);

    liberarMemoriaDePrimitivas(objetos);

    transformarFicheroPPM("./cornell.ppm", 1);
    transformarFicheroPPM("./cornell.ppm", 2);
    transformarFicheroPPM("./cornell.ppm", 3);
    transformarFicheroPPM("./cornell.ppm", 4);
    transformarFicheroPPM("./cornell.ppm", 5);
}


int main() {
    int test = 12;
    
    if (test == 1) {
        array<float, 3> arrCoord = {4.44,5.55,6.66};
        /*
        // PRUEBA 1.5 - PUNTODIRECCION
        // quitada porque ahora es abstracta y no se puede instanciar
        sh_ptr<PuntoDireccion> pd1 = std::make_shared<PuntoDireccion> ();
        sh_ptr<PuntoDireccion> pd2 = std::make_shared<PuntoDireccion> (1.11,2.22,3.33);
        sh_ptr<PuntoDireccion> pd3 = std::make_shared<PuntoDireccion> (arrCoord);
        cout << endl << "1.5 - PUNTODIRECCION" << endl;
        cout << *pd1 << endl;
        cout << *pd2 << endl;
        cout << *pd3 << endl;
        */

        // PRUEBA 2 - PUNTO
        sh_ptr<Punto> punto1 = std::make_shared<Punto> ();
        sh_ptr<Punto> punto2 = std::make_shared<Punto> (1.111,2.222,3.333);
        sh_ptr<Punto> punto3 = std::make_shared<Punto> (arrCoord);
        cout << endl << "2 - PUNTO" << endl;
        cout << *punto1 << endl;
        cout << *punto2 << endl;
        cout << *punto3 << endl;
        
        // PRUEBA 3 - DIRECCION
        sh_ptr<Direccion> dir1 = std::make_shared<Direccion> ();
        sh_ptr<Direccion> dir2 = std::make_shared<Direccion> (1.111,2.222,3.333);
        sh_ptr<Direccion> dir3 = std::make_shared<Direccion> (arrCoord);
        sh_ptr<Direccion> dirCopia = std::make_shared<Direccion> (*dir3);
        
        cout << endl << "3 - DIRECCION" << endl;
        cout << *dir1 << endl;
        cout << *dir2 << endl;
        cout << *dir3 << endl;
        cout << *dirCopia << endl;
    
    } else if (test == 2) {
        array<float, 3> arrAux2 = {0.111, 1.222, 2.333};

        
        // PRUEBA 1 - DIRECCIONES
        sh_ptr<Direccion> dir1 = std::make_shared<Direccion> (1.111,2.222,3.333);
        sh_ptr<Direccion> dir2 = std::make_shared<Direccion> (arrAux2);
        sh_ptr<Direccion> dir3 = std::make_shared<Direccion> ();
        sh_ptr<Direccion> dirMod = std::make_shared<Direccion> (0.111, 0.222, 0.333);
        sh_ptr<Direccion> dirNor = std::make_shared<Direccion> (0, 0, 0);
        sh_ptr<Direccion> dirEsc = std::make_shared<Direccion> (1, 1, 1);
        sh_ptr<Direccion> dir90a = std::make_shared<Direccion> (1, 0, 0);
        sh_ptr<Direccion> dir90b = std::make_shared<Direccion> (0, 1, 0);
        
        
        cout << endl << "1 - DIRECCION OPERANDOS" << endl;
        cout << "Suma: " << *dir1 + *dir3 << endl;
        cout << "Resta: " << *dir1 - *dir2 << endl;

        // Multiplicacion y division de direcciones por escalares
        float escalar = 2.0f;
        try {
            cout << "Multiplicacion por " << escalar << ": " << *dir1 * escalar
                    << endl;
            cout << "Division por " << escalar << ": " << *dir1 / escalar
                    << endl;
            cout << "Division por 0 (debe lanzar excepcion): " << *dir1 / 0
                    << endl;
        } catch (const invalid_argument& e) {
            cerr << e.what() << endl;
        }
        
        // Modulo
        cout << "Modulo: " << modulo(*dir1) << endl;
        cout << "Modulo tras resta: " << modulo(*dir1 - *dirMod) << endl;
        
        // Normalizacion
        try {
            cout << "Normalizacion de " << *dir1 << ":" << normalizar(*dir1) << endl;
            cout << "Normalizacion de " << *dirMod << ":" << normalizar(*dirMod) << endl;
            cout << "Normalizacion de " << *dir2 << ":" << normalizar(*dir2) << endl;
            cout << "Normalizacion de " << *dirNor << ":" << normalizar(*dirNor) << endl;
        } catch (const invalid_argument& e) {
            cerr << e.what() << endl;
        }
        
        // Producto escalar y vectorial
        cout << "Producto escalar: " << dot(*dir1, *dir2) << endl;
        cout << "Producto escalar (0 grados): " << dot(*dir1, *dir1) << endl;
        cout << "Producto escalar (90 grados): " << dot(*dirNor, *dirEsc) << endl;
        cout << "Producto vectorial: " << cross(*dir1, *dir2) << endl;
        cout << "Producto vectorial (0 grados): " << cross(*dirEsc, *dirEsc) << endl;
        cout << "Producto vectorial (90 grados): " << cross(*dir90a, *dir90b) << endl;
        
        
        // PRUEBA 2 - PUNTOS
        sh_ptr<Punto> punto1 = std::make_shared<Punto>();
        sh_ptr<Punto> punto2 = std::make_shared<Punto>(arrAux2);
        sh_ptr<Punto> punto3 = std::make_shared<Punto>(1.111, 2.222, 3.333);

        sh_ptr<Direccion> dir4 = std::make_shared<Direccion>(1.111, 2.222, 3.333);
        sh_ptr<Direccion> dir5 = std::make_shared<Direccion>(arrAux2);

        cout << endl << "2 - PUNTO OPERANDOS" << endl;
        cout << "Suma con direccion: " << *punto1 + *dir4 << endl;
        cout << "Suma con direccion: " << *punto2 + *dir5 << endl;
        cout << "Resta con direccion: " << *punto3 - *dir4 << endl;

        // Multiplicacion y division de puntos por escalares
        try {
            cout << "Multiplicacion de punto por " << escalar << ": "
                        << (*punto1 * escalar) << endl;
            cout << "Division de punto por " << escalar << ": "
                        << (*punto3 / escalar) << endl;
            cout << "Division de punto por 0 (debe lanzar excepcion): "
                        << (*punto3 / 0) << endl;
        } catch (const invalid_argument& e) {
            cerr << e.what() << endl;
        }

    } else if (test == 3) {
        sh_ptr<Matriz<3, 3>> matriz1 = std::make_shared<Matriz<3, 3>> ();
        sh_ptr<Matriz<4, 4>> matriz4x4 = std::make_shared<Matriz<4, 4>>(
            init_list<init_list<float>>{
                {1.0f, 2.0f, 3.0f, 4.0f},
                {5.0f, 6.0f, 7.0f, 8.0f},
                {9.0f, 10.0f, 11.0f, 12.0f},
                {13.0f, 14.0f, 15.0f, 16.0f}
            }
        );
        
        sh_ptr<Matriz<3, 3>> matriz3x3 = std::make_shared<Matriz<3, 3>>(
            init_list<init_list<float>>{
                {1.0f, 0.0f, 0.0f},
                {0.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 1.0f},
            }
        );
        
        sh_ptr<Matriz<4, 3>> matriz4x3 = std::make_shared<Matriz<4, 3>>(
            init_list<init_list<float>>{
                {1.0f, 0.0f, 0.0f},
                {0.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 1.0f},
            }
        );
        
        sh_ptr<Matriz<4, 4>> matrizInv = std::make_shared<Matriz<4, 4>>(
            init_list<init_list<float>>{
                {1.0f, 0.0f, 0.0f, 2.0f},
                {0.0f, 1.0f, 2.0f, 0.0f},
                {0.0f, 2.0f, 1.0f, 0.0f},
                {2.0f, 0.0f, 0.0f, 1.0f}
            }
        );

        cout << "Matriz 3x3 vacia:" << endl;
        cout << *matriz1 << endl;
        
        cout << "Matriz 4x4:" << endl;
        cout << *matriz4x4 << endl;
        
        cout << "Matriz 3x3:" << endl;
        cout << *matriz3x3 << endl;
        
        cout << "Matriz 4x3:" << endl;
        cout << *matriz4x3 << endl;

        array<array<float, 3>, 2> arrMult1 = {{{1.0f,2.0f,3.0f}, {4.0f,5.0f,6.0f}}};
        array<array<float, 2>, 3> arrMult2 = {{{10.0f,11.0f}, {20.0f,21.0f}, {30.0f,31.0f}}};

        sh_ptr<Matriz<2, 3>> matrizMult1 = std::make_shared<Matriz<2, 3>>(arrMult1);
        sh_ptr<Matriz<3, 2>> matrizMult2 = std::make_shared<Matriz<3, 2>>(arrMult2);

        sh_ptr<Matriz<2, 2>> resultado = std::make_shared<Matriz<2, 2>>(*matrizMult1 * *matrizMult2);

        cout << "Multiplicacion de m1:" << endl << *matrizMult1 << endl;
        cout << "y m2:" << endl << *matrizMult2 << endl;
        cout << "RESULTADO:" << endl << *resultado << endl;
        cout << "Inversa de m1: " << endl << (*matrizInv).inversa() << endl;

    } else if (test == 4) {
        sh_ptr<Punto> punto1 = std::make_shared<Punto> (1.111,2.222,3.333);
        sh_ptr<Punto> punto2 = std::make_shared<Punto> (1,2,3);
        sh_ptr<Punto> o1 = std::make_shared<Punto> (0,0,0);
        sh_ptr<Punto> o2 = std::make_shared<Punto> (1,2,3);
        float x1=3, y1=2, z1=1, d=90;
        
        sh_ptr<Base> b1 = std::make_shared<Base>(
            init_list<init_list<float>>{
                {1.0f, 0.0f, 0.0f},
                {0.0f, 2.0f, 0.0f},
                {0.0f, 0.0f, 3.0f},
            }
        );
        
        
        cout << "VAMOS A MOVER el punto1:" << endl << *punto1 << endl << endl;
        
        cout << "TRASLACION con x = " << x1 << ", y = " << y1 << ", z = " << z1 << endl;
        cout << "RESULTADO:" << endl << translate(*punto1, x1, y1, z1) << endl;
        
        cout << "ESCALADO con x = " << x1 << ", y = " << y1 << ", z = " << z1 << endl;
        cout << "RESULTADO:" << endl << scale(*punto1, x1, y1, z1) << endl;
        
        cout << "ROTACION en X con d = " << d << endl;
        cout << "RESULTADO:" << endl << rotateX(*punto1, d) << endl;
        
        cout << "ROTACION en Y con d = " << d << endl;
        cout << "RESULTADO:" << endl << rotateY(*punto1, d) << endl;
        
        cout << "ROTACION en Z con d = " << d << endl;
        cout << "RESULTADO:" << endl << rotateZ(*punto1, d) << endl;
        
        cout << "CAMBIOBASE de " << *punto2 << ":\n" << cambioBase(*punto2, *b1, *o1) << endl;
        
    } else if (test == 5) {
        sh_ptr<Base> base1 = std::make_shared<Base> ();
        
        sh_ptr<Base> base2 = std::make_shared<Base>(
             init_list<init_list<float>>{
                 {1.0f, 0.0f, 0.0f},
                 {0.0f, 1.0f, 0.0f},
                 {0.0f, 0.0f, 1.0f},
             }
        );
        
        array<array<float, 3>, 3> arrBase3 = {
            {{1.0f,2.0f,0.0f}, {1.0f,0.0f,5.0f}, {0.0f, 7.0f, 0.0f}}
        };
        sh_ptr<Base> base3 = std::make_shared<Base>(arrBase3);
        
        sh_ptr<Base> baseError = std::make_shared<Base>(
             init_list<init_list<float>>{
                 {1.0f, 2.0f, 3.0f, 4.0f},
                 {5.0f, 6.0f, 7.0f, 8.0f},
                 {9.0f, 10.0f, 11.0f, 12.0f},
                 {13.0f, 14.0f, 15.0f, 16.0f}
             }
        );
        
        cout << "Base constructor default:" << endl;
        cout << *base1 << endl;
        
        cout << "Base constructor inicializer_list:" << endl;
        cout << *base2 << endl;
        
        cout << "Base constructor array:" << endl;
        cout << *base3 << endl;
        
        cout << "Base dimensiones incorrectas:" << endl;
        cout << *baseError << endl;
        
    } else if (test == 6) {
        sh_ptr<Punto> centro1 = std::make_shared<Punto>(0,0,0);
        sh_ptr<Punto> cref1 = std::make_shared<Punto>(1,0,0);
        sh_ptr<Direccion> eje1 = std::make_shared<Direccion>(0,2,0);
        sh_ptr<Direccion> eje2 = std::make_shared<Direccion>(0,4,0);
        sh_ptr<Punto> origenUCS = std::make_shared<Punto>(0,0,0);
        
        cout << "--- Pruebas planetas ---" << endl;
        sh_ptr<Planeta> planeta1 = std::make_shared<Planeta>(*centro1, *eje1, *cref1, 45, 0);
        //sh_ptr<Planeta> planeta1 = std::make_shared<Planeta>(*centro1, *eje1, *cref1, 90, 0);
        cout << "\nPlaneta1:\n" << *planeta1 << endl;
        
        try {
            sh_ptr<Planeta> planeta2 = std::make_shared<Planeta>(*centro1, *eje2, *cref1, 90, 90);
            cout << "\nPlaneta: " << *planeta2 << endl;
        } catch (const invalid_argument& e) {
            cerr << e.what() << endl;
        }
        
        
        sh_ptr<Punto> centro3 = std::make_shared<Punto>(5,0,0);
        sh_ptr<Punto> cref3 = std::make_shared<Punto>(3,0,0);
        sh_ptr<Direccion> eje3 = std::make_shared<Direccion>(0,4,0);
        sh_ptr<Planeta> planeta3 = std::make_shared<Planeta>(*centro3, *eje3, *cref3, -45, 0);
        //sh_ptr<Planeta> planeta3 = std::make_shared<Planeta>(*centro3, *eje3, *cref3, 90, 180);
        cout << "\nPlaneta3:\n" << *planeta3 << endl;
        
        
        cout << "--- Pruebas interconexion planetaria ---" << endl;
        bool resultado = interconexionPlanetaria(*planeta1, *planeta3);
        cout << "Interconexion planetaria exitosa? " << resultado << endl;
        
    } else if (test == 7) {
        sh_ptr<Base> ejemplo_print = std::make_shared<Base>(
             init_list<init_list<float>>{
                { 1.111, -2.22, 333.333 },
                { 10.1, 25.555, -0.333 },
                { -1123.456, 0.0, 7.89912 }
             }
        );

        cout << *ejemplo_print;

        sh_ptr<Matriz<4, 4>> ejemplo_print2 = std::make_shared<Matriz<4, 4>>(
            init_list<init_list<float>>{
                { 1.111, -2.22, 333.333, 0.0 },
                { 10.1, 25.555, -0.333, 44.44 },
                { -123.456, 0.0, 7.89, 3.14159 },
                { -13.456, 100.0, -7.89, 3.14159 }
            }
        );

        cout << *ejemplo_print2;
        
    } else if (test == 8){
        string nombreFichero = "/Users/ming/github_things/Informatica-Grafica/ppms/forest_path.ppm";
        return transformarFicheroPPM(nombreFichero, 2);

    } else if (test == 9){
        string nombreFichero = "/Users/ming/github_things/Informatica-Grafica/ppms/forest_path.ppm";
        transformarFicheroPPM(nombreFichero, 1);
        transformarFicheroPPM(nombreFichero, 2);
        transformarFicheroPPM(nombreFichero, 3);
        transformarFicheroPPM(nombreFichero, 4);
        transformarFicheroPPM(nombreFichero, 5);
        
    } else if (test == 10) {
        cout << endl << "PRUEBA 10 - INTERSECCIONES " << endl;
        BSDFs emision;
        
        cout << endl;
        cout << "--- Pruebas interseccion Rayo Esfera ---" << endl;

        // Cogido del test 6
        sh_ptr<Punto> centro3 = std::make_shared<Punto>(5,0,0);
        sh_ptr<Punto> cref3 = std::make_shared<Punto>(3,0,0);
        sh_ptr<Direccion> eje3 = std::make_shared<Direccion>(0,4,0);
        sh_ptr<Planeta> planeta = std::make_shared<Planeta>(*centro3, *eje3, *cref3, -45, 0);

        sh_ptr<Direccion> dir = std::make_shared<Direccion>(1,0,0);
        sh_ptr<Punto> pto = std::make_shared<Punto>(1,0,0);
        vector<Punto> ptosIntersec;
        sh_ptr<Esfera> esfera = std::make_shared<Esfera>(*planeta);
        esfera->interseccion(Rayo(*dir, *pto), ptosIntersec, emision);

        if (!ptosIntersec.empty()) {
            for (Punto p : ptosIntersec) {
                cout << "!Hay interseccion! El punto de interseccion es: " << p << endl;
            }
        } else {
            cout << "No hay interseccion." << endl;
        }
        
        cout << "---" << endl;
        ptosIntersec.clear();
        Esfera esfera2 = Esfera(Punto(0,0,0), 1, RGB());
        esfera2.interseccion(Rayo(Direccion(1,0,0), Punto(1,0,0)), ptosIntersec, emision);

        if (!ptosIntersec.empty()) {
            for (Punto p : ptosIntersec) {
                cout << "!Hay interseccion! El punto de interseccion es: " << p << endl;
            }
        } else {
            cout << "No hay interseccion." << endl;
        }

        // ------------------------------------------------------
        ptosIntersec.clear();

        cout << endl << endl;
        cout << "--- Pruebas interseccion Rayo Plano ---" << endl;
        sh_ptr<Direccion> normalPlano = std::make_shared<Direccion>(1,0,0);
        float distanciaPlano = -5.0f;   // El plano está a 5 unidades del origen UCS
        sh_ptr<Plano> plano = std::make_shared<Plano>(*normalPlano, distanciaPlano);
        //cout << *plano << endl;
        plano->interseccion(Rayo(*dir, *pto), ptosIntersec, emision);

        if (!ptosIntersec.empty()) {
            for (Punto p : ptosIntersec) {
                cout << "!Hay interseccion! El punto de interseccion es: " << p << endl;
            }
        } else {
            cout << "No hay interseccion." << endl;
        }

        // ------------------------------------------------------
        ptosIntersec.clear();

        cout << endl << endl;
        cout << "--- Pruebas interseccion Rayo Triangulo ---" << endl;

        // Vértices del triángulo
        array<float, 3> arrV0 = {0.0f, 0.0f, 0.0f};
        array<float, 3> arrV1 = {1.0f, 0.0f, 0.0f};
        array<float, 3> arrV2 = {0.0f, 1.0f, 0.0f};

        Punto v0(arrV0);
        Punto v1(arrV1);
        Punto v2(arrV2);
        Triangulo triangulo(v0, v1, v2);

        array<float, 3> arrOrigenRayo = {0.5f, 0.5f, -1.0f};
        array<float, 3> arrDireccionRayo = {0.0f, 0.0f, 1.0f};


        Punto origenRayo(arrOrigenRayo);
        Direccion direccionRayo(arrDireccionRayo);

        triangulo.interseccion(Rayo(direccionRayo, origenRayo), ptosIntersec, emision);

        if (!ptosIntersec.empty()) {
            for (Punto p : ptosIntersec) {
                cout << "!Hay interseccion! El punto de interseccion es: " << p << endl;
            }
        } else {
            cout << "No hay interseccion." << endl;
        }

    } else if (test == 11) {
        cout << endl << "PRUEBA 11 - OBTENER RAYOS DE UN PIXEL" << endl << endl;

        Camara cam2 = Camara({0.0f, 0.0f, 0.0f},
                             {10.0f, 0.0f, 0.0f},
                             {0.0f, 2.0f, 0.0f},
                             {0.0f, 0.0f, 2.0f});

        float anchoPorPixel2 = cam2.calcularAnchoPixel(4);
        float altoPorPixel2 = cam2.calcularAltoPixel(4);

        Rayo rayoEsquina1 = cam2.obtenerRayoEsquinaPixel(0, anchoPorPixel2, 0, altoPorPixel2);
        cout << "Esquina1 = " << rayoEsquina1 << endl;
        Rayo rayoEsquina2 = cam2.obtenerRayoEsquinaPixel(3, anchoPorPixel2, 0, altoPorPixel2);
        cout << "Esquina2 = " << rayoEsquina2 << endl;
        Rayo rayoEsquina3 = cam2.obtenerRayoEsquinaPixel(0, anchoPorPixel2, 3, altoPorPixel2);
        cout << "Esquina3 = " << rayoEsquina3 << endl;
        Rayo rayoEsquina4 = cam2.obtenerRayoEsquinaPixel(3, anchoPorPixel2, 3, altoPorPixel2);
        cout << "Esquina4 = " << rayoEsquina4 << endl;

        cout << endl;

        Rayo rayoCentro1 = cam2.obtenerRayoCentroPixel(0, anchoPorPixel2, 0, altoPorPixel2);
        cout << "Centro1 = " << rayoCentro1 << endl;
        Rayo rayoCentro2 = cam2.obtenerRayoCentroPixel(3, anchoPorPixel2, 0, altoPorPixel2);
        cout << "Centro2 = " << rayoCentro2 << endl;
        Rayo rayoCentro3 = cam2.obtenerRayoCentroPixel(0, anchoPorPixel2, 3, altoPorPixel2);
        cout << "Centro3 = " << rayoCentro3 << endl;
        Rayo rayoCentro4 = cam2.obtenerRayoCentroPixel(3, anchoPorPixel2, 3, altoPorPixel2);
        cout << "Centro4 = " << rayoCentro4 << endl;

        cout << endl;

        Rayo rayoRand1 = cam2.obtenerRayoAleatorioPixel(3, anchoPorPixel2, 3, altoPorPixel2);
        cout << "Rand1 = " << rayoRand1 << endl;
        Rayo rayoRand2 = cam2.obtenerRayoAleatorioPixel(3, anchoPorPixel2, 3, altoPorPixel2);
        cout << "Rand2 = " << rayoRand2 << endl;
        Rayo rayoRand3 = cam2.obtenerRayoAleatorioPixel(3, anchoPorPixel2, 3, altoPorPixel2);
        cout << "Rand3 = " << rayoRand3 << endl;
        Rayo rayoRand4 = cam2.obtenerRayoAleatorioPixel(3, anchoPorPixel2, 3, altoPorPixel2);
        cout << "Rand4 = " << rayoRand4 << endl;

    } else if (test == 12){    

        cajaDeCornell();

    } else {
        printf("ERROR: No se ha encontrado el numero de prueba.\n");
    }
        
    return 0;
}
