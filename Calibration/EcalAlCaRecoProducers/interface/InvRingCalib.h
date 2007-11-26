/**
  * \file InvRingCalib.h
  * \class InvRingCalib
  * \brief ECAL TB 2006 calibration with matrix inversion technique
  * $Date: 2007/11/20 17:23:42 $
  * $Revision: 1.14 $
  * \author 
  *
*/
#ifndef __CINT__
#ifndef InvRingCalib_H
#define InvRingCalib_H
#include "Calibration/Tools/interface/matrixSaver.h"
#include "Calibration/Tools/interface/InvMatrixUtils.h"
#include "Calibration/EcalCalibAlgos/interface/EcalCalibBlock.h"
#include "Calibration/Tools/interface/smartSelector.h"
#include "FWCore/Framework/interface/EDLooper.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "CondFormats/EcalObjects/interface/EcalIntercalibConstants.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"
#include <iostream>
#include <string>
#include <vector>
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/CaloRecHit/interface/CaloRecHit.h"
class InvRingCalib : public edm::EDLooper {
  public:

    //! ctor
    explicit InvRingCalib(const edm::ParameterSet&);
    //! dtor
    ~InvRingCalib();
    void beginOfJob(const edm::EventSetup&) ;
    void endOfJob();
    void startingNewLoop(unsigned int) ;
    Status duringLoop(const edm::Event&, const edm::EventSetup&) ;
    Status endOfLoop(const edm::EventSetup&,unsigned int iCounter) ;
    
 //end

  private:

    //! write on plain text file the results
    int makeReport (std::string baseName="output") ;
    //! Tells in which region you are.
    int EBRingCheck (const int eta, const int phi) const ;
/*    //! fill a TH12F from a vector
    TH1F * fillTrend (std::vector<double> const & vettore, 
                      const int & index) ;
*/
    //! reset arrays
    template <class Type>
    void
    resetEB (Type Barrel[170][360], const Type val) 
      {
        for (int e = 0 ; e < SCMaxEta ; ++e)
          for (int p = 0 ; p < SCMaxPhi ; ++p)
            {         
              Barrel[e][p] = val ;
            }
      }

    template <class Type>
    void
    resetEE (Type Endcap[100][100], const Type val) 
      {
        for (int e = 0 ; e < 100 ; ++e)
          for (int p = 0 ; p < 100 ; ++p)
            {         
              Endcap[e][p] = val ;
            }
      }
   
  //!LP numero delle regioni lungo il raggio (onion rings) (da fare divisione lungo phi)
  inline int EERegionNum () const ;
  //!LP numero delle regioni in EB
  inline int EBRegionNum () const ;
  //!LP checks if the values of ics and ips are in EE or not
//  int isMyRegion (const int , const int , const int, const int ) ;

  //!LP Defines the regions in the barrel
  void EBRegionDef () ;
  //!LP Defines the rins in the endcap
  void EERingDef (const edm::EventSetup&);
  //!LP Defines the regions in the endcap
  void EERegionDef ();
//!LP reset the intercalibration coefficients
void resetEB(double *,double);
  void resetEE(double *,double);
  //!LP Gives back in which region you are:
  int EBRegId(const int) const;
  //!LP is zero if the region you want to calibrate is inside the limits given by the cfg file
  int EBRegionCheck (const int, const int) const;
  //!gives back in which region of the endcap you are. Doesn't it?
  int EERegId ( int) ;
  typedef reco::PixelMatchGsfElectronCollection::const_iterator eleIterator;
  //! fills the barrel energy map to be sent to the CalibBlock
    void fillEBMap (EBDetId, const EcalRecHitCollection *, std::map<int, double> &, int, double &);
 //! fills the endcap energy map to be sent to the CalibBlock
    void fillEEMap (EEDetId, const EcalRecHitCollection *, std::map<int, double> &, int, double &);
//! Find the most energetic Xtals    
    DetId findMaxHit ( const std::vector<DetId> & v1,
                       const EBRecHitCollection* EBhits , 
		       const EERecHitCollection* EEhits );
  private:

    //! EcalBarrel Input Collection name
    edm::InputTag m_barrelAlCa ;
    //! EcalEndcap Input Collection name
    edm::InputTag m_endcapAlCa ;
    //! To take the electrons
    edm::InputTag m_ElectronLabel ;
    //! reconstruction window size
    int m_recoWindowSide ;
    //! minimum energy per crystal cut
    double m_minEnergyPerCrystal ;
    //! maximum energy per crystal cut
    double m_maxEnergyPerCrystal ;
    //! eta start of the zone of interest
    int m_etaStart ;   //PG ECAL region to be calibrated
    //! eta end of the zone of interest
    int m_etaEnd ;
    //! eta size of the regions 
    int m_etaWidth ;   //PG sub matrix size and borders
    //! maximum number of events per crystal
//    int m_maxSelectedNumPerXtal ; //FIXME not yet used
    //! for statistical studies
//    int m_smallestFraction ; //FIXME not yet usedq
    //! for statistical studies
//    int m_howManyFractions ; //FIXME not yet usedq
    //! for statistical studies
//    smartSelector m_eventSelector ; //FIXME not yet usedq
    //! for statistical studies
//    int halfSelecting ; //FIXME not yet usedq
    //! for statistical studies
//    int takeOdd ; //FIXME not yet usedq

    //! name of the statistics matrix to be saved
//    std::string m_statisticsMatrixFileName ; //FIXME not yet usedq

    //! single blocks calibrators
    std::vector<EcalCalibBlock> m_ecalCalibBlocks ;
    //! minimum coefficient accepted (RAW)
    double m_minCoeff ;
    //! maximum coefficient accepted (RAW)
    double m_maxCoeff ;
    //! to exclude the blocksolver 
    int m_usingBlockSolver ;
    //! calibration factor
    //!LP Intercalibration Coeff between the xtals in the same ring
//    double m_EBRingRecalibFactor[170][360];
//    double m_EEPRingRecalibFactor[100][100];
//    double m_EEMRingRecalibFactor[100][100];
    //!LP position of the cell, borders, coords etc...
    std::map<int,GlobalPoint> m_cellPos;
    std::map<int,int> m_cellPhi;
    
    //!LP Intercalibration Coeff between the rings
    std::vector <double> m_InterRings;
    //!Temporary coeffs, one per xtal
    std::map <int, double> m_fakeCoeffs;//FIXME
    //! delta eta of the region of interest
//    int m_Deta ;
    //! evolution graphs 
//    std::vector<std::vector<double> > m_etaSliceCoeff ;
    //! LP sets the number of loops to do
    int m_loops ;
    //! LP define the EE region to calibrate
    int m_startRing;
    int m_endRing;
    //!The number of the ring in which the XY xtal is
//    int m_EEXtlRing[100][100];
    //!The number of the region in which the XY xtal is
//    int m_EEXtlReg[100][100];
    //! The number of the ring in which the xtal in the region frame
//    int m_EERingNum[100][100];
    //!The same as before but for the barrel;
//    int m_EBXtlReg[170][360];
//    int m_EBRingNum[170];
    std::map<int,int> m_xtalRing;
    std::map<int,int> m_xtalRegionId;
    std::map<int,int> m_RinginRegion;
    //! geometry things used all over the file
    std::vector<DetId> m_barrelCells;
    std::vector<DetId> m_endcapCells;
};
#endif
#endif
