#include <memory>
#include <math.h>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Calibration/EcalAlCaRecoProducers/interface/TBInvMatrixLooper.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "TH2.h"
#include "TFile.h"
//LP includes to read/write the original coeff
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "CondFormats/DataRecord/interface/EcalIntercalibConstantsRcd.h"
#include "Calibration/Tools/interface/calibXMLwriter.h"
#include "CalibCalorimetry/CaloMiscalibTools/interface/CaloMiscalibTools.h"
//DS verify all these include
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"
#include "TBDataFormats/EcalTBObjects/interface/EcalTBEventHeader.h"


//DS LogMessages
#include "FWCore/MessageLogger/interface/MessageLogger.h" 



//----------------------------------------------------------------


//!LP ctor
TBInvMatrixLooper::TBInvMatrixLooper (const edm::ParameterSet& iConfig) :
      m_barrelAlCa (iConfig.getParameter<edm::InputTag> ("alcaBarrelHitCollection")) ,
      m_recoWindowSide (iConfig.getParameter<int> ("recoWindowSide")) ,
      m_etaWidth (iConfig.getParameter<int> ("etaWidth")) ,
      m_etaBorder (iConfig.getParameter<int> ("etaBorder")) ,
      m_phiWidthEB (iConfig.getParameter<int> ("phiWidthEB")) ,
      m_phiBorderEB (iConfig.getParameter<int> ("phiBorderEB")) ,
      m_etaStart (etaShifter (iConfig.getParameter<int> ("etaStart"))) , 
      m_etaEnd (etaShifter (iConfig.getParameter<int> ("etaEnd"))) ,
      m_phiStartEB (iConfig.getParameter<int> ("phiStartEB")) , 
      m_phiEndEB (iConfig.getParameter<int> ("phiEndEB")),
      m_halfXBand (iConfig.getParameter<double> ("halfXBand")) ,
      m_halfYBand (iConfig.getParameter<double> ("halfYBand")) ,
      m_maxSelectedNumPerXtal (iConfig.getParameter<int> ("maxSelectedNumPerCrystal")) ,  //attualmente non usato ma vorremmo
      m_outputFolder (iConfig.getParameter<std::string> ("outputFolder")) ,
      m_outputFileName (m_outputFolder + "/" + iConfig.getParameter<std::string> ("outputFileName")) ,
      m_EBcalibMapFileName (iConfig.getParameter<std::string> ("EBcalibMapFileName")),
      m_statisticsMatrixFileName (m_outputFolder + "/" + iConfig.getParameter<std::string> ("statisticMatrixFileName")),
      m_maxEnergyPerCrystal (iConfig.getParameter<double> ("maxEnergyPerCrystal")) ,
      m_minCoeff (iConfig.getParameter<double> ("minCoeff")) ,
      m_maxCoeff (iConfig.getParameter<double> ("maxCoeff")) ,
      m_usingBlockSolver (iConfig.getParameter<int> ("usingBlockSolver")) ,
      m_minAccept (iConfig.getParameter<double> ("minAccept")) ,
      m_maxAccept (iConfig.getParameter<double> ("maxAccept")) ,
      m_loops (iConfig.getParameter<int> ("loops")),
      m_ElectronLabel (iConfig.getParameter<edm::InputTag> ("electronLabel")),
      eventHeaderProducer_ (iConfig.getParameter<std::string> ("eventHeaderProducer")) ,
      eventHeaderCollection_ (iConfig.getParameter<std::string> ("eventHeaderCollection")) ,
      uncalibRecHitProducer_ (iConfig.getParameter<std::string> ("uncalibRecHitProducer")) ,
      EBuncalibRecHitCollection_ (iConfig.getParameter<std::string> ("EBuncalibRecHitCollection")) ,
      m_ADCtoGeV (iConfig.getParameter<double> ("ADCtoGeV"))
  
  //Controls the parameters and their conversions
  {

   edm::LogInfo ("IML") << "[TBInvMatrixLooper][ctor] asserts" ;

   assert (m_etaStart >=0 && m_etaStart < 170) ;
   assert (m_etaEnd >= m_etaStart && m_etaEnd <= 170) ;
   assert (m_phiStartEB >=0 && m_phiStartEB < 360) ;
   assert (m_phiEndEB >= m_phiStartEB && m_phiEndEB <= 360) ;

   edm::LogInfo ("IML") << "[TBInvMatrixLooper][ctor] entering " ;

   for (int a=0 ; a<170 ; ++a)
     for (int b=0 ; b<360 ; ++b)
       m_EBrecalibFactor[a][b] = 1. ;
       
   edm::LogInfo ("IML") << "[TBInvMatrixLooper][ctor] region definition" ;
   EBRegionDefinition () ;

  //Graphs to ckeck the region definition
  TH2F * EBRegion = new TH2F ("EBRegion","EBRegion",171,0,170,360,0,360) ;
   for (int eta = 0 ; eta<170 ; ++eta)
     for (int phi = 0 ; phi <360 ; ++phi)
       {
         int zona= -10 ;
         if (eta >= m_etaStart && eta < m_etaEnd && 
             phi >= m_phiStartEB && phi < m_phiEndEB) zona = 0 ;
         if (m_EBxtlReg[eta][phi]%2) zona = 10 ;
           EBRegion->Fill (eta, phi, zona) ;
       }

   TFile out ("EBZone.root", "recreate") ;
   EBRegion->Write () ;
   out.Close () ;

   edm::LogInfo ("IML") << "[TBInvMatrixLooper][ctor] Calib Block" ;
   //LP Passiamo a EcalCalibBlock cio' di cui necessita
   //DS EcalBarrel

   for (int region = 0 ; region < EBregionsNum () ; region++)
     m_ecalCalibBlocks.push_back (EcalCalibBlock (m_EBregions.at (region))) ; 
     
   edm::LogInfo ("IML") << "[TBInvMatrixLooper][ctor] Matrix Saver" ;

   if (m_EBcalibMapFileName == "") resetEB (m_EBrecalibFactor, 1.) ;
   else
     {
        matrixSaver leggo ;
        CLHEP::HepMatrix * recalibHep = static_cast<CLHEP::HepMatrix*>
                                     (leggo.getMatrix (m_EBcalibMapFileName)) ;
        std::cout << "reading energetic centres from file\n" ;
        transferEB (m_EBrecalibFactor, recalibHep) ;
        delete recalibHep ;
      }
   //DS!! pay attention here set valid limits with asserts
   for (int eta=m_etaStart ; eta < m_etaEnd ; ++eta)               
     {
       m_etaSliceCoeff.push_back (std::vector<double> ()) ;
     }

   std::string xCrossMtrFile = iConfig.getParameter<std::string> ("xCrossMtrFile") ;
   std::string yCrossMtrFile = iConfig.getParameter<std::string> ("yCrossMtrFile") ;

   resetEB (m_EBrecalibFactor, 1.000) ;
}


