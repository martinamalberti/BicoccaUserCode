// -*- C++ -*-
//
// Package:   SimpleNtple
// Class:     SimpleNtple
//
#include "WAnalysis/WenuAnalysis/plugins/SimpleNtple.h"

#define PI 3.141592654
#define TWOPI 6.283185308



SimpleNtple::SimpleNtple(const edm::ParameterSet& iConfig)
{
  edm::Service<TFileService> fs ;
  outTree_  =        fs -> make <TTree>("SimpleNtple","SimpleNtple"); 
  outTreeNameEleId = fs -> make <TTree>("NameEleId","NameEleId");
  outTreeNameEleId->Branch("eleId_names",&eleId_names_);
  
  
  HLTTag_ = iConfig.getParameter<edm::InputTag>("HLTTag");
  L1Tag_ = iConfig.getParameter<edm::InputTag>("L1Tag");
  
  EleTag_ = iConfig.getParameter<edm::InputTag>("EleTag");
  MuTag_ = iConfig.getParameter<edm::InputTag>("MuTag");
  
  CALOMetTag_ = iConfig.getParameter<edm::InputTag>("CALOMetTag");
  TCMetTag_ = iConfig.getParameter<edm::InputTag>("TCMetTag");
  PFMetTag_ = iConfig.getParameter<edm::InputTag>("PFMetTag");
  
  JetTag_ = iConfig.getParameter<edm::InputTag>("JetTag");
  
  TrackTag_ = iConfig.getParameter<edm::InputTag>("TrackTag");
  
  //eleIDCut_LooseInputTag_ = iConfig.getParameter<edm::InputTag> ("eleIDCut_LooseInputTag");
  //eleIDCut_RLooseInputTag_ = iConfig.getParameter<edm::InputTag> ("eleIDCut_RLooseInputTag");
  //eleIDCut_TightInputTag_ = iConfig.getParameter<edm::InputTag> ("eleIDCut_TightInputTag");
  //eleIDCut_RTightInputTag_ = iConfig.getParameter<edm::InputTag> ("eleIDCut_RTightInputTag");
  
//   recHitCollection_EB_ = iConfig.getParameter<edm::InputTag>("recHitCollection_EB");
//   recHitCollection_EE_ = iConfig.getParameter<edm::InputTag>("recHitCollection_EE");
  
  std::vector<std::string> empty;
  eleId_names_  = iConfig.getUntrackedParameter< std::vector<std::string> >("eleId_names",empty);
  outTreeNameEleId->Fill();
  
  
  
  //---- flags ----
  dataFlag_     = iConfig.getUntrackedParameter<bool> ("dataFlag_", false);
  saveL1_       = iConfig.getUntrackedParameter<bool> ("saveL1", true);
  saveHLT_      = iConfig.getUntrackedParameter<bool> ("saveHLT", true);
  saveMu_       = iConfig.getUntrackedParameter<bool> ("saveMu", true);
  saveEle_      = iConfig.getUntrackedParameter<bool> ("saveEle", true);
  saveJet_      = iConfig.getUntrackedParameter<bool> ("saveJet", true);
  saveCALOMet_  = iConfig.getUntrackedParameter<bool> ("saveCALOMet", true);
  saveTCMet_    = iConfig.getUntrackedParameter<bool> ("saveTCMet", true);
  savePFMet_    = iConfig.getUntrackedParameter<bool> ("savePFMet", true);
  
  verbosity_ = iConfig.getUntrackedParameter<bool>("verbosity","False");
  
  
  
  NtupleFactory_ = new NtupleFactory(outTree_);
  
  NtupleFactory_->AddInt("runId"); 
  NtupleFactory_->AddInt("lumiId"); 
  NtupleFactory_->AddInt("BXId"); 
  NtupleFactory_->AddInt("eventId"); 
  NtupleFactory_->AddInt("eventNaiveId"); 
  eventNaiveId_ = 0;
  
  if(saveHLT_)
  {
    NtupleFactory_ -> AddFloat ("HLT_WasRun");
    NtupleFactory_ -> AddFloat ("HLT_Accept");
    NtupleFactory_ -> AddFloat ("HLT_Error");
    NtupleFactory_ -> AddString("HLT_Names");
  }
  
  if(saveL1_)
  {
    NtupleFactory_->AddFloat("L1_techL1Bit"); 
    NtupleFactory_->AddFloat("L1_algoL1Bit"); 
  }
  
  if(saveMu_)
  {
    NtupleFactory_->Add4V("muons");
    
    NtupleFactory_->AddInt("muons_global");
    NtupleFactory_->AddInt("muons_goodMuon");
    NtupleFactory_->AddFloat("muons_charge"); 
    NtupleFactory_->AddFloat("muons_nTkIsoR03"); 
    NtupleFactory_->AddFloat("muons_nTkIsoR05"); 
    NtupleFactory_->AddFloat("muons_tkIsoR03"); 
    NtupleFactory_->AddFloat("muons_tkIsoR05"); 
    NtupleFactory_->AddFloat("muons_emIsoR03"); 
    NtupleFactory_->AddFloat("muons_emIsoR05"); 
    NtupleFactory_->AddFloat("muons_hadIsoR03"); 
    NtupleFactory_->AddFloat("muons_hadIsoR05"); 
  }
  
  if(saveEle_)
  {
    NtupleFactory_->Add4V("electrons");
    NtupleFactory_->AddFloat("electrons_charge"); 
    NtupleFactory_->AddInt("electrons_isEB"); 

    NtupleFactory_->Add3V("electrons_superClusterPosition");
    NtupleFactory_->AddInt("electrons_classification");
    NtupleFactory_->AddInt("electrons_basicClustersSize");

    NtupleFactory_->AddFloat("electrons_track_d0");
    NtupleFactory_->AddFloat("electrons_track_dz");
    NtupleFactory_->AddFloat("electrons_track_d0err");
    NtupleFactory_->AddFloat("electrons_track_dzerr");

    NtupleFactory_->AddFloat("electrons_scE");
    NtupleFactory_->AddFloat("electrons_scEt");
    NtupleFactory_->AddFloat("electrons_scEta");
    NtupleFactory_->AddFloat("electrons_scPhi");
    NtupleFactory_->AddFloat("electrons_SC_phiWidth");
    NtupleFactory_->AddFloat("electrons_SC_etaWidth");

    NtupleFactory_->AddFloat("electrons_eOverP");
    NtupleFactory_->AddFloat("electrons_eSeed");
    NtupleFactory_->AddFloat("electrons_pin");
    NtupleFactory_->AddFloat("electrons_pout");
    NtupleFactory_->AddFloat("electrons_pcalo");
    NtupleFactory_->AddFloat("electrons_fbrem");
    NtupleFactory_->AddFloat("electrons_sigmaIetaIeta");
    NtupleFactory_->AddFloat("electrons_hOverE");
    NtupleFactory_->AddFloat("electrons_deltaPhiIn");
    NtupleFactory_->AddFloat("electrons_deltaEtaIn");

    NtupleFactory_->AddFloat("electrons_deltaEtaSuperClusterAtVtx");
    NtupleFactory_->AddFloat("electrons_deltaEtaSeedClusterAtCalo");
    NtupleFactory_->AddFloat("electrons_deltaEtaEleClusterAtCalo");
    NtupleFactory_->AddFloat("electrons_deltaPhiEleClusterAtCalo");
    NtupleFactory_->AddFloat("electrons_deltaPhiSuperClusterAtVtx");
    NtupleFactory_->AddFloat("electrons_deltaPhiSeedClusterAtCalo");

    //~~~~ see http://cmslxr.fnal.gov/lxr/source/DataFormats/EgammaCandidates/interface/GsfElectron.h#184
    
    for( std::vector<std::string>::const_iterator iEleId = eleId_names_.begin(); iEleId != eleId_names_.end(); iEleId++ )
      NtupleFactory_->AddFloat(*iEleId);
    
    NtupleFactory_->AddFloat("electrons_tkIso03"); 
    NtupleFactory_->AddFloat("electrons_tkIso04"); 
    NtupleFactory_->AddFloat("electrons_emIso03"); 
    NtupleFactory_->AddFloat("electrons_emIso04"); 
    NtupleFactory_->AddFloat("electrons_hadIso03_1"); 
    NtupleFactory_->AddFloat("electrons_hadIso03_2"); 
    NtupleFactory_->AddFloat("electrons_hadIso04_1"); 
    NtupleFactory_->AddFloat("electrons_hadIso04_2"); 
    
    NtupleFactory_->Add3V("electrons_p_atVtx");
    NtupleFactory_->Add3V("electrons_p_atCalo");
    NtupleFactory_->AddFloat("electrons_e1x5");
    NtupleFactory_->AddFloat("electrons_e2x5Max");
    NtupleFactory_->AddFloat("electrons_e5x5");
    
    NtupleFactory_->AddInt("electrons_mishits");
    NtupleFactory_->AddInt("electrons_nAmbiguousGsfTracks");
    NtupleFactory_->AddFloat("electrons_dist");
    NtupleFactory_->AddFloat("electrons_dcot");
    
    NtupleFactory_->AddInt("electrons_seedSeverityLevel");
    NtupleFactory_->AddInt("electrons_seedFlag");

    NtupleFactory_->AddFloat("electrons_SwissCross");

    NtupleFactory_->AddFloat("electrons_ES");
    
  }
  
  if(saveJet_)
  {
    NtupleFactory_->Add4V("jets");
  }
  
  if(saveCALOMet_)
  {
    NtupleFactory_->Add4V("CALOMet");
    NtupleFactory_->AddFloat("CALOSumEt");
  }
  
  if(saveTCMet_)
  {
    NtupleFactory_->Add4V("TCMet");
    NtupleFactory_->AddFloat("TCSumEt");
  }
  
  if(savePFMet_)
  {
    NtupleFactory_->Add4V("PFMet");
    NtupleFactory_->AddFloat("PFSumEt");
  }


}

