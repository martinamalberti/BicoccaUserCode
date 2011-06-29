#ifndef ClusterCalibTools_h
#define ClusterCalibTools_h

#include <map>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>

#include "TH2F.h"

#ifdef _MAKECINT_
#pragma link C++ class EBregionBuilder+;
#endif

/* to build the regions where to run the calibration
  - all the methods have as input eta index defined in:
    [-85,-1] U [1,85] (EBDetId::ieta ())
    and the conversion to a usable 
    reference system [0,169] is done via etaShifter
  - all the methods have as input phi index defined in
    [1,360] (EBDetId::iphi ())
  - the hashed index is built on an eta index which spans
    in [0,169] x [0,359]
*/



class EBregionBuilder
{
 public :
  
  //! ctor
  EBregionBuilder(int etaStart,int etaEnd,int etaWidth,
                  int phiStart,int phiEnd,int phiWidth,
                  const std::string& histoName = "h_EBRegion");
  
  //! get the number of regions
  int EBregionsNum() const;

  //! get the phi width of region
  int EBRegionPhiWidth() const{
    return m_phiWidth;
  }

  //! get the phi width of region
  int EBRegionEtaWidth() const{
    return m_etaWidth;
  }

  //! get the average phi of region
  double EBRegionPhi(int id) const;

  //! get the average eta of region
  double EBRegionEta(int id) const;
  
  //! get the region Id
  int EBRegionId(int etaXtl, int phiXtl) const;
    
  //! checks if the xtal is in the region to be calibrated
  int EBregionCheck(int eta,int phi) const;
  
  //! change the coordinate system
  int etaShifter(int) const;
  
  //! get the region histogram
  TH2F* DrawEBRegion() const
  {
    return m_EBRegionHisto;
  }
    
  int xtalNumInRegion(int regionNum) const
  {
    return m_xtalNumInRegion.at(regionNum);
  }
  
  int xtalRegionId(int hashedIndex)
  {
    return m_xtalRegionId[hashedIndex];
  }
  
  int xtalPositionInRegion(int hashedIndex)
  {
    return m_xtalPositionInRegion[hashedIndex];
  }
  

  
 private :
  
  //! eta start of the region of interest
  int m_etaStart;
  
  //! eta end of the region of interest
  int m_etaEnd;
  
  //! eta size of the sub-matrix
  int m_etaWidth;
  
  //! phi start of the region of interest
  int m_phiStart;
  
  //! phi end of the region of interest
  int m_phiEnd;
  
  //! phi size of the sub-matrix
  int m_phiWidth;

  std::map<int,int> m_xtalRegionId;
  std::map<int,int> m_xtalPositionInRegion;
  std::vector<int>  m_xtalNumInRegion;
  
  std::string m_EBRegionHistoName;
  TH2F* m_EBRegionHisto;
  
  
  
  //! to define the regions for each cristal
  void EBRegionDefinition();
};



//! Build a EB region reading from a cfg file
EBregionBuilder* BuildEBRegion(const std::string& inFileName, const std::string& histoName = "h_EBRegion");

#endif
