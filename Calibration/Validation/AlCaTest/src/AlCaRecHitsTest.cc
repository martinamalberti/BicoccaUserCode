// system include files
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "Validation/AlCaTest/interface/AlCaRecHitsTest.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
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
#include "DataFormats/EcalDetId/interface/ESDetId.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"

//DS momentum
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"
#include "DataFormats/EgammaReco/interface/BasicClusterShapeAssociation.h"

#include "CondFormats/EcalObjects/interface/EcalIntercalibConstants.h"
#include "CondFormats/DataRecord/interface/EcalIntercalibConstantsRcd.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "CalibCalorimetry/CaloMiscalibTools/interface/EcalRecHitRecalib.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"

#include <math.h>
#include "TFile.h"
#include <Math/VectorUtil.h>


using namespace reco ;
using namespace edm ;


// ----------------------------------------------------------------


AlCaRecHitsTest::AlCaRecHitsTest (const edm::ParameterSet& iConfig) :
  m_ElectronLabel (iConfig.getParameter<edm::InputTag> ("electronLabel")),
  m_barrelRecHits (iConfig.getParameter<edm::InputTag> ("alcaBarrelRecHits")) ,
  m_endcapRecHits (iConfig.getParameter<edm::InputTag> ("alcaEndcapRecHits")), 
  m_preshowerRecHits (iConfig.getParameter<edm::InputTag> ("alcaPreshowerRecHits")), 
  m_outputFileName (iConfig.getUntrackedParameter<std::string>
                      ("HistOutFile",std::string ("AlCaRecHitsTest.root"))) 
{}


// ----------------------------------------------------------------


void 
AlCaRecHitsTest::beginJob ()
{
  //Maps of th electrons hits
  m_barrelGlobalCrystalsMap = new TH2F ("m_barrelGlobalCrystalsMap","m_barrelGlobalCrystalsMap",170,-85,85,360,0,360) ;
  m_barrelGlobalCrystalsMap ->SetDirectory (0) ;
  m_endcapGlobalCrystalsMap = new TH2F ("m_endcapGlobalCrystalsMap","m_endcapGlobalCrystalsMap",100,0,100,100,0,100) ;
  m_endcapGlobalCrystalsMap ->SetDirectory (0) ;
  m_preshowerGlobalChannelsMap = new TH2F ("m_preshowerGlobalChannelsMap","m_preshowerGlobalChannelsMap",100,0,40,100,0,40) ;
  m_preshowerGlobalChannelsMap ->SetDirectory (0) ;

  //Energy around the MOX 
  m_barrelLocalCrystalsMap = new TH2F ("m_barrelLocalCrystalsMap","m_barrelLocalCrystalsMap",20,-10,10,20,-10,10) ;
  m_barrelLocalCrystalsMap ->SetDirectory (0) ;
  m_endcapLocalCrystalsMap = new TH2F ("m_endcapLocalCrystalsMap","m_endcapLocalCrystalsMap",20,-10,10,20,-10,10) ;
  m_endcapLocalCrystalsMap ->SetDirectory (0) ;
  m_preshowerLocalChannelsMap = new TH2F ("m_preshowerLocalChannelsMap","m_preshowerLocalChannelsMap",20,-10,10,20,-10,10) ;
  m_preshowerLocalChannelsMap ->SetDirectory (0) ;
 return ;
}


// ----------------------------------------------------------------


void 
AlCaRecHitsTest::endJob ()
{      
  TFile output (m_outputFileName.c_str (), "recreate") ;
  m_barrelGlobalCrystalsMap->Write () ; 
  m_endcapGlobalCrystalsMap->Write () ;  
  m_preshowerGlobalChannelsMap->Write () ;  
  m_barrelLocalCrystalsMap->Write () ;
  m_endcapLocalCrystalsMap->Write () ;
  m_preshowerLocalChannelsMap->Write () ;
  output.Close () ;
  return ;
}


// ----------------------------------------------------------------


