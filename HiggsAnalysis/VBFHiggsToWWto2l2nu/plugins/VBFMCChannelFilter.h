#ifndef VBFMCChannelFilter_h#define VBFMCChannelFilter_h// -*- C++ -*-//// Package:    VBFMCChannelFilter// Class:      VBFMCChannelFilter// /*  Description: filter events based on the Pythia ProcessID and the Pt_hat Implementation: inherits from generic EDFilter     *///// $Id: VBFMCChannelFilter.h,v 1.1 2008/03/17 17:01:17 govoni Exp $////// system include files#include <memory>// user include files#include "FWCore/Framework/interface/Frameworkfwd.h"#include "FWCore/Framework/interface/EDFilter.h"#include "FWCore/Framework/interface/Event.h"#include "FWCore/Framework/interface/MakerMacros.h"#include "FWCore/ParameterSet/interface/ParameterSet.h"// using namespace edm;// using namespace std;//// class decleration//class VBFMCChannelFilter : public edm::EDFilter 
{
   public:

      explicit VBFMCChannelFilter (const edm::ParameterSet&) ;      ~VBFMCChannelFilter () ;      virtual bool filter (edm::Event&, const edm::EventSetup&) ;

   private:
      
      edm::InputTag m_MCtruthInputTag ;
      //! the decay channel: 
      //! * 1 = uu
      //! * 2 = ee
      //! * 3 = eu
      int m_channel ;

};
       
#endif

