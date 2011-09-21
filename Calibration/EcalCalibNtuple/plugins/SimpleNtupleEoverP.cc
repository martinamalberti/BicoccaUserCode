// -*- C++ -*-
//
// Package:   SimpleNtupleEoverP
// Class:     SimpleNtupleEoverP
//
#include "Calibration/EcalCalibNtuple/plugins/SimpleNtupleEoverP.h"

#include "Math/Vector4D.h"
#include "Math/Vector3D.h"

#define PI 3.141592654
#define TWOPI 6.283185308



SimpleNtupleEoverP::SimpleNtupleEoverP(const edm::ParameterSet& iConfig)
{
  edm::Service<TFileService> fs ;
  outTree_  =        fs -> make <TTree>("SimpleNtupleEoverP","SimpleNtupleEoverP"); 

  recHitCollection_EB_ = iConfig.getParameter<edm::InputTag>("recHitCollection_EB");
  recHitCollection_EE_ = iConfig.getParameter<edm::InputTag>("recHitCollection_EE");
  
  EleTag_ = iConfig.getParameter<edm::InputTag>("EleTag");
    
  CALOMetTag_ = iConfig.getParameter<edm::InputTag>("CALOMetTag");
  
  eventType_ = iConfig.getUntrackedParameter<int>("eventType", 1);
    
  //---- flags ----
  verbosity_ = iConfig.getUntrackedParameter<bool>("verbosity", false);
  
  NtupleFactory_ = new NtupleFactory(outTree_);
  
  NtupleFactory_->AddInt("runId"); 
  NtupleFactory_->AddInt("lumiId"); 
  NtupleFactory_->AddInt("BXId"); 
  NtupleFactory_->AddInt("eventId"); 
  NtupleFactory_->AddInt("eventNaiveId"); 
  NtupleFactory_->AddInt("timeStampLow"); 
  NtupleFactory_->AddInt("timeStampHigh"); 
  NtupleFactory_->AddInt("isZ"); 
  eventNaiveId_ = 0;
  
  // general variables
  NtupleFactory_->AddFloat("ele1_charge"); 
  NtupleFactory_->AddFloat("ele1_eta"); 
  NtupleFactory_->AddFloat("ele1_phi"); 
  
  // track variables
  NtupleFactory_->AddFloat("ele1_p_atVtx");
  
  // supercluster variables
  NtupleFactory_->AddFloat("ele1_scE");
  NtupleFactory_->AddFloat("ele1_scEt");
  NtupleFactory_->AddFloat("ele1_scEta");
  NtupleFactory_->AddFloat("ele1_scPhi");
  NtupleFactory_->AddFloat("ele1_scAvgLaserCorrection");
      
  // seed variables
  NtupleFactory_->AddInt("ele1_seedIeta");
  NtupleFactory_->AddInt("ele1_seedIphi");
  NtupleFactory_->AddInt("ele1_seedIx");
  NtupleFactory_->AddInt("ele1_seedIy");
  NtupleFactory_->AddInt("ele1_seedZside");
  
  // preshower variables
  NtupleFactory_->AddFloat("ele1_ES");
  
  // id variables
  NtupleFactory_->AddFloat("ele1_eOverP");
  
 
  // general variables
  NtupleFactory_->AddFloat("ele2_charge"); 
  NtupleFactory_->AddFloat("ele2_eta"); 
  NtupleFactory_->AddFloat("ele2_phi"); 
  
  // track variables
  NtupleFactory_->AddFloat("ele2_p_atVtx");
  
  // supercluster variables
  NtupleFactory_->AddFloat("ele2_scE");
  NtupleFactory_->AddFloat("ele2_scEt");
  NtupleFactory_->AddFloat("ele2_scEta");
  NtupleFactory_->AddFloat("ele2_scPhi");
  NtupleFactory_->AddFloat("ele2_scAvgLaserCorrection");
      
  // seed variables
  NtupleFactory_->AddInt("ele2_seedIeta");
  NtupleFactory_->AddInt("ele2_seedIphi");
  NtupleFactory_->AddInt("ele2_seedIx");
  NtupleFactory_->AddInt("ele2_seedIy");
  NtupleFactory_->AddInt("ele2_seedZside");
  
  // preshower variables
  NtupleFactory_->AddFloat("ele2_ES");
  
  // id variables
  NtupleFactory_->AddFloat("ele2_eOverP");

  // invariant mass
  NtupleFactory_->AddFloat("ele1_ele2_mass");
  NtupleFactory_->AddFloat("ele1_ele2_scMass");

}

