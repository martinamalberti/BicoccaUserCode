#ifndef VBFUtils_h#define VBFUtils_h// -*- C++ -*-//// Package:    VBFProcessFilter// Class:      VBFProcessFilter// /*  Description: filter events based on the Pythia ProcessID and the Pt_hat
Implementation: inherits from generic EDFilter

*/
//
// $Id: VBFProcessFilter.h,v 1.4 2007/12/07 16:17:27 govoni Exp $
//
//
// system include files
#include <memory>

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"

typedef reco::CaloJetCollection::const_iterator VBFjetIt ;

std::pair<VBFjetIt,VBFjetIt>	
findTagJets (VBFjetIt begin, VBFjetIt end,
             double jetPtMin, double jetEtaMax) ;


#endif


