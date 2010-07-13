// -*- C++ -*-
//
// Package:   WprimeTree
// Class:     WprimeTree
//
 
#include "WAnalysis/WenuAnalysis/plugins/WenuTree.h"

#include "DataFormats/DetId/interface/DetId.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Flags.h"
#include "DataFormats/PatCandidates/interface/Lepton.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/Candidate/interface/Particle.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "DataFormats/Common/interface/ValueMap.h"

#include "TSystem.h"
#include <memory>
#include <vector>
#include <iostream>
#include <iterator>




#define PI 3.141592654
#define TWOPI 6.283185308

using namespace cms ;
using namespace edm ;
using namespace std ;
using namespace reco;

WenuTree::WenuTree (const edm::ParameterSet& iConfig)
{
 
  recHitCollection_EB_       = iConfig.getParameter<edm::InputTag>("recHitCollection_EB");
  recHitCollection_EE_       = iConfig.getParameter<edm::InputTag>("recHitCollection_EE");

  eleLabel_       = iConfig.getParameter<edm::InputTag>("electronTag");
  calometLabel_   = iConfig.getParameter<edm::InputTag>("calometTag");
  tcmetLabel_     = iConfig.getParameter<edm::InputTag>("tcmetTag");
  pfmetLabel_     = iConfig.getParameter<edm::InputTag>("pfmetTag");
  jetLabel_       = iConfig.getParameter<edm::InputTag>("jetTag");
  muonLabel_      = iConfig.getParameter<edm::InputTag>("muonTag");

  HLTInputTag_    = iConfig.getParameter<edm::InputTag>("HLTInputTag");
  L1InputTag_     = iConfig.getParameter<edm::InputTag>("L1InputTag");

  electronID_     = iConfig.getUntrackedParameter<std::string>("electronID") ;
  btagAlgo_       = iConfig.getUntrackedParameter<std::string>("btagAlgo") ;


  runOnMC_         = iConfig.getParameter<bool>("runOnMC");
  storePDFWeights_ = iConfig.getParameter<bool>("storePDFWeights");
  pdfWeightsLabel_ = iConfig.getParameter<edm::InputTag>("pdfWeightsTag"); // or any other PDF set


  naiveId_ = 0;


}


// -----------------------------------------------------------------------------------------


WenuTree::~WenuTree ()
{
}


// -----------------------------------------------------------------------------------------