// --------------------------------------------------------------------



SimpleNtupleEoverP::~SimpleNtupleEoverP ()
{
  cout<< "Analyzed " <<  eventNaiveId_ << " events" <<endl;
  NtupleFactory_->WriteNtuple();
  delete NtupleFactory_;
}

// -----------------------------------------------------------------------------------------



void SimpleNtupleEoverP::analyze (const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  ++eventNaiveId_;
 
  NtupleFactory_->FillInt("runId", iEvent.id().run());
  NtupleFactory_->FillInt("lumiId", iEvent.luminosityBlock());
  NtupleFactory_->FillInt("BXId", iEvent.bunchCrossing());
  NtupleFactory_->FillInt("eventId", iEvent.id().event());
  NtupleFactory_->FillInt("eventNaiveId", eventNaiveId_);
  NtupleFactory_->FillInt("timeStampLow", (int)(0xFFFFFFFF& iEvent.time().value()));
  NtupleFactory_->FillInt("timeStampHigh", (int)(iEvent.time().value() >> 32));

  
  bool isGoodEvent = false;
  
  NtupleFactory_->ClearNtuple();

  ///---- fill electrons and check if the event is good----
  isGoodEvent = fillEleInfo (iEvent, iSetup);

  ///---- save the entry of the tree only if W/Z event ----
  if ( isGoodEvent ) NtupleFactory_->FillNtuple();
  
}



// -----------------------------------------------------------------------------------------


