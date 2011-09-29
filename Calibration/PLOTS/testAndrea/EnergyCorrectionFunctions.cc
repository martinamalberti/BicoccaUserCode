#include "EnergyCorrectionFunctions.h"



float fEta(const float& energy, const float& eta, const float* params)
{
  if( fabs(eta) > 1.479 ) return energy;
  
  float ieta = fabs(eta)*(5/0.087);
  float p0 = params[0];
  float p1 = params[1];
  
  float correctedEnergy = energy;
  if( ieta < p0 ) correctedEnergy = energy;
  else            correctedEnergy = energy / (1.0 + p1*(ieta-p0)*(ieta-p0) );
  
  return correctedEnergy;
}



float fBrem(const float& energy, const float& eta, float brem, const float* params)
{
  //Make No Corrections if brem is invalid! 
  if ( brem == 0 ) return energy; 
  
  int offset = 0;
  if( fabs(eta) > 1.479 ) offset = 21;
  
  float bremLowThr  = params[2+offset];
  float bremHighThr = params[3+offset];
  if( brem < bremLowThr )  brem = bremLowThr;
  if( brem > bremHighThr ) brem = bremHighThr;
  
  // Parameters provided in cfg file
  float p0 = params[4+offset];
  float p1 = params[5+offset];
  float p2 = params[6+offset];
  float p3 = params[7+offset];
  float p4 = params[8+offset];
  
  float threshold = p4;  
  
  float y = p0*threshold*threshold + p1*threshold + p2; 
  float yprime = 2*p0*threshold + p1; 
  float a = p3; 
  float b = yprime - 2*a*threshold; 
  float c = y - a*threshold*threshold - b*threshold; 
   
  float fCorr = 1; 
  if ( brem < threshold )  
    fCorr = p0*brem*brem + p1*brem + p2; 
  else  
    fCorr = a*brem*brem + b*brem + c; 
  
  
  return energy/fCorr; 
}



float fEtEta(const float& et, const float& eta, const float* params)
{
  float fCorr = 0.;

  int offset = 0;
  if( fabs(eta) > 1.479 ) offset = 21;
  
  if( fabs(eta) < 1.479 )
  {
    float p0 = params[9+offset] + params[10+offset]/(et + params[11+offset]) + params[12+offset]/(et*et);  
    float p1 = params[13+offset] + params[14+offset]/(et + params[15+offset]) + params[16+offset]/(et*et);  
    
    fCorr = p0 +  p1*atan(params[17+offset]*(params[18+offset]-fabs(eta))) + params[19+offset]*fabs(eta); 
  }
  
  else
  {
    float p0 = params[9+offset]  + params[10+offset]/sqrt(et);
    float p1 = params[11+offset] + params[12+offset]/sqrt(et);
    float p2 = params[13+offset] + params[14+offset]/sqrt(et);
    float p3 = params[15+offset] + params[16+offset]/sqrt(et);        
    
    fCorr = p0 + p1*fabs(eta) + p2*eta*eta + p3/fabs(eta);
  }
  
  // cap the correction at 50%
  if ( fCorr < 0.5 ) fCorr = 0.5;  
  if ( fCorr > 1.5 ) fCorr = 1.5;   
  
  return et/fCorr;
}



float fClusterCorrections(const float& ERaw, const float& eta, float brem, const float* params)
{
  float absEta = fabs(eta);
  
  float E_fEta = fEta(ERaw, absEta, params);
  float E_fEta_fBrem = fBrem(E_fEta, absEta, brem, params);
  float ET_fEta_fBrem_fEtEta = fEtEta(E_fEta_fBrem/cosh(absEta), absEta, params);
  return ET_fEta_fBrem_fEtEta * cosh(absEta);
}



void InitializeParams(float* params)
{
int offset;

//*******
// barrel
//*******
offset = 0;

// fEta
params[0+offset] =  4.021980E+01;
params[1+offset] = -3.031030E-06;

// fBrem
params[2+offset] =  1.100000E+00;
params[3+offset] =  8.000000E+00;
params[4+offset] = -5.289000E-02;
params[5+offset] =  1.374000E-01;
params[6+offset] =  9.141000E-01;
params[7+offset] = -6.690000E-04;
params[8+offset] =  1.380000E+00;

// fEtEta
params[9+offset]  =  1.000000E+00;
params[10+offset] = -6.980000E-01;
params[11+offset] =  0.000000E+00;
params[12+offset] =  0.000000E+00;

params[13+offset] =  0.000000E+00;
params[14+offset] =  6.605000E-01;
params[15+offset] =  8.825000E+00;
params[16+offset] =  8.410000E-01;

params[17+offset] =  7.600000E+00;
params[18+offset] =  1.081000E+00;
params[19+offset] = -1.810000E-03;

// scale
params[20+offset] =  1.000000E-00;



//*******
// endcap
//*******
offset = 21;

// fEta
params[0+offset] =  0.000000E+00;
params[1+offset] =  0.000000E+00;

// fBrem
params[2+offset] =  9.000000E-01;
params[3+offset] =  6.500000E+00;
params[4+offset] = -7.945000E-02;
params[5+offset] =  1.298000E-01;
params[6+offset] =  9.147000E-01;
params[7+offset] = -1.565000E-03;
params[8+offset] =  9.000000E-01;

// fEtEta
params[9+offset]  = -3.516000E+00;
params[10+offset] = -2.362000E+00;
params[11+offset] =  2.151000E+00;
params[12+offset] =  1.572000E+00;

params[13+offset] = -3.360000E-01;
params[14+offset] = -2.807000E-01;
params[15+offset] =  3.200000E+00;
params[16+offset] =  0.000000E+00;

params[17+offset] =  1.000000E+00;
params[18+offset] =  0.000000E+00;
params[19+offset] =  0.000000E+00;

// scale
params[20+offset] =  1.000000E-00;
}