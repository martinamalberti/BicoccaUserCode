#ifndef VBFProcessFilter_h#define VBFProcessFilter_h// -*- C++ -*-//// Package:    VBFProcessFilter// Class:      VBFProcessFilter// /*  Description: filter events based on the Pythia ProcessID and the Pt_hat Implementation: inherits from generic EDFilter     *///// $Id: VBFProcessFilter.h,v 1.1 2007/12/07 14:10:18 govoni Exp $////// system include files#include <memory>// user include files#include "FWCore/Framework/interface/Frameworkfwd.h"#include "FWCore/Framework/interface/EDFilter.h"#include "FWCore/Framework/interface/Event.h"#include "FWCore/Framework/interface/MakerMacros.h"#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"

class VBFProcessFilter : public edm::EDFilter 
{
   typedef reco::CaloJetCollection::const_iterator jetIt ;
   public:
      //! ctor
      explicit VBFProcessFilter (const edm::ParameterSet&) ;
      //! dtor
      ~VBFProcessFilter();
      //! the filtering method
      virtual bool filter (edm::Event&, const edm::EventSetup&) ;
      
   private:
              //! find the jet tags candidates
     std::pair<jetIt,jetIt>	
     findTagJets (jetIt begin, jetIt end) ;

   private:
            edm::InputTag m_jetInputTag ;
      double m_jetEtaMax ;
      double m_jetPtMin ;
};

#endif


