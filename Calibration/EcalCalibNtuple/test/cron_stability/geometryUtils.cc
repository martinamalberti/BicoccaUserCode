#include "geometryUtils.h"

// --- EE ---
// default constructor, reading the map from file                                                                                                                          
TEndcapRegions::TEndcapRegions() {
  FILE *fRing;
  fRing = fopen("eerings.dat","r");
  std::cout << "Inizializing endcap geometry from: eerings.dat" << std::endl;
  int ix,iy,iz,ir;
  while(fscanf(fRing,"(%d,%d,%d) %d \n",&ix,&iy,&iz,&ir) !=EOF ) {
    if (iz<0) iz=0;
    iEndcapRing[ix][iy][iz] = ir;
  }
  return;
}

//dtor
TEndcapRegions::~TEndcapRegions() { return;}


//methods
int TEndcapRegions::GetEndcapRing(int ix, int iy, int iz, int nRings=39){
  int iSide = iz;
  if (iSide<0) iSide=0;
  int ringWidth = int(39/nRings);
  return int(iEndcapRing[ix][iy][iSide]/ringWidth);
}

int TEndcapRegions::GetEndcapIeta(int ix, int iy, int iz){
  int iSide = iz;
  if (iSide<0) iSide=0;
  int iEtaOffset = 86*iz;
  int iEta = iEtaOffset + iz*iEndcapRing[ix][iy][iSide];
  return iEta;
}



// --- EB ---
// default constructor (NEEDED FOR INPUT REGIONS FROM EXTERNAL FILE)
TBarrelRegions::TBarrelRegions() {
  FILE *fRegion;
  fRegion = fopen("ebregions.dat","r");
  std::cout << "Inizializing barrel geometry from: ebregions.dat" << std::endl;
  int ieta,iphi,sm,tt,lmr;
  while(fscanf(fRegion,"(%d ,%d) %d %d %d\n",&ieta,&iphi,&sm,&tt,&lmr) !=EOF ) 
    {
      iLMR[ieta+85][iphi-1] = lmr;
      iSM[ieta+85][iphi-1] = sm;
      iTT[ieta+85][iphi-1] = tt;
    }
  return;
}

//dtor                                                                                                                                                                     
TBarrelRegions::~TBarrelRegions() 
{
  return;
}


//methods
int TBarrelRegions::GetNRegions(const std::string& type)
{
  if( type == "ALL" ) return 1;
  if( type == "SM" ) return 36;
  if( type == "LMR" ) return 324;
  if( type == "TT" ) return 2448;

  return -1;
}

int TBarrelRegions::GetRegionId(int iEta, int iPhi, const std::string& type)
{
  if( (iEta < -85) || (iEta == 0) || (iEta > 85) ) return -1;
  if( (iPhi < 1) || (iPhi > 360) ) return -1;
  
  if( type == "ALL" )
    {
      return 0;
    }
  if( type == "SM" )
    {
      return iSM[iEta+85][iPhi-1];
    }
  if( type == "LMR" )
    {
      return iLMR[iEta+85][iPhi-1];
    }
  if( type == "TT" )
    {
      return iTT[iEta+85][iPhi-1];
    }
  return -1;
}


// // ---- geometry functions ----


// void GetRegionIetaIphi(float& iEta, float& iPhi,
//                        const int& regionId, char* EBEE, const std::string& type)
// {
//   if( strcmp(EBEE,"EB") == 0 )
//   {
//     int nPhiRegions;
//     float regionEtaWidth;
//     float regionPhiWidth;
    
//     if( type == "SM" )
//     {
//       nPhiRegions = 18;
//       regionEtaWidth = 85.;
//       regionPhiWidth = 20.;
//     }
//     if( type == "TT" )
//     {
//       nPhiRegions = 72;
//       regionEtaWidth = 5.;
//       regionPhiWidth = 5.;
//     }
    
//     iPhi = regionPhiWidth/2. + regionPhiWidth * (regionId % nPhiRegions);
//     iEta = regionEtaWidth/2. + regionPhiWidth * (regionId / nPhiRegions);
    
//     iPhi += 1.;
//     if( iEta  < 75 ) iEta = -1. * (iEta+1.);
//     if( iEta >= 75 ) iEta -= 74.;
//   }
  
//   else
//   {
//   }
// }
