// -*- C++ -*-
//
// Package:   deltaR
// Class:     deltaR
//
/**\class deltaR deltaR.cc

Description: <one line class summary>

 */


#include "CRUZET/DataTest/plugins/deltaR.h"

#include "DataFormats/DetId/interface/DetId.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"

#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"


#include <vector>

using namespace cms ;
using namespace edm ;
using namespace std ;



deltaR::deltaR (const edm::ParameterSet& iConfig) :
  barrelSuperClusterCollection_ (iConfig.getParameter<edm::InputTag> ("barrelSuperclusterCollection")),
  endcapSuperClusterCollection_ (iConfig.getParameter<edm::InputTag> ("endcapSuperclusterCollection")),
  muonTracksCollection_ (iConfig.getParameter<edm::InputTag> ("muonTracksCollection")),
  fileName_ (iConfig.getUntrackedParameter<std::string> ("fileName", std::string ("ecalCosmicTree.root")))
{
  edm::ParameterSet trkParameters = iConfig.getParameter<edm::ParameterSet> ("TrackAssociatorParameters") ;
  trackParameters_.loadParameters ( trkParameters ) ;
  trackAssociator_.useDefaultPropagator () ;

//  // Create File
////  fileName_ += "-"+intToString (runNum_)+".tree.root" ;
//  file_ = new TFile (fileName_.c_str () , "RECREATE") ;
//  file_->cd () ;
//PG sost con TFileService?
}


// -----------------------------------------------------------------------------------------


deltaR::~deltaR ()
{
}


// -----------------------------------------------------------------------------------------


void deltaR::analyze (const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  int ievt = iEvent.id ().event () ;
  LogDebug ("deltaR") << "event " << ievt ;

  //superclusters
  edm::Handle<reco::SuperClusterCollection> bscHandle ;
  iEvent.getByLabel (barrelSuperClusterCollection_, bscHandle) ;
  edm::Handle<reco::SuperClusterCollection> escHandle ;
  iEvent.getByLabel (endcapSuperClusterCollection_, escHandle) ;

  if (! (bscHandle.isValid ()) && ! (escHandle.isValid ()))
    {
      LogWarning ("EcalCosmicsHists") << barrelSuperClusterCollection_ 
                                     << " and "
                                     << endcapSuperClusterCollection_
                                     << " not available" ;
      return ;
    }


  edm::Handle<reco::TrackCollection> recoTracks ;
  iEvent.getByLabel (muonTracksCollection_, recoTracks) ;

  //PG loop over muons
  for (reco::TrackCollection::const_iterator recoTrack = recoTracks->begin () ; 
      recoTrack != recoTracks->end () ; 
      ++recoTrack)
    {
      //PG with the track associator
      TrackDetMatchInfo info = 
        trackAssociator_.associate (
          iEvent, 
          iSetup, 
          *recoTrack, 
          trackParameters_) ;

      info.trkGlobPosAtEcal.eta () ;
      info.trkGlobPosAtEcal.phi () ;
      
      //PG loop on superclusters
      for (reco::SuperClusterCollection::const_iterator sclus = bscHandle->begin () ; 
           sclus != bscHandle->end () ; 
           ++sclus) 
        {
           sclus->position ().eta () ;
           sclus->position ().phi () ;        
        } //PG loop on superclusters
    
    } //PG loop over muons



//  tree_->Fill () ;
}


// -----------------------------------------------------------------------------------------


void 
deltaR::endJob ()
{
//  file_->cd () ;
//  tree_->Write () ;
//  file_->Close () ;

}


