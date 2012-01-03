#include "ConfigParser.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TMinuit.h"
#include "Math/GenVector/VectorUtil.h"
#include "TRandom3.h"
#include <time.h>
#include <sstream>
#include "ECALAlignmentFramework.h"

#include "../test/Read.cc"
#include "PUclass.h"


///
///   
///        \         |      |     \ \        /     _)         |      |   
///       _ \     _` |   _` |      \ \  \   /  _ \  |   _` |  __ \   __| 
///      ___ \   (   |  (   |       \ \  \ /   __/  |  (   |  | | |  |   
///    _/    _\ \__,_| \__,_|        \_/\_/  \___| _| \__, | _| |_| \__| 
///                                                   |___/              
///   


int main(int argc, char** argv){ 
 //Check if all nedeed arguments to parse are there                                                                                                                               
 if(argc != 2)
 {
  std::cerr << ">>>>> analysis.cpp::usage: " << argv[0] << " configFileName" << std::endl ;
  return 1;
 }
 
 
 std::cout << "  " << std::endl;
 std::cout << "  " << std::endl;
 std::cout << "         \\         |      |     \\ \\        /     _)         |      |    " << std::endl;
 std::cout << "        _ \\     _` |   _` |      \\ \\  \\   /  _ \\  |   _` |  __ \\   __|  " << std::endl;
 std::cout << "       ___ \\   (   |  (   |       \\ \\  \\ /   __/  |  (   |  | | |  |    " << std::endl;
 std::cout << "     _/    _\\ \\__,_| \\__,_|        \\_/\\_/  \\___| _| \\__, | _| |_| \\__|  " << std::endl;
 std::cout << "                                                    |___/               " << std::endl;
 std::cout << " " << std::endl;
 std::cout << " " << std::endl;
 std::cout << " " << std::endl;
 
 
 
 // Parse the config file                                                                                                                                                          
 parseConfigFile (argv[1]) ;
 
 std::string treeName     = gConfigParser -> readStringOption("Input::treeName");
 std::string treeNameDir  = "";
 try {
  treeNameDir = gConfigParser -> readStringOption("Input::treeNameDir");
 }
 catch (char const* exceptionString){
  std::cerr << " exception = " << exceptionString << std::endl;
 }
 std::cout << ">>>>> input::treeNameDir  " << treeNameDir  << std::endl;  
 
 
 std::string fileSamples = gConfigParser -> readStringOption("Input::fileSamples");
 std::string inputDirectory = gConfigParser -> readStringOption("Input::inputDirectory");
 
 TTree *treeJetLepVect[1000];
 
 char nameFileIn[1000];
 char *nameSample[1000];
 int PUScenarioIndex[1000];
 sprintf(nameFileIn,"%s",fileSamples.c_str());
 int numberOfSamples = ReadFileWeight(nameFileIn, nameSample, PUScenarioIndex); 
 
 std::cout << std::endl;
 std::cout << " numberOfSamples = " << numberOfSamples << std::endl;
 std::cout << std::endl;
 
//  std::cout << " PUScenarioIndex[0] = " << PUScenarioIndex[0]  << std::endl;
   
 ///==== PU reweight (begin) ====
 int numPUScenarios   = gConfigParser -> readIntOption("PU::PUScenarios");

 std::vector<std::string> PUVAR;
 std::vector<double> PUMC[100];
 std::vector<double> PUDATA[100];
 PUclass PU[100];
 
 for (int iScPU = 0; iScPU < numPUScenarios; iScPU++) {
  TString namePU;
  namePU = Form("PU::PUMC%d",iScPU);
  PUMC[iScPU]   = gConfigParser -> readDoubleListOption(namePU.Data());
  namePU = Form("PU::PUDATA%d",iScPU);
  PUDATA[iScPU] = gConfigParser -> readDoubleListOption(namePU.Data());
  namePU = Form("PU::PUVAR%d",iScPU);
  std::string tempName = gConfigParser -> readStringOption(namePU.Data());
  PUVAR.push_back(tempName);
  
  if (PUMC[iScPU].size() != PUDATA[iScPU].size()) {
   std::cout << " PUVAR["  << iScPU << "]        = " << PUVAR.at(iScPU) << std::endl;
   std::cout << " PUMC["   << iScPU << "].size() = " << PUMC[iScPU].size()   << std::endl;
   std::cout << " PUDATA[" << iScPU << "].size() = " << PUDATA[iScPU].size() << std::endl;
   std::cerr << " ERROR " << std::endl;
   return 1;
  }
  
  double sumPUMC = 0;
  for (int itVPU = 0; itVPU < PUMC[iScPU].size(); itVPU++ ){
   sumPUMC += PUMC[iScPU].at(itVPU);  
  }
  double sumPUDATA = 0;
  for (int itVPU = 0; itVPU < PUDATA[iScPU].size(); itVPU++ ){
   sumPUDATA += PUDATA[iScPU].at(itVPU);  
  } 
  
  for (int itVPU = 0; itVPU < PUMC[iScPU].size(); itVPU++ ){
   PU[iScPU].PUWeight.push_back(PUDATA[iScPU].at(itVPU) / PUMC[iScPU].at(itVPU) * sumPUMC / sumPUDATA);
  }
 }
 
 ///==== PU reweight (end) ====
 
 
 ///==== debug flag ==== 
 bool  debug = false; 
 try {
  debug = gConfigParser -> readBoolOption("Input::debug");
 }
 catch (char const* exceptionString){
  std::cerr << " exception = " << exceptionString << std::endl;
 }
 std::cout << ">>>>> input::debug  " << debug  << std::endl;  
 
 
 for (int iSample=0; iSample<numberOfSamples; iSample++){
  std::cout << " nameSample[" << iSample << ":" << numberOfSamples << "] = " << nameSample[iSample] << std::endl;
  
  ///==== GetTree (begin) ==== 
  char nameFile[20000];
  sprintf(nameFile,"%s/out_%s.root",inputDirectory.c_str(),nameSample[iSample]);  
  std::cout << " nameFile = " << nameFile << std::endl;
  TFile* f = new TFile(nameFile, "update");
  f->cd(treeNameDir.c_str());
  
  treeJetLepVect[iSample] = (TTree*) f->Get(treeName.c_str());
//   char nameTreeJetLep[100];
//   sprintf(nameTreeJetLep,"treeJetLep_%d",iSample); 
//   treeJetLepVect[iSample]->SetName(nameTreeJetLep);
  ///==== GetTree (end) ====
  
  std::cout << " >>>>> GetEntries " << treeJetLepVect[iSample]->GetEntries() << std::endl;   
  
  double weight;
//   TBranch *newBranch;
  ///==== add new branch ====
  TBranch *newBranch = treeJetLepVect[iSample] -> Branch("weight_PU",&weight,"weight_PU/D");
  int numPUMC;
  treeJetLepVect[iSample] -> SetBranchAddress(PUVAR.at(PUScenarioIndex[iSample]).c_str(),&numPUMC);
  Long64_t nentries = treeJetLepVect[iSample]->GetEntries();
  for (Long64_t iEntry = 0; iEntry < nentries; iEntry++){
   if((iEntry%((nentries+10)/10)) == 0) std::cout << ">>>>> analysis::GetEntry " << iEntry << " : " << nentries << std::endl;   
   treeJetLepVect[iSample]->GetEntry(iEntry);
   weight = PU[ PUScenarioIndex[iSample] ].getPUWeight(numPUMC);
   newBranch->Fill();
  }
  // save only the new version of the tree
  treeJetLepVect[iSample]->Write("", TObject::kOverwrite);
 }
 
 
 std::cout << " *** end *** " << std::endl;
}

