// $Id: VBFElePlots.cc,v 1.8 2008/02/14 14:43:55 govoni Exp $
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
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/TrackReco/interface/TrackExtraBase.h"
#include "DataFormats/TrackReco/interface/TrackExtraFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Common/interface/Handle.h"

//CLHEP
#include <CLHEP/Vector/LorentzVector.h>

class VBFEleTrackerIsolationAlgo {

  public:
  
    typedef edm::View<reco::PixelMatchGsfElectron> electronCollection ;
    typedef reco::PixelMatchGsfElectron* electronPtr ;
    typedef reco::PixelMatchGsfElectronRef electronRef ;
    typedef edm::View<reco::Track> trackCollection ; 
  
    //! constructor
    VBFEleTrackerIsolationAlgo (double coneRadius,
                                double vetoRadius,
                                double otherVetoRadius,
                                double ptMin,
                                double lipMax) ;
  
    //!destructor 
    ~VBFEleTrackerIsolationAlgo () ;
  
    float calcSumOfPt (const electronCollection & electrons,
                       const trackCollection & tracks,
                       const electronPtr mainElectron) const ;

    float calcIsolationValue (const electronCollection & electrons,
                              const trackCollection & tracks,
                              const electronPtr mainElectron) const ;
  
 private:

    double m_coneRadius ;
    double m_vetoRadius ;
    double m_otherVetoRadius ;
    double m_ptMin ;
    double m_lipMax ;

};

#endif
