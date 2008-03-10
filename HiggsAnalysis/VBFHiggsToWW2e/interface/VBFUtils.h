#ifndef VBFUtils_h#define VBFUtils_h// -*- C++ -*-//// Package:    VBFProcessFilter// Class:      VBFProcessFilter// /*  Description: filter events based on the Pythia ProcessID and the Pt_hat
Implementation: inherits from generic EDFilter

*/
//
// $Id: VBFUtils.h,v 1.9 2008/03/10 09:18:17 govoni Exp $
//
//
// system include files
#include <memory>

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/Common/interface/View.h"
#include "TLorentzVector.h"

//PG FIXME to be inserted in a vbfhww2l namespace to avoid clashes

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
findMaxPtJetsPair (VBFjetIt begin, VBFjetIt end,
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

// --------------------------------------------------------------------------------


//! find a RefToBase in a edm::View
template <typename T>
typename edm::View<T>::const_iterator
findInView (typename edm::Handle<edm::View<T> > collection,
            typename edm::RefToBase<T> element) 
  {
    for (typename edm::View<T>::const_iterator it = collection->begin () ; 
         it != collection->end () ;
         ++it)
      {
        if (collection->refAt (it - collection->begin ()) == element) return it ;
      }   
    return collection->end () ;             
  }


#endif


