#ifndef VBFSkimEfficiencyNtuple_h
#define VBFSkimEfficiencyNtuple_h
// -*- C++ -*-
//
// Package:    VBFSkimEfficiencyNtuple
// Class:      VBFSkimEfficiencyNtuple
// 
/* 

 Description: number of events counter

*/
// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFUtils.h"

#include <TNtuple.h>


class VBFSkimEfficiencyNtuple : public edm::EDAnalyzer 
{
 typedef edm::View<reco::PixelMatchGsfElectron> electronCollection ;
 typedef edm::View<reco::Muon> muonCollection ;
 typedef reco::CaloJetCollection::const_iterator VBFjetIt ;

 public:
      //! ctor
    explicit VBFSkimEfficiencyNtuple (const edm::ParameterSet&) ;
      //! dtor
      ~VBFSkimEfficiencyNtuple();
      //! the filtering method
      virtual void beginJob (const edm::EventSetup&);
      virtual void analyze (const edm::Event&, const edm::EventSetup&);
      virtual void endJob ();
      
 private:

  edm::InputTag m_GSFInputTag ;
  edm::InputTag m_muInputTag ;
  
  double m_elePtMin;
  double m_muPtMin;
  
  int m_minLeptonsNum ;
  
  TNtuple *m_ntuple;
  int numEvents_analyzed;
  int numEvents_selected;
    
};

#endif


