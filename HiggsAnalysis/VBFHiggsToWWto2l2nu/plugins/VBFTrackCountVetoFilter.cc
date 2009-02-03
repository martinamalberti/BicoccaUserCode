// $Id: VBFTrackCountVetoFilter.cc,v 1.3 2008/03/31 09:29:15 govoni Exp $

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/plugins/VBFTrackCountVetoFilter.h"
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidate.h>
#include <DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h>

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include <iostream>
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include <Math/VectorUtil.h>

//! ctor
VBFTrackCountVetoFilter::VBFTrackCountVetoFilter (const edm::ParameterSet& iConfig) :
  m_GSFInputTag (iConfig.getParameter<edm::InputTag> ("GSFInputTag")) ,
  m_tracksInputTag (iConfig.getParameter<edm::InputTag> ("tracksInputTag")) ,
  m_tagJetInputTag (iConfig.getParameter<edm::InputTag> ("tagJetInputTag")) ,
  m_deltaEta (iConfig.getParameter<double> ("deltaEta")) ,
  m_maxTracksNum (iConfig.getParameter<int> ("maxTracksNum")) ,
  m_otherVetoRadius (iConfig.getParameter<double> ("otherVetoRadius")) ,
  m_ptMin (iConfig.getParameter<double> ("ptMin")) ,
  m_lipMax (iConfig.getParameter<double> ("lipMax")) ,
  m_useTkQuality (iConfig.getUntrackedParameter<bool> ("useTkQuality",true))
{}


// ------------------------------------------------------------------------------------


//! dtor
VBFTrackCountVetoFilter::~VBFTrackCountVetoFilter ()
{}


// ------------------------------------------------------------------------------------


//! filtering method
bool 
VBFTrackCountVetoFilter::filter (edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //PG get the tracks collection
  edm::Handle<trackCollection> TrackHandle ;
  iEvent.getByLabel (m_tracksInputTag, TrackHandle) ;

  //PG get the GSF electrons collection
  edm::Handle<electronCollection> GSFHandle;
  iEvent.getByLabel (m_GSFInputTag,GSFHandle); 

  // Get the tag jets
  edm::Handle<reco::RecoChargedCandidateCollection> jetTagsHandle ;
  iEvent.getByLabel (m_tagJetInputTag, jetTagsHandle) ;

  //PG assume there are two tag jets
  LorentzVector firstTag = (*jetTagsHandle)[0].p4 () ;
  LorentzVector secondTag = (*jetTagsHandle)[1].p4 () ;

  double etaMax = firstTag.eta () ;
  double etaMin = secondTag.eta () ;
  if (etaMin > etaMax) 
    {
      double tempo = etaMin ;
      etaMin = etaMax ; 
      etaMax = tempo ;
    }

  etaMin -= m_deltaEta ;
  etaMax += m_deltaEta ;

  int counter = 0 ;

//FIXME  std::cerr << "PIETRO LOOP on particles\n" ;
  //PG loop over tracks
  for (trackCollection::const_iterator trackIt = TrackHandle->begin () ;
       trackIt != TrackHandle->end () ; 
       ++trackIt)
    {    
      //PG min pT threshold
//      math::XYZVector tmpTrackMomentumAtVtx = (*trackIt).momentum () ; 
//      double this_pt = (*trackIt).pt () ;
      math::XYZVector tmpTrackMomentumAtVtx = (*trackIt).innerMomentum () ; 
      double this_pt  = sqrt (tmpTrackMomentumAtVtx.Perp2 ()) ;
  
      if ( this_pt < m_ptMin ) continue ;  
      //PG impact parameter threshold
      if (fabs( (*trackIt).dz () ) > m_lipMax) continue ;

      if (m_useTkQuality && !testTrackerTrack (trackIt)) continue ;
      bool countTrack = true ;

      //PG loop over electrons
      if (m_otherVetoRadius > 0.0001) //PG to avoid useless caclulations 
        {
//FIXME          std::cerr << "PIETRO entering the veto\n" ;
          for (electronCollection::const_iterator eleIt = GSFHandle->begin () ; 
               eleIt != GSFHandle->end () ;
               ++eleIt)
            {
                math::XYZVector eleMomentum = eleIt->trackMomentumAtVtx () ; 
//FIXME                std::cerr << "PIETRO match check " 
//FIXME                          << tmpTrackMomentumAtVtx.eta () << " " 
//FIXME                          << eleMomentum.eta () << " --- " 
//FIXME                          << this_pt << " " 
//FIXME                          << eleIt->pt () << "...\n" ;
//                math::XYZVector eleVtx = eleIt->TrackPositionAtVtx () ; 
//                if (fabs( (*trackIt).dz () - dz (eleVtx,eleMomentum) ) > m_lipMax) continue ;
                double eleDr = ROOT::Math::VectorUtil::DeltaR (tmpTrackMomentumAtVtx,eleMomentum) ;
                if (eleDr < m_otherVetoRadius) 
                  {
//FIXME                    std::cerr << "PIETRO match FOUND " 
//FIXME                              << tmpTrackMomentumAtVtx.eta () << " " 
//FIXME                              << eleMomentum.eta () << " --- " 
//FIXME                              << this_pt << " " 
//FIXME                              << eleIt->pt ()  << "\n" ;
                    countTrack = false ;
                    break ;
                  }
            } //PG loop over electrons
         } //PG if (m_otherVetoRadius > 0.0001)
         
      if (countTrack && 
          tmpTrackMomentumAtVtx.eta () < etaMax && 
          tmpTrackMomentumAtVtx.eta () > etaMin)
        {
          ++counter ;
        }
    } //PG loop over tracks

  if (counter > m_maxTracksNum) return false ;
  return true ;
}


// ------------------------------------------------------------------------------------------------


//! cloned from:
//! http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/HiggsAnalysis/HiggsToZZ2e2m/plugins/HZZ2e2muIsolationAnalyzer.cc?revision=1.2&view=markup
//! and used for the isolation also
bool 
VBFTrackCountVetoFilter::testTrackerTrack (trackCollection::const_iterator & itTrack) const
{

  // Extract track properties
  float d0  = fabs (itTrack->d0 ()) ;
  float ed0 = itTrack->d0Error () ;
  float dz  = fabs (itTrack->dz ()) ;
  float edz = itTrack->dzError () ;
  int nhits = itTrack->recHitsSize () ;

  if ( nhits < 8 ) {
    if ( d0 > 0.04 ) return false;
    if ( dz > 0.50 ) return false;
    if ( d0 / ed0 > 7.0 ) return false;
    if ( dz / edz > 10. ) return false;
  }
  else if ( nhits < 10 ) {
    if ( d0 > 0.20 ) return false;
    if ( dz > 2.00 ) return false;
    if ( d0 / ed0 > 10. ) return false;
    if ( dz / edz > 10. ) return false;
  }
  else {
    if ( d0 > 1.00 ) return false;
    if ( dz > 5.00 ) return false;
  }

  return true ;

}
