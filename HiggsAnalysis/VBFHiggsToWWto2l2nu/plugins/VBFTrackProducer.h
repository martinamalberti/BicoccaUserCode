#ifndef __VBFTrackProducer_H__
#define __VBFTrackProducer_H__


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
  
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/TrackReco/interface/TrackExtraBase.h"
#include "DataFormats/TrackReco/interface/TrackExtraFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
//#include "DataFormats/TrackReco/interface/Track.h"

//-------------------- VBF includes
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFObjects/classes.h"
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFEleTrackerIsolationAlgo.h"


class VBFTrackProducer : public edm::EDProducer {

 public:
 
      typedef edm::View<reco::Track> trackCollection ; 
    
      explicit VBFTrackProducer (const edm::ParameterSet&) ;
      ~VBFTrackProducer () ;

   private:
   
      virtual void beginJob (const edm::EventSetup&) ;
      virtual void produce (edm::Event&, const edm::EventSetup&) ;
      virtual void endJob () ;

      edm::InputTag m_trackInputTag ;
} ;
  
#endif
 


