#ifndef VBFMCProcessFilter_h
#define VBFMCProcessFilter_h
// -*- C++ -*-
//
// Package:    VBFMCProcessFilter
// Class:      VBFMCProcessFilter
// 
/* 

 Description: filter events based on the Pythia ProcessID and the Pt_hat

 Implementation: inherits from generic EDFilter
     
*/
//
// $Id: VBFMCProcessFilter.h,v 1.4 2007/11/15 16:30:42 tancini Exp $
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


// using namespace edm;
// using namespace std;
//
// class decleration
//

class VBFMCProcessFilter : public edm::EDFilter {
   public:
      explicit VBFMCProcessFilter(const edm::ParameterSet&);
      ~VBFMCProcessFilter();


      virtual bool filter(edm::Event&, const edm::EventSetup&);
   private:
      // ----------member data ---------------------------
      
       std::string label_;
       std::vector<int> processID;  
       std::vector<double> pthatMin;
       std::vector<double> pthatMax;  
};

#endif
