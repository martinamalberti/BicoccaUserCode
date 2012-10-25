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
  int iEndcapRing[100][100][2]; 
 public:
  TEndcapRegions(); 
  virtual ~TEndcapRegions();
  int GetEndcapRing(int,int,int, int);
  int GetEndcapIeta(int,int,int);

};



/// EB Geometry
class TBarrelRegions {

 private:
  std::map<int,int> LmrEtaMap;
  int iLMR[171][360];
  int iTT[171][360];
  int iSM[171][360];

 public:
  TBarrelRegions();
  virtual ~TBarrelRegions();
  int GetNRegions(const std::string& type); 
  int GetRegionId(const int,const int,const std::string&);
  int GetRegionIdIeta(const int& regionId, const std::string& type);


};



#endif