void WenuTree::analyze (const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  naiveId_++;

 
  //*********** Algo and Technical L1 bits
  edm::ESHandle<L1GtTriggerMenu> menuRcd;
  iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
   
  edm::Handle<L1GlobalTriggerReadoutRecord> gtRecord;
  iEvent.getByLabel(L1InputTag_, gtRecord);

  //*********** HLT INFO
  Handle<TriggerResults> hltresults;
  iEvent.getByLabel(HLTInputTag_,hltresults);
  
  const edm::TriggerNames & triggerNames = iEvent.triggerNames(*hltresults);

  //*********** CALO TOPOLOGY
  edm::ESHandle<CaloTopology> pTopology;
  iSetup.get<CaloTopologyRecord>().get(pTopology);
  const CaloTopology *topology = pTopology.product();

  //*********** EB REC HITS
  edm::Handle<EcalRecHitCollection> recHitsEB;
  iEvent.getByLabel( recHitCollection_EB_, recHitsEB );
  const EcalRecHitCollection* theBarrelEcalRecHits = recHitsEB.product () ;
  if ( ! recHitsEB.isValid() ) {
    std::cerr << "WenuTree::analyze --> recHitsEB not found" << std::endl; 
  }
  
  //*********** EE REC HITS
  edm::Handle<EcalRecHitCollection> recHitsEE;
  iEvent.getByLabel( recHitCollection_EE_, recHitsEE );
  const EcalRecHitCollection* theEndcapEcalRecHits = recHitsEE.product () ;
  if ( ! recHitsEE.isValid() ) {
    std::cerr << "WenuTree::analyze --> recHitsEE not found" << std::endl; 
  }

  //************* ELECTRONS
  Handle<View<pat::Electron> > electronHandle;
  iEvent.getByLabel(eleLabel_,electronHandle);
  View<pat::Electron> electrons = *electronHandle;





  //************* MET  : all 3 met types
  edm::Handle<edm::View<pat::MET> > calometHandle;
  iEvent.getByLabel(calometLabel_,calometHandle);
  View<pat::MET>  calomets = *calometHandle;

  edm::Handle<edm::View<pat::MET> > tcmetHandle;
  iEvent.getByLabel(tcmetLabel_,tcmetHandle);
  View<pat::MET>  tcmets = *tcmetHandle;
 
  edm::Handle<edm::View<pat::MET> > pfmetHandle;
  iEvent.getByLabel(pfmetLabel_,pfmetHandle);
  View<pat::MET>  pfmets = *pfmetHandle;



  //************* JETS
  Handle<View<pat::Jet> > jetHandle;
  iEvent.getByLabel(jetLabel_,jetHandle);
  View<pat::Jet> jets = *jetHandle;


  //************* MUONS
  Handle<View<pat::Muon> > muonHandle;
  iEvent.getByLabel(muonLabel_,muonHandle);
  View<pat::Muon> muons = *muonHandle;
  
  
  //Fill Tree
  initializeBranches(tree_, myTreeVariables_);

 
//   //********* GEN LEVEL INFO : PDF REWEIGHTING
//   if( storePDFWeights_) {
//     edm::Handle<std::vector<double> > weightHandle;
//     iEvent.getByLabel(pdfWeightTag, weightHandle);
    
    
//     std::vector<double> weights = (*weightHandle);
//     unsigned int nmembers = weights.size();
//     for (unsigned int j=0; j<nmembers; j++) {
//       myTreeVariables_.pdfWeights[j] = weights [j];
//     }

//   }

  myTreeVariables_.lumiId       = iEvent.luminosityBlock();
  myTreeVariables_.BX           = iEvent.bunchCrossing();
  myTreeVariables_.runId        = iEvent.id ().run () ;
  myTreeVariables_.eventId      = iEvent.id ().event () ;
  myTreeVariables_.eventNaiveId = naiveId_ ;


  dumpElectronInfo(  electrons, theBarrelEcalRecHits, theEndcapEcalRecHits, topology, myTreeVariables_) ;
  dumpCALOMetInfo (   calomets, myTreeVariables_) ;
  dumpTCMetInfo   (     tcmets, myTreeVariables_) ;
  dumpPFMetInfo   (     pfmets, myTreeVariables_) ;
  dumpJetInfo     (       jets, myTreeVariables_) ;
  dumpMuonInfo    (      muons, myTreeVariables_) ;
  dumpL1Info      (   gtRecord, myTreeVariables_) ;
  dumpHLTInfo     ( hltresults, triggerNames, myTreeVariables_) ;

  tree_ -> Fill();
}






// -----------------------------------------------------------------------------------------

void WenuTree::endJob ()
{
  cout<< "Analyzed " <<  naiveId_ << " events" <<endl;
}

// ----------------------------------------------------------------------------------------

void WenuTree::beginJob()
{
  //file to save output
  edm::Service<TFileService> fs;
  // Initialize Tree
  tree_ = fs->make<TTree>("WprimeAnalysisTree","WprimeAnalysisTree");
  setBranches (tree_, myTreeVariables_) ;
}






// -----------------------------------------------------------------------------------------

