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
int TEndcapRegions::GetEndcapRing(int ix, int iy, int iz){
  int iSide = iz;
  if (iSide<0) iSide=0;
  return iEndcapRing[ix][iy][iSide];
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
TBarrelRegions::TBarrelRegions() {return;}

//dtor                                                                                                                                                                     
TBarrelRegions::~TBarrelRegions() {return;}


//methods
int TBarrelRegions::GetNRegions(const std::string& type)
{
  if( type == "SM" ) return 36;
  if( type == "TT" ) return 2448;

  return -1;
}

int TBarrelRegions::GetRegionId(int iEta, int iPhi, const std::string& type)
{
  if( (iEta < -85) || (iEta == 0) || (iEta > 85) ) return -1;
  if( (iPhi < 0) || (iPhi > 360) ) return -1;
  
  // redefine iEta and iPhi such that iEta=[0,169] and iPhi=[0,359]
  int newEta = iEta;
  if( iEta < 0) newEta += 85;
  if( iEta > 0) newEta += 84;
  int newPhi = iPhi - 1;
  
  if( type == "SM" )
    {
      return (newPhi/20) + 18 * (newEta/85);
    }
  if( type == "TT" )
    {
      return (newPhi/5) + 72 * (newEta/5);
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