//--------------------------------------------------------------------------- 


//!LP destructor
TBInvMatrixLooper::~TBInvMatrixLooper ()
{
  edm::LogInfo ("IML") << "[TBInvMatrixLooper][dtor]" ;
}


//--------------------------------------------------------------------------- 


//!BeginOfJob
void 
TBInvMatrixLooper::beginOfJob (const edm::EventSetup & iSetup) 
{
    edm::LogInfo ("IML") << "[TBInvMatrixLooper][beginOfJob]" ;
   //if necessary histos definition

  edm::ESHandle<CaloGeometry> geoHandle ;
  iSetup.get<IdealGeometryRecord> ().get (geoHandle) ;
  const CaloGeometry& geometry = *geoHandle ;
  m_barrelCells = geometry.getValidDetIds (DetId::Ecal, EcalBarrel) ;
  EcalIntercalibConstants::EcalIntercalibConstantMap imap ;
  edm::ESHandle<EcalIntercalibConstants> pIcal ;
   try {
      iSetup.get<EcalIntercalibConstantsRcd> ().get (pIcal) ;
      std::cout << "Taken EcalIntercalibConstants" << std::endl ; //FIXME with msg logger
      imap = pIcal.product ()->getMap () ;
      std::cout << "imap.size () = " << imap.size () << std::endl ; //FIXME with msg logger
     } 
   catch ( std::exception& ex ) {     
      std::cerr << "Error! can't get EcalIntercalibConstants " << std::endl ;
     }
     
  //Fills the vectors of the coefficients from the map in the barrel. 
  for (std::vector<DetId>::const_iterator barrelIt = m_barrelCells.begin () ; 
       barrelIt!=m_barrelCells.end () ; 
       ++barrelIt) 
    {
        EBDetId eb (*barrelIt) ;
        int i=etaShifter (eb.ieta ()) ;
        int j=eb.iphi () ;
        m_EBrecalibFactor[i][j] = (imap.find (eb.rawId ()))->second ;
    }

  // prints out the initial coeffs
  for (int e = 0 ; e < 170 ; ++e) 
      for (int p = 0 ; p < 360 ; ++p)
        {
          if ( EBregionCheck (e,p)) continue ;
          std::cout << "[TBInvMatrixLooper][beginofjob] eta: " << e
                    << "\t phi: " << p 
                    << "\t xtal: " << e + 170 * p
                    << "\t calib: " << m_EBrecalibFactor[e][p]
                    << std::endl ; 
        }

  //To see the initial coefficients
  TH1F * EBcoeff = new TH1F ("EBRegion","EBRegion",200,0.8,1.2) ;
   for (int eta = 0 ; eta<170 ; ++eta)
     for (int phi = 0 ; phi <360 ; ++phi)
       EBcoeff->Fill (m_EBrecalibFactor[eta][phi]) ;
      
  TFile zout ("EBcoeff.root", "recreate") ;
  EBcoeff->Write () ;
  zout.Close () ;
}


