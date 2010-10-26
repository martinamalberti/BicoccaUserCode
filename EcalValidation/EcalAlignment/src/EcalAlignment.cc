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
// $Id: EcalAlignment.cc,v 1.1 2010/10/26 07:33:16 amassiro Exp $
//
//

#include "EcalValidation/EcalAlignment/interface/EcalAlignment.h"

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
   
   
  ///==== tags ====
  EleTag_ = iConfig.getParameter<edm::InputTag>("EleTag"); 
  CALOMetTag_ = iConfig.getParameter<edm::InputTag>("CALOMetTag");
  TrackTag_ = iConfig.getParameter<edm::InputTag>("TrackTag");
  
  recHitCollection_EB_ = iConfig.getParameter<edm::InputTag>("recHitCollection_EB");
  recHitCollection_EE_ = iConfig.getParameter<edm::InputTag>("recHitCollection_EE");
  
  std::vector<std::string> empty;
  eleId_names_  = iConfig.getUntrackedParameter< std::vector<std::string> >("eleId_names",empty);



  //==== output ====

  if (eleId_names_.size() != 0) {
   eleId_ = new int [eleId_names_.size()];
   for( std::vector<std::string>::const_iterator iEleId = eleId_names_.begin(); iEleId != eleId_names_.end(); iEleId++ ){ 
    TString nameBranch = Form("%s/I",iEleId->c_str());
     myTree_ -> Branch(iEleId->c_str(),&(eleId_[iEleId-eleId_names_.begin()]),nameBranch.Data());
   } 
  }

  
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
  myTree_ -> Branch("DeltaEtaIn",&DeltaEtaIn_,"DeltaEtaIn/D");
  myTree_ -> Branch("DeltaPhiIn",&DeltaPhiIn_,"DeltaPhiIn/D");
  myTree_ -> Branch("etaSC",&etaSC_,"etaSC/D");
  myTree_ -> Branch("phiSC",&phiSC_,"phiSC/D");
  myTree_ -> Branch("nGoodElectrons",&nGoodElectrons_,"nGoodElectrons/I");
  myTree_ -> Branch("dphiMETEle",&dphiMETEle_,"dphiMETEle/D");    
  myTree_ -> Branch("eleCharge",&eleCharge_,"eleCharge/D");  
  myTree_ -> Branch("eleSwissCross",&eleSwissCross_,"eleSwissCross/D");  
  myTree_ -> Branch("seedSeverityLevel",&seedSeverityLevel_,"seedSeverityLevel/I");

  myTree_ -> Branch("eleTrkIso",&eleTrkIso_,"eleTrkIso/D");  
  myTree_ -> Branch("eleEcalIso",&eleEcalIso_,"eleEcalIso/D");      
  myTree_ -> Branch("eleHcalIsoD1",&eleHcalIsoD1_,"eleHcalIsoD1/D");
  myTree_ -> Branch("eleHcalIsoD2",&eleHcalIsoD2_,"eleHcalIsoD2/D");

  myTree_ -> Branch("iDetEB",&iDetEB_,"iDetEB/I");
  myTree_ -> Branch("iDetEE",&iDetEE_,"iDetEE/I");

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
   pat::Electron electron = electrons.at(i);
   reco::GsfTrackRef eleTrack  = electron.gsfTrack () ; 
   reco::SuperClusterRef scRef = electron.superCluster();

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


   float cphi = (electron.p4().x() * metP.p4().Px() 
     + electron.p4().y() * metP.p4().Py()) 
     / (met_*electron.p4().Pt());

   MT_   = sqrt(2 * ET_ * met_ * (1-cphi));
   
   // cluster shape variables
   E3x3_ = 0;
   E2x2_ = 0;
   
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
   
   if(electron.isEB())
   {
     std::pair<DetId, float> id = EcalClusterTools::getMaximum(scRef->hitsAndFractions(), theBarrelEcalRecHits);
     
     // severity level - SwissCross 
     sev = EcalSeverityLevelAlgo::severityLevel(id.first, *theBarrelEcalRecHits, *(theChannelStatus.product()));
     eleSwissCross_ = EcalSeverityLevelAlgo::swissCross(id.first, *theBarrelEcalRecHits);   

     // flag - OutOfTime
     EcalRecHitCollection::const_iterator it = theBarrelEcalRecHits->find(id.first);
     
     if( it != theBarrelEcalRecHits->end() )
     {
       const EcalRecHit& rh = (*it);
       flag = rh.recoFlag();
     }
   }
 
   else
   {
     std::pair<DetId, float> id = EcalClusterTools::getMaximum(scRef->hitsAndFractions(), theEndcapEcalRecHits);
     
     // severity level - SwissCross 
     sev = EcalSeverityLevelAlgo::severityLevel(id.first, *theEndcapEcalRecHits, *(theChannelStatus.product()));
     eleSwissCross_ = EcalSeverityLevelAlgo::swissCross(id.first, *theEndcapEcalRecHits);   

     // flag - OutOfTime
     EcalRecHitCollection::const_iterator it = theEndcapEcalRecHits->find(id.first);
     
     if( it != theEndcapEcalRecHits->end() )
     {
       const EcalRecHit& rh = (*it);
       flag = rh.recoFlag();
     }
   }
    
   seedSeverityLevel_ = sev;
  
   double seed_energy_temp = -1;
   int iSC;
   int iSM;
   int numRecHit = 0;
   const std::vector<std::pair<DetId,float> > & hits= electron.superCluster()->hitsAndFractions();
   for (std::vector<std::pair<DetId,float> > ::const_iterator rh = hits.begin(); rh!=hits.end(); ++rh){
    if ((*rh).first.subdetId()== EcalBarrel){
     EBRecHitCollection::const_iterator itrechit = theBarrelEcalRecHits->find((*rh).first);
     if (itrechit==theBarrelEcalRecHits->end()) continue;
     EBDetId barrelId (itrechit->id ()); 
     if (itrechit->energy() > seed_energy_temp) {
      seed_energy_temp = itrechit->energy();
      iSC_ = -1000;
      iSM_ = barrelId.ism();      
      iDetEE_  = -1000;
      iDetEB_  = EcalBarrelGeometry::alignmentTransformIndexLocal(barrelId);
     }
    }
    if ((*rh).first.subdetId()== EcalEndcap){
     EERecHitCollection::const_iterator itrechit = theEndcapEcalRecHits->find((*rh).first);
     if (itrechit==theEndcapEcalRecHits->end()) continue;
     EEDetId endcapId (itrechit->id ()); 
     if (itrechit->energy() > seed_energy_temp) {
      seed_energy_temp = itrechit->energy();
      iSC_ = endcapId.isc();
      iSM_ = -1000;
      iDetEE_  = EcalEndcapGeometry::alignmentTransformIndexLocal(endcapId);
      iDetEB_  = -1000;
     }   
    }
   }
   
   dphiMETEle_ = deltaPhi(metP.phi(),electron.p4().phi());  
   
   
   
   ///==== Selections ====
   ///==== if an electron doesn't pass these selections 
   ///==== it's not saved
   
   float EtaCutEB    = 1.5;
   float EtaCutEE    = 1.5;
   float EtaMax      = 3.0;
  
   if ( met_ < 20. ) continue;
   if ( ETSC_ < 20. ) continue;
   if ( dphiMETEle_ < 0.75) continue;   
   
   if ( fabs(eta_)> EtaCutEB && fabs(eta_)< EtaCutEE ) continue;
   if ( fabs(eta_)> EtaMax ) continue;

   if (seedSeverityLevel_ != 0) continue;
   if (eleSwissCross_ > 0.95) continue;
   
   
   if (eleMisHits_ > 0) continue;
   if (fabs(eta_) <= EtaCutEB && ((eleTrkIso_ + eleEcalIso_ + eleHcalIsoD1_ + eleHcalIsoD2_)/pT_ > 0.07)) continue;
   if (fabs(eta_) <= EtaCutEB && (eleTrkIso_/pT_>0.09 || eleEcalIso_/pT_>0.07 || (eleHcalIsoD1_+eleHcalIsoD2_)/pT_>0.09)) continue;
   if (fabs(eta_) <= EtaCutEB && (HoE_>0.040 || SigmaIEtaIEta_>0.01)) continue;
//    if (fabs(eta_) <= EtaCutEB && DeltaPhiIn_ > 0.06) continue;
//    if (fabs(eta_) <= EtaCutEB && DeltaEtaIn_ > 0.004) continue;
   
   if (fabs(eta_) > EtaCutEB && ((eleTrkIso_ + eleEcalIso_ + eleHcalIsoD1_ + eleHcalIsoD2_)/pT_ > 0.06)) continue;
   if (fabs(eta_) > EtaCutEB && (eleTrkIso_/pT_>0.04 || eleEcalIso_/pT_>0.05 || (eleHcalIsoD1_+eleHcalIsoD2_)/pT_>0.025)) continue;
   if (fabs(eta_) > EtaCutEB && (HoE_>0.025 || SigmaIEtaIEta_>0.03)) continue;

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
