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
  ebClusterCollection_ = iConfig.getParameter<edm::InputTag> ("ebClusterCollection");
  ebDigiCollection_    = iConfig.getParameter<edm::InputTag> ("ebDigiCollection");
  L1InputTag_          = iConfig.getParameter<edm::InputTag> ("L1InputTag");
  ak5CaloJets_         = iConfig.getParameter<edm::InputTag> ("ak5CaloJets");

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

  // Algo and Technical L1 bits
  //   edm::ESHandle<L1GtTriggerMenu> menuRcd;
  //   iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
  //   const L1GtTriggerMenu* L1Menu = menuRcd.product();
  
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

  //ak5CaloJets
  edm::Handle<CaloJetCollection> ak5CaloJets;
  iEvent.getByLabel(ak5CaloJets_, ak5CaloJets);
  const CaloJetCollection* theJetCollection = ak5CaloJets.product () ;   

  //Fill Tree
  initializeBranches(tree_, myTreeVariables_);

  myTreeVariables_.lumiId       = iEvent.luminosityBlock();
  myTreeVariables_.BX           = iEvent.bunchCrossing();
  myTreeVariables_.runId        = iEvent.id ().run () ;
  myTreeVariables_.eventId      = iEvent.id ().event () ;
  myTreeVariables_.eventNaiveId = naiveId_ ;

  dumpL1Info(gtRecord, myTreeVariables_) ;
  bool saveTree = dumpBarrelInfo(topology, geometry, theEcalBarrelDigis, theBarrelEcalRecHits, myTreeVariables_) ;
  dumpJetInfo(topology, geometry, theJetCollection, myTreeVariables_) ;

  if (saveTree == true) tree_ -> Fill();

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
      
      
      // ecal activity in R = 0.3
      CaloConeSelector *sel03 = new CaloConeSelector(0.3, geometry , DetId::Ecal, EcalBarrel);
      
      std::auto_ptr<CaloRecHitMetaCollectionV> chosen1r03 = sel03->select(ebid.ieta(), ebid.iphi(), mhits);
      std::auto_ptr<CaloRecHitMetaCollectionV> chosen2r03 = sel03->select(-ebid.ieta(), -ebid.iphi(), mhits);
      
      float myIso03 = 0;
      for (CaloRecHitMetaCollectionV::const_iterator recIt = chosen1r03->begin(); 
	   recIt!= chosen1r03->end () ; ++recIt) {
	if ( recIt->energy() < energyCutForIso_) continue;  //dont fill if below E noise value
	if ( EBDetId(recIt->detid()).ieta() ==  ebid.ieta() && 
	     EBDetId(recIt->detid()).iphi() ==  ebid.iphi() ) continue;
	GlobalPoint mycell = geometry->getPosition(recIt->detid());
	myIso03 +=  recIt->energy()*sin(2*atan(exp(mycell.eta())));
      }
      
      myTreeVariables_.ecalRecHitIso03 [ myTreeVariables_.nEcalRecHits ][0] = myIso03  ;
      
  
      myIso03 = 0;
      for (CaloRecHitMetaCollectionV::const_iterator recIt = chosen2r03->begin(); 
	   recIt!= chosen2r03->end () ; ++recIt) {
	if ( recIt->energy() < energyCutForIso_) continue;  //dont fill if below E noise value	  
	if ( EBDetId(recIt->detid()).ieta() == -ebid.ieta() && 
	     EBDetId(recIt->detid()).iphi() == -ebid.iphi() ) continue;
	GlobalPoint mycell = geometry->getPosition(recIt->detid());
	myIso03 +=  recIt->energy()*sin(2*atan(exp(mycell.eta())));
      }
      
      myTreeVariables_.ecalRecHitIso03 [ myTreeVariables_.nEcalRecHits ][1] = myIso03 ;
  
      
      delete  sel03;
      
      
      
      // ecal activity in R = 0.4
      CaloConeSelector *sel04 = new CaloConeSelector(0.4, geometry , DetId::Ecal, EcalBarrel);
      
      std::auto_ptr<CaloRecHitMetaCollectionV> chosen1r04 = sel04->select(ebid.ieta(), ebid.iphi(), mhits);
      std::auto_ptr<CaloRecHitMetaCollectionV> chosen2r04 = sel04->select(-ebid.ieta(), -ebid.iphi(), mhits);
      
      float myIso04 = 0;
      for (CaloRecHitMetaCollectionV::const_iterator recIt = chosen1r04->begin(); 
	   recIt!= chosen1r04->end () ; ++recIt) {
	if ( recIt->energy() < energyCutForIso_) continue;  //dont fill if below E noise value
	if ( EBDetId(recIt->detid()).ieta() ==  ebid.ieta() && 
	     EBDetId(recIt->detid()).iphi() ==  ebid.iphi() ) continue;
	GlobalPoint mycell = geometry->getPosition(recIt->detid());
	myIso04 +=  recIt->energy()*sin(2*atan(exp(mycell.eta())));
      }
      
      myTreeVariables_.ecalRecHitIso04 [ myTreeVariables_.nEcalRecHits ][0] = myIso04  ;
      
      
      myIso04 = 0;
      for (CaloRecHitMetaCollectionV::const_iterator recIt = chosen2r04->begin(); 
	   recIt!= chosen2r04->end () ; ++recIt) {
	if ( recIt->energy() < energyCutForIso_) continue;  //dont fill if below E noise value	  
	if ( EBDetId(recIt->detid()).ieta() == -ebid.ieta() && 
	     EBDetId(recIt->detid()).iphi() == -ebid.iphi() ) continue;
	GlobalPoint mycell = geometry->getPosition(recIt->detid());
	myIso04 +=  recIt->energy()*sin(2*atan(exp(mycell.eta())));
      }
      
      myTreeVariables_.ecalRecHitIso04 [ myTreeVariables_.nEcalRecHits ][1] = myIso04  ;
      
      delete  sel04;
      
      
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
