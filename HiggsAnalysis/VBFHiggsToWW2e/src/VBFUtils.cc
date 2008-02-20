// $Id: VBFUtils.cc,v 1.6 2008/02/20 10:38:16 tancini Exp $

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFUtils.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include <iostream>

void setMomentum (TLorentzVector & myvector, const reco::Candidate & gen)
{
    myvector.SetPx (gen.px ()) ;
    myvector.SetPy (gen.py ()) ;
    myvector.SetPz (gen.pz ()) ;
    myvector.SetE (gen.energy ()) ;
}


// --------------------------------------------------------------------


std::pair<VBFjetIt,VBFjetIt>	
findTagJets (VBFjetIt begin, VBFjetIt end,
             double jetPtMin, double jetEtaMax) 
{

  std::pair<VBFjetIt,VBFjetIt> tagJets (begin,begin) ;
  double maxInvMass = 0. ;

  //PG find the tagging jets

  //PG first loop over jets
  for (VBFjetIt firstJet = begin ; 
       firstJet != end ; 
       ++firstJet ) 
    {
      if (firstJet->pt () < jetPtMin || 
          fabs (firstJet->eta ()) > jetEtaMax) continue ;
      math::XYZTLorentzVector firstLV = firstJet->p4 () ;
      //PG second loop over jets
      for (VBFjetIt secondJet = firstJet + 1 ; 
           secondJet != end ; 
           ++secondJet ) 
        {
          if (secondJet->pt () < jetPtMin || 
              fabs (secondJet->eta ()) > jetEtaMax) continue ;
          math::XYZTLorentzVector sumLV = secondJet->p4 () + firstLV ;
          if (sumLV.M () > maxInvMass)
            {
              maxInvMass = sumLV.M () ;
              tagJets.first = firstJet ;
              tagJets.second = secondJet ;
            }
        } //PG second loop over jets
    } //PG first loop over jets

  return tagJets ;
}

// --------------------------------------------------------------------


std::pair<VBFjetIt,VBFjetIt>
findMaxPtJetsPair (reco::CaloJetCollection &coll,
             double jetPtMin, double jetEtaMax)
{

  ptSorting<reco::CaloJet> ptComparator;
  std::sort (coll.begin(), coll.end(), ptComparator);

  VBFjetIt begin = coll.begin();
  VBFjetIt end = coll.end();
  std::pair<VBFjetIt,VBFjetIt> tagJets (begin,begin) ;

  VBFjetIt first ;
  VBFjetIt second ;

  double firstMaxPt = 0 ;
  double secondMaxPt = 0 ;

  //PG first loop over jets
  for (VBFjetIt Jet = begin ;
       Jet != end ;
       ++Jet )
    {
      if (Jet->pt () < jetPtMin ||
          fabs (Jet->eta ()) > jetEtaMax) continue ;

      if (firstMaxPt < Jet->pt ())
        {
          secondMaxPt = firstMaxPt ;
          second = first ;
          firstMaxPt = Jet->pt () ;
          first = Jet ;
        }
      else if (secondMaxPt < Jet->pt ())
        {
          secondMaxPt = Jet->pt () ;
          second = Jet ;
        }  
    } //close loop

  tagJets.first = first ;
  tagJets.second = second ;

  return tagJets ;

    }

// --------------------------------------------------------------------------------

double deltaPhi (double phi1, double phi2)
{

  double deltaphi=fabs(phi1-phi2);
  if (deltaphi > 6.283185308) deltaphi -= 6.283185308;
  if (deltaphi > 3.141592654) deltaphi = 6.283185308-deltaphi;
  return deltaphi;
}