void 
AlCaRecHitsTest::analyze (const edm::Event& iEvent, 
                            const edm::EventSetup& iSetup)
{
  double pTk = 0. ;
  //get the EB rechit collection
  Handle<EBRecHitCollection> barrelRecHitsHandle ;
  iEvent.getByLabel (m_barrelRecHits, barrelRecHitsHandle) ;
  const EBRecHitCollection * barrelHitsCollection = 0 ;
  barrelHitsCollection = barrelRecHitsHandle.product () ; // get a ptr to the product
  // this is necessary for the hitsAndFractions to work!

  //get the EE rechit collection
  Handle<EERecHitCollection> endcapRecHitsHandle ;
  iEvent.getByLabel (m_endcapRecHits, endcapRecHitsHandle) ;
  const EERecHitCollection * endcapHitsCollection = 0 ; 
  endcapHitsCollection = endcapRecHitsHandle.product () ; // get a ptr to the product
  
  //get the ES rechit collection
  Handle<ESRecHitCollection> preshowerRecHitsHandle ;
  iEvent.getByLabel (m_preshowerRecHits, preshowerRecHitsHandle) ;
  const ESRecHitCollection * preshowerHitsCollection = 0 ; 
  preshowerHitsCollection = preshowerRecHitsHandle.product () ; // get a ptr to the product
  
  edm::Handle<reco::GsfElectronCollection> pElectrons ;
  iEvent.getByLabel (m_ElectronLabel, pElectrons) ;


  //PG fill rechit maps
  // ------------------
  
  for (EcalRecHitCollection::const_iterator iRecHit = barrelRecHitsHandle->begin () ;
     iRecHit != barrelRecHitsHandle->end () ;
     ++iRecHit)
    {
      EBDetId elementId = iRecHit->id () ; 
      m_barrelGlobalCrystalsMap->Fill (elementId.ieta (), elementId.iphi ()) ;
    }   
  
  for (EcalRecHitCollection::const_iterator iRecHit = endcapRecHitsHandle->begin () ;
     iRecHit != endcapRecHitsHandle->end () ;
     ++iRecHit)
    {
      EEDetId elementId = iRecHit->id () ; 
      m_endcapGlobalCrystalsMap->Fill (elementId.ix (), elementId.iy ()) ;
    }   
 
  for (EcalRecHitCollection::const_iterator iRecHit = preshowerRecHitsHandle->begin () ;
     iRecHit != preshowerRecHitsHandle->end () ;
     ++iRecHit)
    {
      ESDetId elementId = iRecHit->id () ; 
      m_preshowerGlobalChannelsMap->Fill (elementId.six (), elementId.siy ()) ;
    }   

  
  //PG loop on the electrons
  int regionIDfailures = 0 ;
  for (reco::GsfElectronCollection::const_iterator eleIt = pElectrons->begin () ;
       eleIt != pElectrons->end () ;
       ++eleIt) 
    {
      //PG barrel
      if (eleIt->isEB ())
        {
//          DetId Max = EcalClusterTools::getMaximum (eleIt->superCluster ()->hitsAndFractions (), barrelHitsCollection).first ;
          DetId Max = findMax (barrelHitsCollection) ;
          EBDetId EBMax (Max) ;

          for (EcalRecHitCollection::const_iterator iRecHit = barrelRecHitsHandle->begin () ;
             iRecHit != barrelRecHitsHandle->end () ;
             ++iRecHit)
            {
              EBDetId elementId = iRecHit->id () ; 
              //PG some of these will be out of range, that's ok
              m_barrelLocalCrystalsMap->Fill (elementId.ieta () - EBMax.ieta (), elementId.iphi () - EBMax.iphi ()) ;
            }
        }
    
      if (eleIt->isEE ())
        {
          DetId Max = findMax (endcapHitsCollection) ;
          EEDetId EEMax (Max) ;
        
          for (EcalRecHitCollection::const_iterator iRecHit = endcapRecHitsHandle->begin () ;
            iRecHit != endcapRecHitsHandle->end () ;
            ++iRecHit)
           {
             EEDetId elementId = iRecHit->id () ; 
             m_endcapLocalCrystalsMap->Fill (elementId.ix () - EEMax.ix (), elementId.iy () - EEMax.iy ()) ;
           }   

          Max = findMax (preshowerHitsCollection) ;
          ESDetId EEMax (Max) ;

          for (EcalRecHitCollection::const_iterator iRecHit = preshowerRecHitsHandle->begin () ;
             iRecHit != preshowerRecHitsHandle->end () ;
             ++iRecHit)
            {
              ESDetId elementId = iRecHit->id () ; 
              m_preshowerLocalChannelsMap->Fill (elementId.six () - ESMax.six (), elementId.siy () - ESMax.siy ()) ;
            }          
        }
    
      const std::vector<std::pair<DetId,float> > & hits = eleIt->superCluster ()->hitsAndFractions () ;
      //PG loop on SC crystals Ids
      for (std::vector<std::pair<DetId,float> >::const_iterator rh = hits.begin () ;
           rh!=hits.end () ; ++rh)
        {
          if ( (*rh).first.subdetId () == EcalBarrel)
            {
              EBRecHitCollection::const_iterator itrechit = barrelHitsCollection->find ( (*rh).first) ;
              if (itrechit == barrelHitsCollection->end ()) continue ;
            }
      
          if ( (*rh).first.subdetId () == EcalEndcap)
            {
              EERecHitCollection::const_iterator itrechit = endcapHitsCollection->find ( (*rh).first) ;
              if (itrechit == endcapHitsCollection->end ()) continue ;
            }
          else
            { 
              ++regionIDfailures ;
//              std::cerr<<"something is wrong about where the hit is\n" ;
//              std::cerr<<"subDetID= "<< (*rh).first.subdetId ()<<"\n" ;
            }
    
        } //PG loop on SC crystals Ids
     //PG look for the max detid in the cluster relative to the electron
     DetId Max = 0 ;
     if ( (fabs (eleIt->eta ())<1.49))
       Max = EcalClusterTools::getMaximum (eleIt->superCluster ()->hitsAndFractions (), barrelHitsCollection).first ;
     else 
       Max = EcalClusterTools::getMaximum (eleIt->superCluster ()->hitsAndFractions (), endcapHitsCollection).first ;
     if (Max.det () == 0) { continue ; }  
//     if ( Max.subdetId () == EcalBarrel  ) //PG in the barrel
//       {
//         EBDetId EBMax (Max) ;    
//         EBRecHitCollection::const_iterator itrechit ;
//         itrechit = barrelHitsCollection->find (Max) ;
////         m_barrelGlobalCrystalsMap->Fill (
////                EBMax.ieta () ,
////                EBMax.iphi () 
////              ) ;
////         fillAroundBarrel (
////             barrelHitsCollection, 
////             EBMax.ieta (), 
////             EBMax.iphi (),
////             pTk 
////            ) ;
//       } //PG in the barrel
//     
//     else //PG in the endcap
//       {      
//         EE=1 ;
//         EEDetId EEMax (Max) ;  
//         EERecHitCollection::const_iterator itrechit ;
//         itrechit = endcapHitsCollection->find (Max) ;
////         m_endcapGlobalCrystalsMap->Fill (
////                EEMax.ix () ,
////                EEMax.iy () 
////              ) ;
////         fillAroundEndcap (
////            endcapHitsCollection, 
////            EEMax.ix (), 
////            EEMax.iy (),
////            pTk
////            ) ;
//       } //PG in the endcap
   } //PG loop over electrons

}


