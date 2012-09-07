// system include files
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "Validation/AlCaTest/interface/AlCaSuperClustersTest.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
// #include "Calibration/EcalAlCaRecoProducers/interface/AlCaElectronsProducer.h"
#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/PreshowerCluster.h"
#include "DataFormats/EgammaReco/interface/PreshowerClusterFwd.h"

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
using namespace std ;


// ----------------------------------------------------------------


AlCaSuperClustersTest::AlCaSuperClustersTest (const edm::ParameterSet& iConfig) :
  m_nSCColl (10),
  m_EB_SC         (iConfig.getParameter<edm::InputTag> ("EB_SC")),
  m_EB_SC_corr    (iConfig.getParameter<edm::InputTag> ("EB_SC_corr")),
  m_EE_SC         (iConfig.getParameter<edm::InputTag> ("EE_SC")),
  m_EE_SC_ES      (iConfig.getParameter<edm::InputTag> ("EE_SC_ES")),
  m_EE_SC_ES_corr (iConfig.getParameter<edm::InputTag> ("EE_SC_ES_corr")),
  m_ES_SC_X       (iConfig.getParameter<edm::InputTag> ("ES_SC_X")),
  m_ES_SC_Y       (iConfig.getParameter<edm::InputTag> ("ES_SC_Y")),
  m_HF_SC         (iConfig.getParameter<edm::InputTag> ("HF_SC")),
  m_PF_SC         (iConfig.getParameter<edm::InputTag> ("PF_SC")),
  m_merge_SC      (iConfig.getParameter<edm::InputTag> ("merge_SC")),
  m_outputFileName (iConfig.getUntrackedParameter<std::string>
                      ("HistOutFile",std::string ("AlCaSuperClustersTest.root"))) 
{
  vector<double> dummy (2, 0.) ;
  for (int iSCColl = 0 ; iSCColl < m_nSCColl ; ++iSCColl) m_getCollStatus.push_back (dummy) ;
  double min = 0. ;
  double max = 100. ;
  int bins = 100 ;
  m_energies.push_back (new TH1F ("EB_SC_E", "EB_SC_E", bins, min, max)) ; 
  m_energies.push_back (new TH1F ("EB_SC_corr_E", "EB_SC_corr_E", bins, min, max)) ; 
  m_energies.push_back (new TH1F ("EE_SC_E", "EE_SC_E", bins, min, max)) ; 
  m_energies.push_back (new TH1F ("EE_SC_ES_E", "EE_SC_ES_E", bins, min, max)) ; 
  m_energies.push_back (new TH1F ("EE_SC_ES_corr_E", "EE_SC_ES_corr_E", bins, min, max)) ; 
  m_energies.push_back (new TH1F ("ES_SC_E_X", "ES_SC_E_X", bins, min, max)) ; 
  m_energies.push_back (new TH1F ("ES_SC_E_Y", "ES_SC_E_Y", bins, min, max)) ; 
  m_energies.push_back (new TH1F ("ES_HF_E", "ES_HF_E", bins, min, max)) ; 
  m_energies.push_back (new TH1F ("ES_PF_E", "ES_PF_E", bins, min, max)) ; 
  m_energies.push_back (new TH1F ("ES_merge_E", "ES_merge_E", bins, min, max)) ; 
}


// ----------------------------------------------------------------


void 
AlCaSuperClustersTest::beginJob ()
{
 return ;
}


// ----------------------------------------------------------------


void 
AlCaSuperClustersTest::endJob ()
{      
  TH2F readingSummary ("readingSummary", "success = 0", m_nSCColl, 0, m_nSCColl, 2, 0, 2) ;
  for (int iSCColl = 0 ; iSCColl < m_nSCColl ; ++iSCColl) 
    {
      readingSummary.Fill (iSCColl, 0., m_getCollStatus.at (iSCColl).at (0)) ;  
      readingSummary.Fill (iSCColl, 1., m_getCollStatus.at (iSCColl).at (1)) ;  
    }
  readingSummary.SetStats (0) ;  
    
  TFile output (m_outputFileName.c_str (),"recreate") ;
  for (int iSCColl = 0 ; iSCColl < m_nSCColl ; ++iSCColl) m_energies.at (iSCColl)->Write () ;
  readingSummary.Write () ;
  output.Close () ;
  return ;
}


