#ifndef VBFJetNumFilter_h
#define VBFJetNumFilter_h
// -*- C++ -*-
//
// Package:    VBFJetNumFilter
// Class:      VBFJetNumFilter
// 
/* 

 Description: filter events based on the Pythia ProcessID and the Pt_hat
Implementation: inherits from generic EDFilter

*/
//
// $Id: VBFJetNumFilter.h,v 1.2 2008/12/31 16:39:31 govoni Exp $
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

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronID.h"
#include "AnalysisDataFormats/Egamma/interface/ElectronIDAssociation.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFUtils.h"


class VBFJetNumFilter : public edm::EDFilter 
{

   typedef reco::CaloJetCollection::const_iterator VBFjetIt ;

   public:
      //! ctor
      explicit VBFJetNumFilter (const edm::ParameterSet&) ;
      //! dtor
      ~VBFJetNumFilter () ;
      //! the filtering method
      virtual bool filter (edm::Event&, const edm::EventSetup&) ;
      
   private:

      edm::InputTag m_jetInputTag ;

      int m_minJetNum ;
      int m_maxJetNum ;
};

#endif


