//*****************************************************************
// File:   toneMapping.h
// Author: Ming Tao, Ye   NIP: 839757, Puig Rubio, Manel Jorda  NIP: 839304
// Date:   octubre 2024
// Coms:   Práctica 1 de Informática Gráfica
//*****************************************************************

#include "toneMapping.h"


// GAMMA puede ser otro, pero 2.2 es un valor común
// utilizado para la corrección gamma en monitores estándar.
const float GAMMA = 2.2;


void clamp(vector<float>& valores){
    clampAndEqualize(valores, 1);   // Igual que la función clamp+equalize si threshold = 1
}


void equalize(vector<float>& valores, const float maxValue){
    clampAndEqualize(valores, maxValue);    // Igual que la función clamp+equalize si todos 
                                            // los valores caen por debajo del threshold
                                            // (threshold = máximo valor)
}

void clampAndEqualize(vector<float>& valores, const float threshold){
    for(size_t i = 0; i < valores.size(); i++){
        if(valores[i] <= threshold){
            valores[i] /= threshold; // Clamping
        } else {
            valores[i] = 1; // Equalization
        }
    }
}

void gamma(vector<float>& valores, const float maxValue){
    gammaAndClamp(valores, maxValue);   // Igual que la función gamma+clamp si todos 
                                        // los valores caen por debajo del threshold
                                        // (threshold = máximo valor)
}

void gammaAndClamp(vector<float>& valores, const float threshold){
    size_t size = valores.size();
    for(size_t i = 0; i < size; i++){
        if(valores[i] <= threshold){
            valores[i] /= threshold; // Ecualization
            valores[i] = pow(valores[i], 1/GAMMA); // Gamma
        } else {
            valores[i] = 1; // Clamping
        }
    }
}