void WenuTree::dumpElectronInfo ( View<pat::Electron> electrons,
				        const EcalRecHitCollection* theBarrelEcalRecHits,
					const EcalRecHitCollection* theEndcapEcalRecHits,
					const CaloTopology* topology,
					WenuTreeContent & myTreeVariables_)
{
  
  // Loop over electrons
  for ( unsigned int i=0; i<electrons.size(); ++i ) {
    
    pat::Electron electron = electrons.at(i);
    
    reco::SuperClusterRef scRef = electron.superCluster();
    double R  = TMath::Sqrt(scRef->x()*scRef->x() + scRef->y()*scRef->y() +scRef->z()*scRef->z());
    double Rt = TMath::Sqrt(scRef->x()*scRef->x() + scRef->y()*scRef->y());

    myTreeVariables_.elePx[ myTreeVariables_.nElectrons ]  = electron.trackMomentumAtVtx().x() ;
    myTreeVariables_.elePy[ myTreeVariables_.nElectrons ]  = electron.trackMomentumAtVtx().y() ;
    myTreeVariables_.elePz[ myTreeVariables_.nElectrons ]  = electron.trackMomentumAtVtx().z() ;
    myTreeVariables_.eleCharge[ myTreeVariables_.nElectrons ] = electron.gsfTrack()->charge();

    myTreeVariables_.eleE [ myTreeVariables_.nElectrons ]  = scRef->energy() ;
    myTreeVariables_.eleEt[ myTreeVariables_.nElectrons ]  = scRef->energy()*(Rt/R) ;
    myTreeVariables_.eleEta[ myTreeVariables_.nElectrons ] = scRef->eta();
    myTreeVariables_.elePhi[ myTreeVariables_.nElectrons ] = scRef->phi() ;
    

    float E1 = 0;
    float E4 = 0;

    if ( electron.isEB() ) {
      E1 = EcalClusterTools::eMax( *scRef , theBarrelEcalRecHits);
      E4 = EcalClusterTools::eTop( *scRef, theBarrelEcalRecHits, topology)+
	   EcalClusterTools::eRight( *scRef, theBarrelEcalRecHits, topology)+
	   EcalClusterTools::eBottom( *scRef, theBarrelEcalRecHits, topology)+
	   EcalClusterTools::eLeft( *scRef, theBarrelEcalRecHits, topology);
    }

    if ( electron.isEE() ) {
      E1 = EcalClusterTools::eMax( *scRef , theEndcapEcalRecHits);
      E4 = EcalClusterTools::eTop( *scRef, theEndcapEcalRecHits, topology)+
	   EcalClusterTools::eRight( *scRef, theEndcapEcalRecHits, topology)+
	   EcalClusterTools::eBottom( *scRef, theEndcapEcalRecHits, topology)+
	   EcalClusterTools::eLeft( *scRef, theEndcapEcalRecHits, topology);
    }

    myTreeVariables_.eleSeedSwissCross[ myTreeVariables_.nElectrons ] = (1. - E4/E1); 


    myTreeVariables_.eleId[ myTreeVariables_.nElectrons ] = electron.electronID(electronID_);


//     cout << " cut based - loose " << electron.electronID("eidRobustLoose") << endl;
//     cout << " cut based - tight " << electron.electronID("eidRobustTight") << endl;
//     cout << " cut based - h.e.  " << electron.electronID("eidRobustHighEnergy") << endl;
//     cout << " cIc based - loose  " << electron.electronID("eidLoose") << endl;
//     cout << " cIc based - tight  " << electron.electronID("eidTight") << endl;

    myTreeVariables_.eleSigmaIEtaIEta[ myTreeVariables_.nElectrons ] =  electron.sigmaIetaIeta();
    myTreeVariables_.eleE1x5[ myTreeVariables_.nElectrons ] = electron.e1x5();
    myTreeVariables_.eleE2x5[ myTreeVariables_.nElectrons ] = electron.e2x5Max();
    myTreeVariables_.eleE5x5[ myTreeVariables_.nElectrons ] = electron.e5x5();
    
    myTreeVariables_.eleTrkIso   [ myTreeVariables_.nElectrons ] = electron.dr03TkSumPt();
    myTreeVariables_.eleEcalIso  [ myTreeVariables_.nElectrons ] = electron.dr03EcalRecHitSumEt();
    myTreeVariables_.eleHcalIsoD1[ myTreeVariables_.nElectrons ] = electron.dr03HcalDepth1TowerSumEt();
    myTreeVariables_.eleHcalIsoD2[ myTreeVariables_.nElectrons ] = electron.dr03HcalDepth2TowerSumEt();


//     // gen level matching -- only for MC
//     if ( electron.genLepton() ){
//       myTreeVariables_.genelePt [ myTreeVariables_.nElectrons ] =  electron.genLepton()->pt();
//       myTreeVariables_.geneleEta[ myTreeVariables_.nElectrons ] =  electron.genLepton()->eta();
//       myTreeVariables_.genelePhi[ myTreeVariables_.nElectrons ] =  electron.genLepton()->phi();
//     }

    ++myTreeVariables_.nElectrons;

  }// end loop over electron candidates
  
  return ;
  
} // dumpElectronInfo  


