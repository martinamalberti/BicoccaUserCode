// -*- C++ -*-
//
// Package:    VBFReadEvent
// Class:      VBFReadEvent
// 
/**\class VBFReadEvent VBFReadEvent.cc HiggsAnalysis/VBFReadEvent/src/VBFReadEvent.cc

 Description: <one line class summary>
 
 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Pietro Govoni
//         Created:  Wed Nov 14 17:32:25 CET 2007
// $Id: VBFReadEvent.h,v 1.3 2007/11/15 16:21:30 govoni Exp $
//
//

#ifndef VBFReadEvent_H
#define VBFReadEvent_H

// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


class VBFReadEvent : public edm::EDAnalyzer {

  public:

      //! ctor
      explicit VBFReadEvent (const edm::ParameterSet&) ;
      //! dtor
      ~VBFReadEvent () ;

   private:

      virtual void beginJob (const edm::EventSetup&) ;
      virtual void analyze (const edm::Event&, const edm::EventSetup&) ;
      virtual void endJob () ;

   private:

      std::string m_metColletion ;
      std::string m_genMetColletion ;
      std::string m_jetColletion ;
      std::string m_genJetColletion ;
      std::string m_eleColletion ;
      std::string m_MCtruthColletion ;
      std::string m_muColletion ;

} ;

#endif
