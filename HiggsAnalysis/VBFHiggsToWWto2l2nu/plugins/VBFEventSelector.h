#ifndef VBFEventSelector_h
#define VBFEventSelector_h
// -*- C++ -*-
//
// Package:    VBFEventSelector
// Class:      VBFEventSelector
// 
/* 

 Description: filter events based on jetDeltaEtaMin + jetPtMin + MjjMin

*/
// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFUtils.h"


class VBFEventSelector : public edm::EDFilter 
{
     typedef math::XYZTLorentzVector LorentzVector ;

   public:
      //! ctor
      explicit VBFEventSelector (const edm::ParameterSet&) ;
      //! dtor
      ~VBFEventSelector();
      //! the filtering method
      virtual bool filter (edm::Event&, const edm::EventSetup&) ;
      
   private:


      edm::InputTag m_tagJetInputTag ;
      double m_jetDeltaEtaMin ;
      double m_jetPtMin ;   
      double m_MjjMin ;   
};

#endif


