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
    
  PFMetTag_ = iConfig.getParameter<edm::InputTag>("PFMetTag");
  
  eventType_ = iConfig.getUntrackedParameter<int>("eventType", 1);
    
  //---- flags ----
  verbosity_ = iConfig.getUntrackedParameter<bool>("verbosity", false);
  
  eventNaiveId_ = 0;
  //---- Initialize tree branches ----
  
  // event variables
  outTree_ -> Branch("eventId",       &eventId,                 "eventId/I");
  outTree_ -> Branch("lumiId",        &lumiId,                 "lumiId/I");
  outTree_ -> Branch("runId",         &runId,                 "runId/I");
  outTree_ -> Branch("timeStampHigh", &timeStampHigh, "timeStampHigh/I");
  outTree_ -> Branch("isW", &isW, "isW/I");
  outTree_ -> Branch("isZ", &isZ, "isZ/I");  
  outTree_ -> Branch("ele1_ele2_mass", &ele1_ele2_mass, "ele1_ele2_mass/F");
  outTree_ -> Branch("ele1_ele2_scMass", &ele1_ele2_scMass, "ele1_ele2_scMass/F");

  // electron variables
  outTree_ -> Branch("ele1_tkP",         &ele1_tkP,             "ele1_tkP/F");
  outTree_ -> Branch("ele1_e5x5",        &ele1_e5x5,             "ele1_e5x5/F");
  outTree_ -> Branch("ele1_scNxtal",     &ele1_scNxtal,          "ele1_scNxtal/F");
  outTree_ -> Branch("ele1_scE",         &ele1_scE,                 "ele1_scE/F");
  outTree_ -> Branch("ele1_scEta",       &ele1_scEta,             "ele1_scEta/F");
  outTree_ -> Branch("ele1_scPhi",       &ele1_scPhi,             "ele1_scPhi/F");
  outTree_ -> Branch("ele1_5x5LaserCorr", &ele1_scLaserCorr, "ele1_scLaserCorr/F");
  outTree_ -> Branch("ele1_scLaserCorr", &ele1_scLaserCorr, "ele1_scLaserCorr/F");
  outTree_ -> Branch("ele1_es",          &ele1_es,                   "ele1_es/F");
  outTree_ -> Branch("ele1_seedE",       &ele1_seedE,             "ele1_seedE/F");
  outTree_ -> Branch("ele1_seedLaserCorr", &ele1_seedLaserCorr,             "ele1_seedLaserCorr/F");
  outTree_ -> Branch("ele1_seedIeta",    &ele1_seedIeta,             "ele1_seedIeta/I");
  outTree_ -> Branch("ele1_seedIphi",    &ele1_seedIphi,             "ele1_seedIphi/I");
  outTree_ -> Branch("ele1_seedIx",      &ele1_seedIx,                 "ele1_seedIx/I");
  outTree_ -> Branch("ele1_seedIy",      &ele1_seedIy,                 "ele1_seedIy/I");
  outTree_ -> Branch("ele1_seedZside",   &ele1_seedZside,           "ele1_seedZside/I");
  outTree_ -> Branch("ele1_EOverP",      &ele1_EOverP,           "ele1_EOverP/F");

  outTree_ -> Branch("ele2_tkP",     &ele2_tkP,             "ele2_tkP/F");
  outTree_ -> Branch("ele2_e5x5",        &ele2_e5x5,             "ele2_e5x5/F");
  outTree_ -> Branch("ele2_scNxtal",     &ele2_scNxtal,          "ele2_scNxtal/F");
  outTree_ -> Branch("ele2_scE",         &ele2_scE,                 "ele2_scE/F");
  outTree_ -> Branch("ele2_scEta",       &ele2_scEta,             "ele2_scEta/F");
  outTree_ -> Branch("ele2_scPhi",       &ele2_scPhi,             "ele2_scPhi/F");
  outTree_ -> Branch("ele2_scLaserCorr", &ele2_scLaserCorr, "ele2_scLaserCorr/F");
  outTree_ -> Branch("ele2_5x5LaserCorr", &ele2_scLaserCorr, "ele2_scLaserCorr/F");
  outTree_ -> Branch("ele2_es",          &ele2_es,                   "ele2_es/F");
  outTree_ -> Branch("ele2_seedE",       &ele2_seedE,             "ele2_seedE/F");
  outTree_ -> Branch("ele2_seedLaserCorr", &ele2_seedLaserCorr,             "ele2_seedLaserCorr/F");
  outTree_ -> Branch("ele2_seedIeta",    &ele2_seedIeta,             "ele2_seedIeta/I");
  outTree_ -> Branch("ele2_seedIphi",    &ele2_seedIphi,             "ele2_seedIphi/I");
  outTree_ -> Branch("ele2_seedIx",      &ele2_seedIx,                 "ele2_seedIx/I");
  outTree_ -> Branch("ele2_seedIy",      &ele2_seedIy,                 "ele2_seedIy/I");
  outTree_ -> Branch("ele2_seedZside",   &ele2_seedZside,           "ele2_seedZside/I");
  outTree_ -> Branch("ele2_EOverP",      &ele2_EOverP,           "ele2_EOverP/F");


}

