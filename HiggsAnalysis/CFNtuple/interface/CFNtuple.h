/*
NAMETEMPLATE
*/
#ifndef NAMETEMPLATE_h
#define NAMETEMPLATE_h

#include <vector>
#include <cmath>

#include "treeReader.h"
#include "TString.h"
#include "TGraphErrors.h"
#include "TH1F.h"

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#endif

#include "Math/GenVector/VectorUtil.h"

TGraphErrors* XBinCenterCorrectedHisto(const TH1F& thLargeBin, const TH1F& thSlimBin);
 
#endif


