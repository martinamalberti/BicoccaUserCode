// $Id: VBFProcessFilter.cc,v 1.7 2007/12/07 16:17:27 govoni Exp $

#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFUtils.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include <iostream>


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
      if (firstJet->et () < jetPtMin || 
          fabs (firstJet->eta ()) > jetEtaMax) continue ;
      math::XYZTLorentzVector firstLV = firstJet->p4 () ;
      //PG second loop over jets
      for (VBFjetIt secondJet = firstJet + 1 ; 
           secondJet != end ; 
           ++secondJet ) 
        {
          if (secondJet->et () < jetPtMin || 
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