// --------------------------------------------------------------------



SimpleNtple::~SimpleNtple ()
{
 std::cout << "Analyzed " <<  eventNaiveId_ << " events" << std::endl;
 NtupleFactory_->WriteNtuple();
 delete NtupleFactory_;
}

// -----------------------------------------------------------------------------------------



void SimpleNtple::analyze (const edm::Event& iEvent, const edm::EventSetup& iSetup) {

 ++eventNaiveId_;
 
 NtupleFactory_->FillInt("runId", iEvent.id().run());
 NtupleFactory_->FillInt("lumiId", iEvent.luminosityBlock());
 NtupleFactory_->FillInt("BXId", iEvent.bunchCrossing());
 NtupleFactory_->FillInt("eventId", iEvent.id().event());
 NtupleFactory_->FillInt("eventNaiveId", eventNaiveId_);

 ///---- fill L1 ----
 if (saveL1_) fillL1Info (iEvent, iSetup);

 ///---- fill HLT ----
 if (saveHLT_) fillHLTInfo (iEvent, iSetup);

 ///---- fill electrons ----
 if (saveEle_)  fillEleInfo (iEvent, iSetup);

 ///---- fill muons ----
 if (saveMu_) fillMuInfo (iEvent, iSetup);

 ///---- fill jets ---- 
 if (saveJet_) fillJetInfo (iEvent, iSetup);

 ///---- fill CALOMet ---- 
 if (saveCALOMet_) fillCALOMetInfo (iEvent, iSetup);

 ///---- fill TCMet ---- 
 if (saveTCMet_) fillTCMetInfo (iEvent, iSetup);

 ///---- fill PFMet ---- 
 if (savePFMet_) fillPFMetInfo (iEvent, iSetup);

 ///---- save the entry of the tree ----
 NtupleFactory_->FillNtuple();

}



