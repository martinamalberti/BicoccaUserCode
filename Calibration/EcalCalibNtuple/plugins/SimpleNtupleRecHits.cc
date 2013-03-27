// -*- C++ -*-
//
// Package:   SimpleNtupleRecHits
// Class:     SimpleNtupleRecHits
//
#include "Calibration/EcalCalibNtuple/plugins/SimpleNtupleRecHits.h"

#include "Math/Vector4D.h"
#include "Math/Vector3D.h"

#define PI 3.141592654
#define TWOPI 6.283185308



SimpleNtupleRecHits::SimpleNtupleRecHits(const edm::ParameterSet& iConfig)
{
  std::cout<< ">>> SimpleNtupleRecHits::SimpleNtupleRecHits begin <<<" << std::endl;
  
  
  edm::Service<TFileService> fs ;
  outTree_  = fs -> make <TTree>("SimpleNtupleEoverP","SimpleNtupleEoverP"); 
  
  
  //---- tags ----
  PVTag_               = iConfig.getParameter<edm::InputTag>("PVTag");
  rhoTag_              = iConfig.getParameter<edm::InputTag>("rhoTag");
  MCPileupTag_         = iConfig.getParameter<edm::InputTag>("MCPileupTag");
  digiCollection_EB_   = iConfig.getParameter<edm::InputTag>("digiCollection_EB");
  digiCollection_EE_   = iConfig.getParameter<edm::InputTag>("digiCollection_EE");
  recHitCollection_EB_ = iConfig.getParameter<edm::InputTag>("recHitCollection_EB");
  recHitCollection_EE_ = iConfig.getParameter<edm::InputTag>("recHitCollection_EE");
  
  
  //---- flags ----
  saveMCPU_     = iConfig.getUntrackedParameter<bool>("saveMCPU", false);
  verbosity_    = iConfig.getUntrackedParameter<bool>("verbosity", false);
  dataFlag_     = iConfig.getUntrackedParameter<bool>("dataFlag", true) ;
  eventNaiveId_ = 0;
  
  
  //---- Initialize tree branches ----
  // event variables
  outTree_ -> Branch("runId",         &runId,                 "runId/I");
  outTree_ -> Branch("lumiId",        &lumiId,               "lumiId/I");
  outTree_ -> Branch("eventId",       &eventId,             "eventId/L");
  outTree_ -> Branch("bxId",          &bxId,                   "bxId/L");
  outTree_ -> Branch("timeStampHigh", &timeStampHigh, "timeStampHigh/I");
  
  outTree_ -> Branch("PV_n",     &PV_n,         "PV_n/I");
  outTree_ -> Branch("PV_z",     &PV_z,         "PV_z/F");
  outTree_ -> Branch("PV_d0",    &PV_d0,       "PV_d0/F");
  outTree_ -> Branch("PV_SumPt", &PV_SumPt, "PV_SumPt/F");

  outTree_ -> Branch("rho",   &rho,  "rho/F");
  
  // recHit variables
  outTree_ -> Branch("EBRecHit_E",               "std::vector<float>", &EBRecHit_E);
  outTree_ -> Branch("EBRecHit_flag",            "std::vector<int>",   &EBRecHit_flag);
  outTree_ -> Branch("EBRecHit_hashedIndex",     "std::vector<int>",   &EBRecHit_hashedIndex);
  outTree_ -> Branch("EBRecHit_ietaORix",        "std::vector<int>",   &EBRecHit_ietaORix);
  outTree_ -> Branch("EBRecHit_iphiORiy",        "std::vector<int>",   &EBRecHit_iphiORiy);
  outTree_ -> Branch("EBRecHit_zside",           "std::vector<int>",   &EBRecHit_zside);
  outTree_ -> Branch("EBRecHit_laserCorrection", "std::vector<float>", &EBRecHit_laserCorrection);
  outTree_ -> Branch("EBRecHit_ICConstant",      "std::vector<float>", &EBRecHit_ICConstant);
  outTree_ -> Branch("EBRecHit_samples",         "std::vector<float>", &EBRecHit_samples);
  
  outTree_ -> Branch("EERecHit_E",               "std::vector<float>", &EERecHit_E);
  outTree_ -> Branch("EERecHit_flag",            "std::vector<int>",   &EERecHit_flag);
  outTree_ -> Branch("EERecHit_hashedIndex",     "std::vector<int>",   &EERecHit_hashedIndex);
  outTree_ -> Branch("EERecHit_ietaORix",        "std::vector<int>",   &EERecHit_ietaORix);
  outTree_ -> Branch("EERecHit_iphiORiy",        "std::vector<int>",   &EERecHit_iphiORiy);
  outTree_ -> Branch("EERecHit_zside",           "std::vector<int>",   &EERecHit_zside);
  outTree_ -> Branch("EERecHit_laserCorrection", "std::vector<float>", &EERecHit_laserCorrection);
  outTree_ -> Branch("EERecHit_ICConstant",      "std::vector<float>", &EERecHit_ICConstant);
  outTree_ -> Branch("EERecHit_samples",         "std::vector<float>", &EERecHit_samples);
    
  // pileup variables
  if(saveMCPU_)
  {
    outTree_ -> Branch("PUit_TrueNumInteractions", &PUit_TrueNumInteractions, "PUit_TrueNumInteractions/F");
    outTree_ -> Branch("PUit_NumInteractions",     &PUit_NumInteractions,         "PUit_NumInteractions/I");
    outTree_ -> Branch("PUit_zpositions",          &PUit_zpositions,                   "PUit_zpositions/F");
    outTree_ -> Branch("PUit_sumpT_lowpT",         &PUit_sumpT_lowpT,                 "PUit_sumpT_lowpT/F");
    outTree_ -> Branch("PUit_sumpT_highpT",        &PUit_sumpT_highpT,               "PUit_sumpT_highpT/F");
    outTree_ -> Branch("PUit_ntrks_lowpT",         &PUit_ntrks_lowpT,                 "PUit_ntrks_lowpT/F");
    outTree_ -> Branch("PUit_ntrks_highpT",        &PUit_ntrks_highpT,               "PUit_ntrks_highpT/F");
    
    outTree_ -> Branch("PUoot_early_TrueNumInteractions", &PUoot_early_TrueNumInteractions, "PUoot_early_TrueNumInteractions/F");
    outTree_ -> Branch("PUoot_early",                     &PUoot_early,                                         "PUoot_early/I");
    outTree_ -> Branch("PUoot_early_zpositions",          &PUoot_early_zpositions,                   "PUoot_early_zpositions/F");
    outTree_ -> Branch("PUoot_early_sumpT_lowpT",         &PUoot_early_sumpT_lowpT,                 "PUoot_early_sumpT_lowpT/F");
    outTree_ -> Branch("PUoot_early_sumpT_highpT",        &PUoot_early_sumpT_highpT,               "PUoot_early_sumpT_highpT/F");
    outTree_ -> Branch("PUoot_early_ntrks_lowpT",         &PUoot_early_ntrks_lowpT,                 "PUoot_early_ntrks_lowpT/F");
    outTree_ -> Branch("PUoot_early_ntrks_highpT",        &PUoot_early_ntrks_highpT,               "PUoot_early_ntrks_highpT/F");
    
    outTree_ -> Branch("PUoot_late_TrueNumInteractions", &PUoot_late_TrueNumInteractions, "PUoot_late_TrueNumInteractions/F");
    outTree_ -> Branch("PUoot_late",                     &PUoot_late,                                         "PUoot_late/I");
    outTree_ -> Branch("PUoot_late_zpositions",          &PUoot_late_zpositions,                   "PUoot_late_zpositions/F");
    outTree_ -> Branch("PUoot_late_sumpT_lowpT",         &PUoot_late_sumpT_lowpT,                 "PUoot_late_sumpT_lowpT/F");
    outTree_ -> Branch("PUoot_late_sumpT_highpT",        &PUoot_late_sumpT_highpT,               "PUoot_late_sumpT_highpT/F");
    outTree_ -> Branch("PUoot_late_ntrks_lowpT",         &PUoot_late_ntrks_lowpT,                 "PUoot_late_ntrks_lowpT/F");
    outTree_ -> Branch("PUoot_late_ntrks_highpT",        &PUoot_late_ntrks_highpT,               "PUoot_late_ntrks_highpT/F");
  }
  
  
  std::cout<< ">>> SimpleNtupleRecHits::SimpleNtupleRecHits end <<<" << std::endl;
}



