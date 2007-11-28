#include "FakeRate/EffAnalysis/plugins/FakeRateTkIsolation.h"
#include "FakeRate/EffAnalysis/plugins/TkIsolation.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "RecoCaloTools/Selectors/interface/CaloConeSelector.h"

#include <iostream>
#include <algorithm>

//ROOT includes
#include <Math/VectorUtil.h>

using namespace reco;
using namespace std;
//using namespace ROOT::Math::VectorUtil ;

FakeRateTkIsolation::FakeRateTkIsolation(const edm::ParameterSet& conf)
{

  tracksLabel_ = conf.getParameter<string>("tracks");
  radiusConeExt_  = conf.getParameter<double>("radiusConeExt");
  radiusConeInt_ = conf.getParameter<double>("radiusConeInt");
  pTMin_ = conf.getParameter<double>("pTMin");
  lip_ = conf.getParameter<double>("lip");
  cut_ = conf.getParameter<double>("cut");
}  
  
FakeRateTkIsolation::~FakeRateTkIsolation()
{
}

void FakeRateTkIsolation::select(edm::Handle<reco::PixelMatchGsfElectronCollection> c, const edm::Event& e)
{

  selected_.clear();

  // get track collection
  edm::Handle<TrackCollection> tracks;
  e.getByLabel(tracksLabel_, tracks); 
  const reco::TrackCollection* trackCollection = tracks.product () ;
  
  for(int i = 0 ; i < c->size(); ++i) 
   {
    TkIsolation myTkIsolation (&((*c)[i]),trackCollection) ;
    myTkIsolation.setExtRadius (radiusConeExt_) ; 
    myTkIsolation.setIntRadius (radiusConeInt_) ;
    myTkIsolation.setPtLow (pTMin_) ;
    myTkIsolation.setLip (lip_) ;
//    double isoValue = myTkIsolation.getPtTracks()  ;
    double isoValue = myTkIsolation.getPtTracks()/(*c)[i].pt() ;
//    double isoValue = myTkIsolation.getSqPtTracks()/(*c)[i].pt()/(*c)[i].pt() ;
    if ( isoValue < cut_ ) 
    	selected_.push_back (electron (c, i)) ;
  }  

}

