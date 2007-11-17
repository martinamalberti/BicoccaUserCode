// system include files
#include "Calibration/EcalAlCaRecoProducers/interface/AlCaElectronsTest.h"
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Calibration/EcalAlCaRecoProducers/interface/AlCaElectronsProducer.h"
#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"

//DS momentum
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"

#include "CondFormats/EcalObjects/interface/EcalIntercalibConstants.h"
#include "CondFormats/DataRecord/interface/EcalIntercalibConstantsRcd.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "CalibCalorimetry/CaloMiscalibTools/interface/EcalRecHitRecalib.h"



#include "TFile.h"
#include <Math/VectorUtil.h>


using namespace reco ;
using namespace edm ;


// ----------------------------------------------------------------


AlCaElectronsTest::AlCaElectronsTest (const edm::ParameterSet& iConfig) :
  m_ElectronLabel (iConfig.getParameter<edm::InputTag> ("electronLabel")),
  m_barrelAlCa (iConfig.getParameter<edm::InputTag> ("alcaBarrelHitCollection")) ,
  m_endcapAlCa (iConfig.getParameter<edm::InputTag> ("alcaEndcapHitCollection")) 
{}


// ----------------------------------------------------------------


void 
AlCaElectronsTest::beginJob ( const edm::EventSetup& iSetup)
{
  edm::Service<TFileService> fs;
  //Maps of th electrons hits
  m_barrelGlobalCrystalsMap = fs->make<TH2F> ("m_barrelGlobalCrystalsMap","m_barrelGlobalCrystalsMap",170,-85,85,360,0,360) ;
  m_endcapGlobalCrystalsMap = fs->make<TH2F> ("m_endcapGlobalCrystalsMap","m_endcapGlobalCrystalsMap",100,0,100,100,0,100) ;
 //Maps of the energy deposit 
  m_barrelGlobalCrystalsEnergyMap = fs->make<TH2F> ("m_barrelGlobalCrystalsEnergy","m_barrelGlobalCrystalsEnergy",170,-85,85,360,0,360) ;
  m_endcapGlobalCrystalsEnergyMap = fs->make<TH2F> ("m_endcapGlobalCrystalsEnergy","m_endcapGlobalCrystalsEnergy",100,0,100,100,0,100) ;
  //Energy around the MOX 
  m_barrelLocalCrystalsEnergy = fs->make<TH2F> ("m_barrelLocalCrystalsEnergy","m_barrelLocalCrystalsEnergy",20,-10,10,20,-10,10) ;
  m_endcapLocalCrystalsEnergy = fs->make<TH2F> ("m_endcapLocalCrystalsEnergy","m_endcapLocalCrystalsEnergy",20,-10,10,20,-10,10) ;
  //Energy on 25
  m_barrelEoE25 = fs->make<TH1F> ("m_barrelEoE25","m_barrelEoE25",200,0,2) ;
  m_endcapEoE25 = fs->make<TH1F> ("m_endcapEoE25","m_endcapEoE25",200,0,2) ;
  //E and P distros
  m_barrelPdistro = fs->make<TH1F> ("pDistro_barrel","pDistro_barrel",300,0,300) ;
  m_endcapPdistro = fs->make<TH1F> ("pDistro_endcap","pDistro_endcap",500,0,500) ;
  m_barrelEdistro = fs->make<TH1F> ("Edistro_barrel","Edistro_barrel",300,0,300) ;
  m_endcapEdistro = fs->make<TH1F> ("Edistro_endcap","Edistro_endcap",500,0,500) ;
  m_barrelE25distro = fs->make<TH1F> ("barrelE25distro","barrelE25distro",300,0,300)  ;
  m_endcapE25distro = fs->make<TH1F> ("endcapE25distro","endcapE25distro",300,0,300) ;
  //EoP
  m_barrelEoPFive = fs->make<TH1F> ("EoP_barrelFive","EoP_barrelFive",500,0,5) ;
  m_endcapEoPFive = fs->make<TH1F> ("EoP_endcapFive","EoP_endcapFive",500,0,5) ;
  m_barrelEoP = fs->make<TH1F> ("EoP_endcap","EoP_endcap",500,0,5) ;
  m_endcapEoP = fs->make<TH1F> ("EoP_endcap","EoP_endcap",500,0,5) ;
  
  m_etaTkDistro = fs->make<TH1F> ("m_etaTkDistro","m_etaTkDistro",100,0,3) ;
  m_phiTkDistro = fs->make<TH1F> ("m_phiTkDistro","m_phiTkDistro",100,-10,10) ;

///LP added to check the correlations
  m_E25oPdiff = fs->make<TH2F> ("E25OverPdiff","E25OverPdiff",110,0,2,100,-0.5,3);
  m_E25oEseedOpout = fs->make<TH2F> ("E25oEseedOpout","E25oEseedOpout",110,0,2,100,0,3);
  m_E25oESCOpin = fs->make <TH2F> ("E25oESCOpin","E25oESCOpin",100,0,2,100,0,3);
  m_EseedVSesc = fs->make <TH2F> ("EseedVSesc","EseedVSesc",200,0,3,200,0,3);

  m_E25vsEta = fs->make<TH2F> ("E25vsEta","E25vsEta",200,-3,3,100,0,200) ;
  m_E25vsEtaIndexEB = fs->make<TH2F> ("E25vsEtaIndexEB","E25vsEtaIndexEB",171,-85,86,100,0,200) ;
  m_E25vsRadIndexEE = fs->make<TH2F> ("E25vsRadIndexEE","E25vsRadIndexEE",50,0,50,100,0,200) ;

  m_EseedOpout = new varHisto ("EseedOpout",500,0,2,fs) ;
  m_ESCOpin = new varHisto ("ESCOpin",500,0,2,fs) ;
  m_piMpoOpi = new varHisto ("piMpoOpi",1000,-2,2,fs) ; 

 return ;
}


