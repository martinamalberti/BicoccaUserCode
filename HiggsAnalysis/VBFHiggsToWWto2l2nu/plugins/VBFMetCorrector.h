#ifndef VBFMetCorrector_h
#define VBFMetCorrector_h

// system include files

#include <memory>
#include <iostream>

// user include files

#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CorrMETData.h"
#include "DataFormats/METReco/interface/SpecificCaloMETData.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFUtils.h"

//#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
//#include "TrackingTools/TrackAssociator/interface/TrackAssociatorParameters.h"

//
// class declaration
//

class VBFMetCorrector: public edm::EDProducer 
{

  public:

    //! ctor
    explicit VBFMetCorrector (const edm::ParameterSet& conf) ;
    //! dtor
    ~VBFMetCorrector () ;
   
    virtual void beginJob (const edm::EventSetup&) ;
    virtual void produce (edm::Event&, const edm::EventSetup&) ;
    virtual void endJob () ;
   

  private:

    edm::InputTag caloMetLabel_;
    edm::InputTag muonLabel_;
 
    bool muonCorrection_;
    bool muonDepositCor_;
    double corrmetMin_;
    double muonPtMin_;
    double muonEtaMax_;
    double muonTrackD0Max_;
    double muonTrackDzMax_;
    double muonNHitsMin_;  
    double muonDPtMax_; 
    double muonChiSqMax_;   
} ;

#endif

