// $Id: VBFEleTrackerIsolationAlgo.cc,v 1.3 2008/12/31 16:39:31 govoni Exp $
#include "HiggsAnalysis/VBFHiggsToWWto2l2nu/interface/VBFEleTrackerIsolationAlgo.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include <Math/VectorUtil.h>
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"


// ------------------------------------------------------------------------------------------------


VBFEleTrackerIsolationAlgo::VBFEleTrackerIsolationAlgo (double coneRadius,
                                                        double vetoRadius,
                                                        double otherVetoRadius,
                                                        double ptMin,
                                                        double lipMax,
                                                        bool useTkQuality) :
  m_coneRadius (coneRadius) ,
  m_vetoRadius (vetoRadius) ,
  m_otherVetoRadius (otherVetoRadius) ,
  m_ptMin (ptMin) ,
  m_lipMax (lipMax) ,
  m_useTkQuality (useTkQuality)
{
//  std::cerr << "[VBFEleTrackerIsolationAlgo][ctor] "
//            << " m_coneRadius " << m_coneRadius
//            << " m_vetoRadius " << m_vetoRadius
//            << " m_otherVetoRadius " << m_otherVetoRadius
//            << " m_ptMin " << m_ptMin
//            << " m_lipMax " << m_lipMax << std::endl ;
}                                                        


// ------------------------------------------------------------------------------------------------


VBFEleTrackerIsolationAlgo::~VBFEleTrackerIsolationAlgo () {}


// ------------------------------------------------------------------------------------------------


int 
VBFEleTrackerIsolationAlgo::countNumOfTracks (const edm::Handle<electronCollection> & electrons,
                                              const edm::Handle<trackCollection> & tracks,
                                              const electronRef mainElectron) const 
{
  int counter = 0 ;

  //Take the electron track
  math::XYZVector tmpElectronMomentumAtVtx = mainElectron->trackMomentumAtVtx () ; 
  math::XYZVector tmpElectronPositionAtVtx = mainElectron->TrackPositionAtVtx () ; 
//  reco::GsfTrackRef tmpTrack = mainElectron->gsfTrack () ;
//  math::XYZVector tmpElectronMomentumAtVtx = (*tmpTrack).momentum () ; 

  //PG loop over tracks
  for (trackCollection::const_iterator trackIt = tracks->begin () ;
       trackIt != tracks->end () ; 
       ++trackIt)
    {    
      //PG min pT threshold
//      math::XYZVector tmpTrackMomentumAtVtx = (*trackIt).momentum () ; 
//      double this_pt = (*trackIt).pt () ;
      math::XYZVector tmpTrackMomentumAtVtx = (*trackIt).innerMomentum () ; 
      double this_pt  = sqrt (tmpTrackMomentumAtVtx.Perp2 ()) ;
  
      if ( this_pt < m_ptMin ) continue ;  
      //PG impact parameter threshold
      if (fabs( (*trackIt).dz () - dz (tmpElectronPositionAtVtx,tmpElectronMomentumAtVtx) ) 
          > m_lipMax) continue ;

      if (m_useTkQuality && !testTrackerTrack (trackIt)) continue ;
      bool countTrack = true ;

      //PG loop over electrons
      if (m_otherVetoRadius > 0.0001) //PG to avoid useless caclulations 
        {
          for (electronCollection::const_iterator eleIt = electrons->begin () ; 
               eleIt != electrons->end () ;
               ++eleIt)
            {
                electronBaseRef electronBaseReference = electrons->refAt (eleIt - electrons->begin ()) ;
                electronRef electronReference = electronBaseReference.castTo<electronRef> () ;
                if (electronReference == mainElectron) continue ;
                  
                math::XYZVector eleMomentum = eleIt->trackMomentumAtVtx () ; 
//                math::XYZVector eleVtx = eleIt->TrackPositionAtVtx () ; 
//                if (fabs( (*trackIt).dz () - dz (eleVtx,eleMomentum) ) > m_lipMax) continue ;
                double eleDr = ROOT::Math::VectorUtil::DeltaR (tmpTrackMomentumAtVtx,eleMomentum) ;
                if (eleDr < m_otherVetoRadius) 
                  {
                    countTrack = false ;
                    break ;
                  }
            } //PG loop over electrons
         } //PG if (m_otherVetoRadius > 0.0001)
         
      double dr = ROOT::Math::VectorUtil::DeltaR (tmpTrackMomentumAtVtx,tmpElectronMomentumAtVtx) ;
      if ( countTrack && 
           dr < m_coneRadius && 
           dr >= m_vetoRadius )
        {
          ++counter ;
        }
    } //PG loop over tracks
  
  return counter ;

}                                              


// ------------------------------------------------------------------------------------------------