// -----------------------------------------------------------------------------------------


//---- dump L1Info
void SimpleNtple::fillL1Info (const edm::Event & iEvent, const edm::EventSetup & iSetup)
{
 edm::ESHandle<L1GtTriggerMenu> menuRcd;
 iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;

 edm::Handle<L1GlobalTriggerReadoutRecord> gtRecord;
 iEvent.getByLabel(L1Tag_, gtRecord);

 DecisionWord AlgoWord = gtRecord->decisionWord();
 TechnicalTriggerWord TechWord = gtRecord->technicalTriggerWord();
 
  // Loop over the technical bits
 for (unsigned int ibit = 0; ibit < TechWord.size(); ibit++) 
 {
  NtupleFactory_->FillFloat("L1_techL1Bit",TechWord[ibit]);
 }
  
  // Loop over the algo bits
 for (unsigned int ibit = 0; ibit < AlgoWord.size(); ibit++) 
 {
  NtupleFactory_->FillFloat("L1_algoL1Bit",AlgoWord[ibit]);
 }

 return ;

}

// -----------------------------------------------------------------------------------------



void SimpleNtple::fillHLTInfo (const edm::Event & iEvent, const edm::EventSetup & iSetup)
{
  //std::cout << "SimpleNtple::fillHLTInfo::begin" << std::endl;
  
  
  edm::Handle<edm::TriggerResults> triggerResultsHandle;
  iEvent.getByLabel(HLTTag_, triggerResultsHandle);
  
  const edm::TriggerNames& triggerNames = iEvent.triggerNames(*triggerResultsHandle);
  
  
  
  for(unsigned int iHLT = 0; iHLT < triggerResultsHandle->size(); ++iHLT)
  {
    //std::cout << "bit: " << std::fixed << setw(3)<< iHLT << "   name: " << triggerNames.triggerName(iHLT) << std::endl;
                                                             
    if( triggerResultsHandle -> wasrun(iHLT) )
      NtupleFactory_ -> FillFloat("HLT_WasRun", 1);
    else
      NtupleFactory_ -> FillFloat("HLT_WasRun", 0);
    
    
    if( triggerResultsHandle -> accept(iHLT) )
      NtupleFactory_ -> FillFloat("HLT_Accept", 1);
    else
      NtupleFactory_ -> FillFloat("HLT_Accept", 0);
    
    
    if( triggerResultsHandle -> error(iHLT) )
      NtupleFactory_->FillFloat("HLT_Error", 1);
    else
      NtupleFactory_->FillFloat("HLT_Error", 0);
    
    
    NtupleFactory_ -> FillString("HLT_Names", triggerNames.triggerName(iHLT));
  }
}