// -----------------------------------------------------------------------------------------

// dump CALO MET Info
void WenuTree::dumpCALOMetInfo ( View<pat::MET>  mets ,
				   WenuTreeContent & myTreeVariables_)

{

  for ( unsigned int i=0; i<mets.size(); ++i ) {
    pat::MET met = mets.at(i);

    // corrected MET
    myTreeVariables_.caloMet = met.et();
    myTreeVariables_.caloMex = met.px();
    myTreeVariables_.caloMey = met.py();
    myTreeVariables_.caloMetPhi = met.phi();
    myTreeVariables_.caloSumEt  = met.sumEt();

  }

  return ;
  
} // dumpMetInfo 

// -----------------------------------------------------------------------------------------

// dump TC MET Info
void WenuTree::dumpTCMetInfo ( View<pat::MET>  mets ,
				 WenuTreeContent & myTreeVariables_)

{
  
  for ( unsigned int i=0; i<mets.size(); ++i ) {
    pat::MET met = mets.at(i);

    // corrected MET
    myTreeVariables_.tcMet = met.et();
    myTreeVariables_.tcMex = met.px();
    myTreeVariables_.tcMey = met.py();
    myTreeVariables_.tcMetPhi = met.phi();
    myTreeVariables_.tcSumEt  = met.sumEt();
  }

  return ;
  
} // dumpMetInfo 


// -----------------------------------------------------------------------------------------

// dump PF MET Info
void WenuTree::dumpPFMetInfo ( View<pat::MET>  mets ,
				 WenuTreeContent & myTreeVariables_)

{
  
  for ( unsigned int i=0; i<mets.size(); ++i ) {
    pat::MET met = mets.at(i);

    // corrected MET
    myTreeVariables_.pfMet = met.et();
    myTreeVariables_.pfMex = met.px();
    myTreeVariables_.pfMey = met.py();
    myTreeVariables_.pfMetPhi = met.phi();
    myTreeVariables_.pfSumEt  = met.sumEt();

  }

  return ;
  
} // dumpMetInfo 


// -----------------------------------------------------------------------------------------

// dump JET Info
void WenuTree::dumpJetInfo ( View<pat::Jet>  jets ,
			       WenuTreeContent & myTreeVariables_)