// --------------------------------------------------------------------



SimpleNtupleRecHits::~SimpleNtupleRecHits()
{
  std::cout<< ">>> SimpleNtupleRecHits::~SimpleNtupleRecHits <<< analyzed " <<  eventNaiveId_ << " events" << std::endl;
  // save tree
  // outTree_ -> Write();
}



// -----------------------------------------------------------------------------------------



void SimpleNtupleRecHits::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  if( verbosity_ )
    std::cout<< ">>> SimpleNtupleRecHits::analyze begin <<<" << std::endl;
  
  
  ++eventNaiveId_;
  
  
  // event variables 
  runId = iEvent.id().run();
  lumiId = iEvent.luminosityBlock();
  eventId = iEvent.id().event();
  bxId = iEvent.bunchCrossing();
  timeStampHigh = (int)(iEvent.time().value() >> 32);
  
  PUit_TrueNumInteractions= -99.;
  PUit_NumInteractions= -99;
  PUit_zpositions = -99.;
  PUit_sumpT_lowpT = -99.;
  PUit_sumpT_highpT = -99.;
  PUit_ntrks_lowpT = -99.;
  PUit_ntrks_highpT = -99.;

  PUoot_early_TrueNumInteractions = -99.;
  PUoot_early = -99;
  PUoot_early_zpositions = -99.;
  PUoot_early_sumpT_lowpT = -99.;
  PUoot_early_sumpT_highpT = -99.;
  PUoot_early_ntrks_lowpT = -99.;
  PUoot_early_ntrks_highpT = -99.;

  PUoot_late_TrueNumInteractions = -99.;
  PUoot_late = -99;
  PUoot_late_zpositions = -99.;
  PUoot_late_sumpT_lowpT = -99.;
  PUoot_late_sumpT_highpT = -99.; 
  PUoot_late_ntrks_lowpT = -99.;
  PUoot_late_ntrks_highpT = -99.;
  
  EBRecHit_E.clear();
  EBRecHit_flag.clear();
  EBRecHit_hashedIndex.clear();
  EBRecHit_ietaORix.clear();
  EBRecHit_iphiORiy.clear();
  EBRecHit_zside.clear();
  EBRecHit_laserCorrection.clear();
  EBRecHit_ICConstant.clear();
  EBRecHit_samples.clear();
  
  EERecHit_E.clear();
  EERecHit_flag.clear();
  EERecHit_hashedIndex.clear();
  EERecHit_ietaORix.clear();
  EERecHit_iphiORiy.clear();
  EERecHit_zside.clear();
  EERecHit_laserCorrection.clear();
  EERecHit_ICConstant.clear();
  EERecHit_samples.clear();
  
  
  
  //************* VERTEXES
  fillPVInfo(iEvent,iSetup);
  
  //************* RHO for ISO
  //fillRhoInfo(iEvent,iSetup);
  
  //************* MC PU
  if(saveMCPU_) fillMCPUInfo(iEvent,iSetup);
  
  //************* REC HITS
  fillRecHitsInfo(iEvent,iSetup); 
  
  
  outTree_ -> Fill();
  
  
  if( verbosity_ )
    std::cout<< ">>> SimpleNtupleRecHits::analyze end <<<" << std::endl;
}



