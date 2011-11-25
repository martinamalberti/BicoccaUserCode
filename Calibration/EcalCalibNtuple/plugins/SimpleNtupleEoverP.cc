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
  // Initialize parameters for cluster corrections
  InitializeParams(params);

  edm::Service<TFileService> fs ;
  outTree_  =        fs -> make <TTree>("SimpleNtupleEoverP","SimpleNtupleEoverP"); 

  PVTag_       = iConfig.getParameter<edm::InputTag>("PVTag");

  recHitCollection_EB_ = iConfig.getParameter<edm::InputTag>("recHitCollection_EB");
  recHitCollection_EE_ = iConfig.getParameter<edm::InputTag>("recHitCollection_EE");
  
  EleTag_ = iConfig.getParameter<edm::InputTag>("EleTag");
    
  PFMetTag_ = iConfig.getParameter<edm::InputTag>("PFMetTag");
  
  rhoTag_ = iConfig.getParameter<edm::InputTag>("rhoTag");
      
  eventType_ = iConfig.getUntrackedParameter<int>("eventType", 1);
    
  //---- flags ----
  verbosity_ = iConfig.getUntrackedParameter<bool>("verbosity", false);
  doTighterSel_ = iConfig.getUntrackedParameter<bool>("doTighterSel", false);
  
  eventNaiveId_ = 0;
  //---- Initialize tree branches ----
  
  // event variables
  outTree_ -> Branch("eventId",       &eventId,                 "eventId/I");
  outTree_ -> Branch("lumiId",        &lumiId,                 "lumiId/I");
  outTree_ -> Branch("runId",         &runId,                 "runId/I");
  outTree_ -> Branch("timeStampHigh", &timeStampHigh, "timeStampHigh/I");
  outTree_ -> Branch("isW", &isW, "isW/I");
  outTree_ -> Branch("isZ", &isZ, "isZ/I");  

  outTree_ -> Branch("PV_n",        &PV_n,                 "PV_n/I");

  // electron variables
  outTree_ -> Branch("ele1_fEta",     &ele1_fEta,         "ele1_fEta/F");
  outTree_ -> Branch("ele1_fEtaCorr",     &ele1_fEtaCorr,         "ele1_fEtaCorr/F");
  outTree_ -> Branch("ele1_tkP",         &ele1_tkP,             "ele1_tkP/F");
  outTree_ -> Branch("ele1_tkP",         &ele1_tkP,             "ele1_tkP/F");
  outTree_ -> Branch("ele1_e5x5",        &ele1_e5x5,             "ele1_e5x5/F");
  outTree_ -> Branch("ele1_e3x3",        &ele1_e3x3,             "ele1_e3x3/F");
  outTree_ -> Branch("ele1_scNxtal",     &ele1_scNxtal,          "ele1_scNxtal/F");
  outTree_ -> Branch("ele1_scE",         &ele1_scE,                 "ele1_scE/F");
  outTree_ -> Branch("ele1_scEta",       &ele1_scEta,             "ele1_scEta/F");
  outTree_ -> Branch("ele1_scPhi",       &ele1_scPhi,             "ele1_scPhi/F");
  outTree_ -> Branch("ele1_5x5LaserCorr", &ele1_5x5LaserCorr, "ele1_5x5LaserCorr/F");
  outTree_ -> Branch("ele1_3x3LaserCorr", &ele1_3x3LaserCorr, "ele1_3x3LaserCorr/F");
  outTree_ -> Branch("ele1_scLaserCorr", &ele1_scLaserCorr, "ele1_scLaserCorr/F");
  outTree_ -> Branch("ele1_es",          &ele1_es,                   "ele1_es/F");
  outTree_ -> Branch("ele1_seedE",       &ele1_seedE,             "ele1_seedE/F");
  outTree_ -> Branch("ele1_seedLaserAlpha", &ele1_seedLaserAlpha, "ele1_seedLaserAlpha/F");
  outTree_ -> Branch("ele1_seedLaserCorr", &ele1_seedLaserCorr,             "ele1_seedLaserCorr/F");
  outTree_ -> Branch("ele1_seedICConstant",&ele1_seedICConstant,            "ele1_seedICConstant/F");
  outTree_ -> Branch("ele1_seedIeta",    &ele1_seedIeta,             "ele1_seedIeta/I");
  outTree_ -> Branch("ele1_seedIphi",    &ele1_seedIphi,             "ele1_seedIphi/I");
  outTree_ -> Branch("ele1_seedIx",      &ele1_seedIx,                 "ele1_seedIx/I");
  outTree_ -> Branch("ele1_seedIy",      &ele1_seedIy,                 "ele1_seedIy/I");
  outTree_ -> Branch("ele1_seedZside",   &ele1_seedZside,           "ele1_seedZside/I");
  outTree_ -> Branch("ele1_EOverP",      &ele1_EOverP,           "ele1_EOverP/F");
  outTree_ -> Branch("ele1_recHit_E",              "std::vector<float>", &ele1_recHit_E);
  outTree_ -> Branch("ele1_recHit_hashedIndex", "std::vector<int>",   &ele1_recHit_hashedIndex);
  outTree_ -> Branch("ele1_recHit_ietaORix", "std::vector<int>",   &ele1_recHit_ietaORix);
  outTree_ -> Branch("ele1_recHit_iphiORiy", "std::vector<int>",   &ele1_recHit_iphiORiy);
  outTree_ -> Branch("ele1_recHit_zside", "std::vector<int>",   &ele1_recHit_zside);
  outTree_ -> Branch("ele1_recHit_laserCorrection","std::vector<float>", &ele1_recHit_laserCorrection);
  outTree_ -> Branch("ele1_recHit_ICConstant",     "std::vector<float>", &ele1_recHit_ICConstant);
  

  outTree_ -> Branch("ele2_fEta",     &ele2_fEta,         "ele2_fEta/F");
  outTree_ -> Branch("ele2_fEtaCorr",     &ele2_fEtaCorr,         "ele2_fEtaCorr/F");
  outTree_ -> Branch("ele2_tkP",         &ele2_tkP,             "ele2_tkP/F");
  outTree_ -> Branch("ele2_e5x5",        &ele2_e5x5,             "ele2_e5x5/F");
  outTree_ -> Branch("ele2_e3x3",        &ele2_e3x3,             "ele2_e3x3/F");
  outTree_ -> Branch("ele2_scNxtal",     &ele2_scNxtal,          "ele2_scNxtal/F");
  outTree_ -> Branch("ele2_scE",         &ele2_scE,                 "ele2_scE/F");
  outTree_ -> Branch("ele2_scEta",       &ele2_scEta,             "ele2_scEta/F");
  outTree_ -> Branch("ele2_scPhi",       &ele2_scPhi,             "ele2_scPhi/F");
  outTree_ -> Branch("ele2_scLaserCorr", &ele2_scLaserCorr, "ele2_scLaserCorr/F");
  outTree_ -> Branch("ele2_5x5LaserCorr", &ele2_5x5LaserCorr, "ele2_5x5LaserCorr/F");
  outTree_ -> Branch("ele2_3x3LaserCorr", &ele2_3x3LaserCorr, "ele2_3x3LaserCorr/F");
  outTree_ -> Branch("ele2_es",          &ele2_es,                   "ele2_es/F");
  outTree_ -> Branch("ele2_seedE",       &ele2_seedE,             "ele2_seedE/F");
  outTree_ -> Branch("ele2_seedLaserAlpha", &ele2_seedLaserAlpha, "ele2_seedLaserAlpha/F");
  outTree_ -> Branch("ele2_seedLaserCorr", &ele2_seedLaserCorr,             "ele2_seedLaserCorr/F");
  outTree_ -> Branch("ele2_seedICConstant",&ele2_seedICConstant,             "ele2_seedICConstant/F");
  outTree_ -> Branch("ele2_seedIeta",    &ele2_seedIeta,             "ele2_seedIeta/I");
  outTree_ -> Branch("ele2_seedIphi",    &ele2_seedIphi,             "ele2_seedIphi/I");
  outTree_ -> Branch("ele2_seedIx",      &ele2_seedIx,                 "ele2_seedIx/I");
  outTree_ -> Branch("ele2_seedIy",      &ele2_seedIy,                 "ele2_seedIy/I");
  outTree_ -> Branch("ele2_seedZside",   &ele2_seedZside,           "ele2_seedZside/I");
  outTree_ -> Branch("ele2_EOverP",      &ele2_EOverP,           "ele2_EOverP/F");
  outTree_ -> Branch("ele2_recHit_E",           "std::vector<float>", &ele2_recHit_E);
  outTree_ -> Branch("ele2_recHit_hashedIndex", "std::vector<int>",   &ele2_recHit_hashedIndex);
  outTree_ -> Branch("ele2_recHit_ietaORix", "std::vector<int>",   &ele2_recHit_ietaORix);
  outTree_ -> Branch("ele2_recHit_iphiORiy", "std::vector<int>",   &ele2_recHit_iphiORiy);
  outTree_ -> Branch("ele2_recHit_zside", "std::vector<int>",   &ele2_recHit_zside);
  outTree_ -> Branch("ele2_recHit_laserCorrection","std::vector<float>", &ele2_recHit_laserCorrection);
  outTree_ -> Branch("ele2_recHit_ICConstant",     "std::vector<float>", &ele2_recHit_ICConstant);


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
  
  //************* VERTEXES
  edm::Handle<reco::VertexCollection> vertexes;
  iEvent.getByLabel(PVTag_, vertexes);
  PV_n = vertexes -> size();
  
  isW = -1;
  isZ = -1;

  // electron variables  
  ele1_tkP = -99.;
  ele1_fEta = -99.;
  ele1_fEtaCorr = -99.;
  ele1_e5x5 = -99.;
  ele1_e3x3 = -99.;
  ele1_scNxtal = -99.;
  ele1_scE = -99.;
  ele1_scEta = -99.;
  ele1_scPhi = -99.;
  ele1_scLaserCorr = -99.;
  ele1_5x5LaserCorr = -99.;
  ele1_3x3LaserCorr = -99.;
  ele1_es = -99.;
  ele1_seedE = -99.;
  ele1_seedLaserAlpha = -99.;
  ele1_seedLaserCorr = -99.;
  ele1_seedICConstant = -99.;
  ele1_seedIeta = -9999;
  ele1_seedIphi = -9999;
  ele1_seedIx = -9999;
  ele1_seedIy = -9999;
  ele1_seedZside = -9999;
  ele1_EOverP = -99.;
  ele1_recHit_E.clear();
  ele1_recHit_hashedIndex.clear();
  ele1_recHit_ietaORix.clear();
  ele1_recHit_iphiORiy.clear();
  ele1_recHit_zside.clear();  
  ele1_recHit_laserCorrection.clear();
  ele1_recHit_ICConstant.clear();

  ele2_tkP = -99.;
  ele2_fEta = -99.;
  ele2_fEtaCorr = -99.;
  ele2_e5x5 = -99.;
  ele2_e3x3 = -99.;
  ele2_scNxtal = -99.;
  ele2_scE = -99.;
  ele2_scEta = -99.;
  ele2_scPhi = -99.;
  ele2_scLaserCorr = -99.;
  ele2_5x5LaserCorr = -99.;
  ele2_3x3LaserCorr = -99.;
  ele2_es = -99.;
  ele2_seedE = -99.;
  ele2_seedLaserAlpha = -99.;
  ele2_seedLaserCorr = -99.;
  ele2_seedICConstant = -99.;
  ele2_seedIeta = -9999;
  ele2_seedIphi = -9999;
  ele2_seedIx = -9999;
  ele2_seedIy = -9999;
  ele2_seedZside = -9999;
  ele2_EOverP = -99.;
  ele2_recHit_E.clear();
  ele2_recHit_hashedIndex.clear();
  ele2_recHit_ietaORix.clear();
  ele2_recHit_iphiORiy.clear();
  ele2_recHit_zside.clear();
  ele2_recHit_laserCorrection.clear();
  ele2_recHit_ICConstant.clear();

  //************* ELECTRONS
  Handle<View<reco::GsfElectron> > electronHandle;
  iEvent.getByLabel(EleTag_,electronHandle);
  View<reco::GsfElectron> electrons = *electronHandle;

  ///---- get the number of the electron in the event to know if it's a W or a Z ----
  int nEle = electrons.size();
  if ( nEle == 1 ) { isW = 1; isZ = 0; }
  if ( nEle == 2 ) { isW = 0; isZ = 1; }
  
  ///---- check if the event is good----
  if ( isW == 1 ) isGoodEvent = myWselection ( iEvent, iSetup, doTighterSel_); // true = WP70, false = WP90 (already applied by Meridiani)
  if ( isZ == 1 ) isGoodEvent = myZselection ( iEvent, iSetup, doTighterSel_); // true = >1 WP80, false = 2 WP90 (already applied by Meridiani)

  ///---- fill the electron variables----
  if ( isW == 1 && isGoodEvent ) { 
    fillEleInfo ( iEvent, iSetup, 0, "ele1" ); 
  }
  if ( isZ == 1 && isGoodEvent ) { 
    fillEleInfo ( iEvent, iSetup, 0, "ele1" ); 
    fillEleInfo ( iEvent, iSetup, 1, "ele2" ); 
  }

  ///---- save the entry of the tree only if W/Z event ----
  if ( isGoodEvent )   outTree_ -> Fill();
  
}


