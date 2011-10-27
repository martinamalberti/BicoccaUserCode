#ifndef DumpJEC_h
#define DumpJEC_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>






//---------------------------
//---- class declaration ----
//---------------------------

class DumpJEC : public edm::EDAnalyzer
{
 public:
  
  //! ctor
  explicit DumpJEC(const edm::ParameterSet&);
  
  //! dtor
  ~DumpJEC();
  
  
  
 private:

  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  
  double etaMin_;
  double etaMax_;
  int nBinsEta_;
  
  double ptMin_;
  double ptMax_;
  int nBinsPt_;
  
  std::string outFileName_;
};

#endif