// -----------------------------------------------------------------------------------------


 
void SimpleNtupleRecHits::fillPVInfo(const edm::Event& iEvent, const edm::EventSetup& iESetup)
{
  if( verbosity_ )
    std::cout<< ">>> SimpleNtupleRecHits::fillPVInfo start <<<" << std::endl;
  
  edm::Handle<reco::VertexCollection> vertexes;
  iEvent.getByLabel(PVTag_,vertexes);
  PV_n = vertexes -> size();
  
  reco::Vertex PV;
  bool PVfound = (vertexes -> size() != 0);
  if(PVfound)
  {
    PV = vertexes->at(0);
    PV_z = PV.z();
    PV_d0 = PV.position().Rho();
  }
  else
  {
    //creating a dummy PV
    PV_z=-999.;
    PV_d0=-999.;
  }  
  
  math::XYZPoint PVPoint(PV.position().x(), PV.position().y(), PV.position().z());
  PVPoint_ = PVPoint;
  
  if( verbosity_ )
    std::cout<< ">>> SimpleNtupleRecHits::fillPVInfo end <<<" << std::endl;
}



//------------------------------------------------------------------------------------------------------------



void SimpleNtupleRecHits::fillRhoInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  if( verbosity_ )
    std::cout<< ">>> SimpleNtupleRecHits::fillRhoInfo start <<<" << std::endl;
  
  edm::Handle<double> rhoHandle;
  iEvent.getByLabel(rhoTag_,rhoHandle);
  rho = *rhoHandle;  
  
  if( verbosity_ )
    std::cout<< ">>> SimpleNtupleRecHits::fillRhoInfo end <<<" << std::endl;
}



