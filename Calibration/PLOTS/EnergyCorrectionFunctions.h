#ifndef EnergyCorrectionFunctions_h
#define EnergyCorrectionFunctions_h

#include <iostream>
#include <cmath>



float fEta(const float& energy, const float& eta, const float* params);
float fBrem(const float& energy, const float& eta, float brem, const float* params);
float fEtEta(const float& et, const float& eta, const float* params);

float fClusterCorrections(const float& ERaw, const float& eta, float brem, const float* params);

void InitializeParams(float* params);

#endif