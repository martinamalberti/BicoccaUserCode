// $Id: SealModule.cc,v 1.2 2007/11/14 17:34:43 govoni Exp $

#include "PluginManager/ModuleDef.h"#include "FWCore/Framework/interface/InputSourceMacros.h"#include "FWCore/Framework/interface/MakerMacros.h"#include "Configuration/CSA06Skimming/interface/VBFMCProcessFilter.h"DEFINE_SEAL_MODULE () ;DEFINE_ANOTHER_FWK_MODULE (VBFMCProcessFilter) ;