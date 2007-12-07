#ifndef VBFProcessFilter_h#define VBFProcessFilter_h// -*- C++ -*-//// Package:    VBFProcessFilter// Class:      VBFProcessFilter// /*  Description: filter events based on the Pythia ProcessID and the Pt_hat
Implementation: inherits from generic EDFilter

*/
//
// $Id: VBFProcessFilter.h,v 1.4 2007/12/07 16:17:27 govoni Exp $
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

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFUtils.h"


class VBFProcessFilter : public edm::EDFilter 
{
   typedef reco::CaloJetCollection::const_iterator VBFjetIt ;
   public:
      //! ctor
      explicit VBFProcessFilter (const edm::ParameterSet&) ;
      //! dtor
      ~VBFProcessFilter();
      //! the filtering method
      virtual bool filter (edm::Event&, const edm::EventSetup&) ;
      
   private:

      //PG preselection variables on the tag jet candidates
      edm::InputTag m_jetInputTag ;
      double m_jetEtaMax ;
      double m_jetPtMin ;
      
      //PG selections on the jet tags
      double m_tagJetsDeltaEtaMin ;
      double m_tagJetsDeltaEtaMax ;
      double m_tagJetsInvMassMin ;
      double m_tagJetsInvMassMax ;
      bool m_checkOpposite ;
};

#endif