// ----------------------------------------------------------------


void 
AlCaSuperClustersTest::analyze (const edm::Event& iEvent, 
                            const edm::EventSetup& iSetup)
{
  
  // get each collection. save the output code.
  int outCode = 0 ;
  int iSCColl = 0 ;
  Handle<SuperClusterCollection> h_EB_SC ;        
  outCode = getCollection (h_EB_SC, m_EB_SC, iEvent) ;
  ++m_getCollStatus.at (iSCColl++).at (outCode) ;
  if (!outCode) fillHisto (m_energies.at (iSCColl - 1), h_EB_SC) ;  

  Handle<SuperClusterCollection> h_EB_SC_corr ;   
  outCode = getCollection (h_EB_SC_corr, m_EB_SC_corr, iEvent) ;
  ++m_getCollStatus.at (iSCColl++).at (outCode) ;
  if (!outCode) fillHisto (m_energies.at (iSCColl - 1), h_EB_SC_corr) ;  

  Handle<SuperClusterCollection> h_EE_SC ;
  outCode = getCollection (h_EE_SC, m_EE_SC, iEvent) ;        
  ++m_getCollStatus.at (iSCColl++).at (outCode) ;
  if (!outCode) fillHisto (m_energies.at (iSCColl - 1), h_EE_SC) ;  

  Handle<SuperClusterCollection> h_EE_SC_ES ;   
  outCode = getCollection (h_EE_SC_ES, m_EE_SC_ES, iEvent) ;
  ++m_getCollStatus.at (iSCColl++).at (outCode) ;
  if (!outCode) fillHisto (m_energies.at (iSCColl - 1), h_EE_SC_ES) ;  

  Handle<SuperClusterCollection> h_EE_SC_ES_corr ;
  outCode = getCollection (h_EE_SC_ES_corr, m_EE_SC_ES_corr, iEvent) ;
  ++m_getCollStatus.at (iSCColl++).at (outCode) ;
  if (!outCode) fillHisto (m_energies.at (iSCColl - 1), h_EE_SC_ES_corr) ;  

  Handle<PreshowerClusterCollection> h_ES_SC_X ;       
  outCode = getCollection (h_ES_SC_X, m_ES_SC_X, iEvent) ; 
  ++m_getCollStatus.at (iSCColl++).at (outCode) ;
  if (!outCode) fillHisto (m_energies.at (iSCColl - 1), h_ES_SC_X) ;  

  Handle<PreshowerClusterCollection> h_ES_SC_Y ;       
  outCode = getCollection (h_ES_SC_Y, m_ES_SC_Y, iEvent) ; 
  ++m_getCollStatus.at (iSCColl++).at (outCode) ;
  if (!outCode) fillHisto (m_energies.at (iSCColl - 1), h_ES_SC_Y) ;  

  Handle<SuperClusterCollection> h_HF_SC ;        
  outCode = getCollection (h_HF_SC, m_HF_SC, iEvent) ;
  ++m_getCollStatus.at (iSCColl++).at (outCode) ;
  if (!outCode) fillHisto (m_energies.at (iSCColl - 1), h_HF_SC) ;  

  Handle<SuperClusterCollection> h_PF_SC ;  //PG FIXME controlla qs   
  outCode = getCollection (h_PF_SC, m_PF_SC, iEvent) ;   
  ++m_getCollStatus.at (iSCColl++).at (outCode) ;
  if (!outCode) fillHisto (m_energies.at (iSCColl - 1), h_PF_SC) ;  

  Handle<SuperClusterCollection> h_merge_SC ;  
  outCode = getCollection (h_merge_SC, m_merge_SC, iEvent) ;   
  ++m_getCollStatus.at (iSCColl++).at (outCode) ;
  if (!outCode) fillHisto (m_energies.at (iSCColl - 1), h_merge_SC) ;  

}




//PG FIXME ctrl di avere tutti i cristalli <-- questo con un altro analyzer
//PG FIXME ctrl quanti SC fanno parte degli elettroni