// --------------------------------------------------------------------



SimpleNtupleEoverP::~SimpleNtupleEoverP ()
{
  cout<< "Analyzed " <<  eventNaiveId_ << " events" <<endl;
  // save tree
  outTree_ -> Write();
}

// -----------------------------------------------------------------------------------------



void SimpleNtupleEoverP::analyze (const edm::Event& iEvent, const edm::EventSetup& iSetup) {

  ++eventNaiveId_;
  
  bool isGoodEvent = false;
  
  // event variables 
  runId = iEvent.id().run();
  lumiId = iEvent.luminosityBlock();
  eventId = iEvent.id().event();
  timeStampHigh = (int)(iEvent.time().value() >> 32);
  
  isW = -1;
  isZ = -1;
  ele1_ele2_mass = -1;
  ele1_ele2_scMass = -1;

  // electron variables  
  ele1_tkP = -99.;
  ele1_e5x5 = -99.;
  ele1_scNxtal = -99.;
  ele1_scE = -99.;
  ele1_scEta = -99.;
  ele1_scPhi = -99.;
  ele1_scLaserCorr = -99.;
  ele1_5x5LaserCorr = -99.;
  ele1_es = -99.;
  ele1_seedE = -99.;
  ele1_seedLaserCorr = -99.;
  ele1_seedIeta = -9999;
  ele1_seedIphi = -9999;
  ele1_seedIx = -9999;
  ele1_seedIy = -9999;
  ele1_seedZside = -9999;
  ele1_EOverP = -99.;

  ele2_tkP = -99.;
  ele2_e5x5 = -99.;
  ele2_scNxtal = -99.;
  ele2_scE = -99.;
  ele2_scEta = -99.;
  ele2_scPhi = -99.;
  ele2_scLaserCorr = -99.;
  ele2_5x5LaserCorr = -99.;
  ele2_es = -99.;
  ele2_seedE = -99.;
  ele2_seedLaserCorr = -99.;
  ele2_seedIeta = -9999;
  ele2_seedIphi = -9999;
  ele2_seedIx = -9999;
  ele2_seedIy = -9999;
  ele2_seedZside = -9999;
  ele2_EOverP = -99.;

  ///---- fill electrons and check if the event is good----
  isGoodEvent = fillEleInfo (iEvent, iSetup);

  ///---- save the entry of the tree only if W/Z event ----
  if ( isGoodEvent )   outTree_ -> Fill();
  
}



// -----------------------------------------------------------------------------------------