// -----------------------------------------------------------------------------------------

bool SimpleNtupleEoverP::myWselection (const edm::Event & iEvent, const edm::EventSetup & iSetup, const bool doTighterSel)
{ 
  //*********** MET
  edm::Handle<edm::View<reco::MET> > PFmetHandle;
  iEvent.getByLabel(PFMetTag_,PFmetHandle);
  View<reco::MET>  mets = *PFmetHandle;
  reco::MET met = mets.at(0);

  //************* ELECTRONS
  Handle<View<reco::GsfElectron> > electronHandle;
  iEvent.getByLabel(EleTag_,electronHandle);
  View<reco::GsfElectron> electrons = *electronHandle;
  
  //************* RHO for isolation correction
  Handle< double > rhoHandle;
  iEvent.getByLabel(rhoTag_,rhoHandle);
  double rho = *rhoHandle;
  
  // Get the electron
  reco::GsfElectron electron = electrons.at(0);

  //Get variables used for the selection
  float pt = electron.pt();
  float eta = electron.eta();
      
  float tkIso  = electron.dr03TkSumPt();
  float emIso  = electron.dr03EcalRecHitSumEt();
  float hadIso = electron.dr03HcalDepth1TowerSumEt()+electron.dr03HcalDepth2TowerSumEt();
      
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
  
  float combIso = (tkIso + emIso + hadIso) - rho*0.3*0.3*3.14159;
  
  // Now perform W selection
  if( ( doTighterSel ) && pt < 30. ) return false;
  if( ( doTighterSel ) && fabs(eta) > 2.5 ) return false;
  // EleID WP70 - 2010
  if( ( doTighterSel ) && ( isEB == 1 ) && ( combIso/pt > 0.04 ) ) return false;
  if( ( doTighterSel ) && ( isEB == 1 ) && ( fabs(DphiIn) > 0.030 ) ) return false;
  if( ( doTighterSel ) && ( isEB == 1 ) && ( fabs(DetaIn) > 0.004 ) ) return false;
  if( ( doTighterSel ) && ( isEB == 1 ) && ( HOverE > 0.025 ) ) return false;
  if( ( doTighterSel ) && ( isEB == 1 ) && ( sigmaIetaIeta > 0.010 ) ) return false;

  if( ( doTighterSel ) && ( isEE == 1 ) && ( combIso/pt > 0.03 ) ) return false;
  if( ( doTighterSel ) && ( isEE == 1 ) && ( fabs(DphiIn) > 0.020 ) ) return false;
  if( ( doTighterSel ) && ( isEE == 1 ) && ( fabs(DetaIn) > 0.005 ) ) return false;
  if( ( doTighterSel ) && ( isEE == 1 ) && ( HOverE > 0.025 ) ) return false;
  if( ( doTighterSel ) && ( isEE == 1 ) && ( sigmaIetaIeta > 0.030 ) ) return false;
      
  if( ( doTighterSel ) && ( mishits != 0 ) ) return false;
  if( ( doTighterSel ) && ( nAmbiguousGsfTracks != 0 ) ) return false;
  if( ( doTighterSel ) && ( fabs(dist) < 0.02 ) ) return false;
  if( ( doTighterSel ) && ( fabs(dcot) < 0.02 ) ) return false;

  math::XYZTLorentzVector  met4V = met.p4();
  math::XYZTLorentzVector  ele4V = electron.p4();
  float lepMetDphi = fmod ((ele4V.Phi()-met4V.Phi()),2*3.14159265);
  float lepMetMt  = sqrt( 2* ele4V.Pt() * met4V.Et() * ( 1-cos(lepMetDphi)) );

  if( ( doTighterSel ) && met4V.Et()       < 25.00 ) return false;
  if( ( doTighterSel ) && lepMetMt   < 50.00 ) return false;
  if( ( doTighterSel ) && fabs(lepMetDphi) <  1.57 ) return false;

  return true;
  
}

