#ifndef geometryUtils_h
#define geometryUtils_h

//ECAL GEOMETRY

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>
#include <map>
#include <algorithm>
#include <math.h>
#include <vector>


/// EE Geometry ///
class TEndcapRegions {
 private:
  float iEndcapRing[100][100][2]; 
 public:
  TEndcapRegions(); 
  virtual ~TEndcapRegions();
  int GetEndcapRing(int,int,int);
  int GetEndcapIeta(int,int,int);

};



/// EB Geometry
class TBarrelRegions {
 private:
  float iBarrelRegion[171][360];
 public:
  TBarrelRegions();
  virtual ~TBarrelRegions();
  int GetNRegions(const std::string& type); 
  int GetRegionId(int,int,const std::string&);
/* void GetRegionIetaIphi(float& iEta, float& iPhi, const int& regionId, char* EBEE, const std::string& type); */


};



#endif
