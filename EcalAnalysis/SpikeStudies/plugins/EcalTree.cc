// -*- C++ -*-
//
// Package:   EcalTree
// Class:     EcalTree
//
 
#include "EcalAnalysis/SpikeStudies/plugins/EcalTree.h"

#include "DataFormats/DetId/interface/DetId.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"

#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"

#include "CondFormats/EcalObjects/interface/EcalIntercalibConstants.h"
#include "CondFormats/DataRecord/interface/EcalIntercalibConstantsRcd.h"

#include "RecoCaloTools/MetaCollections/interface/CaloRecHitMetaCollections.h"
#include "RecoCaloTools/Selectors/interface/CaloConeSelector.h"

#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"

#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETFwd.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METFwd.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETFwd.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronCore.h"

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

#include "TSystem.h"
#include <memory>
#include <vector>
#include <iostream>
#include <iterator>

using namespace cms ;
using namespace edm ;
using namespace std ;
using namespace reco;

EcalTree::EcalTree (const edm::ParameterSet& iConfig)
{
  ebRecHitCollection_  = iConfig.getParameter<edm::InputTag> ("ebRecHitCollection");
  //ebClusterCollection_ = iConfig.getParameter<edm::InputTag> ("ebClusterCollection");
  ebDigiCollection_    = iConfig.getParameter<edm::InputTag> ("ebDigiCollection");
  eeRecHitCollection_  = iConfig.getParameter<edm::InputTag> ("eeRecHitCollection");
  //eeClusterCollection_ = iConfig.getParameter<edm::InputTag> ("eeClusterCollection");
  eeDigiCollection_    = iConfig.getParameter<edm::InputTag> ("eeDigiCollection");
  GsfEleTag_           = iConfig.getParameter<edm::InputTag>("GsfEleTag");
  L1InputTag_          = iConfig.getParameter<edm::InputTag> ("L1InputTag");
  ak5CaloJets_         = iConfig.getParameter<edm::InputTag> ("ak5CaloJets");
  MetTag_              = iConfig.getParameter<edm::InputTag>("MetTag");
  PFMetTag_            = iConfig.getParameter<edm::InputTag>("PFMetTag");
  TcMetTag_            = iConfig.getParameter<edm::InputTag>("TcMetTag");

  //radiusForIso_        = iConfig.getParameter<double> ("radiusForIso");
  energyCutForIso_     = iConfig.getUntrackedParameter<double> ("energyCutForIso",0.);
  minRecHitEnergy_     = iConfig.getUntrackedParameter<double> ("minRecHitEnergy",-1000.);
  
  naiveId_ = 0;

}


// -----------------------------------------------------------------------------------------


EcalTree::~EcalTree ()
{
}


// -----------------------------------------------------------------------------------------