// -----------------------------------------------------------------------------------------



void SimpleNtple::fillEleInfo (const edm::Event & iEvent, const edm::EventSetup & iSetup)
{ 
 //*********** CALO TOPOLOGY
//  edm::ESHandle<CaloTopology> pTopology;
//  iSetup.get<CaloTopologyRecord>().get(pTopology);
//  const CaloTopology *topology = pTopology.product();

 //*********** CHANNEL STATUS
//  edm::ESHandle<EcalChannelStatus> theChannelStatus;
//  iSetup.get<EcalChannelStatusRcd>().get(theChannelStatus);

 //*********** EB REC HITS
//  edm::Handle<EcalRecHitCollection> recHitsEB;
//  iEvent.getByLabel( recHitCollection_EB_, recHitsEB );
//  const EcalRecHitCollection* theBarrelEcalRecHits = recHitsEB.product () ;
//  if ( ! recHitsEB.isValid() ) {
//   std::cerr << "SimpleNtple::analyze --> recHitsEB not found" << std::endl; 
//  }
  
 //*********** EE REC HITS
//  edm::Handle<EcalRecHitCollection> recHitsEE;
//  iEvent.getByLabel( recHitCollection_EE_, recHitsEE );
//  const EcalRecHitCollection* theEndcapEcalRecHits = recHitsEE.product () ;
//  if ( ! recHitsEE.isValid() ) {
//   std::cerr << "SimpleNtple::analyze --> recHitsEE not found" << std::endl; 
//  }

  //************* ELECTRONS
 edm::Handle<edm::View<pat::Electron> > electronHandle;
 iEvent.getByLabel(EleTag_,electronHandle);
 edm::View<pat::Electron> electrons = *electronHandle;
 
  //************* TRACKS
// edm::Handle<reco::TrackCollection> trackHandle;
// iEvent.getByLabel(TrackTag_, trackHandle);
 
  //************* BFIELD
 edm::Handle<DcsStatusCollection> dcsHandle;
 iEvent.getByLabel(DCSTag_, dcsHandle);
 double bField;
 
 if (dataFlag_)
 {
   // scale factor = 3.801/18166.0 which are
   // average values taken over a stable two
   // week period
   float currentToBFieldScaleFactor = 2.09237036221512717e-04;
   float current = (*dcsHandle)[0].magnetCurrent();
   bField = current*currentToBFieldScaleFactor;
 }
 else
 {
   edm::ESHandle<MagneticField> magneticField;
   iSetup.get<IdealMagneticFieldRecord>().get(magneticField);
   bField = magneticField->inTesla(GlobalPoint(0.,0.,0.)).z();
 }
 
 
 
 // Loop over electrons
 for ( unsigned int i=0; i<electrons.size(); ++i )
 {
   pat::Electron electron = electrons.at(i);
   
   
   // basic variables
   NtupleFactory_->Fill4V("electrons",electron.p4());
   NtupleFactory_->FillFloat("electrons_charge",(electron.charge()));
   NtupleFactory_->FillInt("electrons_isEB",(electron.isEB()));
   
   
   // track variables
   reco::GsfTrackRef eleTrack  = electron.gsfTrack () ; 
   NtupleFactory_->FillFloat("electrons_track_d0", eleTrack->d0 ());
   NtupleFactory_->FillFloat("electrons_track_dz", eleTrack->dz ());
   NtupleFactory_->FillFloat("electrons_track_d0err", eleTrack->d0Error ());
   NtupleFactory_->FillFloat("electrons_track_dzerr", eleTrack->dzError ());
   
   
   // SC variables
   reco::SuperClusterRef scRef = electron.superCluster();
//   const edm::Ptr<reco::CaloCluster>& seedCluster = scRef->seed();
   
   double R  = TMath::Sqrt(scRef->x()*scRef->x() + scRef->y()*scRef->y() +scRef->z()*scRef->z());
   double Rt = TMath::Sqrt(scRef->x()*scRef->x() + scRef->y()*scRef->y());
   
   NtupleFactory_->FillFloat("electrons_scE",scRef->energy());
   NtupleFactory_->FillFloat("electrons_scEt",scRef->energy()*(Rt/R));
   NtupleFactory_->FillFloat("electrons_scEta",scRef->eta());
   NtupleFactory_->FillFloat("electrons_scPhi",scRef->phi());   
   NtupleFactory_->FillFloat("electrons_SC_phiWidth",scRef->phiWidth());
   NtupleFactory_->FillFloat("electrons_SC_etaWidth",scRef->etaWidth());
   
   NtupleFactory_->Fill3V("electrons_p_atVtx",electron.trackMomentumAtVtx());
   NtupleFactory_->Fill3V("electrons_p_atCalo",electron.trackMomentumAtCalo());

   // eleid variables
   NtupleFactory_->FillFloat("electrons_eOverP",electron.eSuperClusterOverP());
  // NtupleFactory_->FillFloat("electrons_eSeed",electron.superCluster()->seed()->energy());
   NtupleFactory_->FillFloat("electrons_pin",electron.trackMomentumAtVtx().R());
   NtupleFactory_->FillFloat("electrons_pout",electron.trackMomentumOut().R());
   NtupleFactory_->FillFloat("electrons_pcalo",electron.trackMomentumAtCalo().R());
   NtupleFactory_->FillFloat("electrons_fbrem",electron.fbrem());
   NtupleFactory_->FillFloat("electrons_sigmaIetaIeta",electron.sigmaIetaIeta());
   NtupleFactory_->FillFloat("electrons_hOverE",electron.hadronicOverEm());
   NtupleFactory_->FillFloat("electrons_deltaPhiIn",electron.deltaPhiSuperClusterTrackAtVtx());
   NtupleFactory_->FillFloat("electrons_deltaEtaIn",electron.deltaEtaSuperClusterTrackAtVtx());
   
   
   NtupleFactory_->FillFloat("electrons_deltaEtaSuperClusterAtVtx",electron.deltaEtaSuperClusterTrackAtVtx());
   NtupleFactory_->FillFloat("electrons_deltaEtaSeedClusterAtCalo",electron.deltaEtaSeedClusterTrackAtCalo());
   NtupleFactory_->FillFloat("electrons_deltaEtaEleClusterAtCalo",electron.deltaEtaEleClusterTrackAtCalo());
   NtupleFactory_->FillFloat("electrons_deltaPhiEleClusterAtCalo",electron.deltaPhiEleClusterTrackAtCalo());
   NtupleFactory_->FillFloat("electrons_deltaPhiSuperClusterAtVtx",electron.deltaPhiSuperClusterTrackAtVtx());
   NtupleFactory_->FillFloat("electrons_deltaPhiSeedClusterAtCalo",electron.deltaPhiSeedClusterTrackAtCalo());


   for(std::vector<std::string>::const_iterator iEleId = eleId_names_.begin(); iEleId != eleId_names_.end(); iEleId++)
     NtupleFactory_->FillFloat(*iEleId, electron.electronID(*iEleId));
   
   
   // isolation variables
   NtupleFactory_->FillFloat("electrons_tkIso03",(electron.dr03TkSumPt()));
   NtupleFactory_->FillFloat("electrons_tkIso04",(electron.dr04TkSumPt()));
   NtupleFactory_->FillFloat("electrons_emIso03",(electron.dr03EcalRecHitSumEt()));
   NtupleFactory_->FillFloat("electrons_emIso04",(electron.dr04EcalRecHitSumEt()));
   NtupleFactory_->FillFloat("electrons_hadIso03_1",(electron.dr03HcalDepth1TowerSumEt()));
   NtupleFactory_->FillFloat("electrons_hadIso03_2",(electron.dr03HcalDepth2TowerSumEt()));
   NtupleFactory_->FillFloat("electrons_hadIso04_1",(electron.dr04HcalDepth1TowerSumEt()));
   NtupleFactory_->FillFloat("electrons_hadIso04_2",(electron.dr04HcalDepth2TowerSumEt()));
   
   
   // cluster shape variables
   
   NtupleFactory_->FillFloat("electrons_e1x5",electron.e1x5());
   NtupleFactory_->FillFloat("electrons_e2x5Max",electron.e2x5Max());
   NtupleFactory_->FillFloat("electrons_e5x5",electron.e5x5());
   
   
   // conversion rejection variables
//   ConversionFinder convFinder;
//   ConversionInfo convInfo = convFinder.getConversionInfo(electron, trackHandle, bField);
   
   NtupleFactory_->FillInt("electrons_mishits",electron.gsfTrack()->trackerExpectedHitsInner().numberOfHits());
   NtupleFactory_->FillInt("electrons_nAmbiguousGsfTracks",electron.ambiguousGsfTracksSize());
   NtupleFactory_->FillFloat("electrons_dist", electron.convDist());
   NtupleFactory_->FillFloat("electrons_dcot", electron.convDcot());
   
   
   // preshower variables 
   NtupleFactory_->FillFloat("electrons_ES",scRef->preshowerEnergy());
   
   NtupleFactory_->FillInt("electrons_basicClustersSize",electron.basicClustersSize());
//   NtupleFactory_->Fill3V("electrons_superClusterPosition",ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>(electron.superClusterPosition()));
   
   NtupleFactory_->FillInt("electrons_classification",electron.classification());
  
  } // end loop over electron candidates

}