// ----------------------------------------------------------------


void 
AlCaElectronsTest::endJob ()
{      
  edm::LogWarning ("loop") << "PIETRO endjob" ;
//   m_EseedOpout->print () ;
//   m_ESCOpin->print () ;
//   m_piMpoOpi->print () ; 
   return ;
}


// ----------------------------------------------------------------


void 
AlCaElectronsTest::analyze (const edm::Event& iEvent, 
                            const edm::EventSetup& iSetup)
{
  double energia5 = 0 ;
  double pTk = 0. ;
  //get the EB rechit collection
  Handle<EBRecHitCollection> barrelRecHitsHandle ;
  const EBRecHitCollection*  barrelHitsCollection = 0 ;
  iEvent.getByLabel (m_barrelAlCa, barrelRecHitsHandle) ;
  barrelHitsCollection = barrelRecHitsHandle.product () ; // get a ptr to the product

  Handle<EERecHitCollection> endcapRecHitsHandle ;
  const EERecHitCollection*  endcapHitsCollection = 0 ; 
  iEvent.getByLabel (m_endcapAlCa,endcapRecHitsHandle) ;
  endcapHitsCollection = endcapRecHitsHandle.product () ; // get a ptr to the product
//  try {
//      iEvent.getByLabel (m_barrelAlCa, barrelRecHitsHandle) ;
//      barrelHitsCollection = barrelRecHitsHandle.product () ; // get a ptr to the product
//      iEvent.getByLabel (m_endcapAlCa,endcapRecHitsHandle) ;
//      endcapHitsCollection = endcapRecHitsHandle.product () ; // get a ptr to the product
//    }
//  catch ( std::exception& ex ) {
//       LogDebug ("") << "EcalREcHitMiscalib: Error! can't get product! caught"<<ex.what () << std::endl ;
//    }
  
  edm::Handle<reco::PixelMatchGsfElectronCollection> pElectrons ;
  iEvent.getByLabel (m_ElectronLabel, pElectrons) ;
//  try {
//       iEvent.getByLabel (m_ElectronLabel, pElectrons) ;
//      }
//  catch (...) {
//       edm::LogError ("reading") << m_ElectronLabel << "NotFound" ;
//       return ;
//     }
  
  //PG loop on the electrons
  for (reco::PixelMatchGsfElectronCollection::const_iterator eleIt = pElectrons->begin () ;
       eleIt != pElectrons->end () ;
       ++eleIt) 
    {
     m_etaTkDistro->Fill (eleIt->eta ()) ;
     m_phiTkDistro->Fill (eleIt->phi ()) ;
  
     pTk = eleIt->trackMomentumAtVtx ().R () ; 

     m_EseedOpout->fill (eleIt->eSeedClusterOverPout (), eleIt->eta (), pTk) ; 
     m_ESCOpin->fill (eleIt->eSuperClusterOverP (), eleIt->eta (), pTk) ;
     m_piMpoOpi->fill ((pTk - eleIt->trackMomentumOut ().R ()) / pTk, eleIt->eta (), pTk) ; 

     //PG look for the max detid in the cluster relative to the electron
     DetId Max = findMaxHit (eleIt->superCluster ()->getHitsByDetId (), 
                             barrelHitsCollection,  endcapHitsCollection) ;
     if (Max.det () == 0) continue ; 

     if ( Max.subdetId () == EcalBarrel  ) //PG in the barrel
       {
         EBDetId EBMax (Max) ;    
         EBRecHitCollection::const_iterator itrechit ;
         energia5 = Energy25Barrel (EBMax.ieta (), EBMax.iphi (), barrelHitsCollection) ;
         itrechit = barrelHitsCollection->find (Max) ;
         m_barrelGlobalCrystalsMap->Fill (
                EBMax.ieta () ,
                EBMax.iphi () 
              ) ;
         m_barrelPdistro->Fill (pTk) ;
         m_barrelEdistro->Fill (itrechit->energy ()) ;
         m_barrelE25distro->Fill (energia5) ;
         m_barrelEoP->Fill (itrechit->energy ()/pTk) ;
         m_barrelEoE25->Fill (itrechit->energy ()/energia5) ;
         m_barrelEoPFive->Fill ( energia5 / pTk) ; 
         m_E25oESCOpin->Fill(energia5/pTk, eleIt->eSuperClusterOverP());
         m_E25oEseedOpout->Fill(energia5/pTk,eleIt->eSeedClusterOverPout());
         m_E25oPdiff->Fill(energia5/pTk,(pTk - eleIt->trackMomentumOut().R())/pTk);
         m_EseedVSesc->Fill(eleIt->eSuperClusterOverP(),eleIt->eSeedClusterOverPout());
         m_E25vsEta->Fill (eleIt->eta (),energia5) ;
         m_E25vsEtaIndexEB->Fill (EBMax.ieta (),energia5)  ;
      
         fillAroundBarrel (
             barrelHitsCollection, 
             EBMax.ieta (), 
             EBMax.iphi (),
             pTk 
            ) ;
       } //PG in the barrel
     
     else //PG in the endcap
       {      
         EEDetId EEMax (Max) ;  
         EERecHitCollection::const_iterator itrechit ;
         itrechit = endcapHitsCollection->find (Max) ;
         energia5 = Energy25Endcap (EEMax.ix (), EEMax.iy (), 1, endcapHitsCollection) ;
         m_endcapGlobalCrystalsMap->Fill (
                EEMax.ix () ,
                EEMax.iy () 
              ) ;
         m_endcapPdistro->Fill (pTk) ;
         m_endcapEdistro->Fill (itrechit->energy ()) ;
         m_endcapE25distro->Fill (energia5) ;
         m_endcapEoP->Fill (itrechit->energy ()/pTk) ;
         m_endcapEoPFive->Fill ( energia5 / pTk) ; 
         m_endcapEoE25->Fill (itrechit->energy ()/energia5) ;
	 
         m_E25oESCOpin->Fill(energia5/pTk, eleIt->eSuperClusterOverP());
         m_E25oEseedOpout->Fill(energia5/pTk,eleIt->eSeedClusterOverPout());
         m_E25oPdiff->Fill(energia5/pTk,(pTk - eleIt->trackMomentumOut().R())/pTk);
         m_EseedVSesc->Fill(eleIt->eSuperClusterOverP(),eleIt->eSeedClusterOverPout());
         m_E25vsEta->Fill (eleIt->eta (),energia5) ;
         int rad = static_cast<int> (sqrt ((EEMax.ix () - 50) * (EEMax.ix () - 50) + 
                                           (EEMax.iy () - 50) * (EEMax.iy ()- 50))) ;
         m_E25vsRadIndexEE->Fill (rad, energia5)  ;

         fillAroundEndcap (
            endcapHitsCollection, 
            EEMax.ix (), 
            EEMax.iy (),
            pTk
            ) ;
       } //PG in the endcap
   } //PG loop over electrons

}