// ------------------------------------------------------------------------------------------------------------



void SimpleNtupleRecHits::fillRecHitsInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  if( verbosity_ )
    std::cout<< ">>> SimpleNtupleRecHits::fillRecHitsInfo start <<<" << std::endl;
  
  
  //*********** IC CONSTANTS
  edm::ESHandle<EcalIntercalibConstants> theICConstants;
  iSetup.get<EcalIntercalibConstantsRcd>().get(theICConstants);
  const EcalIntercalibConstantMap& ICMap = theICConstants->getMap();
  
  //*********** ADCToGeV
  edm::ESHandle<EcalADCToGeVConstant> theADCToGeV;
  iSetup.get<EcalADCToGeVConstantRcd>().get(theADCToGeV);
  
  //*********** LASER CORRECTION
  edm::ESHandle<EcalLaserDbService> theLaser;
  iSetup.get<EcalLaserDbRecord>().get(theLaser);
  
  //*********** EB DIGI HITS
  edm::Handle<EBDigiCollection> ebDigis;
  iEvent.getByLabel(digiCollection_EB_,ebDigis);
  edm::Handle<EEDigiCollection> eeDigis;
  iEvent.getByLabel(digiCollection_EE_,eeDigis);
  
  //*********** EB REC HITS
  edm::Handle<EcalRecHitCollection> recHitsEB;
  iEvent.getByLabel(recHitCollection_EB_,recHitsEB);
  const EcalRecHitCollection* theBarrelEcalRecHits = recHitsEB.product();
  
  //*********** EE REC HITS
  edm::Handle<EcalRecHitCollection> recHitsEE;
  iEvent.getByLabel(recHitCollection_EE_,recHitsEE);
  const EcalRecHitCollection* theEndcapEcalRecHits = recHitsEE.product();
  
  
  
  //--------
  // fill EB
  
  float theLaserCorrection = -1.;
  float theICCorrection = -1.;
  
  for(EcalRecHitCollection::const_iterator itRecHit = theBarrelEcalRecHits->begin(); itRecHit != theBarrelEcalRecHits->end(); ++itRecHit) 
  {
    EBDetId barrelId = itRecHit -> id();
    
    bool digiFound = false;
    for(EBDigiCollection::const_iterator digiItr = ebDigis->begin(); digiItr != ebDigis->end(); ++digiItr)
    {
      if(digiItr->id() != barrelId ) continue;
      digiFound = true;
      EcalDataFrame df = *digiItr;
      for(int iSample = 0; iSample < 10; ++iSample)
        EBRecHit_samples.push_back(df.sample(iSample).adc());
      }
    if( digiFound == false ) continue;
    
    // laser correction
    theLaserCorrection = theLaser->getLaserCorrection(barrelId, iEvent.time());
    // IC correction
    EcalIntercalibConstantMap::const_iterator ICMapIt = ICMap.find(barrelId);
    theICCorrection = *ICMapIt;
    
    EBRecHit_E.push_back(itRecHit->energy());
    EBRecHit_flag.push_back(itRecHit->recoFlag());
    EBRecHit_hashedIndex.push_back(barrelId.hashedIndex());
    EBRecHit_ietaORix.push_back(barrelId.ieta());
    EBRecHit_iphiORiy.push_back(barrelId.iphi());
    EBRecHit_zside.push_back(0);
    EBRecHit_laserCorrection.push_back(theLaserCorrection);
    EBRecHit_ICConstant.push_back(theICCorrection);
  }
  
  for(EcalRecHitCollection::const_iterator itRecHit = theEndcapEcalRecHits->begin(); itRecHit != theEndcapEcalRecHits->end(); ++itRecHit) 
  {
    EEDetId endcapId = itRecHit -> id();
    
    bool digiFound = false;
    for(EEDigiCollection::const_iterator digiItr = eeDigis->begin(); digiItr != eeDigis->end(); ++digiItr)
    {
      if(digiItr->id() != endcapId ) continue;
      digiFound = true;
      EcalDataFrame df = *digiItr;
      for(int iSample = 0; iSample < 10; ++iSample)
        EERecHit_samples.push_back(df.sample(iSample).adc());
      }
    if( digiFound == false ) continue;
    
    // laser correction
    theLaserCorrection = theLaser->getLaserCorrection(endcapId, iEvent.time());
    // IC correction
    EcalIntercalibConstantMap::const_iterator ICMapIt = ICMap.find(endcapId);
    theICCorrection = *ICMapIt;
    
    EERecHit_E.push_back(itRecHit->energy());
    EERecHit_flag.push_back(itRecHit->recoFlag());
    EERecHit_hashedIndex.push_back(endcapId.hashedIndex());
    EERecHit_ietaORix.push_back(endcapId.ix());
    EERecHit_iphiORiy.push_back(endcapId.iy());
    EERecHit_zside.push_back(endcapId.zside());
    EERecHit_laserCorrection.push_back(theLaserCorrection);
    EERecHit_ICConstant.push_back(theICCorrection);
  }
  
  
  
  if( verbosity_ )
    std::cout<< ">>> SimpleNtupleRecHits::fillRecHitsInfo end <<<" << std::endl;
}



