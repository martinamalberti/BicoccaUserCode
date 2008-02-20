#ifndef VBFUtils_h#define VBFUtils_h// -*- C++ -*-//// Package:    VBFProcessFilter// Class:      VBFProcessFilter// /*  Description: filter events based on the Pythia ProcessID and the Pt_hat
Implementation: inherits from generic EDFilter

*/
//
// $Id: VBFUtils.h,v 1.5 2008/02/20 10:38:17 tancini Exp $
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
findMaxPtJetsPair (reco::CaloJetCollection& coll,
		   double jetPtMin, double jetEtaMax) ;

// --------------------------------------------------------------------------------

double deltaPhi (double phi1, double phi2) ;

// --------------------------------------------------------------------------------

template<typename T>
struct ptSorting
{
  typedef T first_argument_type;
  typedef T second_argument_type;
  bool operator()( const T &t1, const T &t2 ) const {
  return t1.pt()> t2.pt();
}
};

#endif