// ----------------------------------------------------------------


//DS New find Max
DetId
AlCaElectronsTest::findMaxHit (const std::vector<DetId> & v1,
                               const EBRecHitCollection* EBhits,
                               const EERecHitCollection* EEhits) 
{

  double currEnergy = 0. ;
  DetId maxHit ;
  for (std::vector<DetId>::const_iterator idsIt = v1.begin () ; 
       idsIt != v1.end () ; ++idsIt)
    {
            
     if (idsIt->subdetId () == EcalBarrel) 
       {              
         EBRecHitCollection::const_iterator itrechit ;
         itrechit = EBhits->find (*idsIt) ;
         if (itrechit == EBhits->end () )
           {
             edm::LogInfo ("reading")
             << "[findMaxHit] rechit not found! " ;
             continue ;
           }
                
         if (itrechit->energy () > currEnergy)
           {
             currEnergy = itrechit->energy () ;
             maxHit= *idsIt ;
           }
       }
     else 
       {     
         EERecHitCollection::const_iterator itrechit ;
         itrechit = EEhits->find (*idsIt) ;
         if (itrechit == EEhits->end () )
           {
             edm::LogInfo ("reading")
             << "[findMaxHit] rechit not found! " ;
             continue ;
           }
              
         if (itrechit->energy () > currEnergy)
           {
            currEnergy=itrechit->energy () ;
            maxHit= *idsIt ;
           }
       }
    }
  return maxHit ;
}


