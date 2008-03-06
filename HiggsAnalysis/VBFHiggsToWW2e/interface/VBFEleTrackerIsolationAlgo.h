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
//
// Original Author:  Chiara Rovelli
//
//


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

using namespace edm;
using namespace std;
using namespace reco;

class VBFEleTrackerIsolationAlgo{
 public:
  
  //constructors
  VBFEleTrackerIsolationAlgo();
  VBFEleTrackerIsolationAlgo(const PixelMatchGsfElectron *gsfEle, const TrackCollection trackColl);

  //methods
  void setExtRadius (float extRadius);
  void setIntRadius (float intRadius);
  
  float getPtTracks () const;
  float minDeltaR (float minPt) const;
  float minDeltaR_withVeto (float minPt) const;
  bool isIsolated (float ptCut = 0.05) const;

  //destructor 
  ~VBFEleTrackerIsolationAlgo();
  
 private:

  const PixelMatchGsfElectron*_myGsfEle;  	  
  const TrackCollection _tracks;
  
  float _extRadius;
  float _intRadius;
};

#endif
