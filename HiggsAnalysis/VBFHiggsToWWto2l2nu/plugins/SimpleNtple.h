// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFEleTrackerIsolationAlgo.h"

#include "TTree.h"
#include <TLorentzVector.h>
#include <TClonesArray.h>
#include "TParticle.h"

//
// class decleration
//

class SimpleNtple : public edm::EDAnalyzer {
 public:
  explicit SimpleNtple(const edm::ParameterSet&);
  ~SimpleNtple();

  typedef edm::View<reco::Track> trackCollection ;
  typedef math::XYZTLorentzVector LorentzVector ;
  
 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  void Init();
  void FillKindEvent(const edm::Event&, const edm::EventSetup&);
  void FillEle (const edm::Event&, const edm::EventSetup&);
  void FillMu (const edm::Event&, const edm::EventSetup&);
  void FillMet (const edm::Event&, const edm::EventSetup&);
  void FillTagJet (const edm::Event&, const edm::EventSetup&);
  void FillJet (const edm::Event&, const edm::EventSetup&, int);
  void FillTracks(const edm::Event&, const edm::EventSetup&);
  void FillGenParticles(const edm::Event&, const edm::EventSetup&);
  void FillGenJet(const edm::Event&, const edm::EventSetup&);
  void FillGenMet(const edm::Event&, const edm::EventSetup&);
    
  void setMomentum (TLorentzVector &myvector, const LorentzVector & mom) ;
  
  // ----------member data ---------------------------
  TTree *mytree_;
  //electrons;
  int nEle;
  float IsolEleSumPt_VBF[30],IsolEleNTracks_VBF[30];
  float IsolEleECal[30],IsolEleHCal[30],IsolEleTr[30];
  int EleId[30];
  //muons
  int nMu;
  int IdEvent;
  float IsolMuSumPt[30],IsolMuNTracks[30];
  float IsolMuECal[30],IsolMuHCal[30],IsolMuTr[30];
  float MinvTags;
  
  //other jets
  std::vector<float> * m_otherJets_SisCone5CaloJets_alpha ;
  std::vector<float> * m_otherJets_IterativeCone5CaloJets_alpha ;
  std::vector<float> * m_otherJets_SisCone5PFJets_alpha ;
  std::vector<float> * m_otherJets_IterativeCone5PFJets_alpha ;

  std::vector<float> *  m_otherJets_L2L3_SisCone5CaloJets_alpha ;
  std::vector<float> *  m_otherJets_L2L3_IterativeCone5CaloJets_alpha ;
  std::vector<float> *  m_otherJets_L2L3_SisCone5PFJets_alpha ;
  std::vector<float> *  m_otherJets_L2L3_IterativeCone5PFJets_alpha ;

  std::vector<float> *  m_otherJets_JPT_SisCone5CaloJets_alpha ;
  std::vector<float> *  m_otherJets_JPT_IterativeCone5CaloJets_alpha ;
  std::vector<float> *  m_otherJets_JPT_SisCone5PFJets_alpha ;
  std::vector<float> *  m_otherJets_JPT_IterativeCone5PFJets_alpha ;
  
    
  TClonesArray * m_tagJets ;
  TClonesArray * m_otherJets ;
  TClonesArray * m_electrons ;
  TClonesArray * m_muons ;
  TClonesArray * m_MET ;
  TClonesArray * m_tracks ;
  TClonesArray * m_genParticles;
  TClonesArray * m_genJets;
  TClonesArray * m_genMet;
  
  TClonesArray * m_otherJets_SisCone5CaloJets ;
  TClonesArray * m_otherJets_IterativeCone5CaloJets ;
  TClonesArray * m_otherJets_SisCone5PFJets ;
  TClonesArray * m_otherJets_IterativeCone5PFJets ;

  TClonesArray *  m_otherJets_L2L3_SisCone5CaloJets ;
  TClonesArray *  m_otherJets_L2L3_IterativeCone5CaloJets ;
  TClonesArray *  m_otherJets_L2L3_SisCone5PFJets ;
  TClonesArray *  m_otherJets_L2L3_IterativeCone5PFJets ;

  TClonesArray *  m_otherJets_JPT_SisCone5CaloJets ;
  TClonesArray *  m_otherJets_JPT_IterativeCone5CaloJets ;
  TClonesArray *  m_otherJets_JPT_SisCone5PFJets ;
  TClonesArray *  m_otherJets_JPT_IterativeCone5PFJets ;
  
  
  
