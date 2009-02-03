#ifndef __VBFJetProducer_H__
#define __VBFJetProducer_H__


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
  
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
//#include "DataFormats/TrackReco/interface/Track.h"

//-------------------- VBF includes
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFObjects/classes.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFEleTrackerIsolationAlgo.h"


class VBFJetProducer : public edm::EDProducer {

 public:
 
      typedef reco::CaloJetCollection JetCollection ;
      typedef reco::CaloJetRef jet ;
    
      explicit VBFJetProducer (const edm::ParameterSet&) ;
      ~VBFJetProducer () ;

   private:
   
      virtual void beginJob (const edm::EventSetup&) ;
      virtual void produce (edm::Event&, const edm::EventSetup&) ;
      virtual void endJob () ;

      edm::InputTag m_jetInputTag ;
} ;
  
#endif
 


