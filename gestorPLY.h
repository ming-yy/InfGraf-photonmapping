#include "utilidades.h"
#include "triangulo.h"
#include "esfera.h"

vector<Triangulo> generarModeloPLY(const string& rutaArchivo, const string rutaTextura, 
                                    Esfera& boundingSphere,
                                    const float escala = 1.0f, const Punto& centro = Punto(0.0f, 0.0f, 0.0f),
                                    const float rotacionX = 0.0f, const bool invertirX = false, 
                                    const float rotacionY = 0.0f, const bool invertirY = false,
                                    const float rotacionZ = 0.0f, const bool invertirZ = false);

// Function to calculate the sphere passing through 4 points
Esfera sphereFromFourPoints(const Punto& p1, const Punto& p2, const Punto& p3, const Punto& p4);

// Function to calculate the sphere passing through 3 points
Esfera sphereFromThreePoints(const Punto& p1, const Punto& p2, const Punto& p3);

// Function to calculate the sphere passing through 2 points
Esfera sphereFromTwoPoints(const Punto& p1, const Punto& p2);

// Welzl's algorithm to compute the minimum bounding sphere
Esfera welzlRecursive(const vector<Punto>& points, vector<Punto> boundary, size_t n);

Esfera minimumBoundingSphere(const vector<Punto>& points);