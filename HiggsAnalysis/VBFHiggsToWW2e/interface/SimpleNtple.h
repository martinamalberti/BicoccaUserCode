// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TTree.h"

//
// class decleration
//

class SimpleNtple : public edm::EDAnalyzer {
 public:
  explicit SimpleNtple(const edm::ParameterSet&);
  ~SimpleNtple();
  
  
 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  void Init();
  void FillEle (const edm::Event&, const edm::EventSetup&);
  void FillMu (const edm::Event&, const edm::EventSetup&);
  void FillMet (const edm::Event&, const edm::EventSetup&);
  void FillTagJet (const edm::Event&, const edm::EventSetup&);
  void FillJet (const edm::Event&, const edm::EventSetup&);
  void FillTrack (const edm::Event&, const edm::EventSetup&);
  
  // ----------member data ---------------------------
  TTree *mytree_;
  //electrons;
  int nEle;
  float EtEle[30],EtaEle[30],PhiEle[30],IsolEleSumPt[30],IsolEleNTracks[30];
  int EleId[30];
  //muons
  int nMu;
  float EtMu[30],EtaMu[30],PhiMu[30],IsolMuSumPt[30],IsolMuNTracks[30];
  //missing Et
  float MetX,MetY,Met;
  //tag jets
  float EtTagJet[2],EtaTagJet[2],PhiTagJet[2];
  float MinvTags;
  //other jets
  int nJet;
  float EtJet[50],EtaJet[50],PhiJet[50];

  int nTrack;
  float EtTrack[100],EtaTrack[100],PhiTrack[100];

  

  edm::InputTag EleTag_;
  edm::InputTag IsolEleTag_;
  edm::InputTag MuTag_;
  edm::InputTag IsolMuTag_;
  edm::InputTag MetTag_;
  edm::InputTag TagJetTag_;
  edm::InputTag JetTag_;
  edm::InputTag TrackTag_;
  
};