void EcalTree::analyze (const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  naiveId_++;

  // calo topology
  edm::ESHandle<CaloTopology> pTopology;
  iSetup.get<CaloTopologyRecord>().get(pTopology);
  const CaloTopology *topology = pTopology.product();

  // calo geometry
  edm::ESHandle<CaloGeometry> pGeometry;
  iSetup.get<CaloGeometryRecord>().get(pGeometry);
  const CaloGeometry *geometry = pGeometry.product();

  edm::Handle<L1GlobalTriggerReadoutRecord> gtRecord;
  iEvent.getByLabel(L1InputTag_, gtRecord);
  
  // Ecal barrel RecHits 
  edm::Handle<EcalRecHitCollection> pBarrelEcalRecHits ;
  iEvent.getByLabel (ebRecHitCollection_, pBarrelEcalRecHits) ;
  const EcalRecHitCollection* theBarrelEcalRecHits = pBarrelEcalRecHits.product () ;   
  if (! (pBarrelEcalRecHits.isValid ()) )
    {
      LogWarning ("AnomalousChannelsAnalyzer") << ebRecHitCollection_ 
					       << " not available" ;
      return ;
    }
  
  //ECAL DIGIS
  edm::Handle<EBDigiCollection> ebDigis;
  iEvent.getByLabel (ebDigiCollection_, ebDigis) ;
  const EBDigiCollection* theEcalBarrelDigis = ebDigis.product () ;  
  if (! (ebDigis.isValid ()) )
    {
      LogWarning ("EcalTree") << ebDigiCollection_
			      << " not available" ;
      return ;
    }

  // Ecal endcap RecHits 
  edm::Handle<EcalRecHitCollection> pEndcapEcalRecHits ;
  iEvent.getByLabel (eeRecHitCollection_, pEndcapEcalRecHits) ;
  const EcalRecHitCollection* theEndcapEcalRecHits = pEndcapEcalRecHits.product () ;   
  if (! (pEndcapEcalRecHits.isValid ()) )
    {
      LogWarning ("AnomalousChannelsAnalyzer") << eeRecHitCollection_ 
					       << " not available" ;
      return ;
    }
  
  //ECAL DIGIS
  edm::Handle<EEDigiCollection> eeDigis;
  iEvent.getByLabel (eeDigiCollection_, eeDigis) ;
  const EEDigiCollection* theEcalEndcapDigis = eeDigis.product () ;  
  if (! (eeDigis.isValid ()) )
    {
      LogWarning ("EcalTree") << eeDigiCollection_
			      << " not available" ;
      return ;
    }


  edm::Handle<reco::GsfElectronCollection> eleHandle;
  iEvent.getByLabel(GsfEleTag_,eleHandle);
  const reco::GsfElectronCollection * electrons =  eleHandle.product();
  

  // //ak5CaloJets
  // edm::Handle<CaloJetCollection> ak5CaloJets;
  // iEvent.getByLabel(ak5CaloJets_, ak5CaloJets);
  // const CaloJetCollection* theJetCollection = ak5CaloJets.product () ;   

  //MET
  edm::Handle<reco::CaloMETCollection> MetHandle ;
  iEvent.getByLabel (MetTag_,MetHandle);

  edm::Handle<reco::METCollection> TcMetHandle ;
  iEvent.getByLabel (TcMetTag_,TcMetHandle);

  edm::Handle<reco::PFMETCollection> PFMetHandle ;
  iEvent.getByLabel (PFMetTag_,PFMetHandle);

  //Fill Tree
  initializeBranches(tree_, myTreeVariables_);


  //------MET------
  const reco::CaloMET* CaloMet = &(MetHandle->front());
  myTreeVariables_.CaloMet = CaloMet->et();
  myTreeVariables_.CaloMex = CaloMet->px();
  myTreeVariables_.CaloMey = CaloMet->py();
  myTreeVariables_.CaloMetPhi = CaloMet->phi();
 
  const reco::MET* TcMet = &(TcMetHandle->front());
  myTreeVariables_.TcMet = TcMet->et();
  myTreeVariables_.TcMex = TcMet->px();
  myTreeVariables_.TcMey = TcMet->py();
  myTreeVariables_.TcMetPhi = TcMet->phi();

  const reco::PFMET* PFMet = &(PFMetHandle->front());
  myTreeVariables_.PFMet = PFMet->et();
  myTreeVariables_.PFMex = PFMet->px();
  myTreeVariables_.PFMey = PFMet->py();
  myTreeVariables_.PFMetPhi = PFMet->phi();
  //------END MET------
  

  myTreeVariables_.lumiId       = iEvent.luminosityBlock();
  myTreeVariables_.BX           = iEvent.bunchCrossing();
  myTreeVariables_.runId        = iEvent.id ().run () ;
  myTreeVariables_.eventId      = iEvent.id ().event () ;
  myTreeVariables_.eventNaiveId = naiveId_ ;

  //dumpL1Info(gtRecord, myTreeVariables_) ;
  bool saveEBTree = dumpBarrelInfo(topology, geometry, theEcalBarrelDigis, theBarrelEcalRecHits, electrons, myTreeVariables_) ;
  bool saveEETree = dumpEndcapInfo(topology, geometry, theEcalEndcapDigis, theEndcapEcalRecHits, electrons, myTreeVariables_) ;
  //dumpJetInfo(topology, geometry, theJetCollection, myTreeVariables_) ;

  if (saveEBTree == true || saveEETree == true) tree_ -> Fill();

  return;
}

// -----------------------------------------------------------------------------------------

void EcalTree::endJob ()
{
  cout<< "Analyzed " <<  naiveId_ << " events" <<endl;

  return;
}

// ----------------------------------------------------------------------------------------

void EcalTree::beginJob()
{
  //file to save output
  edm::Service<TFileService> fs;
  // Initialize Tree
  tree_ = fs->make<TTree>("EcalAnalysisTree","EcalAnalysisTree");
  setBranches (tree_, myTreeVariables_) ;

  return;
}






// -----------------------------------------------------------------------------------------