bool SimpleNtupleEoverP::fillEleInfo (const edm::Event & iEvent, const edm::EventSetup & iSetup)
{ 
 
  bool eventIsGood = false;
  bool eventIsBad  = false;
  
  //*********** MET
  edm::Handle<edm::View<reco::MET> > PFmetHandle;
  iEvent.getByLabel(PFMetTag_,PFmetHandle);
  View<reco::MET>  mets = *PFmetHandle;
  reco::MET met = mets.at(0);

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
      
//     //Get variables used for the selection
//     float pt = electron.pt();
//     float eta = electron.eta();
//       
//     float tkIso  = electron.dr03TkSumPt();
//     float emIso  = electron.dr03EcalRecHitSumEt();
//     float hadIso = electron.dr03HcalDepth1TowerSumEt()+electron.dr03HcalDepth2TowerSumEt();
//     float combIso = tkIso + emIso + hadIso;
//       
//     int isEB = electron.isEB();
//     int isEE = electron.isEE();
//       
//     float sigmaIetaIeta = electron.sigmaIetaIeta();
//     float DetaIn        = electron.deltaEtaSuperClusterTrackAtVtx();
//     float DphiIn        = electron.deltaPhiSuperClusterTrackAtVtx();
//     float HOverE        = electron.hadronicOverEm();
//       
//     int mishits             = electron.gsfTrack()->trackerExpectedHitsInner().numberOfHits();
//     int nAmbiguousGsfTracks = electron.ambiguousGsfTracksSize();
//     float dist = electron.convDist();
//     float dcot = electron.convDcot();
//       
//     bool isLooseElectron = false;
//     if( 
//         (isEB == 1 || isEE == 1) &&
//         (pt > 10.) &&
//         (fabs(eta) < 2.5) &&
//         ( ( (isEB == 1) && (combIso/pt    < 0.150) ) || ( (isEB == 0) && (combIso/pt    < 0.100) ) ) &&
//         ( ( (isEB == 1) && (sigmaIetaIeta < 0.010) ) || ( (isEB == 0) && (sigmaIetaIeta < 0.030) ) ) &&
//         ( ( (isEB == 1) && (fabs(DphiIn)  < 0.800) ) || ( (isEB == 0) && (fabs(DphiIn)  < 0.700) ) ) &&
//         ( ( (isEB == 1) && (fabs(DetaIn)  < 0.007) ) || ( (isEB == 0) && (fabs(DetaIn)  < 0.010) ) ) &&
//         ( ( (isEB == 1) && (HOverE        < 0.150) ) || ( (isEB == 0) && (HOverE        < 0.070) ) )
//       ) isLooseElectron = true;
// 
//     bool isTightElectron = false;
//     if(
//        (isEB == 1 || isEE == 1) &&
//        (pt > 20.) &&
//        (fabs(eta) < 2.5) &&
//        ( ( (isEB == 1) && (combIso/pt    < 0.070) ) || ( (isEB == 0) && (combIso/pt    < 0.060) ) ) &&
//        ( ( (isEB == 1) && (sigmaIetaIeta < 0.010) ) || ( (isEB == 0) && (sigmaIetaIeta < 0.030) ) ) &&
//        ( ( (isEB == 1) && (fabs(DphiIn)  < 0.060) ) || ( (isEB == 0) && (fabs(DphiIn)  < 0.030) ) ) &&
//        ( ( (isEB == 1) && (fabs(DetaIn)  < 0.004) ) || ( (isEB == 0) && (fabs(DetaIn)  < 0.007) ) ) &&
//        ( ( (isEB == 1) && (HOverE        < 0.040) ) || ( (isEB == 0) && (HOverE        < 0.025) ) ) &&
//        ( mishits == 0 ) &&
//        ( nAmbiguousGsfTracks == 0 ) &&
//        ( ( fabs(dist) > 0.02 ) || ( fabs(dcot) > 0.02 ) )
//       ) isTightElectron = true;
//     
//     // Discard the event if ther are loose electrons
//     if ( isLooseElectron == true && isTightElectron == false ) eventIsBad = true;
//     
//     // Count events with a tight ele
//     if ( !isTightElectron ) continue;
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

//     // Now perform W selection
//     if(
//        (met4V.Et() > 25.) &&
//        (lepMetMt > 50.) &&
//        (fabs(lepMetDphi) > 1.57) &&
//        (pt > 30.) &&
//        (fabs(eta) < 2.5) &&
//        ( ( (isEB == 1) && (combIso/pt    < 0.040) ) || ( (isEB == 0) && (combIso/pt    < 0.030) ) ) &&
//        ( ( (isEB == 1) && (fabs(DphiIn)  < 0.030) ) || ( (isEB == 0) && (fabs(DphiIn)  < 0.020) ) ) &&
//        ( ( (isEB == 1) && (fabs(DetaIn)  < 0.004) ) || ( (isEB == 0) && (fabs(DetaIn)  < 0.005) ) ) &&
//        ( ( (isEB == 1) && (HOverE        < 0.025) ) || ( (isEB == 0) && (HOverE        < 0.025) ) )
//       ) 
//     { 
    
      eventIsGood = true; 

      // vector boson event type
      isW = 1;
      isZ = 0;
      
      ele1_tkP = electron.trackMomentumAtVtx().R();
          
      // supercluster variables
      reco::SuperClusterRef scRef = electron.superCluster();
      
      ele1_e5x5 = electron.e5x5();
      ele1_scNxtal = scRef->hitsAndFractions().size();
      ele1_scE = scRef->energy();
      ele1_scEta = scRef->eta();
      ele1_scPhi = scRef->phi();
      
      const std::vector<std::pair<DetId,float> >& hits = scRef->hitsAndFractions();
      
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
          ele1_seedE = it->energy();
          ele1_seedLaserCorr = theLaser->getLaserCorrection(EBDetId(id.first), iEvent.time());
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
          ele1_seedE = it->energy();
          ele1_seedLaserCorr = theLaser->getLaserCorrection(EEDetId(id.first), iEvent.time());
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
      
      ele1_seedIeta = ieta;
      ele1_seedIphi = iphi;
      ele1_seedIx = ix;
      ele1_seedIy = iy;
      ele1_seedZside = zside;

      // rechit variables
      int numRecHit = 0;
      float sumRecHitE = 0.;
      float sumLaserCorrectionRecHitE = 0.;
      float sumRecHitE5x5 = 0.;
      float sumLaserCorrectionRecHitE5x5 = 0.;
      
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
          // check if rh is inside the 5x5 matrix
          if ( fabs(barrelId.ieta() - ele1_seedIeta) < 3 && fabs(barrelId.iphi() - ele1_seedIphi) < 3 ) {
            sumRecHitE5x5 += itrechit->energy();
            sumLaserCorrectionRecHitE5x5 += itrechit->energy() * rhLaserCorrection;
          }
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
          // check if rh is inside the 5x5 matrix
          if ( fabs(endcapId.ix() - ele1_seedIx) < 3 && fabs(endcapId.iy() - ele1_seedIy) < 3 ) {
            sumRecHitE5x5 += itrechit->energy();
            sumLaserCorrectionRecHitE5x5 += itrechit->energy() * rhLaserCorrection;
          }

          if( printOut && itrechit->energy() > 1. )
          {
            std::cout << std::fixed
                << "    recHitLC: "    << std::setprecision(6) << std::setw(8) << rhLaserCorrection
                << std::endl;
          }
        }
      }
      
      ele1_scLaserCorr = sumLaserCorrectionRecHitE/sumRecHitE;
      ele1_scE *= ele1_scLaserCorr;
      
      ele1_5x5LaserCorr = sumLaserCorrectionRecHitE5x5/sumRecHitE5x5;
      ele1_e5x5 *= ele1_5x5LaserCorr;
    
      // preshower variables 
      ele1_es = scRef->preshowerEnergy();

      // EsuP variables
      ele1_EOverP = electron.eSuperClusterOverP()*ele1_scLaserCorr;
