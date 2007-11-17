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
// $Id: VBFReadEvent.h,v 1.4 2007/11/17 16:14:25 tancini Exp $
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
//#include <CLHEP/Vector/LorentzVector.h>

//root include
#include <TTree.h>
#include <TFile.h>
#include <TLorentzVector.h>


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

      edm::InputTag m_metInputTag ;
      edm::InputTag m_genMetInputTag ;
      edm::InputTag m_jetInputTag ;
      edm::InputTag m_genJetInputTag ;
      edm::InputTag m_GSFInputTag ;
      edm::InputTag m_electronIDInputTag ;
      edm::InputTag m_MCtruthInputTag ;
      edm::InputTag m_MC ;
      edm::InputTag m_muInputTag ;
    
      TFile *m_outfile;
      TTree* m_genTree;
      TLorentzVector *m_genLepPlus;
      TLorentzVector *m_genLepMinus;
      TLorentzVector *m_genMetPlus;
      TLorentzVector *m_genMetMinus;
      TLorentzVector *m_genqTagF;
      TLorentzVector *m_genqTagB;

} ;

#endif
