#ifndef MCResonancesAnalyzer_h
#define MCResonancesAnalyzer_h
// -*- C++ -*-
//
// Package:    MCResonancesAnalyzer
// Class:      MCResonancesAnalyzer
// 
/**\class MCResonancesAnalyzer MCResonancesAnalyzer.cc IOMC/MCResonancesAnalyzer/src/MCResonancesAnalyzer.cc
 A modified version of the HepMCProducts, which is 
 used to print out the content of HepMC products.
 This version has an additional method to print
 a table of particles and their decay products.

*/
//
// Original Author:  Filip Moortgat
// Additions made: Maria Hansen
//


// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

#include "TFile.h"
#include "TH1F.h"

class MCResonancesAnalyzer : public edm::EDAnalyzer {
   public:
      explicit MCResonancesAnalyzer(const edm::ParameterSet&);
      ~MCResonancesAnalyzer();

   
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      
 private:
  
      int Counter ;
      int Pi0Counter ;
      int EtaCounter ; 

      TFile *thefile;

      TH1F *hNeta ;
      TH1F *hNpi0 ;
      TH1F *hNGammaFromPi0 ;
      TH1F *hNGammaFromEta ;

      TH1F *hPi0Pt;
      TH1F *hEtaPt;

};
#endif