// -----------------------------------------------------------------------------------------

bool SimpleNtupleEoverP::myZselection (const edm::Event & iEvent, const edm::EventSetup & iSetup, const bool doTighterSel)
{ 
  //*********** MET
  edm::Handle<edm::View<reco::MET> > PFmetHandle;
  iEvent.getByLabel(PFMetTag_,PFmetHandle);
  View<reco::MET>  mets = *PFmetHandle;
  reco::MET met = mets.at(0);

  //************* ELECTRONS
  Handle<View<reco::GsfElectron> > electronHandle;
  iEvent.getByLabel(EleTag_,electronHandle);
  View<reco::GsfElectron> electrons = *electronHandle;
  
  //************* RHO for isolation correction
  Handle< double > rhoHandle;
  iEvent.getByLabel(rhoTag_,rhoHandle);
  double rho = *rhoHandle;
  
  // Get the electrons
  reco::GsfElectron electron1 = electrons.at(0);
  reco::GsfElectron electron2 = electrons.at(1);

  // number of WP80 ele
  int nTightEle = 0;
  
  for ( unsigned int i=0; i<electrons.size(); ++i ) {
  
    reco::GsfElectron electron = electrons.at(i);
    //Get variables used for the selection
    float pt = electron.pt();
    float eta = electron.eta();
        
    float tkIso  = electron.dr03TkSumPt();
    float emIso  = electron.dr03EcalRecHitSumEt();
    float hadIso = electron.dr03HcalDepth1TowerSumEt()+electron.dr03HcalDepth2TowerSumEt();
        
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
    
    float combIso = (tkIso + emIso + hadIso) - rho*0.3*0.3*3.14159;
    
    // tight electrons
    bool isTightElectron = false;
    if(
       ( doTighterSel ) && 
       (pt > 20.) &&
       (fabs(eta) < 2.5) &&
          // EleID WP80 - 2010
       ( ( (isEB == 1) && (combIso/pt    < 0.070) ) || ( (isEE == 1) && (combIso/pt    < 0.060) ) ) &&
       ( ( (isEB == 1) && (sigmaIetaIeta < 0.010) ) || ( (isEE == 1) && (sigmaIetaIeta < 0.030) ) ) &&
       ( ( (isEB == 1) && (fabs(DphiIn)  < 0.060) ) || ( (isEE == 1) && (fabs(DphiIn)  < 0.030) ) ) &&
       ( ( (isEB == 1) && (fabs(DetaIn)  < 0.004) ) || ( (isEE == 1) && (fabs(DetaIn)  < 0.007) ) ) &&
          //( ( (isEB == 1) && (HOverE        < 0.040) ) || ( (isEB == 0) && (HOverE        < 0.025) ) ) &&
       ( mishits == 0 ) &&
       ( nAmbiguousGsfTracks == 0 ) &&
       ( ( fabs(dist) > 0.02 ) || ( fabs(dcot) > 0.02 ) )
      )
    {
      isTightElectron = true;
      ++nTightEle;
    }
  }
  
  math::XYZTLorentzVector  met4V = met.p4();
  math::XYZTLorentzVector  ele1_4V = electron1.p4();
  math::XYZTLorentzVector  ele2_4V = electron2.p4();

  if( ( doTighterSel ) && nTightEle < 1 ) return false;
  if( ( doTighterSel ) && met4V.Et()      > 40.00 ) return false;
  if( ( doTighterSel ) && ( ele1_4V + ele2_4V ).M() <  60. ) return false;
  if( ( doTighterSel ) && ( ele1_4V + ele2_4V ).M() >  120. ) return false;
  if( ( doTighterSel ) && ( electron1.charge() * electron2.charge() ) != -1. ) return false;
      
  return true;
  
}

