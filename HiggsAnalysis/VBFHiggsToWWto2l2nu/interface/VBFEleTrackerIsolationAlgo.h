// $Id: VBFEleTrackerIsolationAlgo.h,v 1.2 2008/12/31 16:39:30 govoni Exp $
// -*- C++ -*-
//
// Package:    HtoWWElectrons
// Class:      hwwEleTrackerIsolation
// 
/*
   Description: <one line class summary>
   Electron isolation using tracker info

   Implementation:
 
*/

#ifndef VBFEleTrackerIsolationAlgo_h
#define VBFEleTrackerIsolationAlgo_h

// my includes
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/TrackReco/interface/TrackExtraBase.h"
#include "DataFormats/TrackReco/interface/TrackExtraFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Common/interface/Handle.h"

//CLHEP
#include <Math/LorentzVector.h>

class VBFEleTrackerIsolationAlgo {

  public:
  
    typedef edm::View<reco::PixelMatchGsfElectron> electronCollection ;
    typedef edm::View<reco::PixelMatchGsfElectron> collection ;
    typedef reco::PixelMatchGsfElectronRef electronRef ;
    typedef edm::RefToBase<reco::PixelMatchGsfElectron> electronBaseRef ;
    typedef edm::View<reco::Track> trackCollection ; 
  
    //! constructor
    VBFEleTrackerIsolationAlgo (double coneRadius,
                                double vetoRadius,
                                double otherVetoRadius,
                                double ptMin,
                                double lipMax,
                                bool useTkQuality = true) ;
  
    //!destructor 
    ~VBFEleTrackerIsolationAlgo () ;
  
    int countNumOfTracks (const edm::Handle<electronCollection> & electrons,
                          const edm::Handle<trackCollection> & tracks,
                          const electronRef mainElectron) const ;

    float calcSumOfPt (const edm::Handle<electronCollection> & electrons,
                       const edm::Handle<trackCollection> & tracks,
                       const electronRef mainElectron) const ;

    float calcIsolationValue (const edm::Handle<electronCollection> & electrons,
                              const edm::Handle<trackCollection> & tracks,
                              const electronRef mainElectron) const ;
  

  private :
  
    double dz (const math::XYZVector & vertex,
               const math::XYZVector & momentum) const ;
    bool 
    testTrackerTrack (trackCollection::const_iterator & itTrack) const ;

 private:

    double m_coneRadius ;
    double m_vetoRadius ;
    double m_otherVetoRadius ;
    double m_ptMin ;
    double m_lipMax ;
    
    bool m_useTkQuality ;

};

#endif