  std::vector<float> * bTag_SisCone5CaloJets_;
  std::vector<float> * bTag_IterativeCone5CaloJets_;
  std::vector<float> * bTag_SisCone5PFJets_;
  std::vector<float> * bTag_IterativeCone5PFJets_;
  
  std::vector<float> * bTag_L2L3_SisCone5CaloJets_;
  std::vector<float> * bTag_L2L3_IterativeCone5CaloJets_;
  std::vector<float> * bTag_L2L3_SisCone5PFJets_;
  std::vector<float> * bTag_L2L3_IterativeCone5PFJets_;
  
  std::vector<float> * bTag_JPT_SisCone5CaloJets_;
  std::vector<float> * bTag_JPT_IterativeCone5CaloJets_;
  std::vector<float> * bTag_JPT_SisCone5PFJets_;
  std::vector<float> * bTag_JPT_IterativeCone5PFJets_;
  
  
    
  TLorentzVector myvector ;
  TLorentzVector myvertex ;
//   TParticle myparticle ;
  
  
  //! MC information

  TClonesArray * m_HiggsParticle ;
  TParticle* Higgs ;
  TParticle* V1 ;
  TParticle* V2 ;
  TParticle* f1V1 ;
  TParticle* f2V1 ;
  TParticle* f1V2 ;
  TParticle* f2V2 ;
  TParticle* q1 ;
  TParticle* q2 ;
  
  //! isolation algorithm
  VBFEleTrackerIsolationAlgo m_tkIsolationAlgo ;
  
  //! isolation variable
  edm::InputTag eleTkIsoTag_;
  edm::InputTag eleEcalIsoTag_;
  edm::InputTag eleHcalIsoTag_;
  
  bool doRefCheckTag_;
  edm::InputTag EleRefTag_ ;
  
  edm::InputTag m_eleIDPTDRLooseInputTag ;
  edm::InputTag m_eleIDPTDRMediumInputTag ;
  edm::InputTag m_eleIDPTDRTightInputTag ;
  edm::InputTag TracksTag_;
  edm::InputTag EleTag_;
  edm::InputTag IsolEleTag_;
  edm::InputTag MuTag_;
  edm::InputTag IsolMuTag_;
  edm::InputTag MetTag_;
  edm::InputTag TagJetTag_;
  edm::InputTag JetTag_;
  edm::InputTag MCtruthTag_;
  edm::InputTag genJetTag_;
  edm::InputTag genMetTag_;
  
  edm::InputTag JetTagSisCone5CaloJets_;
  edm::InputTag JetTagIterativeCone5CaloJets_;
  edm::InputTag JetTagSisCone5PFJets_;
  edm::InputTag JetTagIterativeCone5PFJets_;

  edm::InputTag JetTagSisCone5CaloJets_L2L3_;
  edm::InputTag JetTagIterativeCone5CaloJets_L2L3_;
  edm::InputTag JetTagSisCone5PFJets_L2L3_;
  edm::InputTag JetTagIterativeCone5PFJets_L2L3_;

  edm::InputTag JetTagSisCone5CaloJets_JPT_;
  edm::InputTag JetTagIterativeCone5CaloJets_JPT_;
  edm::InputTag JetTagSisCone5PFJets_JPT_;
  edm::InputTag JetTagIterativeCone5PFJets_JPT_;

  
  edm::InputTag JetTagIterativeCone5CaloJets_BTagging_;

  
  bool bool_JetTagSisCone5CaloJets_;
  bool bool_JetTagIterativeCone5CaloJets_;
  bool bool_JetTagSisCone5PFJets_;
  bool bool_JetTagIterativeCone5PFJets_;

  bool bool_JetTagSisCone5CaloJets_L2L3_;
  bool bool_JetTagIterativeCone5CaloJets_L2L3_;
  bool bool_JetTagSisCone5PFJets_L2L3_;
  bool bool_JetTagIterativeCone5PFJets_L2L3_;

  bool bool_JetTagSisCone5CaloJets_JPT_;
  bool bool_JetTagIterativeCone5CaloJets_JPT_;
  bool bool_JetTagSisCone5PFJets_JPT_;
  bool bool_JetTagIterativeCone5PFJets_JPT_;

  
    
  bool bool_JetTagIterativeCone5CaloJets_BTagging_;
    
};
