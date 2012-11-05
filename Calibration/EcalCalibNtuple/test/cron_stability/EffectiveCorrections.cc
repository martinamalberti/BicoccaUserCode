#include "EffectiveCorrections.h"



//! ctor
EffectiveCorrections::EffectiveCorrections(char* EBEE, const std::string& corrType, const std::string& regionType, const std::string& inFileName):
  EBEE_(EBEE),
  corrType_(corrType),
  regionType_(regionType),
  inFile_( TFile::Open(inFileName.c_str(),"READ") )
{
  std::cout << ">>> EffectiveCorrections::ctor" << std::endl;
  
  
  
  //----------------
  // define geometry
  
  int nRegions = -1;
  if( strcmp(EBEE_,"EB") == 0 )
  {
    TBarrelRegions* ebRegion = new TBarrelRegions();
    nRegions = ebRegion->GetNRegions(regionType);
  }
  else
  {
    TEndcapRegions* eeRegion;
    //nRegions = eeRegion->GetNRegions(regionType);
    nRegions = 6; // HARDCODED, FIXME
  }
  
  
  
  //----------------
  // get corrections
  
  inFile_ -> cd();
  
  if( corrType == "regionalAndTimeScaleCorr" )
  {
    char graphName[50];
    for(int iRegion = 0; iRegion < nRegions; ++iRegion)
    {
      sprintf(graphName,"region_%d/g_c_fit_%d",iRegion,iRegion);
      TGraphAsymmErrors* g_c_fit = (TGraphAsymmErrors*)( inFile_->Get(graphName) );
      
      for(int point = 0; point < g_c_fit -> GetN(); ++point)
      {
        double time,timeMin,timeMax;
        double EoC;
        g_c_fit -> GetPoint(point,time,EoC);
        timeMin = time - g_c_fit->GetErrorXlow(point);
        timeMax = time + g_c_fit->GetErrorXhigh(point);
        
        (corrFactor_[iRegion]).push_back( 1./EoC  );
        (   minTime_[iRegion]).push_back( timeMin );
        (   maxTime_[iRegion]).push_back( timeMax );
      }
    }
  }
  
  inFile_ -> Close();
}



//! dtor
EffectiveCorrections::~EffectiveCorrections()
{}




//!methods                                                                                                                                                                       
double EffectiveCorrections::GetEffectiveCorrection(const int& time, const int& regionId)
{
  unsigned int nTimeBins = (corrFactor_[regionId]).size();
  for(unsigned int timeIt = 0; timeIt < nTimeBins; ++timeIt)
  {
    if( (time < (minTime_[regionId]).at(0)) || (time > (maxTime_[regionId]).at(nTimeBins-1)) )
      break;
    
    if( (time >= (minTime_[regionId]).at(timeIt)) && (time < (maxTime_[regionId]).at(timeIt)) )
      return (corrFactor_[regionId]).at(timeIt);
  }
  
  return 1.;
}
