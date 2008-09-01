#ifndef ClusterCalibTools_h
#define ClusterCalibTools_h

#include <map>
#include <vector>

/** to build the regions where to run the calibration
  - all the methods have as input eta index defined in:
    [-85,-1] U [1,85] and the conversion to a usable 
    reference system [0,169] is done via etaShifter
  - all the methods have as input phi index defined in
    [1,360]
*/
class EBregionBuilder
{
  public :

    EBregionBuilder (int etaStart, int etaEnd, int etaWidth,
                     int phiStartEB, int phiEndEB, int phiWidthEB) ;

  //!
  int EBregionsNum () const ;
  //!
  int EBRegionId (int etaXtl, int phiXtl) const ;
  //! to define the regions for each cristal
  void EBRegionDefinition () ;
  //! defines the limit for the tan of phi
  double giveLimit (int) ;

  //! checks if the xtal is in the region to be calibrated
  int EBregionCheck (int eta, int phi) const ;

  //! feeds the xtals map for the calib algo
  void fillEBMap (std::map<int,double> & EBxtlMap,
                  double & pSubtract,
                  const std::map<int, double> & SCComponentsMap,
                  int EBNumberOfRegion) ;

  //!LP Change the coordinate system
  int etaShifter (int) const ;

  //!PG Change the coordinate system
  int phiShifter (int) const ;

  int xtalNumInRegion (int regionNum) const 
    {
      return m_regions.at (regionNum) ;
    }

  private :

    //! eta size of the sub-matrix
    int m_etaWidth ;   //PG sub matrix size and borders
    //! eta size of the additive border to the sub-matrix
//    int m_etaBorder ; //FIXME
    //! phi size of the sub-matrix
    int m_phiWidthEB ;
    //! phi size of the additive border to the sub-matrix
//    int m_phiBorderEB //FIXME;
    
    //! eta start of the region of interest
    int m_etaStart ;   //PG ECAL region to be calibrated
    //! eta end of the region of interest
    int m_etaEnd ;
    //! phi start of the region of interest
    int m_phiStartEB ;
    //! phi end of the region of interest
    int m_phiEndEB ;
    //!DS For the EE
  
    std::vector<int> m_regions ;
    
    std::map<int,int> m_xtalRegionId ;
    std::map<int,int> m_xtalPositionInRegion ;
  

} ;

#endif