float 
VBFEleTrackerIsolationAlgo::calcSumOfPt (const edm::Handle<electronCollection> & electrons,
                                         const edm::Handle<trackCollection> & tracks,
                                         const electronRef mainElectron) const
{
  //std::cout << "VBFEleTrackerIsolationAlgo::calcSumOfPt" << std::endl;
  float ptSum = 0 ;

  //Take the electron track
  math::XYZVector tmpElectronMomentumAtVtx = mainElectron->trackMomentumAtVtx () ; 
  math::XYZVector tmpElectronPositionAtVtx = mainElectron->TrackPositionAtVtx () ; 
//  reco::GsfTrackRef tmpTrack = mainElectron->gsfTrack () ;
//  math::XYZVector tmpElectronMomentumAtVtx = (*tmpTrack).momentum () ; 

  //PG loop over tracks
  for (trackCollection::const_iterator trackIt = tracks->begin () ;
       trackIt != tracks->end () ; 
       ++trackIt)
    {    
      //PG min pT threshold
//      math::XYZVector tmpTrackMomentumAtVtx = (*trackIt).momentum () ; 
//      double this_pt = (*trackIt).pt () ;
      math::XYZVector tmpTrackMomentumAtVtx = (*trackIt).innerMomentum () ; 
      double this_pt  = sqrt (tmpTrackMomentumAtVtx.Perp2 ()) ;
  
      int nhits = (*trackIt).recHitsSize () ; //VT add
	  if ( nhits < 5 ) continue; //VT add
			
      if ( this_pt < m_ptMin ) continue ;
	    
      //PG impact parameter threshold
      //if (fabs( (*trackIt).dz () - dz (tmpElectronPositionAtVtx,tmpElectronMomentumAtVtx) ) 
	  //   > m_lipMax) continue ;
      //std::cout << "lip " << fabs( (*trackIt).vz () - tmpElectronPositionAtVtx.z() ) << std::endl ;
	  if (fabs( (*trackIt).vz () - tmpElectronPositionAtVtx.z() ) //VT add
		 > m_lipMax) continue ;  //VT add	  

      if (!testTrackerTrack (trackIt)) continue ;
      bool countTrack = true ;

      //PG loop over electrons
      if (m_otherVetoRadius > 0.0001) //PG to avoid useless caclulations 
        {
          for (electronCollection::const_iterator eleIt = electrons->begin () ; 
               eleIt != electrons->end () ;
               ++eleIt)
            {
                electronBaseRef electronBaseReference = electrons->refAt (eleIt - electrons->begin ()) ;
                electronRef electronReference = electronBaseReference.castTo<electronRef> () ;
                if (electronReference == mainElectron) continue ;
                  
                math::XYZVector eleMomentum = eleIt->trackMomentumAtVtx () ; 
//                math::XYZVector eleVtx = eleIt->TrackPositionAtVtx () ; 
//                if (fabs( (*trackIt).dz () - dz (eleVtx,eleMomentum) ) > m_lipMax) continue ;
                double eleDr = ROOT::Math::VectorUtil::DeltaR (tmpTrackMomentumAtVtx,eleMomentum) ;
                if (eleDr < m_otherVetoRadius) 
                  {
                    countTrack = false ;
                    break ;
                  }
            } //PG loop over electrons
         } //PG if (m_otherVetoRadius > 0.0001)
         
      double dr = ROOT::Math::VectorUtil::DeltaR (tmpTrackMomentumAtVtx,tmpElectronMomentumAtVtx) ;
      if ( countTrack && 
           dr < m_coneRadius && 
           dr >= m_vetoRadius )
        {
//          ++counter ;
          ptSum += this_pt ;
        }
    } //PG loop over tracks
  
  return ptSum ;
}


// ------------------------------------------------------------------------------------------------


float 
VBFEleTrackerIsolationAlgo::calcIsolationValue (const edm::Handle<electronCollection> & electrons,
                                                const edm::Handle<trackCollection> & tracks,
                                                const electronRef mainElectron) const
{

  float isoVal = calcSumOfPt (electrons, tracks, mainElectron) ;
  isoVal /= mainElectron->pt () ;
  return isoVal ;
}


// ------------------------------------------------------------------------------------------------


double VBFEleTrackerIsolationAlgo::dz (const math::XYZVector & vertex,
                                       const math::XYZVector & momentum) const 
{ 
  double invpt = 1. / sqrt ( momentum.Perp2 () ) ;
  return vertex.z () - 
        (vertex.x () * momentum.x () + vertex.y () * momentum.y ()) * invpt * 
        (momentum.z () * invpt) ; 
}


// ------------------------------------------------------------------------------------------------


//! cloned from:
//! http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/HiggsAnalysis/HiggsToZZ2e2m/plugins/HZZ2e2muIsolationAnalyzer.cc?revision=1.2&view=markup
bool 
VBFEleTrackerIsolationAlgo::testTrackerTrack (trackCollection::const_iterator & itTrack) const
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
