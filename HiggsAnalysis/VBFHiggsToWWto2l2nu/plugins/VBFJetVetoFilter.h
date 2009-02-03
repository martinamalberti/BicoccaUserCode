#ifndef VBFJetVetoFilter_h#define VBFJetVetoFilter_h// -*- C++ -*-//// Package:    VBFJetVetoFilter// Class:      VBFJetVetoFilter// /*  Description: filter events based on the Pythia ProcessID and the Pt_hat
Implementation: inherits from generic EDFilter

*/
//
// $Id: VBFJetVetoFilter.h,v 1.1 2008/03/17 17:01:16 govoni Exp $
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

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFUtils.h"


class VBFJetVetoFilter : public edm::EDFilter 
{
   typedef reco::CaloJetCollection::const_iterator VBFjetIt ;
   public:
      //! ctor
      explicit VBFJetVetoFilter (const edm::ParameterSet&) ;
      //! dtor
      ~VBFJetVetoFilter();
      //! the filtering method
      virtual bool filter (edm::Event&, const edm::EventSetup&) ;
      
   private:

      //PG preselection variables on the tag jet candidates
      edm::InputTag m_jetInputTag ;
      double m_jetEtaMax ;
      double m_jetPtMin ;      
};

#endif