bool EcalTree::dumpBarrelInfo (	const CaloTopology * topology,
				const CaloGeometry * geometry,
				const EBDigiCollection* theEcalBarrelDigis,
				const EcalRecHitCollection* theBarrelEcalRecHits,
				const GsfElectronCollection* electrons,
				EcalTreeContent & myTreeVariables_)
{
  
  EcalRecHitMetaCollection mhits(*theBarrelEcalRecHits);

  // cerco il BC corrispondente e trovo R9 = E1/E9
  float S1oS9=-9999.;
  float S4oS1=-9999.;

  for (EcalRecHitCollection::const_iterator it = theBarrelEcalRecHits->begin(); it != theBarrelEcalRecHits->end(); ++it ) 
    {

      //only barrel: Emin recHit 
      if (it -> energy() < minRecHitEnergy_) continue;

      myTreeVariables_.ecalRecHitMatrix[ myTreeVariables_.nEcalRecHits ][2][2] = it -> energy();
      myTreeVariables_.ecalRecHitMatrixFlag[ myTreeVariables_.nEcalRecHits ][2][2] = it -> recoFlag();
  
      EBDetId ebid = it -> id();

      //matching with electron seed
      for (reco::GsfElectronCollection::const_iterator eleit = electrons->begin(); eleit != electrons->end(); ++eleit )
	{
	  reco::SuperClusterRef scRef = eleit->superCluster();
	  DetId idEB = EcalClusterTools::getMaximum( scRef->hitsAndFractions(), theBarrelEcalRecHits ).first;
	  if(ebid.rawId() == idEB.rawId()) 
	    myTreeVariables_.ecalRecHitIsEleSeed [ myTreeVariables_.nEcalRecHits ] = 1;
	  else
	    myTreeVariables_.ecalRecHitIsEleSeed [ myTreeVariables_.nEcalRecHits ] = 0;
	}
	
	

      for(int xx = 0; xx < 5; ++xx)
	for(int yy = 0; yy < 5; ++yy)
	    {
	      if(xx == 2 && yy == 2) continue;
	      std::vector<DetId> vector =  EcalClusterTools::matrixDetId(topology, ebid, xx-2, xx-2, yy-2, yy-2);
	      if(vector.size() == 0) continue;
	      EcalRecHitCollection::const_iterator iterator = theBarrelEcalRecHits->find (vector.at(0)) ;
	      if(iterator == theBarrelEcalRecHits->end()) continue;
	      myTreeVariables_.ecalRecHitMatrix[ myTreeVariables_.nEcalRecHits ][xx][yy] = iterator -> energy();
	      myTreeVariables_.ecalRecHitMatrixFlag[ myTreeVariables_.nEcalRecHits ][xx][yy] = iterator -> recoFlag();
	    }
      

      S1oS9 = EcalSeverityLevelAlgo::E1OverE9( ebid, *theBarrelEcalRecHits, 0. );
      S4oS1 = EcalSeverityLevelAlgo::swissCross( ebid, *theBarrelEcalRecHits, 0. );
      
      
      myTreeVariables_.ecalRecHitType     [ myTreeVariables_.nEcalRecHits ] = 0;
      myTreeVariables_.ecalRecHitEnergy   [ myTreeVariables_.nEcalRecHits ] = it -> energy();
      myTreeVariables_.ecalRecHitOutOfTimeEnergy   [ myTreeVariables_.nEcalRecHits ] = it -> outOfTimeEnergy();
      myTreeVariables_.ecalRecHitIEta     [ myTreeVariables_.nEcalRecHits ] = ebid.ieta();
      myTreeVariables_.ecalRecHitIPhi     [ myTreeVariables_.nEcalRecHits ] = ebid.iphi();
      myTreeVariables_.ecalRecHitTime     [ myTreeVariables_.nEcalRecHits ] = it -> time();
      myTreeVariables_.ecalRecHitChi2     [ myTreeVariables_.nEcalRecHits ] = it -> chi2() ;
      myTreeVariables_.ecalRecHitOutOfTimeChi2 [ myTreeVariables_.nEcalRecHits ] = it -> outOfTimeChi2();
      myTreeVariables_.ecalRecHitRawId    [ myTreeVariables_.nEcalRecHits ] = ebid.rawId();
      myTreeVariables_.ecalRecHitRecoFlag [ myTreeVariables_.nEcalRecHits ] = it -> recoFlag();
      myTreeVariables_.ecalRecHitR9       [ myTreeVariables_.nEcalRecHits ] = S1oS9;
      myTreeVariables_.ecalRecHitS4oS1    [ myTreeVariables_.nEcalRecHits ] = S4oS1;
      
      
      // DIGIS
      EBDigiCollection::const_iterator digiItr = theEcalBarrelDigis->begin();
      while(digiItr != theEcalBarrelDigis->end() && ((*digiItr).id() != ebid))
	{
	  ++digiItr;
	}
      EcalDataFrame df = *digiItr;
      for (int i=0; i < df.size(); i++ ) {
	myTreeVariables_.ecalDigis       [ myTreeVariables_.nEcalRecHits ][i] = df.sample(i).adc();
	myTreeVariables_.ecalGainId      [ myTreeVariables_.nEcalRecHits ][i] = df.sample(i).gainId();
      }
      
      ++myTreeVariables_.nEcalRecHits;
      
    }
  
  //save only if something interesting is in the event
  if(myTreeVariables_.nEcalRecHits > 0) return true;
  else return false;
  
}//dumpBarrelInfo
// -----------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------

