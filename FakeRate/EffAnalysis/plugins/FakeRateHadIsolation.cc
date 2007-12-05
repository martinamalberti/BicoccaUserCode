#include "FakeRate/EffAnalysis/plugins/FakeRateHadIsolation.h"
#include "FakeRate/EffAnalysis/plugins/HadIsolation.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "RecoCaloTools/Selectors/interface/CaloConeSelector.h"

#include <iostream>
#include <algorithm>

using namespace reco;
using namespace std;

FakeRateHadIsolation::FakeRateHadIsolation(const edm::ParameterSet& conf)
{
 
  hcalrhitsLabel_ = conf.getParameter<string>("hcalrhits");
  radiusConeExt_  = conf.getParameter<double>("radiusConeExt");
  radiusConeInt_ = conf.getParameter<double>("radiusConeInt");
  eTMin_ = conf.getParameter<double>("eTMin");
  cut_ = conf.getParameter<double>("cut");
}  
  
FakeRateHadIsolation::~FakeRateHadIsolation()
{
}

void FakeRateHadIsolation::select(edm::Handle<reco::PixelMatchGsfElectronCollection> c, const edm::Event& e, const edm::EventSetup& es )
{

  selected_.clear();

  //get the HCAL rechit collection
  edm::Handle<HBHERecHitCollection> hbhe;
  e.getByLabel(hcalrhitsLabel_, hbhe);//getByType(hbhe);  
//  HBHERecHitMetaCollection *mhbhe = 0;
//  mhbhe =  &HBHERecHitMetaCollection(*hbhe);  //FIXME, generates warning
  HBHERecHitMetaCollection mhbhe = HBHERecHitMetaCollection(*hbhe);  

  //services
  //get calo geometry
  es.get<IdealGeometryRecord>().get(theCaloGeom_);
  //product the geometry
  theCaloGeom_.product() ;

  for(int i = 0 ; i < c->size(); ++i) 
   {
    HadIsolation myHadIsolation(theCaloGeom_,&mhbhe,&((*c)[i])) ;
    myHadIsolation.setExtRadius (radiusConeExt_) ; 
    myHadIsolation.setEtLow (eTMin_) ;
//    double isoValue = myHadIsolation.getEtHadClusters() ;
    double isoValue = myHadIsolation.getEtHadClusters()/(*c)[i].pt() ;
    if ( isoValue < cut_ ) 
    	selected_.push_back( electron (c, i) );
  }  

}