bool SimpleNtupleEoverP::fillEleInfo (const edm::Event & iEvent, const edm::EventSetup & iSetup)
{ 
 
  bool eventIsGood = false;
  bool eventIsBad  = false;
  
  //*********** MET
  edm::Handle<edm::View<reco::MET> > calometHandle;
  iEvent.getByLabel(CALOMetTag_,calometHandle);
  View<reco::MET>  mets = *calometHandle;
  pat::MET met = mets.at(0);

  //*********** LASER CORRECTION
  edm::ESHandle<EcalLaserDbService> theLaser;
  iSetup.get<EcalLaserDbRecord>().get(theLaser);
 
  //*********** EB REC HITS
  edm::Handle<EcalRecHitCollection> recHitsEB;
  iEvent.getByLabel( recHitCollection_EB_, recHitsEB );
  const EcalRecHitCollection* theBarrelEcalRecHits = recHitsEB.product () ;
  if ( ! recHitsEB.isValid() ) {
    std::cerr << "SimpleNtupleEoverP::analyze --> recHitsEB not found" << std::endl; 
  }
  
  //*********** EE REC HITS
  edm::Handle<EcalRecHitCollection> recHitsEE;
  iEvent.getByLabel( recHitCollection_EE_, recHitsEE );
  const EcalRecHitCollection* theEndcapEcalRecHits = recHitsEE.product () ;
  if ( ! recHitsEE.isValid() ) {
    std::cerr << "SimpleNtupleEoverP::analyze --> recHitsEE not found" << std::endl; 
  }

  //************* ELECTRONS
  Handle<View<reco::GsfElectron> > electronHandle;
  iEvent.getByLabel(EleTag_,electronHandle);
  View<reco::GsfElectron> electrons = *electronHandle;
 
  int nGoodEle = 0;
  std::vector<int> goodElectronIndex;
  
  // Loop over electrons to find good/bad electrons
  for ( unsigned int i=0; i<electrons.size(); ++i )
  {
    reco::GsfElectron electron = electrons.at(i);
      
    //Get variables used for the selection
    float pt = electron.pt();
    float eta = electron.eta();
      
    float tkIso  = electron.dr03TkSumPt();
    float emIso  = electron.dr03EcalRecHitSumEt();
    float hadIso = electron.dr03HcalDepth1TowerSumEt()+electron.dr03HcalDepth2TowerSumEt();
    float combIso = tkIso + emIso + hadIso;
      
    int isEB = electron.isEB();
    int isEE = electron.isEE();
      
    float sigmaIetaIeta = electron.sigmaIetaIeta();
    float DetaIn        = electron.deltaEtaSuperClusterTrackAtVtx();
    float DphiIn        = electron.deltaPhiSuperClusterTrackAtVtx();
    float HOverE        = electron.hadronicOverEm();
      
    int mishits             = electron.gsfTrack()->trackerExpectedHitsInner().numberOfHits();
    int nAmbiguousGsfTracks = electron.ambiguousGsfTracksSize();
    float dist = electron.convDist();
    float dcot = electron.convDcot();
      
    bool isLooseElectron = false;
    if( 
        (isEB == 1 || isEE == 1) &&
        (pt > 10.) &&
        (fabs(eta) < 2.5) &&
        ( ( (isEB == 1) && (combIso/pt    < 0.150) ) || ( (isEB == 0) && (combIso/pt    < 0.100) ) ) &&
        ( ( (isEB == 1) && (sigmaIetaIeta < 0.010) ) || ( (isEB == 0) && (sigmaIetaIeta < 0.030) ) ) &&
        ( ( (isEB == 1) && (fabs(DphiIn)  < 0.800) ) || ( (isEB == 0) && (fabs(DphiIn)  < 0.700) ) ) &&
        ( ( (isEB == 1) && (fabs(DetaIn)  < 0.007) ) || ( (isEB == 0) && (fabs(DetaIn)  < 0.010) ) ) &&
        ( ( (isEB == 1) && (HOverE        < 0.150) ) || ( (isEB == 0) && (HOverE        < 0.070) ) )
      ) isLooseElectron = true;

    bool isTightElectron = false;
    if(
       (isEB == 1 || isEE == 1) &&
       (pt > 20.) &&
       (fabs(eta) < 2.5) &&
       ( ( (isEB == 1) && (combIso/pt    < 0.070) ) || ( (isEB == 0) && (combIso/pt    < 0.060) ) ) &&
       ( ( (isEB == 1) && (sigmaIetaIeta < 0.010) ) || ( (isEB == 0) && (sigmaIetaIeta < 0.030) ) ) &&
       ( ( (isEB == 1) && (fabs(DphiIn)  < 0.060) ) || ( (isEB == 0) && (fabs(DphiIn)  < 0.030) ) ) &&
       ( ( (isEB == 1) && (fabs(DetaIn)  < 0.004) ) || ( (isEB == 0) && (fabs(DetaIn)  < 0.007) ) ) &&
       ( ( (isEB == 1) && (HOverE        < 0.040) ) || ( (isEB == 0) && (HOverE        < 0.025) ) ) &&
       ( mishits == 0 ) &&
       ( nAmbiguousGsfTracks == 0 ) &&
       ( ( fabs(dist) > 0.02 ) || ( fabs(dcot) > 0.02 ) )
      ) isTightElectron = true;
    
    // Discard the event if ther are loose electrons
    if ( isLooseElectron == true && isTightElectron == false ) eventIsBad = true;
    
    // Count events with a tight ele
    if ( !isTightElectron ) continue;
    nGoodEle++;
    goodElectronIndex.push_back(i);
    
  } // end loop over electron candidates
    
  if ( nGoodEle == 1 ) {
    
    reco::GsfElectron electron = electrons.at(goodElectronIndex[0]);
    //Get variables used for the selection
    float pt = electron.pt();
    float eta = electron.eta();
      
    float tkIso  = electron.dr03TkSumPt();
    float emIso  = electron.dr03EcalRecHitSumEt();
    float hadIso = electron.dr03HcalDepth1TowerSumEt()+electron.dr03HcalDepth2TowerSumEt();
    float combIso = tkIso + emIso + hadIso;
      
    int isEB = electron.isEB();
      
    float DetaIn        = electron.deltaEtaSuperClusterTrackAtVtx();
    float DphiIn        = electron.deltaPhiSuperClusterTrackAtVtx();
    float HOverE        = electron.hadronicOverEm();

    math::XYZTLorentzVector  met4V = met.p4();
    math::XYZTLorentzVector  ele4V = electron.p4();
    float lepMetDphi = fmod ((ele4V.Phi()-met4V.Phi()),2*3.14159265);
    float lepMetMt  = sqrt( 2* ele4V.Pt() * met4V.Et() * ( 1-cos(lepMetDphi)) );

    // Now perform W selection
    if(
       (met4V.Et() > 25.) &&
       (lepMetMt > 50.) &&
       (fabs(lepMetDphi) > 1.57) &&
       (pt > 30.) &&
       (fabs(eta) < 2.5) &&
       ( ( (isEB == 1) && (combIso/pt    < 0.040) ) || ( (isEB == 0) && (combIso/pt    < 0.030) ) ) &&
       ( ( (isEB == 1) && (fabs(DphiIn)  < 0.030) ) || ( (isEB == 0) && (fabs(DphiIn)  < 0.020) ) ) &&
       ( ( (isEB == 1) && (fabs(DetaIn)  < 0.004) ) || ( (isEB == 0) && (fabs(DetaIn)  < 0.005) ) ) &&
       ( ( (isEB == 1) && (HOverE        < 0.025) ) || ( (isEB == 0) && (HOverE        < 0.025) ) )
      ) 
    { 
    
      eventIsGood = true; 

      // vector boson event type
      NtupleFactory_->FillInt("isZ",0);

      // basic variables
      NtupleFactory_->FillFloat("ele1_charge",(electron.charge()));
      NtupleFactory_->FillFloat("ele1_eta",(electron.eta()));
      NtupleFactory_->FillFloat("ele1_phi",(electron.phi()));
      
      // track variables
      NtupleFactory_->FillFloat("ele1_p_atVtx",(electron.trackMomentumAtVtx().R()));
      
      
      // supercluster variables
      reco::SuperClusterRef scRef = electron.superCluster();
      
      double R  = TMath::Sqrt(scRef->x()*scRef->x() + scRef->y()*scRef->y() +scRef->z()*scRef->z());
      double Rt = TMath::Sqrt(scRef->x()*scRef->x() + scRef->y()*scRef->y());
      
      NtupleFactory_->FillFloat("ele1_scE",scRef->energy());
      NtupleFactory_->FillFloat("ele1_scEt",scRef->energy()*(Rt/R));
      NtupleFactory_->FillFloat("ele1_scEta",scRef->eta());
      NtupleFactory_->FillFloat("ele1_scPhi",scRef->phi());   
      
      const std::vector<std::pair<DetId,float> >& hits = scRef->hitsAndFractions();
      
      
      // rechit variables
      int numRecHit = 0;
      float sumRecHitE = 0.;
      float sumLaserCorrectionRecHitE = 0.;
      
      bool printOut = false;
      if( printOut )
      {
        std::cout << "runId: " << iEvent.id().run() 
            << std::fixed
            << "   electron eta: " << std::setprecision(2) << std::setw(5) << electron.eta()
            << "   electron phi: " << std::setprecision(2) << std::setw(5) << electron.phi()
    //           << "   SC energy: "    << std::setprecision(2) << std::setw(6) << scRef -> energy()
            << std::endl;
      } 
          
      for(std::vector<std::pair<DetId,float> >::const_iterator rh = hits.begin(); rh!=hits.end(); ++rh)
      {
        float rhLaserCorrection = -1.;
        
        if ((*rh).first.subdetId()== EcalBarrel)
        {
          EBRecHitCollection::const_iterator itrechit = theBarrelEcalRecHits->find((*rh).first);
          if (itrechit==theBarrelEcalRecHits->end()) continue;
          EBDetId barrelId (itrechit->id ()); 
          ++numRecHit;
                        
            // laser correction
          rhLaserCorrection = theLaser->getLaserCorrection(barrelId, iEvent.time());
          sumRecHitE += itrechit->energy();
          sumLaserCorrectionRecHitE += itrechit->energy() * rhLaserCorrection;
          
          if( printOut && itrechit->energy() > 1. )
          {
            std::cout << std::fixed
                << "    recHitLC: "    << std::setprecision(6) << std::setw(8) << rhLaserCorrection
                << std::endl;
          }
        }
          
        if ((*rh).first.subdetId()== EcalEndcap)
        {
          EERecHitCollection::const_iterator itrechit = theEndcapEcalRecHits->find((*rh).first);
          if (itrechit==theEndcapEcalRecHits->end()) continue;
          EEDetId endcapId (itrechit->id ()); 
          ++numRecHit;
            
                  
            // laser correction
          rhLaserCorrection = theLaser->getLaserCorrection(endcapId, iEvent.time());
          sumRecHitE += itrechit->energy();
          sumLaserCorrectionRecHitE += itrechit->energy() * rhLaserCorrection;
          
          if( printOut && itrechit->energy() > 1. )
          {
            std::cout << std::fixed
                << "    recHitLC: "    << std::setprecision(6) << std::setw(8) << rhLaserCorrection
                << std::endl;
          }
        }
      }
      
      NtupleFactory_->FillFloat("ele1_scAvgLaserCorrection", sumLaserCorrectionRecHitE/sumRecHitE);
      
      
      // seed variables
      int ieta = -999;
      int iphi = -999;
      int ix = -999;
      int iy = -999;
      int zside = -999;
        
      if(electron.isEB())
      {
        std::pair<DetId, float> id = EcalClusterTools::getMaximum(scRef->hitsAndFractions(), theBarrelEcalRecHits);
          
          // flag
        EcalRecHitCollection::const_iterator it = theBarrelEcalRecHits->find(id.first);
          
        if( it != theBarrelEcalRecHits->end() ) 
         {
          ieta = (EBDetId(id.first)).ieta();
          iphi = (EBDetId(id.first)).iphi();
          ix = -999;
          iy = -999;	
          zside = 0;
         }
          
      }
        
      else if (electron.isEE())
      {
        std::pair<DetId, float> id = EcalClusterTools::getMaximum(scRef->hitsAndFractions(), theEndcapEcalRecHits);
          
          // flag - OutOfTime
        EcalRecHitCollection::const_iterator it = theEndcapEcalRecHits->find(id.first);
          
        if( it != theEndcapEcalRecHits->end() )
        {
          ix = (EEDetId(id.first)).ix();
          iy = (EEDetId(id.first)).iy();
          ieta = -999;
          iphi = -999;
          zside = (EEDetId(id.first)).zside();
        }
          
      }
    
        
      if (ieta > 1000) std::cout 
            << ieta << "\n"
            << goodElectronIndex[0] << "\n"
            << electron.isEB() << "\n"
            << electron.isEE() << "\n"
            << "\n" << "\n"
            << std::endl;
      NtupleFactory_->FillInt("ele1_seedIeta", ieta);
      NtupleFactory_->FillInt("ele1_seedIphi", iphi);
      NtupleFactory_->FillInt("ele1_seedIx", ix);
      NtupleFactory_->FillInt("ele1_seedIy", iy);
      NtupleFactory_->FillInt("ele1_seedZside", zside);
    
        // preshower variables 
      NtupleFactory_->FillFloat("ele1_ES",scRef->preshowerEnergy());
    
        // eleid variables
      NtupleFactory_->FillFloat("ele1_eOverP",electron.eSuperClusterOverP());
      }
  }
  /// ------------------ Z events: second electron --------------------------------
  else if ( nGoodEle == 2 ) {
    
    reco::GsfElectron electron1 = electrons.at(goodElectronIndex[0]);
    reco::GsfElectron electron2 = electrons.at(goodElectronIndex[1]);
    math::XYZTLorentzVector  ele1_4V = electron1.p4();
    math::XYZTLorentzVector  ele2_4V = electron2.p4();
    math::XYZTLorentzVector met4V = met.p4();

    if(
       ( met4V.Et() < 30.) &&
       ( (ele1_4V+ele2_4V).M() > 70. && (ele1_4V+ele2_4V).M() < 110. ) &&
       ( electron1.charge()*electron2.charge() < 0. )
      ) 
    {
      
      eventIsGood = true; 

      // vector boson event type
      NtupleFactory_->FillInt("isZ",1);

      ///First electron
      // basic variables
      NtupleFactory_->FillFloat("ele1_charge",(electron1.charge()));
      NtupleFactory_->FillFloat("ele1_eta",(electron1.eta()));
      NtupleFactory_->FillFloat("ele1_phi",(electron1.phi()));
      
      // track variables
      NtupleFactory_->FillFloat("ele1_p_atVtx",(electron1.trackMomentumAtVtx().R()));
      
      
      // supercluster variables
      reco::SuperClusterRef scRef = electron1.superCluster();
      
      double R  = TMath::Sqrt(scRef->x()*scRef->x() + scRef->y()*scRef->y() +scRef->z()*scRef->z());
      double Rt = TMath::Sqrt(scRef->x()*scRef->x() + scRef->y()*scRef->y());
      ROOT::Math::PtEtaPhiEVector ele1_sc(scRef->energy()*sin(2*atan(exp(-1.*scRef->eta()))),scRef->eta(),scRef->phi(),scRef->energy());

      NtupleFactory_->FillFloat("ele1_scE",scRef->energy());
      NtupleFactory_->FillFloat("ele1_scEt",scRef->energy()*(Rt/R));
      NtupleFactory_->FillFloat("ele1_scEta",scRef->eta());
      NtupleFactory_->FillFloat("ele1_scPhi",scRef->phi());   
      
      const std::vector<std::pair<DetId,float> >& hits = scRef->hitsAndFractions();
      
      
      // rechit variables
      int numRecHit = 0;
      float sumRecHitE = 0.;
      float sumLaserCorrectionRecHitE = 0.;
      
      bool printOut = false;
      if( printOut )
      {
        std::cout << "runId: " << iEvent.id().run() 
            << std::fixed
            << "   electron1 eta: " << std::setprecision(2) << std::setw(5) << electron1.eta()
            << "   electron1 phi: " << std::setprecision(2) << std::setw(5) << electron1.phi()
    //           << "   SC energy: "    << std::setprecision(2) << std::setw(6) << scRef -> energy()
            << std::endl;
      } 
          
      for(std::vector<std::pair<DetId,float> >::const_iterator rh = hits.begin(); rh!=hits.end(); ++rh)
      {
        float rhLaserCorrection = -1.;
        
        if ((*rh).first.subdetId()== EcalBarrel)
        {
          EBRecHitCollection::const_iterator itrechit = theBarrelEcalRecHits->find((*rh).first);
          if (itrechit==theBarrelEcalRecHits->end()) continue;
          EBDetId barrelId (itrechit->id ()); 
          ++numRecHit;
                        
            // laser correction
          rhLaserCorrection = theLaser->getLaserCorrection(barrelId, iEvent.time());
          sumRecHitE += itrechit->energy();
          sumLaserCorrectionRecHitE += itrechit->energy() * rhLaserCorrection;
          
          if( printOut && itrechit->energy() > 1. )
          {
            std::cout << std::fixed
                << "    recHitLC: "    << std::setprecision(6) << std::setw(8) << rhLaserCorrection
                << std::endl;
          }
        }
          
        if ((*rh).first.subdetId()== EcalEndcap)
        {
          EERecHitCollection::const_iterator itrechit = theEndcapEcalRecHits->find((*rh).first);
          if (itrechit==theEndcapEcalRecHits->end()) continue;
          EEDetId endcapId (itrechit->id ()); 
          ++numRecHit;
            
                  
            // laser correction
          rhLaserCorrection = theLaser->getLaserCorrection(endcapId, iEvent.time());
          sumRecHitE += itrechit->energy();
          sumLaserCorrectionRecHitE += itrechit->energy() * rhLaserCorrection;
          
          if( printOut && itrechit->energy() > 1. )
          {
            std::cout << std::fixed
                << "    recHitLC: "    << std::setprecision(6) << std::setw(8) << rhLaserCorrection
                << std::endl;
          }
        }
      }
      
      NtupleFactory_->FillFloat("ele1_scAvgLaserCorrection", sumLaserCorrectionRecHitE/sumRecHitE);
      
      
      // seed variables
      int ieta = -999;
      int iphi = -999;
      int ix = -999;
      int iy = -999;
      int zside = -999;
        
      if(electron1.isEB())
      {
        std::pair<DetId, float> id = EcalClusterTools::getMaximum(scRef->hitsAndFractions(), theBarrelEcalRecHits);
          
          // flag
        EcalRecHitCollection::const_iterator it = theBarrelEcalRecHits->find(id.first);
          
        if( it != theBarrelEcalRecHits->end() )
        {
          ieta = (EBDetId(id.first)).ieta();
          iphi = (EBDetId(id.first)).iphi();
          ix = -999;
          iy = -999;
          zside = 0;
        }
          
      }
        
      else if (electron1.isEE())
      {
        std::pair<DetId, float> id = EcalClusterTools::getMaximum(scRef->hitsAndFractions(), theEndcapEcalRecHits);
          
          // flag - OutOfTime
        EcalRecHitCollection::const_iterator it = theEndcapEcalRecHits->find(id.first);
          
        if( it != theEndcapEcalRecHits->end() )
        {
          ix = (EEDetId(id.first)).ix();
          iy = (EEDetId(id.first)).iy();
          ieta = -999;
          iphi = -999;
          zside = (EEDetId(id.first)).zside();
        }
          
      }
    
        
      NtupleFactory_->FillInt("ele1_seedIeta", ieta);
      NtupleFactory_->FillInt("ele1_seedIphi", iphi);
      NtupleFactory_->FillInt("ele1_seedIx", ix);
      NtupleFactory_->FillInt("ele1_seedIy", iy);
      NtupleFactory_->FillInt("ele1_seedZside", zside);
    
        // preshower variables 
      NtupleFactory_->FillFloat("ele1_ES",scRef->preshowerEnergy());
    
        // eleid variables
      NtupleFactory_->FillFloat("ele1_eOverP",electron1.eSuperClusterOverP());
      
    
      /// second electron
      // basic variables
      NtupleFactory_->FillFloat("ele2_charge",(electron2.charge()));
      NtupleFactory_->FillFloat("ele2_eta",(electron2.eta()));
      NtupleFactory_->FillFloat("ele2_phi",(electron2.phi()));
      
      // track variables
      NtupleFactory_->FillFloat("ele2_p_atVtx",(electron2.trackMomentumAtVtx().R()));
      
      
      // supercluster variables
      scRef = electron2.superCluster();
      
      R  = TMath::Sqrt(scRef->x()*scRef->x() + scRef->y()*scRef->y() +scRef->z()*scRef->z());
      Rt = TMath::Sqrt(scRef->x()*scRef->x() + scRef->y()*scRef->y());
      ROOT::Math::PtEtaPhiEVector ele2_sc(scRef->energy()*sin(2*atan(exp(-1.*scRef->eta()))),scRef->eta(),scRef->phi(),scRef->energy());

      NtupleFactory_->FillFloat("ele2_scE",scRef->energy());
      NtupleFactory_->FillFloat("ele2_scEt",scRef->energy()*(Rt/R));
      NtupleFactory_->FillFloat("ele2_scEta",scRef->eta());
      NtupleFactory_->FillFloat("ele2_scPhi",scRef->phi());   
      
      const std::vector<std::pair<DetId,float> >& hits_ele2 = scRef->hitsAndFractions();
      
      
      // rechit variables
      numRecHit = 0;
      sumRecHitE = 0.;
      sumLaserCorrectionRecHitE = 0.;
      
      printOut = false;
      if( printOut )
      {
        std::cout << "runId: " << iEvent.id().run() 
            << std::fixed
            << "   electron2 eta: " << std::setprecision(2) << std::setw(5) << electron2.eta()
            << "   electron2 phi: " << std::setprecision(2) << std::setw(5) << electron2.phi()
    //           << "   SC energy: "    << std::setprecision(2) << std::setw(6) << scRef -> energy()
            << std::endl;
      } 
          
      for(std::vector<std::pair<DetId,float> >::const_iterator rh = hits_ele2.begin(); rh!=hits_ele2.end(); ++rh)
      {
        float rhLaserCorrection = -1.;
        
        if ((*rh).first.subdetId()== EcalBarrel)
        {
          EBRecHitCollection::const_iterator itrechit = theBarrelEcalRecHits->find((*rh).first);
          if (itrechit==theBarrelEcalRecHits->end()) continue;
          EBDetId barrelId (itrechit->id ()); 
          ++numRecHit;
                        
            // laser correction
          rhLaserCorrection = theLaser->getLaserCorrection(barrelId, iEvent.time());
          sumRecHitE += itrechit->energy();
          sumLaserCorrectionRecHitE += itrechit->energy() * rhLaserCorrection;
          
          if( printOut && itrechit->energy() > 1. )
          {
            std::cout << std::fixed
                << "    recHitLC: "    << std::setprecision(6) << std::setw(8) << rhLaserCorrection
                << std::endl;
          }
        }
          
        if ((*rh).first.subdetId()== EcalEndcap)
        {
          EERecHitCollection::const_iterator itrechit = theEndcapEcalRecHits->find((*rh).first);
          if (itrechit==theEndcapEcalRecHits->end()) continue;
          EEDetId endcapId (itrechit->id ()); 
          ++numRecHit;
            
                  
            // laser correction
          rhLaserCorrection = theLaser->getLaserCorrection(endcapId, iEvent.time());
          sumRecHitE += itrechit->energy();
          sumLaserCorrectionRecHitE += itrechit->energy() * rhLaserCorrection;
          
          if( printOut && itrechit->energy() > 1. )
          {
            std::cout << std::fixed
                << "    recHitLC: "    << std::setprecision(6) << std::setw(8) << rhLaserCorrection
                << std::endl;
          }
        }
      }
      
      NtupleFactory_->FillFloat("ele2_scAvgLaserCorrection", sumLaserCorrectionRecHitE/sumRecHitE);
      
      // seed variables
      ieta = -999;
      iphi = -999;
      ix = -999;
      iy = -999;
      zside = -999;

      if(electron2.isEB())
      {
        std::pair<DetId, float> id = EcalClusterTools::getMaximum(scRef->hitsAndFractions(), theBarrelEcalRecHits);
          
          // flag
        EcalRecHitCollection::const_iterator it = theBarrelEcalRecHits->find(id.first);
          
        if( it != theBarrelEcalRecHits->end() )
        {
          ieta = (EBDetId(id.first)).ieta();
          iphi = (EBDetId(id.first)).iphi();
          ix = -999;
          iy = -999;
          zside = 0;
        }
          
      }
        
      else if(electron2.isEE())
      {
        std::pair<DetId, float> id = EcalClusterTools::getMaximum(scRef->hitsAndFractions(), theEndcapEcalRecHits);
          
          // flag - OutOfTime
        EcalRecHitCollection::const_iterator it = theEndcapEcalRecHits->find(id.first);
          
        if( it != theEndcapEcalRecHits->end() )
        {
          ix = (EEDetId(id.first)).ix();
          iy = (EEDetId(id.first)).iy();
          ieta = -999;
          iphi = -999;
          zside = (EEDetId(id.first)).zside();
        }
          
      }
    
        
      NtupleFactory_->FillInt("ele2_seedIeta", ieta);
      NtupleFactory_->FillInt("ele2_seedIphi", iphi);
      NtupleFactory_->FillInt("ele2_seedIx", ix);
      NtupleFactory_->FillInt("ele2_seedIy", iy);
      NtupleFactory_->FillInt("ele2_seedZside", zside);
    
      // preshower variables 
      NtupleFactory_->FillFloat("ele2_ES",scRef->preshowerEnergy());
    
      // eleid variables
      NtupleFactory_->FillFloat("ele2_eOverP",electron2.eSuperClusterOverP());
      
      // Invariant mass
      NtupleFactory_->FillFloat("ele1_ele2_mass",(ele1_4V+ele2_4V).M());
      NtupleFactory_->FillFloat("ele1_ele2_scMass",(ele1_sc+ele2_sc).M());
  
        
    }
  }
      
  
  if ( eventIsGood && !eventIsBad ) return true;
  else return false;

}


// -----------------------------------------------------------------------------------------


DEFINE_FWK_MODULE(SimpleNtupleEoverP);
