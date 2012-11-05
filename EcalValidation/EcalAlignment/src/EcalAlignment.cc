// -*- C++ -*-
//
// Package:    EcalAlignment
// Class:      EcalAlignment
// 
/**\class EcalAlignment EcalAlignment.cc EcalValidation/EcalAlignment/src/EcalAlignment.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
 */
//
// Original Author:  Andrea Massironi
//         Created:  Mon Oct 25 09:35:13 CEST 2010
// $Id: EcalAlignment.cc,v 1.12 2011/08/04 13:26:01 amassiro Exp $
//
//

#include "EcalValidation/EcalAlignment/interface/EcalAlignment.h"

#include "DataFormats/CaloRecHit/interface/CaloClusterFwd.h"

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
EcalAlignment::EcalAlignment(const edm::ParameterSet& iConfig){
  edm::Service<TFileService> fs ;
  myTree_  =        fs -> make <TTree>("myTree","myTree"); 
  m_totalEvents =      fs -> make<TH1F>("totalEvents", "totalEvents", 1,  0., 1.);
  m_passedEvents =     fs -> make<TH1F>("passedEvents", "passedEvents", 1,  0., 1.);
  m_filterEfficiency = fs -> make<TH1F>("filterEfficiency", "filterEfficiency", 1,  0., 1.);

   
  ///==== tags ====
  EleTag_ = iConfig.getParameter<edm::InputTag>("EleTag"); 
  CALOMetTag_ = iConfig.getParameter<edm::InputTag>("CALOMetTag");
  TrackTag_ = iConfig.getParameter<edm::InputTag>("TrackTag");
  vtxTag_ = iConfig.getParameter<edm::InputTag>("vtxTag");

  isMC_ = iConfig.getUntrackedParameter< bool >("isMC",false);

  recHitCollection_EB_ = iConfig.getParameter<edm::InputTag>("recHitCollection_EB");
  recHitCollection_EE_ = iConfig.getParameter<edm::InputTag>("recHitCollection_EE");
  
  std::vector<std::string> empty;
  eleId_names_  = iConfig.getUntrackedParameter< std::vector<std::string> >("eleId_names",empty);

  debug_  = iConfig.getUntrackedParameter< bool >("debug",false);


  //==== output ====

  if (eleId_names_.size() != 0) {
   eleId_ = new int [eleId_names_.size()];
   for( std::vector<std::string>::const_iterator iEleId = eleId_names_.begin(); iEleId != eleId_names_.end(); iEleId++ ){ 
    TString nameBranch = Form("%s/I",iEleId->c_str());
    myTree_ -> Branch(iEleId->c_str(),&(eleId_[iEleId-eleId_names_.begin()]),nameBranch.Data());
   } 
  }

  myTree_ -> Branch("time",            &time_,           "time/I");

  myTree_ -> Branch("numPUMC",            &numPUMC_,           "numPUMC/I");

  myTree_ -> Branch("BX",            &BX_,           "BX/I");
  myTree_ -> Branch("lumiId",        &lumiId_,       "limuId/I");
  myTree_ -> Branch("runId",         &runId_,        "runId/I");
  myTree_ -> Branch("eventId",       &eventId_,      "eventId/I");
  myTree_ -> Branch("eventNaiveId",  &eventNaiveId_, "eventNaiveId/I" );
  
 
  myTree_ -> Branch("sumEt",&sumEt_,"sumEt/D");
  myTree_ -> Branch("met",&met_,"met/D");
  myTree_ -> Branch("eta",&eta_,"eta/D");
  myTree_ -> Branch("phi",&phi_,"phi/D");
  myTree_ -> Branch("pT",&pT_,"pT/D");
  myTree_ -> Branch("ET",&ET_,"ET/D");
  myTree_ -> Branch("MT",&MT_,"MT/D");
  myTree_ -> Branch("EoP",&EoP_,"EoP/D");
  myTree_ -> Branch("eleFBrem",&eleFBrem_,"eleFBrem/D");
  myTree_ -> Branch("eleES",&eleES_,"eleES/D");
  myTree_ -> Branch("E2x2",&E2x2_,"E2x2/D");
  myTree_ -> Branch("E3x3",&E3x3_,"E3x3/D");
  myTree_ -> Branch("E5x5",&E5x5_,"E5x5/D");
  myTree_ -> Branch("ESC",&ESC_,"ESC/D");
  myTree_ -> Branch("ETSC",&ETSC_,"ETSC/D");
  myTree_ -> Branch("Sigma_Phi",&Sigma_Phi_,"Sigma_Phi/D");
  myTree_ -> Branch("Sigma_Eta",&Sigma_Eta_,"Sigma_Eta/D");
  myTree_ -> Branch("pIn",&pIn_,"pIn/D");
  myTree_ -> Branch("pOut",&pOut_,"pOut/D");
  myTree_ -> Branch("pAtCalo",&pAtCalo_,"pAtCalo/D");
  myTree_ -> Branch("p",&p_,"p/D");
  myTree_ -> Branch("E9oE25",&E9oE25_,"E9oE25/D");
  myTree_ -> Branch("iSM",&iSM_,"iSM/I");
  myTree_ -> Branch("iSC",&iSC_,"iSC/I");

  myTree_ -> Branch("deltaEtaSuperClusterAtVtx",&deltaEtaSuperClusterAtVtx_,"deltaEtaSuperClusterAtVtx/D");
  myTree_ -> Branch("deltaEtaSeedClusterAtCalo",&deltaEtaSeedClusterAtCalo_,"deltaEtaSeedClusterAtCalo/D");
  myTree_ -> Branch("deltaEtaEleClusterAtCalo",&deltaEtaEleClusterAtCalo_,"deltaEtaEleClusterAtCalo/D");
  myTree_ -> Branch("deltaPhiEleClusterAtCalo",&deltaPhiEleClusterAtCalo_,"deltaPhiEleClusterAtCalo/D");
  myTree_ -> Branch("deltaPhiSuperClusterAtVtx",&deltaPhiSuperClusterAtVtx_,"deltaPhiSuperClusterAtVtx/D");
  myTree_ -> Branch("deltaPhiSeedClusterAtCalo",&deltaPhiSeedClusterAtCalo_,"deltaPhiSeedClusterAtCalo/D");

  myTree_ -> Branch("mishits",&mishits_,"mishits/I");
  myTree_ -> Branch("nAmbiguousGsfTracks",&nAmbiguousGsfTracks_,"nAmbiguousGsfTracks/I");
  myTree_ -> Branch("dist",&dist_,"dist/D");
  myTree_ -> Branch("dcot",&dcot_,"dcot/D");

  myTree_ -> Branch("electrons_classification",&electrons_classification_,"electrons_classification/I");
  myTree_ -> Branch("electrons_basicClustersSize",&electrons_basicClustersSize_,"electrons_basicClustersSize/I");

  myTree_ -> Branch("SigmaIEtaIEta",&SigmaIEtaIEta_,"SigmaIEtaIEta/D");
  myTree_ -> Branch("HoE",&HoE_,"HoE/D");
  myTree_ -> Branch("eleTrkIso",&eleTrkIso_,"eleTrkIso/D");
  myTree_ -> Branch("eleEcalIso",&eleEcalIso_,"eleEcalIso/D");
  myTree_ -> Branch("eleHcalIsoD1",&eleHcalIsoD1_,"eleHcalIsoD1/D");
  myTree_ -> Branch("eleHcalIsoD2",&eleHcalIsoD2_,"eleHcalIsoD2/D");

  myTree_ -> Branch("DeltaEtaIn",&DeltaEtaIn_,"DeltaEtaIn/D");
  myTree_ -> Branch("DeltaPhiIn",&DeltaPhiIn_,"DeltaPhiIn/D");
  myTree_ -> Branch("etaSC",&etaSC_,"etaSC/D");
  myTree_ -> Branch("phiSC",&phiSC_,"phiSC/D");
  myTree_ -> Branch("nGoodElectrons",&nGoodElectrons_,"nGoodElectrons/I");
  myTree_ -> Branch("dphiMETEle",&dphiMETEle_,"dphiMETEle/D");    
  myTree_ -> Branch("eleCharge",&eleCharge_,"eleCharge/D");  
  myTree_ -> Branch("eleSwissCross",&eleSwissCross_,"eleSwissCross/D");  
  myTree_ -> Branch("seedSeverityLevel",&seedSeverityLevel_,"seedSeverityLevel/I");

  myTree_ -> Branch("iDetEB",&iDetEB_,"iDetEB/I");
  myTree_ -> Branch("iDetEE",&iDetEE_,"iDetEE/I");

  myTree_ -> Branch("nvtx",&nvtx_,"nvtx/I");
  myTree_ -> Branch("xvtx",&xvtx_,"xvtx/D");
  myTree_ -> Branch("yvtx",&yvtx_,"yvtx/D");
  myTree_ -> Branch("zvtx",&zvtx_,"zvtx/D");

}


