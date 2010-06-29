#include <FWCore/PluginManager/interface/ModuleDef.h>
#include <FWCore/Framework/interface/MakerMacros.h>

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SortCollectionSelector.h"

#include "EcalAnalysis/ConversionStudies/plugins/conversionFilter.h"
#include "EcalAnalysis/ConversionStudies/plugins/conversionTree.h"



DEFINE_FWK_MODULE(conversionFilter);
DEFINE_FWK_MODULE(conversionTree);