//       }
  }
  /// ------------------ Z events: second electron --------------------------------
  else if ( nGoodEle == 2 ) {
    
    reco::GsfElectron electron1 = electrons.at(goodElectronIndex[0]);
    reco::GsfElectron electron2 = electrons.at(goodElectronIndex[1]);
    math::XYZTLorentzVector  ele1_4V = electron1.p4();
    math::XYZTLorentzVector  ele2_4V = electron2.p4();
    math::XYZTLorentzVector met4V = met.p4();

//     if(
//        ( met4V.Et() < 30.) &&
//        ( (ele1_4V+ele2_4V).M() > 70. && (ele1_4V+ele2_4V).M() < 110. ) &&
//        ( electron1.charge()*electron2.charge() < 0. )
//       ) 
//     {
      
      eventIsGood = true; 
      
      // vector boson event type
      isW = 0;
      isZ = 1;
      
      ///First electron
      ele1_tkP = electron1.trackMomentumAtVtx().R();

      // supercluster variables
      reco::SuperClusterRef scRef = electron1.superCluster();
      
      double R  = TMath::Sqrt(scRef->x()*scRef->x() + scRef->y()*scRef->y() +scRef->z()*scRef->z());
      double Rt = TMath::Sqrt(scRef->x()*scRef->x() + scRef->y()*scRef->y());
      ROOT::Math::PtEtaPhiEVector ele1_sc(scRef->energy()*sin(2*atan(exp(-1.*scRef->eta()))),scRef->eta(),scRef->phi(),scRef->energy());

      ele1_e5x5 = electron1.e5x5();
      ele1_scNxtal = scRef->hitsAndFractions().size();
      ele1_scE = scRef->energy();
      ele1_scEta = scRef->eta();
      ele1_scPhi = scRef->phi();
      
      const std::vector<std::pair<DetId,float> >& hits = scRef->hitsAndFractions();
      
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
          ele1_seedE = it->energy();
          ele1_seedLaserCorr = theLaser->getLaserCorrection(EBDetId(id.first), iEvent.time());
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
          ele1_seedE = it->energy();
          ele1_seedLaserCorr = theLaser->getLaserCorrection(EEDetId(id.first), iEvent.time());
          ix = (EEDetId(id.first)).ix();
          iy = (EEDetId(id.first)).iy();
          ieta = -999;
          iphi = -999;
          zside = (EEDetId(id.first)).zside();
        }
          
      }
    
        
      ele1_seedIeta = ieta;
      ele1_seedIphi = iphi;
      ele1_seedIx = ix;
      ele1_seedIy = iy;
      ele1_seedZside = zside;

      
      // rechit variables
      int numRecHit = 0;
      float sumRecHitE = 0.;
      float sumLaserCorrectionRecHitE = 0.;
      float sumRecHitE5x5 = 0.;
      float sumLaserCorrectionRecHitE5x5 = 0.;

      
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
          // check if rh is inside the 5x5 matrix
          if ( fabs(barrelId.ieta() - ele1_seedIeta) < 3 && fabs(barrelId.iphi() - ele1_seedIphi) < 3 ) {
            sumRecHitE5x5 += itrechit->energy();
            sumLaserCorrectionRecHitE5x5 += itrechit->energy() * rhLaserCorrection;
          }

          
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
          // check if rh is inside the 5x5 matrix
          if ( fabs(endcapId.ix() - ele1_seedIx) < 3 && fabs(endcapId.iy() - ele1_seedIy) < 3 ) {
            sumRecHitE5x5 += itrechit->energy();
            sumLaserCorrectionRecHitE5x5 += itrechit->energy() * rhLaserCorrection;
          }

          
          if( printOut && itrechit->energy() > 1. )
          {
            std::cout << std::fixed
                << "    recHitLC: "    << std::setprecision(6) << std::setw(8) << rhLaserCorrection
                << std::endl;
          }
        }
      }
      
      ele1_scLaserCorr = sumLaserCorrectionRecHitE/sumRecHitE;
      ele1_scE *= ele1_scLaserCorr;
      
      ele1_5x5LaserCorr = sumLaserCorrectionRecHitE5x5/sumRecHitE5x5;
      ele1_e5x5 *= ele1_5x5LaserCorr;

      // preshower variables 
      ele1_es = scRef->preshowerEnergy();

      // EsuP variables
      ele1_EOverP = electron1.eSuperClusterOverP()*ele1_scLaserCorr;
      
    
      /// second electron
      ele2_tkP = electron1.trackMomentumAtVtx().R();

      // supercluster variables
      scRef = electron2.superCluster();
      
      R  = TMath::Sqrt(scRef->x()*scRef->x() + scRef->y()*scRef->y() +scRef->z()*scRef->z());
      Rt = TMath::Sqrt(scRef->x()*scRef->x() + scRef->y()*scRef->y());
      ROOT::Math::PtEtaPhiEVector ele2_sc(scRef->energy()*sin(2*atan(exp(-1.*scRef->eta()))),scRef->eta(),scRef->phi(),scRef->energy());

      ele2_e5x5 = electron2.e5x5();
      ele2_scNxtal = scRef->hitsAndFractions().size();
      ele2_scE = scRef->energy();
      ele2_scEta = scRef->eta();
      ele2_scPhi = scRef->phi();
      
      const std::vector<std::pair<DetId,float> >& hits_ele2 = scRef->hitsAndFractions();
      
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
          ele2_seedE = it->energy();
          ele2_seedLaserCorr = theLaser->getLaserCorrection(EBDetId(id.first), iEvent.time());
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
          ele2_seedE = it->energy();
          ele2_seedLaserCorr = theLaser->getLaserCorrection(EEDetId(id.first), iEvent.time());
          ix = (EEDetId(id.first)).ix();
          iy = (EEDetId(id.first)).iy();
          ieta = -999;
          iphi = -999;
          zside = (EEDetId(id.first)).zside();
        }
          
      }
    
        
      ele2_seedIeta = ieta;
      ele2_seedIphi = iphi;
      ele2_seedIx = ix;
      ele2_seedIy = iy;
      ele2_seedZside = zside;

      
      // rechit variables
      numRecHit = 0;
      sumRecHitE = 0.;
      sumLaserCorrectionRecHitE = 0.;
      sumRecHitE5x5 = 0.;
      sumLaserCorrectionRecHitE5x5 = 0.;

      
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
          // check if rh is inside the 5x5 matrix
          if ( fabs(barrelId.ieta() - ele2_seedIeta) < 3 && fabs(barrelId.iphi() - ele2_seedIphi) < 3 ) {
            sumRecHitE5x5 += itrechit->energy();
            sumLaserCorrectionRecHitE5x5 += itrechit->energy() * rhLaserCorrection;
          }

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
          // check if rh is inside the 5x5 matrix
          if ( fabs(endcapId.ix() - ele2_seedIx) < 3 && fabs(endcapId.iy() - ele2_seedIy) < 3 ) {
            sumRecHitE5x5 += itrechit->energy();
            sumLaserCorrectionRecHitE5x5 += itrechit->energy() * rhLaserCorrection;
          }

          
          if( printOut && itrechit->energy() > 1. )
          {
            std::cout << std::fixed
                << "    recHitLC: "    << std::setprecision(6) << std::setw(8) << rhLaserCorrection
                << std::endl;
          }
        }
      }
      
      ele2_scLaserCorr = sumLaserCorrectionRecHitE/sumRecHitE;
      ele2_scE *= ele2_scLaserCorr;
      
      ele2_5x5LaserCorr = sumLaserCorrectionRecHitE5x5/sumRecHitE5x5;
      ele2_e5x5 *= ele1_5x5LaserCorr;

      // preshower variables 
      ele2_es = scRef->preshowerEnergy();

      // EsuP variables
      ele2_EOverP = electron2.eSuperClusterOverP()*ele2_scLaserCorr;
      
      // Invariant mass
      ele1_ele2_mass = (ele1_4V+ele2_4V).M();
      ele1_ele2_scMass = (ele1_sc+ele2_sc).M()*sqrt(ele1_scLaserCorr*ele2_scLaserCorr);
//     }
  }
      
  
  if ( eventIsGood && !eventIsBad ) return true;
  else return false;

}


// -----------------------------------------------------------------------------------------


DEFINE_FWK_MODULE(SimpleNtupleEoverP);
