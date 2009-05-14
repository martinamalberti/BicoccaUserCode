#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFMCH4jAnalysis.h"

#define PI 3.141592654

using namespace vbfhww2l;



VBFMCH4jAnalysis::VBFMCH4jAnalysis(const edm::ParameterSet& iConfig):
  srcHepMCProduct_p          (iConfig.getParameter<edm::InputTag>("srcHepMCProduct")),
 srcGenParticles_p          (iConfig.getParameter<edm::InputTag>("srcGenParticles")),
 srcGenMet_p                (iConfig.getParameter<edm::InputTag>("srcGenMet")),
 srcIC5GenJets_p            (iConfig.getParameter<edm::InputTag>("srcIC5GenJets")),
 srcIC5CaloJets_p           (iConfig.getParameter<edm::InputTag>("srcIC5CaloJets")),
 fileName_p                 (iConfig.getUntrackedParameter<std::string> ("fileName", std::string("VBFMCH4jAnalysis"))),
 mass_p                     (iConfig.getParameter<std::string>("mass")),
 verbosity_p                (iConfig.getUntrackedParameter<bool>("verbosity", false)),
 eventsToPrint_p            (iConfig.getUntrackedParameter<int>("eventsToPrint", 0)),
 eventId_p(0),
 naiveId_p(0)
{}



 VBFMCH4jAnalysis::~VBFMCH4jAnalysis()
 {}






 void VBFMCH4jAnalysis::beginJob(const edm::EventSetup& iSetup)
 {
  if(verbosity_p)
   std::cerr << "*** VBFMCH4jAnalysis::beginJob ***" << std::endl;


  // Create File
//   std::string path = "/afs/cern.ch/user/a/abenagli/scratch0/VBF/CMSSW_2_2_7/src/HiggsAnalysis/VBFHiggsToWWto2l2nu/test/";
  std::string path = "/tmp/amassiro/";
  std::string outFileName = path+fileName_p+"_H"+mass_p+".root";
  outFile_p = new TFile(outFileName.c_str(), "RECREATE");
  
  
  
  // -----------------------------------------------------------------
  // define histograms
  // -----------------------------------------------------------------

  mcV1V2_eta = new TH2F("mcV1V2_eta", "mcV1V2_eta", 1000, -5., 5., 1000, -5., 5.);
  mcVPTFDeta = new TH2F("mcVPTFDeta", "mcVPTFDeta", 1000, 0., 10., 1000, 0., 1000.);
  
 }



 void VBFMCH4jAnalysis::endJob()
 {
  if(verbosity_p)
   std::cerr << "*** VBFMCH4jAnalysis::endJob ***" << std::endl;

  // Save histograms
  outFile_p -> cd();
  //TDirectory* sd1;
  
  
  mcV1V2_eta -> Write();
  mcVPTFDeta -> Write();
  
  outFile_p -> Close();
 }






 void VBFMCH4jAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
 {
  if(verbosity_p)
   std::cerr << "*** VBFMCH4jAnalysis::analyze ***" << std::endl;
  
  
  // set event id
  eventId_p = (iEvent.id()).event();
  ++naiveId_p;
  
  if( (naiveId_p%100 == 0) && (verbosity_p == true) )
   std::cout << ">>>>>> VBFMCH4jAnalysis   Analyze event number: " << naiveId_p << std::endl;


  edm::Handle<reco::GenParticleCollection> hGenParticles;
  iEvent.getByLabel(srcGenParticles_p, hGenParticles);
  if( !(hGenParticles.isValid ()) )
  {
   std::cerr << ">>>>>> VBFMCH4jAnalysis::Warning: " << srcGenParticles_p << " not available" << std::endl;
   return;
  }

  edm::ESHandle<ParticleDataTable> hPdt;
  iSetup.getData(hPdt);

  edm::Handle<reco::GenMETCollection> hGenMet;
  iEvent.getByLabel(srcGenMet_p, hGenMet);
  if( !(hGenMet.isValid ()) )
  {
   std::cerr << ">>>>>> VBFMCH4jAnalysis::Warning: " << srcGenMet_p << " not available" << std::endl;
   return;
  }
  
  edm::Handle<reco::GenJetCollection> hIC5GenJets;
  iEvent.getByLabel(srcIC5GenJets_p, hIC5GenJets);
  if( !(hIC5GenJets.isValid ()) )
  {
   std::cerr << ">>>>>> VBFMCH4jAnalysis::Warning: " << srcIC5GenJets_p << " not available" << std::endl;
   return;
  }
  
  edm::Handle<reco::CaloJetCollection> hIC5CaloJets;
  iEvent.getByLabel (srcIC5CaloJets_p, hIC5CaloJets) ;
  if( !(hIC5CaloJets.isValid ()) )
  {
   std::cerr << ">>>>>> VBFMCH4jAnalysis::Warning: " << srcIC5CaloJets_p << " not available" << std::endl;
   return;
  }


  
  
  
  
  // -----------------------------------------------------------------
  // mc analysis
  // -----------------------------------------------------------------  

  const reco::Candidate* mcV1;
  const reco::Candidate* mcV2;

  bool V1_flag = false;
  bool V2_flag = false;
  
  const reco::Candidate* mcV1L1;
  const reco::Candidate* mcV1L2;

  bool V1L1_flag = false;
  bool V1L2_flag = false;

  const reco::Candidate* mcV2L1;
  const reco::Candidate* mcV2L2;

  bool V2L1_flag = false;
  bool V2L2_flag = false;
  
  
  for(reco::GenParticleCollection::const_iterator p = hGenParticles -> begin();
      p != hGenParticles -> end(); ++p)
  {   
   const reco::Candidate* pCurrent = &(*p);
   const reco::Candidate* pMother = 0;
   const reco::Candidate* pDaughter = 0;
   if(pCurrent -> mother())
    pMother = pCurrent -> mother();
   int pdgId  = p -> pdgId();
   int status = p -> status();
   int charge = p -> charge();
   int motherPdgId = 0;
   if(pCurrent -> mother())
    motherPdgId = pMother -> pdgId();
   
   if (abs(motherPdgId) == 6 || abs(motherPdgId) == 25) {//---- t == |6| ---- h0 == 25 ----
    if (abs(pdgId) == 24){//---- W == |24| ----
     if (!V1_flag) {
      mcV1 = pCurrent;
      V1_flag = true;
      
      int n = pCurrent->numberOfDaughters();
      for(size_t j = 0; j < n; ++ j) {
       const Candidate * d = pCurrent->daughter( j );
       int dauId = d->pdgId();
       if (abs(dauId)<=6 && abs(dauId)>=1) {//---- quarks ----
        if (!V1L1_flag) {
         mcV1L1 = d;
         V1L1_flag = true;
        }
        else if (!V1L2_flag){
         mcV1L2 = d;
         V1L2_flag = true;         
        }
       }
      }      
      
     }
     else if (!V2_flag){
      mcV2 = pCurrent;
      V2_flag = true;
      
      int n = pCurrent->numberOfDaughters();
      for(size_t j = 0; j < n; ++ j) {
       const Candidate * d = pCurrent->daughter( j );
       int dauId = d->pdgId();
       if (abs(dauId)<=6 && abs(dauId)>=1) {//---- quarks ----
        if (!V2L1_flag) {
         mcV2L1 = d;
         V2L1_flag = true;
        }
        else if (!V2L2_flag){
         mcV2L2 = d;
         V2L2_flag = true;         
        }
       }
      } 
      
     }
     
     

    
    }
   }
  }
  
  // -----------------------------------------------------------------
  // fill histograms
  // -----------------------------------------------------------------  

  if (V1_flag && V2_flag) {
   mcV1V2_eta -> Fill(mcV1 -> eta(),mcV2 -> eta());
  }
  
  if (V1L1_flag && V1L2_flag) {
   mcVPTFDeta -> Fill (fabs(mcV1L1 -> eta() - mcV1L2 -> eta()),mcV1 -> pt());
//    mcL1L2_eta -> Fill(mcL1 -> eta(),mcL2 -> eta());
//    mcL1L2_SumEta -> Fill(mcL1 -> eta() + mcL2 -> eta());
//    mcL1L2_DiffEta -> Fill(mcL1 -> eta() - mcL2 -> eta());
//    mcL1L2_SumDiffEta -> Fill(mcL1 -> eta() - mcL2 -> eta(),mcL1 -> eta() + mcL2 -> eta());
  }

  if (V2L1_flag && V2L2_flag) {
   mcVPTFDeta -> Fill (fabs(mcV2L1 -> eta() - mcV2L2 -> eta()),mcV2 -> pt());
  }
  
 }

