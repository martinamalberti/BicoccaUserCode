#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
//#include <boost/foreach.hpp>


int main (int argc, char** argv)
{

  std::string fileName (argv[1]) ;
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile (fileName) ;
  boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet () ;
  std::cout << parameterSet->dump () << std::endl ; //PG for testing
  
  edm::ParameterSet subPSetCalib =  
    parameterSet->getParameter<edm::ParameterSet> ("clusterCalib") ;
  std::cout << "etaMin : " << subPSetCalib.getParameter<int> ("etaMin") << std::endl ;
  std::cout << "etaMax : " << subPSetCalib.getParameter<int> ("etaMax") << std::endl ;

  edm::ParameterSet subPSetInput =  
    parameterSet->getParameter<edm::ParameterSet> ("inputNtuples") ;
  std::vector<std::string> inputFiles = 
   subPSetInput.getParameter<std::vector<std::string> > ("inputFiles") ;

  std::cout << "reading : " ;
  for (std::vector<std::string>::const_iterator listIt = inputFiles.begin () ;
       listIt != inputFiles.end () ;
       ++listIt)
    {
      std::cout << *listIt << " " ;
    }
  std::cout << "\n" ;
  return 0 ;
}