// -----------------------------------------------------------------------------------------



// ---- MET ----
void SimpleNtple::fillCALOMetInfo (const edm::Event & iEvent, const edm::EventSetup & iSetup)
{
 //std::cout << "SimpleNtple::fillCALOMetInfo" << std::endl;
 
 edm::Handle<edm::View<pat::MET> > calometHandle;
 iEvent.getByLabel(CALOMetTag_,calometHandle);
 edm::View<pat::MET>  mets = *calometHandle;
 //pat::MET met = mets.at(mets.size()-1);
 //std::cout << " mets.size()-1 = " << mets.size()-1 << std::endl;
 pat::MET met = mets.at(0);
 NtupleFactory_->Fill4V("CALOMet",met.p4());
 NtupleFactory_->FillFloat("CALOSumEt",met.sumEt());
}

void SimpleNtple::fillTCMetInfo (const edm::Event & iEvent, const edm::EventSetup & iSetup)
{
 //std::cout << "SimpleNtple::fillTCMetInfo" << std::endl;
 
 edm::Handle<edm::View<pat::MET> > tcmetHandle;
 iEvent.getByLabel(TCMetTag_,tcmetHandle);
 edm::View<pat::MET>  mets = *tcmetHandle;
 //pat::MET met = mets.at(mets.size()-1);
 //std::cout << " mets.size()-1 = " << mets.size()-1 << std::endl;
 pat::MET met = mets.at(0);
 NtupleFactory_->Fill4V("TCMet",met.p4());
 NtupleFactory_->FillFloat("TCSumEt",met.sumEt());
}