// ----------------------------------------------------------------


void
AlCaRecHitsTest::fillAroundBarrel (const EcalRecHitCollection * recHits, int eta, int phi, double momentum)
{

  for (EcalRecHitCollection::const_iterator elem = recHits->begin () ;
       elem != recHits->end () ;
       ++elem)
    {
      EBDetId elementId = elem->id () ; 
      m_barrelLocalCrystalsMap->Fill (
        elementId.ieta () - eta ,
        elementId.iphi () - phi
      ) ;
    }   
  return ;
}


// ----------------------------------------------------------------


void
AlCaRecHitsTest::fillAroundEndcap (const EcalRecHitCollection * recHits, int ics, int ips, double momentum)
{
  for (EcalRecHitCollection::const_iterator elem = recHits->begin () ;
       elem != recHits->end () ;
       ++elem)
    {
      EEDetId elementId = elem->id () ; 
      m_endcapLocalCrystalsMap->Fill (
        elementId.ix () - ics ,
        elementId.iy () - ips
      ) ;
    }   
  return ;
}


// ----------------------------------------------------------------


DetId 
AlCaRecHitsTest::findMax (const EcalRecHitCollection * recHits)
{
  DetId output ;
  double max = -1. ;
  for (EcalRecHitCollection::const_iterator elem = recHits->begin () ;
       elem != recHits->end () ;
       ++elem)
    {
      double dummy = elem->energy () ;
      if (dummy > max)
        {
          max = dummy ;
          output = elem->id () ;
        }       
    }   
  return output ;
}


