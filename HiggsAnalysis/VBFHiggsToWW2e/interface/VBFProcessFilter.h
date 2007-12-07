#ifndef VBFProcessFilter_h#define VBFProcessFilter_h// -*- C++ -*-//// Package:    VBFProcessFilter// Class:      VBFProcessFilter// /*  Description: filter events based on the Pythia ProcessID and the Pt_hat Implementation: inherits from generic EDFilter     *///// $Id: VBFProcessFilter.h,v 1.1 2007/11/15 17:27:54 tancini Exp $////// system include files#include <memory>// user include files#include "FWCore/Framework/interface/Frameworkfwd.h"#include "FWCore/Framework/interface/EDFilter.h"#include "FWCore/Framework/interface/Event.h"#include "FWCore/Framework/interface/MakerMacros.h"#include "FWCore/ParameterSet/interface/ParameterSet.h"// using namespace edm;// using namespace std;//// class decleration//class VBFProcessFilter : public edm::EDFilter {   public:      explicit VBFProcessFilter (const edm::ParameterSet&) ;      ~VBFProcessFilter();      virtual bool filter (edm::Event&, const edm::EventSetup&) ;

   private:
            edm::InputTag m_jetInputTag ;
      double m_jetEtaMax ;
      double m_jetPtMin ;
};#endif