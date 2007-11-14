// -*- C++ -*-
//
// Package:    VBFHiggsToWW2e
// Class:      VBFHiggsToWW2e
// 
/**\class VBFHiggsToWW2e VBFHiggsToWW2e.cc HiggsAnalysis/VBFHiggsToWW2e/src/VBFHiggsToWW2e.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Pietro Govoni
//         Created:  Wed Nov 14 17:32:25 CET 2007
// $Id: VBFHiggsToWW2e.cc,v 1.1 2007/11/14 16:34:19 govoni Exp $
//
//

#ifndef VBFHiggsToWW2e_H
#define VBFHiggsToWW2e_H

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
//
// class decleration
//

class VBFHiggsToWW2e : public edm::EDAnalyzer {
   public:
      explicit VBFHiggsToWW2e(const edm::ParameterSet&);
      ~VBFHiggsToWW2e();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
};

#endif