EcalAlignment::~EcalAlignment()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
 if (eleId_names_.size() != 0) {
  delete [] eleId_;
 } 
}


//
// member functions
//

// ------------ method called to for each event  ------------
void
  EcalAlignment::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 int nTotalEvents = static_cast<int>(m_totalEvents -> GetBinContent(1));
 int nPassedEvents = static_cast<int>(m_passedEvents -> GetBinContent(1));

 if (debug_) std::cout << ">>>  EcalAlignment::analyze: nPassedEvents " << nPassedEvents << " : " << nTotalEvents << std::endl;

 m_totalEvents -> Fill(0.5);
 m_passedEvents -> Fill(0.5);
 m_filterEfficiency -> SetBinContent(1, 1.*(nPassedEvents+1)/(nTotalEvents+1));

 ///==== save envent INFO ====
 eventNaiveId_++;

 BX_ = iEvent.bunchCrossing();
 lumiId_ = iEvent.luminosityBlock();
 runId_ = iEvent.id ().run ();
 eventId_ = iEvent.id ().event ();
 time_ = (int)(iEvent.time().value() >> 32);

 numPUMC_ = -1;

 ///==== only if MC ====
 if (isMC_) {
  edm::Handle<std::vector<PileupSummaryInfo> > PupInfo;
  iEvent.getByLabel("addPileupInfo", PupInfo);
  std::vector<PileupSummaryInfo>::const_iterator PVI;
  for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {
 // in-time pileup
   if( PVI->getBunchCrossing() == 0 ){
    numPUMC_ = PVI->getPU_NumInteractions();
   }
  }
 }
 ///====






 ///==== save VTX ====
 edm::Handle<reco::VertexCollection> vtxH;
 iEvent.getByLabel(vtxTag_,vtxH);

 nvtx_ = vtxH -> size();
 reco::Vertex PV;
 if(nvtx_!=0){
  xvtx_ = vtxH->at(0).x();
  yvtx_ = vtxH->at(0).y();
  zvtx_ = vtxH->at(0).z();
 }
 else {
  xvtx_ = -999.;
  yvtx_ = -999.;
  zvtx_ = -999.;
 }




 ///==== save MET ====
 edm::Handle<edm::View<pat::MET> > calometHandle;
 iEvent.getByLabel(CALOMetTag_,calometHandle);
 edm::View<pat::MET>  mets = *calometHandle;
 pat::MET metP = mets.at(0);
 sumEt_ = metP.sumEt();
 met_   = metP.p4().Et();
 
 ///==== save ELECTRON ====
 
 //*********** CALO TOPOLOGY
 edm::ESHandle<CaloTopology> pTopology;
 iSetup.get<CaloTopologyRecord>().get(pTopology);
 const CaloTopology *topology = pTopology.product();

 //*********** CHANNEL STATUS
 edm::ESHandle<EcalChannelStatus> theChannelStatus;
 iSetup.get<EcalChannelStatusRcd>().get(theChannelStatus);

 //*********** EB REC HITS
 edm::Handle<EcalRecHitCollection> recHitsEB;
 iEvent.getByLabel( recHitCollection_EB_, recHitsEB );
 const EcalRecHitCollection* theBarrelEcalRecHits = recHitsEB.product () ;
 if ( ! recHitsEB.isValid() ) {
  std::cerr << "SimpleNtple::analyze --> recHitsEB not found" << std::endl; 
 }
  
 //*********** EE REC HITS
 edm::Handle<EcalRecHitCollection> recHitsEE;
 iEvent.getByLabel( recHitCollection_EE_, recHitsEE );
 const EcalRecHitCollection* theEndcapEcalRecHits = recHitsEE.product () ;
 if ( ! recHitsEE.isValid() ) {
  std::cerr << "SimpleNtple::analyze --> recHitsEE not found" << std::endl; 
 }

 //************* ELECTRONS
 edm::Handle<edm::View<pat::Electron> > electronHandle;
 iEvent.getByLabel(EleTag_,electronHandle);
 edm::View<pat::Electron> electrons = *electronHandle;
 


 // Loop over electrons
 for ( unsigned int i=0; i<electrons.size(); ++i ){
  if (debug_) std::cout << ">>> >>> electron " << i << " : " << electrons.size() << std::endl;
  pat::Electron electron = electrons.at(i);
  if (debug_) std::cout << ">>> >>> electron get track" << std::endl;
  reco::GsfTrackRef eleTrack  = electron.gsfTrack () ; 
  if (debug_) std::cout << ">>> >>> electron get SC" << std::endl;
  reco::SuperClusterRef scRef = electron.superCluster();

  electrons_classification_ = electron.classification();
  electrons_basicClustersSize_ = electron.basicClustersSize();

  eta_   = electron.p4().eta();
  phi_   = electron.p4().phi();
  p_     = electron.trackMomentumAtVtx().R();
  pT_    = TMath::Sqrt(electron.trackMomentumAtVtx().Perp2());
  eleCharge_ = electron.charge();

  double R  = TMath::Sqrt(scRef->x()*scRef->x() + scRef->y()*scRef->y() +scRef->z()*scRef->z());
  double Rt = TMath::Sqrt(scRef->x()*scRef->x() + scRef->y()*scRef->y());

  ETSC_ = scRef->energy() * (Rt/R);   
  ESC_ = scRef->energy();   
  ET_ = electron.p4().Et();
  etaSC_ = scRef->eta();
  phiSC_ = scRef->phi();   
   
  Sigma_Phi_ = scRef->phiWidth();
  Sigma_Eta_ = scRef->etaWidth();
   
  pIn_ = electron.trackMomentumAtVtx().R();
  pOut_ = electron.trackMomentumOut().R();   
  pAtCalo_ = electron.trackMomentumAtCalo().R();   
   
  HoE_ = electron.hadronicOverEm();
   
  SigmaIEtaIEta_ = electron.sigmaIetaIeta();
  eleTrkIso_ = electron.dr03TkSumPt();
  eleEcalIso_ = electron.dr03EcalRecHitSumEt();
  eleHcalIsoD1_ = electron.dr03HcalDepth1TowerSumEt();
  eleHcalIsoD2_ = electron.dr03HcalDepth2TowerSumEt();
       
  EoP_ = ESC_ /p_;
  eleFBrem_ = electron.fbrem();

  DeltaEtaIn_ = electron.deltaEtaSuperClusterTrackAtVtx();
  DeltaPhiIn_ = electron.deltaPhiSuperClusterTrackAtVtx();   

  deltaEtaSuperClusterAtVtx_ = electron.deltaEtaSuperClusterTrackAtVtx();
  deltaEtaSeedClusterAtCalo_ = electron.deltaEtaSeedClusterTrackAtCalo();
  deltaEtaEleClusterAtCalo_ = electron.deltaEtaEleClusterTrackAtCalo();
  deltaPhiEleClusterAtCalo_ = electron.deltaPhiEleClusterTrackAtCalo();
  deltaPhiSuperClusterAtVtx_ = electron.deltaPhiSuperClusterTrackAtVtx();
  deltaPhiSeedClusterAtCalo_ = electron.deltaPhiSeedClusterTrackAtCalo();
  mishits_ = electron.gsfTrack()->trackerExpectedHitsInner().numberOfHits();
  nAmbiguousGsfTracks_ = electron.ambiguousGsfTracksSize();
  dist_ = 0;
  dcot_ = 0;

  float cphi = (electron.p4().x() * metP.p4().Px() 
    + electron.p4().y() * metP.p4().Py()) 
     / (met_*electron.p4().Pt());

  MT_   = sqrt(2 * ET_ * met_ * (1-cphi));
   
   // cluster shape variables
  E3x3_ = 0;
  E2x2_ = 0;
   
  if (debug_) std::cout << ">>> >>> electron EB / EE" << std::endl;

  if ( electron.isEB() )
  {
   E3x3_ = EcalClusterTools::e3x3( *scRef, theBarrelEcalRecHits, topology);
   E2x2_ = EcalClusterTools::e2x2( *scRef, theBarrelEcalRecHits, topology);
  }
   
  if ( electron.isEE() )
  {
   E3x3_ = EcalClusterTools::e3x3( *scRef, theEndcapEcalRecHits, topology);
   E2x2_ = EcalClusterTools::e2x2( *scRef, theEndcapEcalRecHits, topology);
  }

  E5x5_ = electron.e5x5();  
   
  eleMisHits_ = electron.gsfTrack()->trackerExpectedHitsInner().numberOfHits();

   // preshower variables 
  eleES_ = scRef->preshowerEnergy();

  E9oE25_ = E3x3_ / E5x5_;
   
  for(std::vector<std::string>::const_iterator iEleId = eleId_names_.begin(); iEleId != eleId_names_.end(); iEleId++){
   eleId_[iEleId-eleId_names_.begin()] = electron.electronID(*iEleId);
  }
     
   // spike removal variables
  int sev = -1;
  int flag = -1;
   
  sev = -1; //---- ??
  seedSeverityLevel_ = sev;
  double seed_energy_temp = -1;
  int iSC;
  int iSM;
  int numRecHit = 0;
  const std::vector<std::pair<DetId,float> > & hits= electron.superCluster()->hitsAndFractions();

  if (debug_) std::cout << ">>> >>> electron get SC" << std::endl;
//    const edm::Ptr<reco::CaloCluster>& seedCluster = scRef->seed();
  if(electron.isEB())
  {
   std::pair<DetId, float> id = EcalClusterTools::getMaximum(hits, theBarrelEcalRecHits);   
   EcalRecHitCollection::const_iterator it = theBarrelEcalRecHits->find(id.first);    
   eleSwissCross_ = EcalTools::swissCross(id.first,*theBarrelEcalRecHits,0.);
   if( it != theBarrelEcalRecHits->end() )
   {
    EBDetId barrelId (it->id ()); 
    iSC_ = -1000;
    iSM_ = barrelId.ism();      
    iDetEE_  = -1000;
    iDetEB_  = EcalBarrelGeometry::alignmentTransformIndexLocal(barrelId);
   }
  }
  if (electron.isEE()){
   std::pair<DetId, float> id = EcalClusterTools::getMaximum(hits, theEndcapEcalRecHits);   
   EcalRecHitCollection::const_iterator it = theEndcapEcalRecHits->find(id.first);
   eleSwissCross_ = EcalTools::swissCross(id.first,*theEndcapEcalRecHits,0.);
   if( it != theEndcapEcalRecHits->end() )
   {
    EEDetId endcapId (it->id ()); 
    iSC_ = endcapId.isc();
    iSM_ = -1000;
    iDetEE_  = EcalEndcapGeometry::alignmentTransformIndexLocal(endcapId);
    iDetEB_  = -1000;
   }
  }


   
  dphiMETEle_ = deltaPhi(metP.phi(),electron.p4().phi());  
   
   
   
   ///==== Selections ====
   ///==== if an electron doesn't pass these selections 
   ///==== it's not saved
   
  float EtaCutEB    = 1.5;
  float EtaCutEE    = 1.5;
  float EtaMax      = 3.0;
  
//    if ( met_ < 10. ) continue;
  if ( ETSC_ < 20. ) continue;
//    if ( fabs(dphiMETEle_) < 0.75) continue;   
   
  if ( fabs(eta_)> EtaCutEB && fabs(eta_)< EtaCutEE ) continue;
  if ( fabs(eta_)> EtaMax ) continue;

  if ( electrons_classification_ != 0 ) continue;
   
   

//    if (seedSeverityLevel_ != 0) continue;
//    if (eleSwissCross_ > 0.95) continue;
   
   
//    if (eleMisHits_ > 0) continue;
//    if (fabs(eta_) <= EtaCutEB && ((eleTrkIso_ + eleEcalIso_ + eleHcalIsoD1_ + eleHcalIsoD2_)/pT_ > 0.07)) continue;
//    if (fabs(eta_) <= EtaCutEB && (eleTrkIso_/pT_>0.09 || eleEcalIso_/pT_>0.07 || (eleHcalIsoD1_+eleHcalIsoD2_)/pT_>0.09)) continue;
//    if (fabs(eta_) <= EtaCutEB && (HoE_>0.040 || SigmaIEtaIEta_>0.01)) continue;
//    if (fabs(eta_) <= EtaCutEB && DeltaPhiIn_ > 0.06) continue;
//    if (fabs(eta_) <= EtaCutEB && DeltaEtaIn_ > 0.004) continue;
   
//    if (fabs(eta_) > EtaCutEB && ((eleTrkIso_ + eleEcalIso_ + eleHcalIsoD1_ + eleHcalIsoD2_)/pT_ > 0.06)) continue;
//    if (fabs(eta_) > EtaCutEB && (eleTrkIso_/pT_>0.04 || eleEcalIso_/pT_>0.05 || (eleHcalIsoD1_+eleHcalIsoD2_)/pT_>0.025)) continue;
//    if (fabs(eta_) > EtaCutEB && (HoE_>0.025 || SigmaIEtaIEta_>0.03)) continue;

  ///==== save ELECTRON variables ====   
  myTree_->Fill();
  
 }//==== end loop over electrons ====


}


// ------------ method called once each job just before starting event loop  ------------
void 
  EcalAlignment::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
  EcalAlignment::endJob() {
  }

//define this as a plug-in
  DEFINE_FWK_MODULE(EcalAlignment);
