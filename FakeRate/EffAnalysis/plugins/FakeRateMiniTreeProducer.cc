#include "FakeRate/EffAnalysis/plugins/FakeRateMiniTreeProducer.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"

#include <TFile.h>
#include <TTree.h>

#include <iostream>
#include <algorithm>

using namespace reco;
using namespace std;

FakeRateMiniTreeProducer::FakeRateMiniTreeProducer(const edm::ParameterSet& conf)
{

  src_ = conf.getParameter<edm::InputTag>("src");
  outfilename_ = conf.getParameter<string>("outfilename");
  
}  
  
FakeRateMiniTreeProducer::~FakeRateMiniTreeProducer()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  out_->Write();
  out_->Close();
  
}

void FakeRateMiniTreeProducer::beginJob(edm::EventSetup const&iSetup)
{
 
  nAnalyzed_ = 0;
  
  out_ = new TFile(outfilename_.c_str(),"RECREATE");
  tree_ = new TTree("FakeRateMiniTree","FakeRateMiniTree");
    
}     

void FakeRateMiniTreeProducer::endJob()
{
}

void FakeRateMiniTreeProducer::analyze(const edm::Event& e, const edm::EventSetup& iSetup)
{

  nAnalyzed_++;
  
  // fill the mini tree 
      
}
