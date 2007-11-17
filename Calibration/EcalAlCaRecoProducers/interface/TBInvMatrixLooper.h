/**
  * \file TBInvMatrixLooper.h
  * \class TBInvMatrixLooper
  * \brief ECAL TB 2006 calibration with matrix inversion technique
  * $Date: 2007/09/05 20:55:40 $
  * $Revision: 1.1 $
  * \author 
  *
*/
#ifndef __CINT__
#ifndef TBInvMatrixLooper_H
#define TBInvMatrixLooper_H
#include "Calibration/Tools/interface/matrixSaver.h"
#include "Calibration/Tools/interface/InvMatrixUtils.h"
#include "Calibration/EcalCalibAlgos/interface/EcalCalibBlock.h"
#include "Calibration/Tools/interface/smartSelector.h"
#include "FWCore/Framework/interface/EDLooper.h"
#include "FWCore/Framework/interface/Event.h"

#include <iostream>
#include <string>
#include <vector>


#include "Calibration/Tools/interface/InvMatrixCommonDefs.h"//FIXME Not used, but it would be useful
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"

class TBInvMatrixLooper : public edm::EDLooper {
  public:

    //! ctor
    explicit TBInvMatrixLooper (const edm::ParameterSet&) ;
    //! dtor
    ~TBInvMatrixLooper () ;
    void beginOfJob (const edm::EventSetup&) ;
    void endOfJob () ;
    void startingNewLoop (unsigned int) ;
    Status duringLoop (const edm::Event&, const edm::EventSetup&) ;
    Status endOfLoop (const edm::EventSetup&, unsigned int iCounter) ;
    

  private:

    typedef reco::PixelMatchGsfElectronCollection::const_iterator eleIterator ;
    typedef edm::Handle<reco::BasicClusterShapeAssociationCollection>  HandleBasicCSAC ; 

    DetId getMaxId (eleIterator EleIt,
                    HandleBasicCSAC  & barrelClShpHandle,
                    HandleBasicCSAC  & endcapClShpHandle) ;

    void fillEBMap (EBDetId EBmax,
                    const EBUncalibratedRecHitCollection * barrelHitsCollection,
                    std::map<int,double> & EBXtlMap,
                    int EBNumberOfRegion, double & pSubtract ) ;


    //! write on plain text file the results
    int makeReport (std::string baseName="output") ;
    //! give the number of chi2 matrices
    int evalKaliX2Num () ;
    //! give the size of a chi2 matrix

    //! get the index of the sub-matrix
    int indexFinder (const int etaWorld, const int phiWorld) ;
    //! fill a TH12F from a vector
    TH1F * fillTrend (std::vector<double> const & vettore, 
                      const int & index) ;
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

   //! look for void lines in the matrix   
   int findVoidLine (const CLHEP::HepMatrix & suspect) ;
   //! transport the calib coeff in the local matrix
   void updateCoeff (ecalIM::coeffBlock range) ;
   //! transfer from a CLHEP matrix to a C-like array
   void transferEB (double output[170][360], 
                  CLHEP::HepMatrix * input) ;
  
  //DS for during loop added 26072007
  EcalRecHit getMaximum (const EcalRecHitCollection * recHits) ;
  void fillAroundBarrel (const EcalRecHitCollection * recHits, int eta, int phi) ;
  void fillAroundEndcap (const EcalRecHitCollection * recHits, int ics, int ips) ;
  //end

  //DS to divide in Regions
  int EBRegionId (const int, const int) const ;
  //DS to define the regions for each cristal
  void EBRegionDefinition () ;
  //DS defines the limit for the tan of phi
  double giveLimit (int) ;

  //DS checks if the values of ics and ips are in EE or not
  int EBregionCheck (const int eta,const int phi) const ;

  //DS
  const int isMyRegion (const int , const int , const int, const int ) ;
  //DS
  int getRegionId (const int ,const int ,const int) ;


  //!LP Change the coordinate system
  int etaShifter (const int) const ;

  private:


