#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFMCttbarAnalysis.h"

#define PI 3.141592654

using namespace vbfhww2l;



VBFMCttbarAnalysis::VBFMCttbarAnalysis(const edm::ParameterSet& iConfig):
  srcHepMCProduct_p          (iConfig.getParameter<edm::InputTag>("srcHepMCProduct")),
 srcGenParticles_p          (iConfig.getParameter<edm::InputTag>("srcGenParticles")),
 srcGenMet_p                (iConfig.getParameter<edm::InputTag>("srcGenMet")),
 srcIC5GenJets_p            (iConfig.getParameter<edm::InputTag>("srcIC5GenJets")),
 srcIC5CaloJets_p           (iConfig.getParameter<edm::InputTag>("srcIC5CaloJets")),
 fileName_p                 (iConfig.getUntrackedParameter<std::string> ("fileName", std::string("VBFMCttbarAnalysis"))),
 mass_p                     (iConfig.getParameter<std::string>("mass")),
 verbosity_p                (iConfig.getUntrackedParameter<bool>("verbosity", false)),
 eventsToPrint_p            (iConfig.getUntrackedParameter<int>("eventsToPrint", 0)),
 eventId_p(0),
 naiveId_p(0)
{}



 VBFMCttbarAnalysis::~VBFMCttbarAnalysis()
 {}






 void VBFMCttbarAnalysis::beginJob(const edm::EventSetup& iSetup)
 {
  if(verbosity_p)
   std::cerr << "*** VBFMCttbarAnalysis::beginJob ***" << std::endl;


  // Create File
//   std::string path = "/afs/cern.ch/user/a/abenagli/scratch0/VBF/CMSSW_2_2_7/src/HiggsAnalysis/VBFHiggsToWWto2l2nu/test/";
  std::string path = "/tmp/amassiro/";
  std::string outFileName = path+fileName_p+"_H"+mass_p+".root";
  outFile_p = new TFile(outFileName.c_str(), "RECREATE");
  
  
  
  // -----------------------------------------------------------------
  // define histograms
  // -----------------------------------------------------------------

  mcV1V2_eta = new TH2F("mcV1V2_eta", "mcV1V2_eta", 1000, -5., 5., 1000, -5., 5.);
  
  mcV1V2_SumEta = new TH1F("mcV1V2_SumEta", "mcV1V2_SumEta", 1000, -10., 10.);
  mcV1V2_DiffEta = new TH1F("mcV1V2_DiffEta", "mcV1V2_DiffEta", 1000, -5., 5.);
  
  mcV1V2_SumDiffEta = new TH2F("mcV1V2_SumDiffEta", "mcV1V2_SumDiffEta", 1000, -10., 10., 1000, -10., 10.);
  
  
  
  
  mcL1L2_eta = new TH2F("mcL1L2_eta", "mcL1L2_eta", 1000, -5., 5., 1000, -5., 5.);
  
  mcL1L2_SumEta = new TH1F("mcL1L2_SumEta", "mcL1L2_SumEta", 1000, -10., 10.);
  mcL1L2_DiffEta = new TH1F("mcL1L2_DiffEta", "mcL1L2_DiffEta", 1000, -5., 5.);
  
  mcL1L2_SumDiffEta = new TH2F("mcL1L2_SumDiffEta", "mcL1L2_SumDiffEta", 1000, -10., 10., 1000, -10., 10.);
 }



 void VBFMCttbarAnalysis::endJob()
 {
  if(verbosity_p)
   std::cerr << "*** VBFMCttbarAnalysis::endJob ***" << std::endl;

  // Save histograms
  outFile_p -> cd();
  //TDirectory* sd1;
  
  
  mcV1V2_eta -> Write();
  mcV1V2_SumEta -> Write();
  mcV1V2_DiffEta -> Write();
  mcV1V2_SumDiffEta -> Write();
  
  mcL1L2_eta -> Write();
  mcL1L2_SumEta -> Write();
  mcL1L2_DiffEta -> Write();
  mcL1L2_SumDiffEta -> Write();
  
  outFile_p -> Close();
 }






 void VBFMCttbarAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
 {
  if(verbosity_p)
   std::cerr << "*** VBFMCttbarAnalysis::analyze ***" << std::endl;
  
  
  // set event id
  eventId_p = (iEvent.id()).event();
  ++naiveId_p;
  
  if( (naiveId_p%100 == 0) && (verbosity_p == true) )
   std::cout << ">>>>>> VBFMCttbarAnalysis   Analyze event number: " << naiveId_p << std::endl;


  edm::Handle<reco::GenParticleCollection> hGenParticles;
  iEvent.getByLabel(srcGenParticles_p, hGenParticles);
  if( !(hGenParticles.isValid ()) )
  {
   std::cerr << ">>>>>> VBFMCttbarAnalysis::Warning: " << srcGenParticles_p << " not available" << std::endl;
   return;
  }

  edm::ESHandle<ParticleDataTable> hPdt;
  iSetup.getData(hPdt);

  edm::Handle<reco::GenMETCollection> hGenMet;
  iEvent.getByLabel(srcGenMet_p, hGenMet);
  if( !(hGenMet.isValid ()) )
  {
   std::cerr << ">>>>>> VBFMCttbarAnalysis::Warning: " << srcGenMet_p << " not available" << std::endl;
   return;
  }
  
  edm::Handle<reco::GenJetCollection> hIC5GenJets;
  iEvent.getByLabel(srcIC5GenJets_p, hIC5GenJets);
  if( !(hIC5GenJets.isValid ()) )
  {
   std::cerr << ">>>>>> VBFMCttbarAnalysis::Warning: " << srcIC5GenJets_p << " not available" << std::endl;
   return;
  }
  
  edm::Handle<reco::CaloJetCollection> hIC5CaloJets;
  iEvent.getByLabel (srcIC5CaloJets_p, hIC5CaloJets) ;
  if( !(hIC5CaloJets.isValid ()) )
  {
   std::cerr << ">>>>>> VBFMCttbarAnalysis::Warning: " << srcIC5CaloJets_p << " not available" << std::endl;
   return;
  }


  
  
  
  
  // -----------------------------------------------------------------
  // mc analysis
  // -----------------------------------------------------------------  

  const reco::Candidate* mcV1;
  const reco::Candidate* mcV2;

  bool V1_flag = false;
  bool V2_flag = false;
  
  const reco::Candidate* mcL1;
  const reco::Candidate* mcL2;

  bool L1_flag = false;
  bool L2_flag = false;
  
  
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
//     std::cerr << "found a t" << std::endl;
    if (abs(pdgId) == 24){//---- W == |24| ----
//      std::cerr << "  -> found a W from t" << std::endl;
     if (!V1_flag) {
      mcV1 = pCurrent;
      V1_flag = true;
     }
     else if (!V2_flag){
      mcV2 = pCurrent;
      V2_flag = true;
     }
     
     
     int n = pCurrent->numberOfDaughters();
     for(size_t j = 0; j < n; ++ j) {
      const Candidate * d = pCurrent->daughter( j );
      int dauId = d->pdgId();
      if (abs(dauId)==11 || abs(dauId)==13) {//---- e == |11| ---- mu == |13| ----
       if (!L1_flag) {
        mcL1 = d;
        L1_flag = true;
       }
       else if (!L2_flag){
        mcL2 = d;
        L2_flag = true;         
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
   mcV1V2_SumEta -> Fill(mcV1 -> eta() + mcV2 -> eta());
   mcV1V2_DiffEta -> Fill(mcV1 -> eta() - mcV2 -> eta());
   mcV1V2_SumDiffEta -> Fill(mcV1 -> eta() - mcV2 -> eta(),mcV1 -> eta() + mcV2 -> eta());
  }
  
  if (L1_flag && L2_flag) {
   mcL1L2_eta -> Fill(mcL1 -> eta(),mcL2 -> eta());
   mcL1L2_SumEta -> Fill(mcL1 -> eta() + mcL2 -> eta());
   mcL1L2_DiffEta -> Fill(mcL1 -> eta() - mcL2 -> eta());
   mcL1L2_SumDiffEta -> Fill(mcL1 -> eta() - mcL2 -> eta(),mcL1 -> eta() + mcL2 -> eta());    
  }

 }