// -----------------------------------------------------------------------------------------


void SimpleNtupleEoverP::fillEleInfo (const edm::Event & iEvent, const edm::EventSetup & iSetup, const int iEle, const std::string eleName )
{ 
   
  //*********** IC CONSTANTS
  edm::ESHandle<EcalIntercalibConstants> theICConstants;
  iSetup.get<EcalIntercalibConstantsRcd>().get(theICConstants);
  const EcalIntercalibConstantMap& ICMap = theICConstants->getMap();
  
   
 //*********** ADCToGeV
  edm::ESHandle<EcalADCToGeVConstant> theADCToGeV;
  iSetup.get<EcalADCToGeVConstantRcd>().get(theADCToGeV);

   
  //*********** LASER ALPHAS
  edm::ESHandle<EcalLaserAlphas> theEcalLaserAlphas;
  iSetup.get<EcalLaserAlphasRcd>().get(theEcalLaserAlphas);
  const EcalLaserAlphaMap* theEcalLaserAlphaMap = theEcalLaserAlphas.product();


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
 
  reco::GsfElectron electron = electrons.at(iEle);
      
  if ( eleName == "ele1" ) {
    ele1_tkP = electron.trackMomentumAtVtx().R();
          
    // supercluster variables
    reco::SuperClusterRef scRef = electron.superCluster();
    
    ele1_scNxtal = scRef->hitsAndFractions().size();
    ele1_scEta = scRef->eta();
    ele1_scPhi = scRef->phi();
    
    ele1_fEta = scRef->energy()/scRef->rawEnergy();
    
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
        // intercalib constant
        EcalIntercalibConstantMap::const_iterator ICMapIt = ICMap.find(EBDetId(id.first));
//          if( ICMapIt != ICMap.end() )
//           std::cout << "rhICConstant " << *ICMapIt << std::endl;
//          std::cout << "theADCToGeV EB " << theADCToGeV->getEBValue() << std::endl;
//          std::cout << "theADCToGeV EE " << theADCToGeV->getEEValue() << std::endl;
        ele1_seedICConstant = *ICMapIt;
        
        // laser alphas
        EcalLaserAlphaMap::const_iterator italpha = theEcalLaserAlphaMap->find(id.first);
        if( italpha != theEcalLaserAlphaMap->end() )
          ele1_seedLaserAlpha = (*italpha);

        ele1_seedLaserCorr = theLaser->getLaserCorrection(EBDetId(id.first), iEvent.time());
        ele1_seedE = ele1_seedICConstant * ele1_seedLaserCorr * it->energy();
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
        // intercalib constant
        EcalIntercalibConstantMap::const_iterator ICMapIt = ICMap.find(EEDetId(id.first));
//          if( ICMapIt != ICMap.end() )
//           std::cout << "rhICConstant " << *ICMapIt << std::endl;
//          std::cout << "theADCToGeV EB " << theADCToGeV->getEBValue() << std::endl;
//          std::cout << "theADCToGeV EE " << theADCToGeV->getEEValue() << std::endl;
        ele1_seedICConstant = *ICMapIt;
        
        // laser alphas
        EcalLaserAlphaMap::const_iterator italpha = theEcalLaserAlphaMap->find(id.first);
        if( italpha != theEcalLaserAlphaMap->end() )
          ele1_seedLaserAlpha = (*italpha);

        ele1_seedLaserCorr = theLaser->getLaserCorrection(EEDetId(id.first), iEvent.time());
        ele1_seedE = ele1_seedLaserCorr*it->energy();
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
    float sumRecHitE3x3 = 0.;
    float sumLaserCorrectionRecHitE3x3 = 0.;
    
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
      float rhIcCorrection = -1.;
      
      if ((*rh).first.subdetId()== EcalBarrel)
      {
        EBRecHitCollection::const_iterator itrechit = theBarrelEcalRecHits->find((*rh).first);
        if (itrechit==theBarrelEcalRecHits->end()) continue;
        EBDetId barrelId (itrechit->id ()); 
        ++numRecHit;
                      
        // IC correction
        EcalIntercalibConstantMap::const_iterator ICMapIt = ICMap.find(barrelId);
        rhIcCorrection = *ICMapIt;
        // laser correction
        rhLaserCorrection = theLaser->getLaserCorrection(barrelId, iEvent.time());
        sumRecHitE += itrechit->energy() * rhLaserCorrection * rhIcCorrection ;
        sumLaserCorrectionRecHitE += rhLaserCorrection * itrechit->energy() * rhLaserCorrection * rhIcCorrection;
        // check if rh is inside the 5x5 matrix
        if ( fabs(barrelId.ieta() - ele1_seedIeta) < 3 && fabs(barrelId.iphi() - ele1_seedIphi) < 3 ) {
          sumRecHitE5x5 += itrechit->energy() * rhLaserCorrection * rhIcCorrection ;
          sumLaserCorrectionRecHitE5x5 += rhLaserCorrection * itrechit->energy() * rhLaserCorrection * rhIcCorrection;
        }
        // check if rh is inside the 3x3 matrix
        if ( fabs(barrelId.ieta() - ele1_seedIeta) < 1 && fabs(barrelId.iphi() - ele1_seedIphi) < 1 ) {
          sumRecHitE3x3 += itrechit->energy() * rhLaserCorrection * rhIcCorrection ;
          sumLaserCorrectionRecHitE3x3 += rhLaserCorrection * itrechit->energy() * rhLaserCorrection * rhIcCorrection ;
        }
        // fill recHit variables
        ele1_recHit_E.push_back(itrechit->energy() * rhLaserCorrection);
        ele1_recHit_hashedIndex.push_back(barrelId.hashedIndex());
        ele1_recHit_ietaORix.push_back(barrelId.ieta());
        ele1_recHit_iphiORiy.push_back(barrelId.iphi());
        ele1_recHit_zside.push_back(0);
        ele1_recHit_laserCorrection.push_back(rhLaserCorrection);
        ele1_recHit_ICConstant.push_back(rhIcCorrection);
 
        if( printOut && itrechit->energy() > 1. )
        {
          std::cout << std::fixed
              << "    recHitLC: "    << std::setprecision(6) << std::setw(8) << rhLaserCorrection
              << "    recHitIC: "    << std::setprecision(6) << std::setw(8) << rhIcCorrection
              << std::endl;
        }
      }
        
      if ((*rh).first.subdetId()== EcalEndcap)
      {
        EERecHitCollection::const_iterator itrechit = theEndcapEcalRecHits->find((*rh).first);
        if (itrechit==theEndcapEcalRecHits->end()) continue;
        EEDetId endcapId (itrechit->id ()); 
        ++numRecHit;
          
        // IC correction
        EcalIntercalibConstantMap::const_iterator ICMapIt = ICMap.find(endcapId);
        rhIcCorrection = *ICMapIt;
        // laser correction
        rhLaserCorrection = theLaser->getLaserCorrection(endcapId, iEvent.time());
        sumRecHitE += itrechit->energy() * rhLaserCorrection * rhIcCorrection ;
        sumLaserCorrectionRecHitE += rhLaserCorrection * itrechit->energy() * rhLaserCorrection * rhIcCorrection;
        // check if rh is inside the 5x5 matrix
        if ( fabs(endcapId.ix() - ele1_seedIx) < 3 && fabs(endcapId.iy() - ele1_seedIy) < 3 ) {
          sumRecHitE5x5 += itrechit->energy() * rhLaserCorrection * rhIcCorrection ;
          sumLaserCorrectionRecHitE5x5 += rhLaserCorrection * itrechit->energy() * rhLaserCorrection * rhIcCorrection;
        }
        // check if rh is inside the 3x3 matrix
        if ( fabs(endcapId.ix() - ele1_seedIx) < 1 && fabs(endcapId.iy() - ele1_seedIy) < 1 ) {
          sumRecHitE3x3 += itrechit->energy() * rhLaserCorrection * rhIcCorrection ;
          sumLaserCorrectionRecHitE3x3 += rhLaserCorrection * itrechit->energy() * rhLaserCorrection * rhIcCorrection ;
        }
        // fill recHit variables
        ele1_recHit_E.push_back(itrechit->energy() * rhLaserCorrection);
        ele1_recHit_hashedIndex.push_back(endcapId.hashedIndex());
        ele1_recHit_ietaORix.push_back(endcapId.ix());
        ele1_recHit_iphiORiy.push_back(endcapId.iy());
        ele1_recHit_zside.push_back(endcapId.zside());
        ele1_recHit_laserCorrection.push_back(rhLaserCorrection);
        ele1_recHit_ICConstant.push_back(rhIcCorrection);

        if( printOut && itrechit->energy() > 1. )
        {
          std::cout << std::fixed
              << "    recHitLC: "    << std::setprecision(6) << std::setw(8) << rhLaserCorrection
              << "    recHitIC: "    << std::setprecision(6) << std::setw(8) << rhIcCorrection
              << std::endl;
        }
      }
    }
    // preshower variables 
    ele1_es = scRef->preshowerEnergy();
    
    ele1_scLaserCorr = sumLaserCorrectionRecHitE/sumRecHitE;
    ele1_scE = ele1_fEta*sumRecHitE;
    ele1_fEtaCorr = fClusterCorrections(sumRecHitE+ele1_es,ele1_scEta,scRef->phiWidth()/scRef->etaWidth(),params)/fClusterCorrections(scRef->rawEnergy()+ele1_es,ele1_scEta,scRef->phiWidth()/scRef->etaWidth(),params);
        
    ele1_5x5LaserCorr = sumLaserCorrectionRecHitE5x5/sumRecHitE5x5;
    ele1_e5x5 = sumRecHitE5x5;
  
    ele1_3x3LaserCorr = sumLaserCorrectionRecHitE3x3/sumRecHitE3x3;
    ele1_e3x3 = sumRecHitE3x3;

    // EsuP variables
    ele1_EOverP = ele1_scE/ele1_tkP;
  }

  if ( eleName == "ele2" ) {
    ele2_tkP = electron.trackMomentumAtVtx().R();
          
    // supercluster variables
    reco::SuperClusterRef scRef = electron.superCluster();
    
    ele2_scNxtal = scRef->hitsAndFractions().size();
    ele2_scEta = scRef->eta();
    ele2_scPhi = scRef->phi();
    
    ele2_fEta = scRef->energy()/scRef->rawEnergy();
    
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
        // intercalib constant
        EcalIntercalibConstantMap::const_iterator ICMapIt = ICMap.find(EBDetId(id.first));
//          if( ICMapIt != ICMap.end() )
//           std::cout << "rhICConstant " << *ICMapIt << std::endl;
//          std::cout << "theADCToGeV EB " << theADCToGeV->getEBValue() << std::endl;
//          std::cout << "theADCToGeV EE " << theADCToGeV->getEEValue() << std::endl;
        ele2_seedICConstant = *ICMapIt;
        
        // laser alphas
        EcalLaserAlphaMap::const_iterator italpha = theEcalLaserAlphaMap->find(id.first);
        if( italpha != theEcalLaserAlphaMap->end() )
          ele2_seedLaserAlpha = (*italpha);

        ele2_seedLaserCorr = theLaser->getLaserCorrection(EBDetId(id.first), iEvent.time());
        ele2_seedE = ele2_seedICConstant * ele2_seedLaserCorr * it->energy();
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
        // intercalib constant
        EcalIntercalibConstantMap::const_iterator ICMapIt = ICMap.find(EEDetId(id.first));
//          if( ICMapIt != ICMap.end() )
//           std::cout << "rhICConstant " << *ICMapIt << std::endl;
//          std::cout << "theADCToGeV EB " << theADCToGeV->getEBValue() << std::endl;
//          std::cout << "theADCToGeV EE " << theADCToGeV->getEEValue() << std::endl;
        ele2_seedICConstant = *ICMapIt;
        
        // laser alphas
        EcalLaserAlphaMap::const_iterator italpha = theEcalLaserAlphaMap->find(id.first);
        if( italpha != theEcalLaserAlphaMap->end() )
          ele2_seedLaserAlpha = (*italpha);

        ele2_seedLaserCorr = theLaser->getLaserCorrection(EEDetId(id.first), iEvent.time());
        ele2_seedE = ele2_seedLaserCorr*it->energy();
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
    int numRecHit = 0;
    float sumRecHitE = 0.;
    float sumLaserCorrectionRecHitE = 0.;
    float sumRecHitE5x5 = 0.;
    float sumLaserCorrectionRecHitE5x5 = 0.;
    float sumRecHitE3x3 = 0.;
    float sumLaserCorrectionRecHitE3x3 = 0.;
    
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
      float rhIcCorrection = -1.;
      
      if ((*rh).first.subdetId()== EcalBarrel)
      {
        EBRecHitCollection::const_iterator itrechit = theBarrelEcalRecHits->find((*rh).first);
        if (itrechit==theBarrelEcalRecHits->end()) continue;
        EBDetId barrelId (itrechit->id ()); 
        ++numRecHit;
                      
        // IC correction
        EcalIntercalibConstantMap::const_iterator ICMapIt = ICMap.find(barrelId);
        rhIcCorrection = *ICMapIt;
        // laser correction
        rhLaserCorrection = theLaser->getLaserCorrection(barrelId, iEvent.time());
        sumRecHitE += itrechit->energy() * rhLaserCorrection * rhIcCorrection ;
        sumLaserCorrectionRecHitE += rhLaserCorrection * itrechit->energy() * rhLaserCorrection * rhIcCorrection;
        // check if rh is inside the 5x5 matrix
        if ( fabs(barrelId.ieta() - ele2_seedIeta) < 3 && fabs(barrelId.iphi() - ele2_seedIphi) < 3 ) {
          sumRecHitE5x5 += itrechit->energy() * rhLaserCorrection * rhIcCorrection ;
          sumLaserCorrectionRecHitE5x5 += rhLaserCorrection * itrechit->energy() * rhLaserCorrection * rhIcCorrection;
        }
        // check if rh is inside the 3x3 matrix
        if ( fabs(barrelId.ieta() - ele2_seedIeta) < 1 && fabs(barrelId.iphi() - ele2_seedIphi) < 1 ) {
          sumRecHitE3x3 += itrechit->energy() * rhLaserCorrection * rhIcCorrection ;
          sumLaserCorrectionRecHitE3x3 += rhLaserCorrection * itrechit->energy() * rhLaserCorrection * rhIcCorrection ;
        }
        // fill recHit variables
        ele2_recHit_E.push_back(itrechit->energy() * rhLaserCorrection * rhIcCorrection);
        ele2_recHit_hashedIndex.push_back(barrelId.hashedIndex());
        ele2_recHit_ietaORix.push_back(barrelId.ieta());
        ele2_recHit_iphiORiy.push_back(barrelId.iphi());
        ele2_recHit_zside.push_back(0);
        ele2_recHit_laserCorrection.push_back(rhLaserCorrection);
        ele2_recHit_ICConstant.push_back(rhIcCorrection);
 
        if( printOut && itrechit->energy() > 1. )
        {
          std::cout << std::fixed
              << "    recHitLC: "    << std::setprecision(6) << std::setw(8) << rhLaserCorrection
              << "    recHitIC: "    << std::setprecision(6) << std::setw(8) << rhIcCorrection
              << std::endl;
        }
      }
        
      if ((*rh).first.subdetId()== EcalEndcap)
      {
        EERecHitCollection::const_iterator itrechit = theEndcapEcalRecHits->find((*rh).first);
        if (itrechit==theEndcapEcalRecHits->end()) continue;
        EEDetId endcapId (itrechit->id ()); 
        ++numRecHit;
          
        // IC correction
        EcalIntercalibConstantMap::const_iterator ICMapIt = ICMap.find(endcapId);
        rhIcCorrection = *ICMapIt;
        // laser correction
        rhLaserCorrection = theLaser->getLaserCorrection(endcapId, iEvent.time());
        sumRecHitE += itrechit->energy() * rhLaserCorrection * rhIcCorrection ;
        sumLaserCorrectionRecHitE += rhLaserCorrection * itrechit->energy() * rhLaserCorrection * rhIcCorrection;
        // check if rh is inside the 5x5 matrix
        if ( fabs(endcapId.ix() - ele2_seedIx) < 3 && fabs(endcapId.iy() - ele2_seedIy) < 3 ) {
          sumRecHitE5x5 += itrechit->energy() * rhLaserCorrection * rhIcCorrection ;
          sumLaserCorrectionRecHitE5x5 += rhLaserCorrection * itrechit->energy() * rhLaserCorrection * rhIcCorrection;
        }
        // check if rh is inside the 3x3 matrix
        if ( fabs(endcapId.ix() - ele2_seedIx) < 1 && fabs(endcapId.iy() - ele2_seedIy) < 1 ) {
          sumRecHitE3x3 += itrechit->energy() * rhLaserCorrection * rhIcCorrection ;
          sumLaserCorrectionRecHitE3x3 += rhLaserCorrection * itrechit->energy() * rhLaserCorrection * rhIcCorrection ;
        }
        // fill recHit variables
        ele2_recHit_E.push_back(itrechit->energy() * rhLaserCorrection * rhIcCorrection);
        ele2_recHit_hashedIndex.push_back(endcapId.hashedIndex());
        ele2_recHit_ietaORix.push_back(endcapId.ix());
        ele2_recHit_iphiORiy.push_back(endcapId.iy());
        ele2_recHit_zside.push_back(endcapId.zside());
        ele2_recHit_laserCorrection.push_back(rhLaserCorrection);
        ele2_recHit_ICConstant.push_back(rhIcCorrection);

        if( printOut && itrechit->energy() > 1. )
        {
          std::cout << std::fixed
              << "    recHitLC: "    << std::setprecision(6) << std::setw(8) << rhLaserCorrection
              << "    recHitIC: "    << std::setprecision(6) << std::setw(8) << rhIcCorrection
              << std::endl;
        }
      }
    }
    
    // preshower variables 
    ele2_es = scRef->preshowerEnergy();
    
    ele2_scLaserCorr = sumLaserCorrectionRecHitE/sumRecHitE;
    ele2_scE = ele2_fEta*sumRecHitE;
    ele2_fEtaCorr = fClusterCorrections(sumRecHitE+ele2_es,ele2_scEta,scRef->phiWidth()/scRef->etaWidth(),params)/fClusterCorrections(scRef->rawEnergy()+ele2_es,ele2_scEta,scRef->phiWidth()/scRef->etaWidth(),params);
        
    ele2_5x5LaserCorr = sumLaserCorrectionRecHitE5x5/sumRecHitE5x5;
    ele2_e5x5 = sumRecHitE5x5;
  
    ele2_3x3LaserCorr = sumLaserCorrectionRecHitE3x3/sumRecHitE3x3;
    ele2_e3x3 = sumRecHitE3x3;

    // EsuP variables
    ele2_EOverP = ele2_scE/ele2_tkP;
  }

}


// -----------------------------------------------------------------------------------------


DEFINE_FWK_MODULE(SimpleNtupleEoverP);
