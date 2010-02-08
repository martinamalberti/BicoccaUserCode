#ifndef ntpleUtils_h
#define ntpleUtils_h

#include <iostream>
#include <fstream>
#include <string>
#include "TChain.h"

#include "ConfigParser.h"






/** fill a chain from a list of files */
bool FillChain(TChain& chain, const std::string& inputFileList);


/** get the parameters from a congiguration file */
int parseConfigFile (const TString& config) ;

#endif