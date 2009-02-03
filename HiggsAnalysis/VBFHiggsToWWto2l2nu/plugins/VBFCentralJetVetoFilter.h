#ifndef VBFCentralJetVetoFilter_h#define VBFCentralJetVetoFilter_h// -*- C++ -*-//// Package:    VBFCentralJetVetoFilter// Class:      VBFCentralJetVetoFilter// /*  Description: filter events based on the Pythia ProcessID and the Pt_hat
Implementation: inherits from generic EDFilter

*/
//
// $Id: VBFCentralJetVetoFilter.h,v 1.2 2008/03/28 08:17:28 govoni Exp $
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


class VBFCentralJetVetoFilter : public edm::EDFilter 
{
     typedef math::XYZTLorentzVector LorentzVector ;
     typedef reco::CaloJetCollection::const_iterator VBFjetIt ;

   public:
      //! ctor
      explicit VBFCentralJetVetoFilter (const edm::ParameterSet&) ;
      //! dtor
      ~VBFCentralJetVetoFilter();
      //! the filtering method
      virtual bool filter (edm::Event&, const edm::EventSetup&) ;
      
   private:

      edm::InputTag m_jetInputTag ;
      edm::InputTag m_tagJetInputTag ;
      double m_jetDeltaEta ;
      double m_jetPtMin ;   
      int m_maxJetsNum ;   
};

#endif