bool EcalTree::dumpEndcapInfo (	const CaloTopology * topology,
				const CaloGeometry * geometry,
				const EEDigiCollection* theEcalEndcapDigis,
				const EcalRecHitCollection* theEndcapEcalRecHits,
				const GsfElectronCollection* electrons,
				EcalTreeContent & myTreeVariables_)
{
  
  EcalRecHitMetaCollection mhits(*theEndcapEcalRecHits);

  // cerco il BC corrispondente e trovo R9 = E1/E9
  float S1oS9=-9999.;
  float S4oS1=-9999.;

  for (EcalRecHitCollection::const_iterator it = theEndcapEcalRecHits->begin(); it != theEndcapEcalRecHits->end(); ++it ) 
    {

      //only barrel: Emin recHit 
      if (it -> energy() < minRecHitEnergy_) continue;

      myTreeVariables_.ecalRecHitMatrix[ myTreeVariables_.nEcalRecHits ][2][2] = it -> energy();
      myTreeVariables_.ecalRecHitMatrixFlag[ myTreeVariables_.nEcalRecHits ][2][2] = it -> recoFlag();
  
      EEDetId eeid = it -> id();

      //matching with electron seed
      for (reco::GsfElectronCollection::const_iterator eleit = electrons->begin(); eleit != electrons->end(); ++eleit )
	{
	  reco::SuperClusterRef scRef = eleit->superCluster();
	  DetId idEE = EcalClusterTools::getMaximum( scRef->hitsAndFractions(), theEndcapEcalRecHits ).first;
	  if(eeid.rawId() == idEE.rawId()) 
	    myTreeVariables_.ecalRecHitIsEleSeed [ myTreeVariables_.nEcalRecHits ] = 1;
	  else
	    myTreeVariables_.ecalRecHitIsEleSeed [ myTreeVariables_.nEcalRecHits ] = 0;	    
	}

      for(int xx = 0; xx < 5; ++xx)
	for(int yy = 0; yy < 5; ++yy)
	    {
	      if(xx == 2 && yy == 2) continue;
	      std::vector<DetId> vector =  EcalClusterTools::matrixDetId(topology, eeid, xx-2, xx-2, yy-2, yy-2);
	      if(vector.size() == 0) continue;
	      EcalRecHitCollection::const_iterator iterator = theEndcapEcalRecHits->find (vector.at(0)) ;
	      if(iterator == theEndcapEcalRecHits->end()) continue;
	      myTreeVariables_.ecalRecHitMatrix[ myTreeVariables_.nEcalRecHits ][xx][yy] = iterator -> energy();
	      myTreeVariables_.ecalRecHitMatrixFlag[ myTreeVariables_.nEcalRecHits ][xx][yy] = iterator -> recoFlag();
	    }
      

      S1oS9 = EcalSeverityLevelAlgo::E1OverE9( eeid, *theEndcapEcalRecHits, 0. );
      S4oS1 = EcalSeverityLevelAlgo::swissCross( eeid, *theEndcapEcalRecHits, 0. );
      
      
      myTreeVariables_.ecalRecHitType     [ myTreeVariables_.nEcalRecHits ] = eeid.zside();
      myTreeVariables_.ecalRecHitEnergy   [ myTreeVariables_.nEcalRecHits ] = it -> energy();
      myTreeVariables_.ecalRecHitOutOfTimeEnergy   [ myTreeVariables_.nEcalRecHits ] = it -> outOfTimeEnergy();
      myTreeVariables_.ecalRecHitIEta     [ myTreeVariables_.nEcalRecHits ] = eeid.ix();
      myTreeVariables_.ecalRecHitIPhi     [ myTreeVariables_.nEcalRecHits ] = eeid.iy();
      myTreeVariables_.ecalRecHitTime     [ myTreeVariables_.nEcalRecHits ] = it -> time();
      myTreeVariables_.ecalRecHitChi2     [ myTreeVariables_.nEcalRecHits ] = it -> chi2() ;
      myTreeVariables_.ecalRecHitOutOfTimeChi2 [ myTreeVariables_.nEcalRecHits ] = it -> outOfTimeChi2();
      myTreeVariables_.ecalRecHitRawId    [ myTreeVariables_.nEcalRecHits ] = eeid.rawId();
      myTreeVariables_.ecalRecHitRecoFlag [ myTreeVariables_.nEcalRecHits ] = it -> recoFlag();
      myTreeVariables_.ecalRecHitR9       [ myTreeVariables_.nEcalRecHits ] = S1oS9;
      myTreeVariables_.ecalRecHitS4oS1    [ myTreeVariables_.nEcalRecHits ] = S4oS1;
      
      
      // DIGIS
      EBDigiCollection::const_iterator digiItr = theEcalEndcapDigis->begin();
      while(digiItr != theEcalEndcapDigis->end() && ((*digiItr).id() != eeid))
	{
	  ++digiItr;
	}
      EcalDataFrame df = *digiItr;
      for (int i=0; i < df.size(); i++ ) {
	myTreeVariables_.ecalDigis       [ myTreeVariables_.nEcalRecHits ][i] = df.sample(i).adc();
	myTreeVariables_.ecalGainId      [ myTreeVariables_.nEcalRecHits ][i] = df.sample(i).gainId();
      }
      
      ++myTreeVariables_.nEcalRecHits;
      
    }
  
  //save only if something interesting is in the event
  if(myTreeVariables_.nEcalRecHits > 0) return true;
  else return false;
  
}//dumpEndcapInfo
// -----------------------------------------------------------------------------------------


