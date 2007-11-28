#ifndef FakeRateMiniTreeProducer_h
#define FakeRateMiniTreeProducer_h

/** \class FakeRateMiniTreeProducer
 *
 * analyzer to produce selection mini tree for H->ZZ->4e analysis
 */  
 
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

class TFile;
class TTree;

class FakeRateMiniTreeProducer : public edm::EDAnalyzer {

 public:
  
  FakeRateMiniTreeProducer(const edm::ParameterSet& conf);
  
  ~FakeRateMiniTreeProducer();
  
  void analyze(const edm::Event& e, const edm::EventSetup& c);
  void beginJob(const edm::EventSetup &iSetup);
  void endJob();
  
 private:

  unsigned int nAnalyzed_;
  edm::InputTag src_;
  std::string outfilename_;

  TFile *out_;
  TTree *tree_;
  
};
  
#endif
 