void SimpleNtple::fillPFMetInfo (const edm::Event & iEvent, const edm::EventSetup & iSetup)
{
 //std::cout << "SimpleNtple::fillPFMetInfo" << std::endl;
 
 edm::Handle<edm::View<pat::MET> > PFmetHandle;
 iEvent.getByLabel(PFMetTag_,PFmetHandle);
 edm::View<pat::MET>  mets = *PFmetHandle;
 //pat::MET met = mets.at(mets.size()-1);
 //std::cout << " mets.size()-1 = " << mets.size()-1 << std::endl;
 pat::MET met = mets.at(0);
 NtupleFactory_->Fill4V("PFMet",met.p4());
 NtupleFactory_->FillFloat("PFSumEt",met.sumEt());
}

// -----------------------------------------------------------------------------------------



void SimpleNtple::fillJetInfo (const edm::Event & iEvent, const edm::EventSetup & iSetup)
{
 //std::cout << "SimpleNtple::fillJetInfo" << std::endl;
 //************* JETS
 edm::Handle<edm::View<pat::Jet> > jetHandle;
 iEvent.getByLabel(JetTag_,jetHandle);
 edm::View<pat::Jet> jets = *jetHandle;
 
 for ( unsigned int i=0; i<jets.size(); ++i ) {
    pat::Jet jet = jets.at(i);
    if ( jet.pt()< 5. ) continue; 
    NtupleFactory_->Fill4V("jets",jet.p4());
 }
} // dumpJetInfo 

