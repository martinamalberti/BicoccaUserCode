// $Id: SealModule.cc,v 1.3 2007/11/14 18:25:17 govoni Exp $

#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "HiggsAnalysis/VBFHiggsToWW2e/plugins/VBFMCProcessFilter.h"

DEFINE_SEAL_MODULE () ;
DEFINE_ANOTHER_FWK_MODULE (VBFMCProcessFilter) ;
