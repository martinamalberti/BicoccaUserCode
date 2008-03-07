// $Id: VBFEleTrackerIsolationAlgo.cc,v 1.6 2008/03/07 11:49:17 govoni Exp $
#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFEleTrackerIsolationAlgo.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include <Math/VectorUtil.h>
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"


// ------------------------------------------------------------------------------------------------


VBFEleTrackerIsolationAlgo::VBFEleTrackerIsolationAlgo (double coneRadius,
                                                        double vetoRadius,
                                                        double otherVetoRadius,
                                                        double ptMin,
                                                        double lipMax) :
  m_coneRadius (coneRadius) ,
  m_vetoRadius (vetoRadius) ,
  m_otherVetoRadius (otherVetoRadius) ,
  m_ptMin (ptMin) ,
  m_lipMax (lipMax)
{}                                                        


// ------------------------------------------------------------------------------------------------


VBFEleTrackerIsolationAlgo::~VBFEleTrackerIsolationAlgo () {}


// ------------------------------------------------------------------------------------------------


float 
VBFEleTrackerIsolationAlgo::calcSumOfPt (const edm::Handle<electronCollection> & electrons,
                                         const edm::Handle<trackCollection> & tracks,
                                         const electronRef mainElectron) const
{
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
      math::XYZVector tmpTrackMomentumAtVtx = (*trackIt).momentum () ; 
      double this_pt = (*trackIt).pt () ;
      if ( this_pt < m_ptMin ) continue ;  
      //PG impact parameter threshold
      if (fabs( (*trackIt).dz () - dz (tmpElectronPositionAtVtx,tmpElectronMomentumAtVtx) ) 
          > m_lipMax) continue ;

      bool countTrack = true ;

      //PG loop over electrons
      if (m_otherVetoRadius > 0.0001) //PG to avoid useless caclulations 
        for (electronCollection::const_iterator eleIt = electrons->begin () ; 
             eleIt != electrons->end () ;
             ++eleIt)
          {
//              reco::GsfTrackRef eleTrack = eleIt->gsfTrack () ;
              math::XYZVector eleMomentum = eleIt->trackMomentumAtVtx () ; 
              math::XYZVector eleVtx = eleIt->TrackPositionAtVtx () ; 
              if (fabs( (*trackIt).dz () - dz (eleVtx,eleMomentum) ) > m_lipMax) continue ;
              double eleDr = ROOT::Math::VectorUtil::DeltaR (tmpTrackMomentumAtVtx,eleMomentum) ;
              if (eleDr < m_otherVetoRadius) 
                {
                  countTrack = false ;
                  break ;
                }
          } //PG loop over electrons
        
      double dr = ROOT::Math::VectorUtil::DeltaR (tmpTrackMomentumAtVtx,tmpElectronMomentumAtVtx) ;
      if ( countTrack && 
           fabs (dr) < m_coneRadius && 
           fabs (dr) >= m_vetoRadius )
        {
//          ++counter ;
          ptSum += this_pt;
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