{
  for ( unsigned int i=0; i<jets.size(); ++i ) {
    pat::Jet jet = jets.at(i);
    if ( jet.pt()< 5. ) continue; 
    myTreeVariables_.jetPx[ myTreeVariables_.nJets ]    = jet.px();
    myTreeVariables_.jetPy[ myTreeVariables_.nJets ]    = jet.py();
    myTreeVariables_.jetPz[ myTreeVariables_.nJets ]    = jet.pz();
    myTreeVariables_.jetPt[ myTreeVariables_.nJets ]    = jet.pt();
    myTreeVariables_.jetEta[ myTreeVariables_.nJets ]   = jet.eta();
    myTreeVariables_.jetPhi[ myTreeVariables_.nJets ]   = jet.phi();
    //  myTreeVariables_.jetBdisc[ myTreeVariables_.nJets ] = jet.bDiscriminator( btagAlgo_);
  
   
//     if ( jet.genJet() ) {
//       myTreeVariables_.genjetPt [ myTreeVariables_.nJets ] = jet.genJet()->pt();
//       myTreeVariables_.genjetEta[ myTreeVariables_.nJets ] = jet.genJet()->eta();
//       myTreeVariables_.genjetPhi[ myTreeVariables_.nJets ] = jet.genJet()->phi();
//     }

    ++myTreeVariables_.nJets;
  }

  return ;
  
} // dumpJetInfo 

// -----------------------------------------------------------------------------------------

void WenuTree::dumpMuonInfo ( View<pat::Muon>  muons ,
			       WenuTreeContent & myTreeVariables_)

{
  for ( unsigned int i=0; i<muons.size(); ++i ) {
    pat::Muon muon = muons.at(i);
    myTreeVariables_.muonPx[ myTreeVariables_.nMuons ]    = muon.px();
    myTreeVariables_.muonPy[ myTreeVariables_.nMuons ]    = muon.py();
    myTreeVariables_.muonPz[ myTreeVariables_.nMuons ]    = muon.pz();
    myTreeVariables_.muonPt[ myTreeVariables_.nMuons ]    = muon.pt();
    myTreeVariables_.muonEta[ myTreeVariables_.nMuons ]   = muon.eta();
    myTreeVariables_.muonPhi[ myTreeVariables_.nMuons ]   = muon.phi();
      
    ++myTreeVariables_.nMuons;
  }

  return ;
  
} // dumpMuonInfo 


// -----------------------------------------------------------------------------------------

// dump L1Info
void WenuTree::dumpL1Info ( edm::Handle<L1GlobalTriggerReadoutRecord>  gtRecord ,
			    WenuTreeContent & myTreeVariables_)
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


// -----------------------------------------------------------------------------------------

// dump HLTInfo
void WenuTree::dumpHLTInfo ( Handle<TriggerResults>  hltresults ,
				   const edm::TriggerNames & triggerNames,
			           WenuTreeContent & myTreeVariables_)
{
 
  unsigned int trigger_size     = 0;
  unsigned int trigger_position = 0;


  if(hltresults.isValid()) {
    
    trigger_size = hltresults->size();
    
    trigger_position = triggerNames.triggerIndex("HLT_Photon10_L1R");
    if (trigger_position < trigger_size) 
      myTreeVariables_.HLT_Photon10_L1R = (int)hltresults->accept(trigger_position);
  
    trigger_position = triggerNames.triggerIndex("HLT_Photon15_L1R");
    if (trigger_position < trigger_size) 
      myTreeVariables_.HLT_Photon15_L1R = (int)hltresults->accept(trigger_position);
 
    trigger_position = triggerNames.triggerIndex("HLT_Photon20_L1R");
    if (trigger_position < trigger_size) 
      myTreeVariables_.HLT_Photon20_L1R = (int)hltresults->accept(trigger_position);

    trigger_position = triggerNames.triggerIndex("HLT_Ele15_LW_L1R");
    if (trigger_position < trigger_size) 
      myTreeVariables_.HLT_Ele15_LW_L1R = (int)hltresults->accept(trigger_position);

  }
  else {
    myTreeVariables_.HLT_Photon10_L1R = 0;
    myTreeVariables_.HLT_Photon15_L1R = 0;
    myTreeVariables_.HLT_Photon20_L1R = 0;
    myTreeVariables_.HLT_Ele15_LW_L1R = 0;
    edm::LogError("WenuTree ---> ") << "ERROR! INVALID  TriggerResults  " ;
  }

  return ;

}// dumpHLTInfo  


