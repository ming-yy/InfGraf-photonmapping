Photon Mapping - Práctica de Informática Gráfica

Autores:
    •    Ming Tao, Ye - NIP: 839757
    •    Puig Rubio, Manel Jorda - NIP: 839304
Fecha: Enero 2025

---
COMPILACIÓN Y EJECUCIÓN

En Windows:

make
main.exe

En Linux:

make
./main

Limpieza

Para limpiar los archivos generados durante la compilación, ejecutar:

make clean


---
ESTRUCTURA DEL PROYECTO

Archivos principales
    •    main.cpp
Configura la escena, la cámara, los parámetros del renderizador y ejecuta el renderizador. Utiliza estructuras de if-else para seleccionar qué test (1-11) o escena (12-13) ejecutar. Tiene funciones de comprobación de aspect-ratio y función que se asegura de que no se mezclan luces puntuales con luces de área.
    •    photonMapping.cpp
Contiene la lógica central para el Photon Tracing y para el renderizado de la imagen final a partir del mapa de fotones. Implementa características clave como photon tracing (paso 1), estimación de densidad (paso 2), paralelización, Ruleta Rusa, kernels, etc.
    •    escena.cpp
Gestiona la lógica de la escena, incluyendo la intermediación de intersecciones entre rayos y objetos (primitivas y luces) y la determinación de si un punto está iluminado por alguna luz.


---
ESTRUCTURA DE CARPETAS
    •    modelos/
Contiene modelos 3D en formato .ply.
    •    texturas/
Contiene texturas en formato .ply.


---
NOTAS ADICIONALES
    •    El código está diseñado para ser ejecutado en sistemas Windows (g++) y Mac (Xcode con Clang).
    •    Se ha empleado el estándar C++20.