// -----------------------------------------------------------------------------------------



void SimpleNtupleRecHits::fillMCPUInfo(const edm::Event & iEvent, const edm::EventSetup & iSetup) 
{
  if( verbosity_ )
    std::cout<< ">>> SimpleNtupleRecHits::fillMCPUInfo start <<<" << std::endl;
  
  edm::Handle<std::vector<PileupSummaryInfo> > PupInfo;
  iEvent.getByLabel(MCPileupTag_, PupInfo);
  
  
  // loop on BX
  std::vector<PileupSummaryInfo>::const_iterator PVI;
  for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI)
  {
    std::vector<float> temp_mc_PU_zpositions   = PVI->getPU_zpositions();
    std::vector<float> temp_mc_PU_sumpT_lowpT  = PVI->getPU_sumpT_lowpT();
    std::vector<float> temp_mc_PU_sumpT_highpT = PVI->getPU_sumpT_highpT();
    std::vector<int> temp_mc_PU_ntrks_lowpT    = PVI->getPU_ntrks_lowpT();
    std::vector<int> temp_mc_PU_ntrks_highpT   = PVI->getPU_ntrks_highpT();
    
    // in-time pileup
    if( PVI->getBunchCrossing() == 0 )
    {
      PUit_TrueNumInteractions = PVI->getTrueNumInteractions();
      PUit_NumInteractions = PVI->getPU_NumInteractions();    
      
      for(std::vector<float>::const_iterator it = temp_mc_PU_zpositions.begin(); it < temp_mc_PU_zpositions.end(); ++it)
        PUit_zpositions = *it;
      
      for(std::vector<float>::const_iterator it = temp_mc_PU_sumpT_lowpT.begin(); it < temp_mc_PU_sumpT_lowpT.end(); ++it)
        PUit_sumpT_lowpT=*it;
      
      for(std::vector<float>::const_iterator it = temp_mc_PU_sumpT_highpT.begin(); it < temp_mc_PU_sumpT_highpT.end(); ++it)
        PUit_sumpT_highpT = *it;
      
      for(std::vector<int>::const_iterator it = temp_mc_PU_ntrks_lowpT.begin(); it < temp_mc_PU_ntrks_lowpT.end(); ++it)
        PUit_ntrks_lowpT = *it;
      
      for(std::vector<int>::const_iterator it = temp_mc_PU_ntrks_highpT.begin(); it < temp_mc_PU_ntrks_highpT.end(); ++it)
        PUit_ntrks_highpT = *it;
    }
    
    // out-of-time pileup
    else
    {
      if (PVI->getBunchCrossing() < 0)
      {
        PUoot_early_TrueNumInteractions = PVI->getTrueNumInteractions();
        PUoot_early = PVI->getPU_NumInteractions();
        
        for(std::vector<float>::const_iterator it = temp_mc_PU_zpositions.begin(); it < temp_mc_PU_zpositions.end(); ++it)
          PUoot_early_zpositions = *it;
        
        for(std::vector<float>::const_iterator it = temp_mc_PU_sumpT_lowpT.begin(); it < temp_mc_PU_sumpT_lowpT.end(); ++it)
          PUoot_early_sumpT_lowpT = *it;
        
        for(std::vector<float>::const_iterator it = temp_mc_PU_sumpT_highpT.begin(); it < temp_mc_PU_sumpT_highpT.end(); ++it)
          PUoot_early_sumpT_highpT = *it;
        
        for(std::vector<int>::const_iterator it = temp_mc_PU_ntrks_lowpT.begin(); it < temp_mc_PU_ntrks_lowpT.end(); ++it)
          PUoot_early_ntrks_lowpT = *it;
        
        for(std::vector<int>::const_iterator it = temp_mc_PU_ntrks_highpT.begin(); it < temp_mc_PU_ntrks_highpT.end(); ++it)
          PUoot_early_ntrks_highpT = *it;
      }
      else
      {
        PUoot_late_TrueNumInteractions = PVI->getTrueNumInteractions();
        PUoot_late = PVI->getPU_NumInteractions();
        
        for(std::vector<float>::const_iterator it = temp_mc_PU_zpositions.begin(); it < temp_mc_PU_zpositions.end(); ++it)
          PUoot_late_zpositions = *it;
        
        for(std::vector<float>::const_iterator it = temp_mc_PU_sumpT_lowpT.begin(); it < temp_mc_PU_sumpT_lowpT.end(); ++it)
          PUoot_late_sumpT_lowpT = *it;
        
        for(std::vector<float>::const_iterator it = temp_mc_PU_sumpT_highpT.begin(); it < temp_mc_PU_sumpT_highpT.end(); ++it)
          PUoot_late_sumpT_highpT = *it;
        
        for(std::vector<int>::const_iterator it = temp_mc_PU_ntrks_lowpT.begin(); it < temp_mc_PU_ntrks_lowpT.end(); ++it)
          PUoot_late_ntrks_lowpT = *it;
        
        for(std::vector<int>::const_iterator it = temp_mc_PU_ntrks_highpT.begin(); it < temp_mc_PU_ntrks_highpT.end(); ++it)
          PUoot_late_ntrks_highpT = *it;          
      }
    }
  } // loop on BX
   
  if( verbosity_ )
    std::cout<< ">>> SimpleNtupleRecHits::fillMCPUInfo end <<<" << std::endl;
}



//----------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(SimpleNtupleRecHits);
