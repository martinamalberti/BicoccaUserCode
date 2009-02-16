#ifndef VBFLeptonsMinNumFilter_h
#define VBFLeptonsMinNumFilter_h
// -*- C++ -*-
//
// Package:    VBFLeptonsMinNumFilter
// Class:      VBFLeptonsMinNumFilter
// 
/* 

 Description: filter events based on the minimum number of reco leptons

*/
//
//
// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFUtils.h"



class VBFLeptonsMinNumFilter : public edm::EDFilter 
{

 typedef edm::View<reco::PixelMatchGsfElectron> electronCollection ;
 typedef edm::View<reco::Muon> muonCollection ;
 typedef reco::CaloJetCollection::const_iterator VBFjetIt ;

 public:
      //! ctor
  explicit VBFLeptonsMinNumFilter (const edm::ParameterSet&) ;
      //! dtor
  ~VBFLeptonsMinNumFilter () ;
      //! the filtering method
  virtual bool filter (edm::Event&, const edm::EventSetup&) ;
      
 private:

  edm::InputTag m_GSFInputTag ;
  edm::InputTag m_muInputTag ;
  
  double m_elePtMin;
  double m_muPtMin;
  
  int m_minLeptonsNum ;

};

#endif