//--------------------------------------------------------


//!startingNewLoop
void TBInvMatrixLooper::startingNewLoop (unsigned int ciclo) 
{
    edm::LogInfo ("IML") << "[InvMatrixCalibLooper][Start] entering loop " << ciclo ;

    for (std::vector<EcalCalibBlock>::iterator calibBlock = m_ecalCalibBlocks.begin () ;
         calibBlock != m_ecalCalibBlocks.end () ;
         ++calibBlock)
      {
        calibBlock->reset () ;
      }
    return ;
}


//--------------------------------------------------------


//!duringLoop
edm::EDLooper::Status
TBInvMatrixLooper::duringLoop (const edm::Event& iEvent,
                             const edm::EventSetup&) 
{
  edm::LogInfo ("IML") << "[InvMatrixCalibLooper][duringLoop] try and catch for barrel and endcap" ; 

  //PG qui devo prendere dal web recuperare le uncalib rec hit FIXME
  //PG aggiungi le variabili FIXME
   edm::Handle< EBUncalibratedRecHitCollection > pEBUncalibRecHits ;   const EBUncalibratedRecHitCollection*  EBuncalibRecHits = 0 ;
   try {       iEvent.getByLabel (uncalibRecHitProducer_,                          EBuncalibRecHitCollection_,                          pEBUncalibRecHits) ;       EBuncalibRecHits = pEBUncalibRecHits.product () ; // get a ptr to the product     } catch ( std::exception& ex ) {       edm::LogError ("missed") << "get collection " << EBuncalibRecHitCollection_                                 << " from producer " << uncalibRecHitProducer_                                << " failed: " << std::endl ;       edm::LogError ("missed") << ex.what () << std::endl ;     }   if (!EBuncalibRecHits || EBuncalibRecHits->size () == 0) return kContinue ;

/*
   //PG fill a fake CaloRecHitCollection
   //PG devo anche riempire la mappa con al geometria? FIXME 
   EBRecHitCollection barrelHitsCollection ;
   for (EBRecHitCollection::const_iterator EBUCRHit = EBuncalibRecHits->begin () ;
        EBUCRHit != EBuncalibRecHits->end () ;
        ++EBUCRHit)
     {
       m_ADCtoGeV * EBUCRHit->energy () ;
       CaloRecHit dummy () ;
       barrelHitsCollection.push_back (dummy) ;
     }   
*/

   //PG FIXME controlla se questo serve 
   HandleBasicCSAC barrelClShpHandle ;
   iEvent.getByLabel ("hybridSuperClusters","hybridShapeAssoc", barrelClShpHandle) ;

   //PG the TB event header
   edm::Handle<EcalTBEventHeader> pEventHeader ;
   const EcalTBEventHeader* evtHeader=0 ;
   try {
     iEvent.getByLabel ( eventHeaderProducer_ , pEventHeader ) ;
     evtHeader = pEventHeader.product () ; // get a ptr to the product
   } catch ( std::exception& ex ) {
     edm::LogError ("missed") << ex.what () << std::endl ;
   }

  int ecalZone = -1 ;
  double pSubtract = 0. ;
  double pTk = 0. ;
  int size = -1 ;

  edm::LogInfo ("IML") << "[InvMatrixCalibLooper][duringLoop] Map creation+filling " ; 

   //PG get the header information

   int xtalNum = -1 ;
   int run = -1 ;
   int event = -1 ;
   int tableIsMoving = -1 ;
   int S6ADC = 0 ;
   std::string eventType = "0" ;
   EBDetId EBmax ;
   
   if (evtHeader)
     {
       //FIXME in questo caso devo creare un EBDetId con il cristallo massimo
     //D LogDebug ("EvtHeaderDebug") << "Found evtHeader " << std::endl ;

       //eventType = evtHeader->eventType () ;
       xtalNum = evtHeader->crystalInBeam () ;
       run = evtHeader->runNumber () ;
       event = evtHeader->eventNumber () ;
       tableIsMoving = evtHeader->tableIsMoving () ;
       S6ADC = evtHeader->S6ADC () ;
       /*
       LogDebug ("EvtHeaderDebug") << "eventType "     << eventType     << "\n"
                                  << "xtalNum "       << xtalNum       << "\n"
                                  << "run "           << run           << "\n"
                                  << "event "         << event         << "\n"
                                  << "tableIsMoving " << tableIsMoving << "\n"
                                  << "S6ADC "         << S6ADC         << "\n" ;
        */         
     }

   //PG fixme
   int useMaxEnergyXtal_ = 1 ;
   if (not evtHeader or useMaxEnergyXtal_)
     {
     //D
       /*
       LogDebug ("EvtHeaderDebug") << "evtHeader= " << evtHeader << " and "
                 << "useMaxEnergyXtal_= " << useMaxEnergyXtal_ << "\n" ;
       */          
       float maxAmplitude = -999999. ;
       DetId MExtal (0) ;
       // loop over uncalibrated rechits
       for (EBUncalibratedRecHitCollection::const_iterator EBUit
                                       = EBuncalibRecHits->begin () ;
            EBUit != EBuncalibRecHits->end () ;
            ++EBUit)
         {
           if (maxAmplitude < EBUit->amplitude ())
             {
               maxAmplitude = EBUit->amplitude () ;
               MExtal = EBUit->id () ;
             }
         } // loop over uncalibrated rechits
       if (maxAmplitude < -999990) return kContinue ;
       EBmax = MExtal ;
       xtalNum = EBmax.ic () ;
       //PG std::cout << "xtalNum " << xtalNum << std::endl ;
   }


  //PG region of interest cut
  if (EBregionCheck (etaShifter (EBmax.ieta ()), EBmax.iphi ())) return kContinue ;

  //PG build the matrix around the header
  std::map<int , double> EBxtlMap ;
  
  std::cerr << "filling EB" << "\n" ;
  fillEBMap (EBmax, EBuncalibRecHits, EBxtlMap,
             EBRegionId (etaShifter (EBmax.ieta ()),EBmax.iphi ()), pSubtract ) ;
  int blockIndex =  EBRegionId (etaShifter (EBmax.ieta ()), EBmax.iphi ()) ;
  m_ecalCalibBlocks.at (blockIndex).Fill (EBxtlMap.begin (), 
                                          EBxtlMap.end (),
                                          pTk,pSubtract) ;




  edm::LogInfo ("IML") << "[InvMatrixCalibLooper][duringLoop] End of during loop" ; 
  return  kContinue ;
}


//---------------------------------------------------------------------


//!LP EndOfLoop
edm::EDLooper::Status 
TBInvMatrixLooper::endOfLoop (const edm::EventSetup& dumb,
                              unsigned int iCounter)
{
for (std::vector<EcalCalibBlock>::iterator calibBlock = m_ecalCalibBlocks.begin () ;
      calibBlock != m_ecalCalibBlocks.end () ;
      ++calibBlock)
      {
        calibBlock->solve (m_usingBlockSolver, m_minCoeff, m_maxCoeff) ;
      }      
  edm::LogInfo ("IML") << "[InvMatrixCalibLooper][endOfLoop] entering..." ;
  //LP loop over the Barrel to get the recalib coefficients.
  for (int eta = 0 ; eta < 170 ; ++eta)
   for (int phi = 0 ; phi < 360 ; ++phi)
    {
      m_EBrecalibFactor[eta][phi] *= m_ecalCalibBlocks.at (m_EBxtlReg[eta][phi]).at (m_EBxtlNum[eta][phi]) ;
    }
  std::cerr << "loop over the Barrel to get the recalib coefficients" << "\n" ;

  //DS! provvisorio, per vedere se cambiano i coeff nei loops
  std::cerr<<"COEFFICIENTI AL LOOP "<<iCounter<<"\n" ;

  for (int e = 0 ; e < 170 ; ++e) 
     for (int p = 0 ; p < 360 ; ++p)
       {
         if ( EBregionCheck (e,p)) continue ;
         std::cout << "[TBInvMatrixLooper][dtor] eta: " << e
                   << "\t phi: " << p 
                   << "\t xtal: " << e + 170 * p
                   << "\t calib: " << m_EBrecalibFactor[e][p]
                   << std::endl ; 
       }
  std::cerr<<"COEFFICIENTI AL LOOP "<<iCounter<<"\n" ;

//DS evolution 
/*   std::string evolSaveName =
    m_outputFolder + "/" + "evolution.root" ;
    TFile evolSave (evolSaveName.c_str (),"recreate") ;
    evolSave.cd () ;
    for (int eta=m_etaStart ; eta < m_etaEnd ; ++eta)
      { 
        TH1F * dummy = fillTrend (m_etaSliceCoeff.at (eta-m_etaStart),eta) ;
        dummy->Write () ;
      }
    evolSave.Close () ; 
*/

  edm::LogInfo ("IML") << "[InvMatrixCalibLooper][endOfLoop] End of endOfLoop" ;
  std::cerr<<"ending endof loop"<<"\n" ;
  
  if (iCounter < m_loops ) return kContinue ;
  else return kStop ; 
}


//---------------------------------------
//LP endOfJob
void 
TBInvMatrixLooper::endOfJob ()
{


  edm::LogInfo ("IML") << "[InvMatrixCalibLooper][endOfJob] saving calib coeffs" ;
 //Works in 

  calibXMLwriter barrelWriter (EcalBarrel) ;

  for (std::vector<DetId>::const_iterator barrelIt =m_barrelCells.begin () ; 
       barrelIt != m_barrelCells.end () ; 
       ++barrelIt) 
    {
      EBDetId eb (*barrelIt) ;
      int leta = etaShifter (eb.ieta ()) ;
      int lphi = eb.iphi () ;
      double coeff = m_EBrecalibFactor[leta][lphi] ; 
      barrelWriter.writeLine (eb,coeff) ;
    }

/*
 std::string fake ;
    matrixSaver disco ;
    CLHEP::HepMatrix recalib (170,360,0) ;
    for (int e = 0 ; e < 170 ; ++e)
     for (int p = 0 ; p < 360 ; ++p)
       recalib[e][p] = 1. ;
    CLHEP::HepMatrix recalibToBeSaved (170,360,0) ;

  for (int e = 0 ; e < 170 ; ++e) 
     for (int p = 0 ; p < 360 ; ++p)
       {
         recalib[e][p] = m_EBrecalibFactor[e][p] ; 
         if ( EBregionCheck (e,p)) continue ;
         recalibToBeSaved[e][p] = m_EBrecalibFactor[e][p] ; 
         std::cout << "[TBInvMatrixLooper][dtor] eta: " << e
                   << "\t phi: " << p 
                   << "\t xtal: " << e + 170 * p
                   << "\t recalib: " << m_EBrecalibFactor[e][p]
                   << "\t factor: " << recalib[e][p]
                   << std::endl ; 
       }


    fake = m_outputFileName + "EB" ;
  edm::LogInfo ("IML") << fake ;
    disco.saveMatrix (fake , &recalibToBeSaved) ;
  */  
  
  edm::LogInfo ("IML") << "[InvMatrixCalibLooper][endOfJob] Exiting" ;
}



//------------------------------------//
//      definition of functions       //
//------------------------------------//



//!DS! passargli l'eta già shiftato!!!
int 
TBInvMatrixLooper::EBregionCheck (const int eta, const int phi) const 
  {
    if (eta < m_etaStart) return 1 ;
    if (eta >= m_etaEnd)   return 2 ;

    if (phi < m_phiStartEB) return 3 ;
    if (phi >= m_phiEndEB)   return 4 ;

    return 0 ;
  }


//--------------------------------------------------


TH1F *
TBInvMatrixLooper::fillTrend (std::vector<double> const & vettore,
                            const int & index)
 {
  char tmp[80] ;
  sprintf (tmp,"trend_%d",index) ;
  TH1F * ritorno = new TH1F (tmp,tmp,
                             vettore.size (),0,vettore.size ()) ;

  for (unsigned int i = 0 ; i < vettore.size () ; ++i)
    {
      ritorno->Fill (i,vettore[i]) ;
    }    
  return ritorno ;
 }


//--------------------------------------------------


void
TBInvMatrixLooper::transferEB (double output[170][360], 
                                 CLHEP::HepMatrix * input)
{
  for (int eta = 0 ; eta < 170 ; ++eta)                              
    for (int phi = 0 ; phi < 360 ; ++phi)                              
      output[eta][phi] = (*input)[eta][phi] ;
  return ;
}


//--------------------------------------------------


//!DS def degrees
 inline double degrees (double radiants)
    {
    return radiants * 180 * (1/M_PI) ;
    }

//DS def radiants
 inline double radiants (int degrees)
    {
      return degrees * M_PI * (1 / 180) ;  
    }    


//--------------------------------------------------


//DS give Limit -> do' un limite alla tangente 
double TBInvMatrixLooper::giveLimit (int degrees)
  {
    //PG 200 > atan (50/0.5)
    if (degrees == 90) return 200 ; 
    return tan (radiants (degrees)) ;      
  } 


//--------------------------------------------------


inline double Mod (double phi)
  {
  if (phi>=360 && phi<720) return phi-360 ;
  if (phi>=720) return phi-720 ;
  return phi ;
  } 


//--------------------------------------------------


//!Reg Id generator EB ----- for the barrel
//!DS! passare l'eta gia' shiftato!!!
int TBInvMatrixLooper::EBRegionId (const int etaXtl,const int phiXtl) const 
{

    assert ( (m_etaEnd-m_etaStart)%m_etaWidth == 0) ; //DS! questo assert è da rivedere per il caso in cui sia contenuto lo 0!
    assert ( (m_phiEndEB-m_phiStartEB)%m_phiWidthEB == 0) ;

  //DS numero di regioni in eta
  int Neta = (m_etaEnd-m_etaStart)/m_etaWidth ;
  //DS numero di regioni in phi
    int phifake = m_phiStartEB ;
    if (m_phiStartEB>m_phiEndEB) phifake = m_phiStartEB - 360 ;

  int Nphi = (m_phiEndEB-phifake)/m_phiWidthEB ;
  int Ntot = Neta*Nphi ;                                             // numero totale di regioni
 // std::cout << "total number of regions (EB) in the selected area: " << Ntot << std::endl ;  

  //DS indice lungo le due coordinate
  int etaI = (etaXtl-m_etaStart)/m_etaWidth ;  //DS! shiftato per errore sul vector... 26 ago 2007 
  int phiI = (phiXtl-m_phiStartEB)/m_phiWidthEB ; 

  //DS trova il numero della regione
  int regionNumEB = phiI + Nphi*etaI ;

  return (int) regionNumEB ;
}


//--------------------------------------------------


//!DS number of regions in EB
inline int 
TBInvMatrixLooper::EBregionsNum () const 
  {
    int phi = m_phiStartEB ;
    if (m_phiStartEB>m_phiEndEB) phi = m_phiStartEB - 360 ;
    return ( (m_etaEnd - m_etaStart)/m_etaWidth) * ( (m_phiEndEB - phi)/m_phiWidthEB) ; 
  }


//--------------------------------------------------


//!DS EB Region Definition
void 
TBInvMatrixLooper::EBRegionDefinition ()
  {
    // reset
    for (int it = 0 ; it < EBregionsNum () ; ++it) m_EBregions.push_back (0) ;   
    // loop sui xtl 

    for (int eta = m_etaStart ; eta < m_etaEnd  ; ++eta)
      for (int phi = m_phiStartEB ; phi < m_phiEndEB ; ++phi)
        {

          m_EBxtlNum[eta][phi] = m_EBregions.at (EBRegionId (eta,phi)) ;
          ++m_EBregions.at (EBRegionId (eta,phi)) ;
          // restiruisce la regione in cui si trova il cristallo
          m_EBxtlReg[eta][phi] = EBRegionId (eta,phi) ;

        }
    for (int it = 0 ; it < EBregionsNum () ; ++it) m_regionSize.push_back (m_EBregions.at (it)) ;
  }


//--------------------------------------------------


//! Picks up the maximum (most energetic cristal)
EcalRecHit
TBInvMatrixLooper::getMaximum (const EcalRecHitCollection * recHits) 
{
  double energy = 0. ;
  EcalRecHit max ;
  for (EcalRecHitCollection::const_iterator elem = recHits->begin () ;
       elem != recHits->end () ;
       ++elem)
    {
      if (elem->energy () > energy)
        {
          energy = elem->energy () ;
          max = *elem ;
        }        
    }   
  return max ;
}


//--------------------------------------------------


//!DS Shifts eta in other coordinates (from 0 to 170)
inline int TBInvMatrixLooper::etaShifter (const int etaOld) const
   {
     if (etaOld < 0) return etaOld + 85 ;
     else if (etaOld > 0) return etaOld + 84 ;
   }


//--------------------------------------------------


//!DS checks if the region you asks for is the correct one
const int 
TBInvMatrixLooper::isMyRegion (const int coordOne, const int coordTwo, 
                               const int regionId, const int ecalZone)
{
  return (m_EBxtlReg[coordOne][coordTwo] == regionId) ;
}


//--------------------------------------------------


//!DS Gets Region Id
int 
TBInvMatrixLooper::getRegionId (const int coordOne, const int coordTwo, 
                                const int ecalZone)
{
  return EBRegionId (coordOne, coordTwo) ;
}


//--------------------------------------------------


//!DS returns the ecal region (EE-, EB, EE+) from eta input (maybe elementId.eta ())
//!DS now you can use elementId.zside () to distinguish EE- from EE+
// ZSIDE!!! use zside
inline const int whichZone (double eta)
{
  int ecalindex ;
  if (eta <= -1.49) return ecalindex = -1 ;         //this is EE-
  else if (eta >= +1.49) return ecalindex = 1 ;     //this is EE+
  else return ecalindex = 0 ;

}


//--------------------------------------------------


DetId TBInvMatrixLooper::getMaxId (eleIterator eleIt,
                                HandleBasicCSAC  & barrelClShpHandle,
                                HandleBasicCSAC  & endcapClShpHandle)
{
   const reco::SuperCluster* mySC =
          & (* ( (eleIt->superCluster ()))) ;

   reco::AlgoId algoId = mySC->seed ()->algo () ;
   reco::BasicClusterShapeAssociationCollection::const_iterator seedShpItr ;


    if (algoId == 1) seedShpItr = barrelClShpHandle->find (mySC->seed ()) ;
    
    else seedShpItr = endcapClShpHandle->find (mySC->seed ()) ;

    return seedShpItr->val->eMaxId () ;

} 


//--------------------------------------------------


//!DS preparing EB map
void TBInvMatrixLooper::fillEBMap (EBDetId EBmax,
                const EBUncalibratedRecHitCollection * barrelHitsCollection,
                std::map<int,double> & EBxtlMap,
                int EBNumberOfRegion, double & pSubtract )
{
   for (unsigned int icry = 0 ; icry< m_etaWidth * m_phiWidthEB ; ++icry)
    {

     unsigned int row = icry / m_etaWidth ;
     unsigned int column= icry % m_etaWidth ;
     int curr_eta=EBmax.ieta () + column - (m_etaWidth/2) ;
     int curr_phi=EBmax.iphi () + row - (m_phiWidthEB/2) ;

     // in ECAL reference system
     if (curr_eta * EBmax.ieta () <= 0) {if (EBmax.ieta () > 0) curr_eta-- ; else curr_eta++ ; }  // JUMP over 0
     if (curr_phi < 1) curr_phi += 360 ;
     if (curr_phi > 360) curr_phi -= 360 ;

     if (EBDetId::validDetId (curr_eta,curr_phi))
       {
         EBDetId det = EBDetId (curr_eta,curr_phi,EBDetId::ETAPHIMODE) ;
        
         EBUncalibratedRecHitCollection::const_iterator curr_recHit = 
           barrelHitsCollection->find (det) ;
         int local_eta = etaShifter (det.ieta ()) ;
  
         double dummy = 0 ;
         dummy = curr_recHit->amplitude () ;
         dummy *= m_ADCtoGeV ;
         dummy *= m_EBrecalibFactor[local_eta][curr_phi] ;
  
         if (isMyRegion (local_eta, curr_phi, EBNumberOfRegion, 0))
           EBxtlMap[m_EBxtlNum[local_eta][curr_phi]] = dummy ;
         else pSubtract += dummy ;
       }//DS if closing
    }//DS for closing
}