// -----------------------------------------------------------------------------------------



///---- muons ----
void SimpleNtple::fillMuInfo (const edm::Event & iEvent, const edm::EventSetup & iSetup)
{
 //std::cout << "SimpleNtple::fillMuInfo" << std::endl;
 //************* MUONS
 
 edm::Handle<edm::View<pat::Muon> > muonHandle;
 iEvent.getByLabel(MuTag_,muonHandle);
 edm::View<pat::Muon> muons = *muonHandle;

 for ( unsigned int i=0; i<muons.size(); ++i ) {
    pat::Muon muon = muons.at(i);

    NtupleFactory_->Fill4V("muons",muon.p4());
    NtupleFactory_->FillInt("muons_global",(muon.isGlobalMuon()));
    NtupleFactory_->FillInt("muons_goodMuon",muon::isGoodMuon(muon, muon::GlobalMuonPromptTight));

    NtupleFactory_->FillFloat("muons_charge",(muon.charge()));
    NtupleFactory_->FillFloat("muons_tkIsoR03",(muon.isolationR03()).sumPt);
    NtupleFactory_->FillFloat("muons_nTkIsoR03",(muon.isolationR03()).nTracks);
    NtupleFactory_->FillFloat("muons_emIsoR03",(muon.isolationR03()).emEt);
    NtupleFactory_->FillFloat("muons_hadIsoR03",(muon.isolationR03()).hadEt);

    NtupleFactory_->FillFloat("muons_tkIsoR05",(muon.isolationR05()).sumPt);
    NtupleFactory_->FillFloat("muons_nTkIsoR05",(muon.isolationR05()).nTracks);
    NtupleFactory_->FillFloat("muons_emIsoR05",(muon.isolationR05()).emEt);
    NtupleFactory_->FillFloat("muons_hadIsoR05",(muon.isolationR05()).hadEt);
  }
} // dumpMuonInfo 

// -----------------------------------------------------------------------------------------
