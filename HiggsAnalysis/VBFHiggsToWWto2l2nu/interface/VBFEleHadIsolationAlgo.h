// $Id: VBFEleHadIsolationAlgo.h,v 1.2 2008/12/31 16:39:30 govoni Exp $
// -*- C++ -*-
//
// 
/*
   Description: <one line class summary>
   Electron isolation using HCAL info

   Implementation:
 
*/

#ifndef VBFEleHadIsolationAlgo_h
#define VBFEleHadIsolationAlgo_h

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
#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "RecoCaloTools/MetaCollections/interface/CaloRecHitMetaCollections.h"


//CLHEP
#include <Math/LorentzVector.h>

class VBFEleHadIsolationAlgo {

  public:
  
    typedef edm::View<reco::PixelMatchGsfElectron> electronCollection ;
    typedef edm::View<reco::PixelMatchGsfElectron> collection ;
    typedef reco::PixelMatchGsfElectronRef electronRef ;
    typedef edm::RefToBase<reco::PixelMatchGsfElectron> electronBaseRef ;
    typedef edm::View<reco::Track> trackCollection ; 
//FIXME    typedef edm::View<reco::Track> hHitCollection ; 
  
    //! constructor
    VBFEleHadIsolationAlgo (double coneRadius,
                            double vetoRadius,
                            double etMin) ;
  
    //!destructor 
    ~VBFEleHadIsolationAlgo () ;
  
    float calcSumOfEt (edm::ESHandle<CaloGeometry> & , 
                       HBHERecHitMetaCollection & ,
                       const electronRef mainElectron ,
                       const edm::Handle<electronCollection> & electrons) const ;  //PG FIXME qs a che serve? 

    float calcIsolationValue (edm::ESHandle<CaloGeometry> & , 
                              HBHERecHitMetaCollection & ,
                              const electronRef mainElectron ,
                              const edm::Handle<electronCollection> & electrons) const ;  //PG FIXME qs a che serve? 

 private:

    double m_coneRadius ;
    double m_vetoRadius ;
    double m_etMin ;

};

#endif
