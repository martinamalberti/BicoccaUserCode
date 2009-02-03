#ifndef VBFTrackCountVetoFilter_h#define VBFTrackCountVetoFilter_h// -*- C++ -*-//// Package:    VBFTrackCountVetoFilter// Class:      VBFTrackCountVetoFilter// /*  Description: filter events based on the Pythia ProcessID and the Pt_hat
Implementation: inherits from generic EDFilter

*/
//
// $Id: VBFTrackCountVetoFilter.h,v 1.3 2008/12/31 16:39:31 govoni Exp $
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

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"

#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFUtils.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/TrackReco/interface/TrackExtraBase.h"
#include "DataFormats/TrackReco/interface/TrackExtraFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"



class VBFTrackCountVetoFilter : public edm::EDFilter 
{
     typedef math::XYZTLorentzVector LorentzVector ;
     typedef reco::CaloJetCollection::const_iterator VBFjetIt ;
     typedef edm::View<reco::Track> trackCollection ; 
     typedef edm::View<reco::PixelMatchGsfElectron> electronCollection ;

   public:
      //! ctor
      explicit VBFTrackCountVetoFilter (const edm::ParameterSet&) ;
      //! dtor
      ~VBFTrackCountVetoFilter();
      //! the filtering method
      virtual bool filter (edm::Event&, const edm::EventSetup&) ;
      
   private:
   
      bool testTrackerTrack (trackCollection::const_iterator & itTrack) const ;

   private:

      edm::InputTag m_GSFInputTag ;
      edm::InputTag m_tracksInputTag ;
      edm::InputTag m_tagJetInputTag ;
      double m_deltaEta ;
      //! max number of tracks accepted in the central eta region
      int m_maxTracksNum ;
      //! size of the veto cone around GSFElectrons
      double m_otherVetoRadius ;
      //! min pt of the accepted tracks
      double m_ptMin ;
      //! max lip of the accepted tracks
      double m_lipMax ;
      //! use or not the track quality reqs implemented for the isolation
      //! based on the TIP and LIP
      //! to be upgraded to a more general version shared with isolation
      //! so far acquired from H->ZZ->4e analysis
      bool m_useTkQuality ;

};

#endif


