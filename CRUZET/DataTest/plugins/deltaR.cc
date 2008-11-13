// -*- C++ -*-
//
// Package:   deltaR
// Class:     deltaR
//
/**\class deltaR deltaR.cc

Description: <one line class summary>
// $Id: deltaR.h,v 1.1 2008/11/12 18:54:47 govoni Exp $

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

#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeUtils.h"

#include <vector>

using namespace cms ;
using namespace edm ;
using namespace std ;



deltaR::deltaR (const edm::ParameterSet& iConfig) :
  barrelSuperClusterCollection_ (iConfig.getParameter<edm::InputTag> ("barrelSuperclusterCollection")),
  endcapSuperClusterCollection_ (iConfig.getParameter<edm::InputTag> ("endcapSuperclusterCollection")),
  muonTracksCollection_ (iConfig.getParameter<edm::InputTag> ("muonTracksCollection"))
{
  edm::ParameterSet trkParameters = iConfig.getParameter<edm::ParameterSet> ("TrackAssociatorParameters") ;
  trackParameters_.loadParameters ( trkParameters ) ;
  trackAssociator_.useDefaultPropagator () ;

  edm::Service<TFileService> fileService ;
  m_deltaRSCMu = fileService->make<TH1F> ("deltaRSCMu","deltaRSCMu",1000,0,5) ;

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

      double e1 = info.trkGlobPosAtEcal.eta () ;
      double p1 = info.trkGlobPosAtEcal.phi () ;
      
      //PG loop on superclusters
      for (reco::SuperClusterCollection::const_iterator sclus = bscHandle->begin () ; 
           sclus != bscHandle->end () ; 
           ++sclus) 
        {
           double e2 = sclus->position ().eta () ;
           double p2 = sclus->position ().phi () ;      
           
           double delta = sqrt (ect::deltaRsq ( e1,  p1,  e2,  p2)) ;
           
           m_deltaRSCMu->Fill (delta) ;  
        } //PG loop on superclusters
    
    } //PG loop over muons


}


// -----------------------------------------------------------------------------------------


void 
deltaR::endJob ()
{
//  file_->cd () ;
//  tree_->Write () ;
//  file_->Close () ;
}