// ----------------------------------------------------------------


void
AlCaElectronsTest::fillAroundBarrel (const EcalRecHitCollection * recHits, int eta, int phi, double momentum)
{

  for (EcalRecHitCollection::const_iterator elem = recHits->begin () ;
       elem != recHits->end () ;
       ++elem)
    {
      EBDetId elementId = elem->id () ; 
      m_barrelGlobalCrystalsEnergyMap->Fill (
        elementId.ieta () ,
        elementId.iphi () ,
        elem->energy ()
      ) ;
      m_barrelLocalCrystalsEnergy->Fill (
        elementId.ieta () - eta ,
        elementId.iphi () - phi ,
        elem->energy ()
      ) ;
    }   
  return ;
}


// ----------------------------------------------------------------


void
AlCaElectronsTest::fillAroundEndcap (const EcalRecHitCollection * recHits, int ics, int ips, double momentum)
{
  for (EcalRecHitCollection::const_iterator elem = recHits->begin () ;
       elem != recHits->end () ;
       ++elem)
    {
      EEDetId elementId = elem->id () ; 
      m_endcapLocalCrystalsEnergy->Fill (
        elementId.ix () - ics ,
        elementId.iy () - ips ,
        elem->energy ()
      ) ;
      m_endcapGlobalCrystalsEnergyMap->Fill (
        elementId.ix () ,
        elementId.iy () ,
        elem->energy ()
      ) ;
    }   
  return ;
}


// ----------------------------------------------------------------


//! MF energy in the 5x5 neighbourhood around MaxId
//! in the barrel
double AlCaElectronsTest::Energy25Barrel (int eta, int phi,
                const EcalRecHitCollection * barrelHitsCollection)
{
  int curr_eta = 0 ;
  int curr_phi = 0 ;
  double dummy = 0. ;
 //PG questo puo' essere in futuro specificato separatamente per eta,phi
     //PG loop on the energy reconstruction window
  for (int ii = 0 ; ii < 5 ; ++ii)
  for (int ij = 0 ; ij < 5 ; ++ij)
    {
      curr_eta = eta - 2 + ii ;
      curr_phi = phi - 2 + ij ;
      if (abs (curr_eta) > 85) continue ;
      if (curr_eta * eta <= 0) 
        {
          if (eta > 0) --curr_eta ; 
          else curr_eta++ ; 
        }  // JUMP over 0
      if (curr_phi < 1) curr_phi += 360 ;
      if (curr_phi > 360) curr_phi -= 360 ;
      if (EBDetId::validDetId (curr_eta,curr_phi))
       {
         EBDetId det = EBDetId (curr_eta,curr_phi,EBDetId::ETAPHIMODE) ;
         EcalRecHitCollection::const_iterator curr_recHit = barrelHitsCollection->find (det) ;
         dummy  = dummy + curr_recHit->energy () ;
        }
    }
 return dummy ;
}


// ----------------------------------------------------------------


//! MF energy in the 5x5 neighbourhood around MaxId
//! in the endcap   
double AlCaElectronsTest::Energy25Endcap (int ics, int ips, int z,
                  const EcalRecHitCollection * endcapHitsCollection)
{
  int curr_x = 0 ;
  int curr_y = 0 ;
  int local_x = 0 ;
  int local_y = 0 ;
  double dummy = 0. ;
  //PG loop on the energy reconstruction window
  for (int ii = 0 ; ii < 5 ; ++ii)
  for (int ij = 0 ; ij < 5 ; ++ij)
   {
    curr_x = ics - 2 + ii ;
    curr_y = ips - 2 + ij ;
    local_x = curr_x - 1 ;
    local_y = curr_y - 1 ;
    if (curr_x>100 || curr_x<0) continue ; //PG prob qs ctrl nn serve visto qllo dopo
    if (curr_y>100 || curr_y<0) continue ; //PG prob qs ctrl nn serve visto qllo dopo
    if (EEDetId::validDetId (curr_x,curr_y,z))
     {
      EEDetId det = EEDetId (curr_x,curr_y,z,EEDetId::XYMODE) ;
      EcalRecHitCollection::const_iterator curr_recHit = endcapHitsCollection->find (det) ;
      dummy  = dummy + curr_recHit->energy () ;
     } 
   } 
  return dummy ;
} 

