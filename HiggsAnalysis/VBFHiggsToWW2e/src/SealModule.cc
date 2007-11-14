//PG non sono sicuro che siano gli include giusti
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"


#include "HiggsAnalysis/VBFHiggsToWW2e/interface/VBFHiggsToWW2e.h"

//define this as a plug-in
DEFINE_SEAL_MODULE () ;
DEFINE_ANOTHER_FWK_MODULE (VBFHiggsToWW2e) ;