// dump L1Info
void EcalTree::dumpL1Info ( edm::Handle<L1GlobalTriggerReadoutRecord>  gtRecord ,
			    EcalTreeContent & myTreeVariables_)
{
  
  DecisionWord AlgoWord = gtRecord->decisionWord();
  TechnicalTriggerWord TechWord = gtRecord->technicalTriggerWord();
  
  // Loop over the technical bits
  for (unsigned int ibit = 0; ibit < TechWord.size(); ibit++) 
    {
      myTreeVariables_.techL1Bit[ibit] = TechWord[ibit];
    }
  
  // Loop over the algo bits
  for (unsigned int ibit = 0; ibit < AlgoWord.size(); ibit++) 
    {
      myTreeVariables_.algoL1Bit[ibit] = AlgoWord[ibit];
    }
  
  return ;
  
}// dumpL1Info  


//---------------------------------------------------------------------------------------------
//dump Jet Info

void EcalTree::dumpJetInfo (	const CaloTopology * topology,
				const CaloGeometry * geometry,
				const CaloJetCollection* theJetCollection,
				EcalTreeContent & myTreeVariables_)
{
  for (CaloJetCollection::const_iterator jetIt = theJetCollection->begin(); jetIt != theJetCollection->end(); ++jetIt ) 
    {
      myTreeVariables_.jetEta    [ myTreeVariables_.nJets ] = jetIt -> eta();
      myTreeVariables_.jetPhi    [ myTreeVariables_.nJets ] = jetIt -> phi();

      myTreeVariables_.jetEnergy    [ myTreeVariables_.nJets ] = jetIt -> energy();      
      myTreeVariables_.jetEt    [ myTreeVariables_.nJets ] = jetIt -> et();      

      myTreeVariables_.jetPx    [ myTreeVariables_.nJets ] = jetIt -> px();
      myTreeVariables_.jetPy    [ myTreeVariables_.nJets ] = jetIt -> py();
      myTreeVariables_.jetPz    [ myTreeVariables_.nJets ] = jetIt -> pz();
      myTreeVariables_.jetP    [ myTreeVariables_.nJets ] = jetIt -> p();
      myTreeVariables_.jetPt    [ myTreeVariables_.nJets ] = jetIt -> pt();

      myTreeVariables_.jetN90   [ myTreeVariables_.nJets ] = jetIt -> n90();
      myTreeVariables_.jetN60   [ myTreeVariables_.nJets ] = jetIt -> n60();

      myTreeVariables_.jetMaxEInEmTowers [ myTreeVariables_.nJets ] = jetIt -> maxEInEmTowers();
      myTreeVariables_.jetEmEnergyFraction [ myTreeVariables_.nJets ] = jetIt -> emEnergyFraction();
      myTreeVariables_.jetEmEnergyInEB [ myTreeVariables_.nJets ] = jetIt -> emEnergyInEB();
      
      myTreeVariables_.jetCharge    [ myTreeVariables_.nJets ] = jetIt -> charge();
      myTreeVariables_.jetMass    [ myTreeVariables_.nJets ] = jetIt -> mass();
      
      ++myTreeVariables_.nJets;
      
    }    
  return;
}



