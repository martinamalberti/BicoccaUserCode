#ifndef VBFLeptonsNumFilter_h#define VBFLeptonsNumFilter_h// -*- C++ -*-//// Package:    VBFLeptonsNumFilter// Class:      VBFLeptonsNumFilter// /*  Description: filter events based on the Pythia ProcessID and the Pt_hat
Implementation: inherits from generic EDFilter

*/
//
// $Id: VBFLeptonsNumFilter.h,v 1.5 2007/12/07 17:46:34 govoni Exp $
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


class VBFLeptonsNumFilter : public edm::EDFilter 
{
   typedef reco::CaloJetCollection::const_iterator VBFjetIt ;
   public:
      //! ctor
      explicit VBFLeptonsNumFilter (const edm::ParameterSet&) ;
      //! dtor
      ~VBFLeptonsNumFilter () ;
      //! the filtering method
      virtual bool filter (edm::Event&, const edm::EventSetup&) ;
      
   private:

      edm::InputTag m_GSFInputTag ;
      edm::InputTag m_muInputTag ;
   
      int m_minLeptonsNum ;
      int m_maxLeptonsNum ;

      double m_eleEtaMax ;
      double m_elePtMin ;
      double m_muEtaMax ;
      double m_muPtMin ;
};

#endif


