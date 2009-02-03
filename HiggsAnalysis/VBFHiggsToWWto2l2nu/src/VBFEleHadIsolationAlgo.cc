// $Id: VBFEleHadIsolationAlgo.cc,v 1.2 2008/12/31 16:39:31 govoni Exp $
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFEleHadIsolationAlgo.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include <Math/VectorUtil.h>
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"

#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "RecoCaloTools/Selectors/interface/CaloDualConeSelector.h"
#include "RecoCaloTools/MetaCollections/interface/CaloRecHitMetaCollections.h"
#include "RecoCaloTools/MetaCollections/interface/CaloRecHitMetaCollectionFast.h" 
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"


// ------------------------------------------------------------------------------------------------


VBFEleHadIsolationAlgo::VBFEleHadIsolationAlgo (double coneRadius,
                                                double vetoRadius,
                                                double etMin) :
  m_coneRadius (coneRadius) ,
  m_vetoRadius (vetoRadius) ,
  m_etMin (etMin)
{
//  std::cerr << "[VBFEleHadIsolationAlgo][ctor] "
//            << " m_coneRadius " << m_coneRadius
//            << " m_vetoRadius " << m_vetoRadius
//            << " m_etMin " << m_etMin << std::endl ;
}                                                        


// ------------------------------------------------------------------------------------------------


VBFEleHadIsolationAlgo::~VBFEleHadIsolationAlgo () {}


// ------------------------------------------------------------------------------------------------


//PG FIXME try and bring out the geometry build once per event
float 
VBFEleHadIsolationAlgo::calcSumOfEt (edm::ESHandle<CaloGeometry> & caloGeom, 
                                     HBHERecHitMetaCollection & mhbhe,
                                     const electronRef mainElectron,
                                     const edm::Handle<electronCollection> & electrons) const
{
  float etSum = 0 ;

  //Take the SC position FIXME questo non puo' essere fatto per ora, perche' non ho i supercluster
  const CaloGeometry * caloGeomPtr = caloGeom.product () ;
  CaloDualConeSelector * sel = new CaloDualConeSelector (m_vetoRadius ,m_coneRadius, caloGeomPtr, DetId::Hcal) ;
//  math::XYZPoint theCaloPosition = mainElectron->caloPosition () ;
  //PG FIXME this is necessary as long as we don't have superclusters
  //PG FIXME in our skims
  math::XYZVector theCaloPosition = mainElectron->TrackPositionAtCalo () ;

  GlobalPoint pclu (theCaloPosition.x () ,
                    theCaloPosition.y () ,
                    theCaloPosition.z () ) ;

  //Compute the HCAL energy behind ECAL
  std::auto_ptr<CaloRecHitMetaCollectionV> chosen = sel->select (pclu, mhbhe) ;
//  std::auto_ptr<CaloRecHitMetaCollectionV> chosen = sel->select (theCaloPosition.eta (), theCaloPosition.phi (), mhbhe) ;
  for (CaloRecHitMetaCollectionV::const_iterator i = chosen->begin () ; 
       i!= chosen->end () ; 
       ++i)
   {
     double hcalHit_eta = caloGeom->getPosition (i->detid ()).eta () ;
     double hcalHit_Et = i->energy () * sin (2 * atan (exp (-hcalHit_eta))) ;
     if ( hcalHit_Et > m_etMin )
       etSum += hcalHit_Et ;
   }

  delete sel ;
  return etSum ;
}


// ------------------------------------------------------------------------------------------------


float 
VBFEleHadIsolationAlgo::calcIsolationValue (edm::ESHandle<CaloGeometry> & caloGeom, 
                                            HBHERecHitMetaCollection & mhbhe,
                                            const electronRef mainElectron,
                                            const edm::Handle<electronCollection> & electrons) const 
{

  float isoVal = calcSumOfEt (caloGeom, mhbhe, mainElectron, electrons) ;
  isoVal /= mainElectron->pt () ;
  return isoVal ;
}

