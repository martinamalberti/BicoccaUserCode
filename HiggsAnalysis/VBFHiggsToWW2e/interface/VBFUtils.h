#ifndef VBFUtils_h#define VBFUtils_h// -*- C++ -*-//// Package:    VBFProcessFilter// Class:      VBFProcessFilter// /*  Description: filter events based on the Pythia ProcessID and the Pt_hat
Implementation: inherits from generic EDFilter

*/
//
// $Id: VBFUtils.h,v 1.4 2008/02/15 16:58:16 tancini Exp $
//
//
// system include files
#include <memory>

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "TLorentzVector.h"

typedef reco::CaloJetCollection::const_iterator VBFjetIt ;

void 
setMomentum (TLorentzVector & myvector, 
             const reco::Candidate & gen) ;

// --------------------------------------------------------------------------------

std::pair<VBFjetIt,VBFjetIt>	
findTagJets (VBFjetIt begin, VBFjetIt end,
             double jetPtMin, double jetEtaMax) ;

// --------------------------------------------------------------------------------

std::pair<VBFjetIt,VBFjetIt>
findMaxPtJetsPair (reco::CaloJetCollection coll,
		   double jetPtMin, double jetEtaMax) ;

// --------------------------------------------------------------------------------

double deltaPhi (double phi1, double phi2) ;

// --------------------------------------------------------------------------------
/*
template <class ParticleType>
struct ptSorting : public 
std::binary_function <ParticleType, ParticleType, bool> 
{
  bool operator() (ParticleType & lept1, ParticleType & lept2) const
  { 
    return lept1.pt () < lept2.pt () ;
  }
};

// --------------------------------------------------------------------------------

template <typename ParticleType>
static void sortByPt (std::vector<ParticleType>& particles)
{
  ptSorting<ParticleType> comp ;
  sort (particles.begin (), particles.end (), comp) ;
}

*/

struct ptSorting
{
  bool operator()( const reco::CaloJet& t1, const reco::CaloJet& t2 ) const {
  return t1.pt()> t2.pt();
}
};

#endif