    //! EcalBarrel Input Collection name
    edm::InputTag m_barrelAlCa ;
  
    //! reconstruction window size
    int m_recoWindowSide ;

    //! eta size of the sub-matrix
    int m_etaWidth ;   //PG sub matrix size and borders
    //! eta size of the additive border to the sub-matrix
    int m_etaBorder ;
    //! phi size of the sub-matrix
    int m_phiWidthEB ;
    //! phi size of the additive border to the sub-matrix
    int m_phiBorderEB ;
    
    //! eta start of the region of interest
    int m_etaStart ;   //PG ECAL region to be calibrated
    //! eta end of the region of interest
    int m_etaEnd ;
    //! phi start of the region of interest
    int m_phiStartEB ;
    //! phi end of the region of interest
    int m_phiEndEB ;
    int m_EBxtlNum[170][360] ;
    int m_EBxtlReg[170][360] ;

    std::vector<int> m_regionSize ;

    //! half width on the front face of the crystal along x
    double m_halfXBand ;
    //! half width on the front face of the crystal along y
    double m_halfYBand ;
    //! maximum number of events per crystal
    int m_maxSelectedNumPerXtal ;
    //! for statistical studies
    int m_smallestFraction ;
    //! for statistical studies
    int m_howManyFractions ;
    //! for statistical studies
    smartSelector m_eventSelector ;
    //! for statistical studies
    int halfSelecting ;
    //! for statistical studies
    int takeOdd ;

    //! where to save the output
    std::string m_outputFolder ;
    //! comment FIXME
    std::string m_outputFileName ;
    //! comment FIXME
    std::string m_EBcalibMapFileName ;
    //! name of the statistics matrix to be saved
    std::string m_statisticsMatrixFileName ;

    //! single blocks calibrators
    std::vector<EcalCalibBlock> m_ecalCalibBlocks ;

    //! minimum energy per crystal cut
    double m_minEnergyPerCrystal ;
    //! maximum energy per crystal cut
    double m_maxEnergyPerCrystal ;
    //! minimum coefficient accepted (RAW)
    double m_minCoeff ;
    //! maximum coefficient accepted (RAW)
    double m_maxCoeff ;
    //! to exclude the blocksolver 
    int m_usingBlockSolver ;

    double m_minAccept ;
    double m_maxAccept ;
    //! array of association beteen uIndex and crystal number
    int * m_xtalNums ;
    //! calibration factor
    double m_EBrecalibFactor[170][360] ;
 /*  
    //! array of x hodoscope coordinates of the energetic centre 
    double m_xHodoCentre[SCMaxEta][SCMaxPhi] ;
    //! array of y hodoscope coordinates of the energetic centre 
    double m_yHodoCentre[SCMaxEta][SCMaxPhi] ;
    
  */
  
 	 //! array of efficiency evaluators (one per crystal)
	 //Efficiency * m_eff ;

		
    //! delta eta of the region of interest
    int m_Deta ;

    //! evolution graphs 
    std::vector<std::vector<double> > m_etaSliceCoeff ;
    //! phi of the index slice
    int m_sliceIndex ;
    //! DS sets the number of loops to do
    int m_loops ;
    //! Used to know in which region you are working (-1 EE, 0 EB, +1 the other EE)
    int m_ecalZone ;
    //! To take the electrons
    edm::InputTag m_ElectronLabel ;

    //DS numero delle regioni in EB
    inline int EBregionsNum () const ;
  
    std::vector<int> m_EBregions ;
    
    std::vector<DetId> m_barrelCells ;
    
    std::string eventHeaderProducer_ ;
    std::string eventHeaderCollection_ ;    /// name of module/plugin/producer making uncalib rechits
    std::string uncalibRecHitProducer_ ; 
    /// secondary name given to collection of EB uncalib rechits
    std::string EBuncalibRecHitCollection_ ; 
    //! energy scale: xxxx ADC_counts * m_ADCtoGeV = yy GeV
    double m_ADCtoGeV ;

} ;
#endif
#endif
