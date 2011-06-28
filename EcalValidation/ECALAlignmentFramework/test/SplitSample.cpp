#include "ConfigParser.h"


#include "ECALAlignmentFramework.h"


#include "TPie.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TMinuit.h"
#include "THStack.h"

#include "../test/TDRStyle.cc"
#include "../test/Read.cc"

#include "TEntryList.h"




#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#endif


/// 
/// 
///    ___|          | _)  |         ___|                            |       
///  \___ \   __ \   |  |  __|     \___ \    _` |  __ `__ \   __ \   |   _ \ 
///        |  |   |  |  |  |             |  (   |  |   |   |  |   |  |   __/ 
///  _____/   .__/  _| _| \__|     _____/  \__,_| _|  _|  _|  .__/  _| \___| 
///          _|                                              _|              
/// 


// std::vector<double> PUWeight;

int GetNumList(std::vector<int> &list){
 int result = 0;
 for (int it = 0; it<list.size(); it++) result += list.at(it);
 return result;
}


int main(int argc, char** argv)
{ 
 TDRStyle();
 
 gStyle->SetPadTopMargin(0.2);
 gStyle->SetPadBottomMargin(0.2);
 gStyle->SetPadLeftMargin(0.07);
 gStyle->SetPadRightMargin(0.23);
 gStyle->cd(); 
 
 
 std::cout << " " << std::endl;
 std::cout << " " << std::endl;
 std::cout << " " << std::endl; 
 std::cout << "     ___|          | _)  |         ___|                            |                   " << std::endl;
 std::cout << "   \\___ \\   __ \\   |  |  __|     \\___ \\    _` |  __ `__ \\   __ \\   |   _ \\     " << std::endl; 
 std::cout << "         |  |   |  |  |  |             |  (   |  |   |   |  |   |  |   __/             " << std::endl;
 std::cout << "   _____/   .__/  _| _| \\__|     _____/  \\__,_| _|  _|  _|  .__/  _| \\___|          " << std::endl;
 std::cout << "           _|                                              _|                          " << std::endl;
 std::cout << " " << std::endl;
 std::cout << " " << std::endl;
 
 
 //Check if all nedeed arguments to parse are there                                                                                                                               
 if(argc != 2)
 {
  std::cerr << ">>>>> analysis.cpp::usage: " << argv[0] << " configFileName" << std::endl ;
  return 1;
 }


 // Parse the config file                                                                                                                                                          
 parseConfigFile (argv[1]) ;
 
 std::string treeName  = gConfigParser -> readStringOption("Input::treeName");
 std::string inputFileName = gConfigParser -> readStringOption("Input::inputFileName");

 int numEventsPerSample = gConfigParser -> readIntOption("Options::numEventsPerSample");

 TTree *treeJetLepVect; 
 
 std::string CutFile = gConfigParser -> readStringOption("Selections::CutFile"); 
 std::vector<std::string> vCut; ///====> only the first cut is used!
 std::cout << " nCuts   = " << ReadFileCut(CutFile, vCut) << std::endl;
 
 ///==== output file ====
 std::string outputDirectory = gConfigParser -> readStringOption("Output::outputDirectory");
 std::string OutFileName    = gConfigParser -> readStringOption("Output::outFileName");
 
 ///==== debug flag (begin) ==== 
 bool  debug = false; 
 try {
  debug = gConfigParser -> readBoolOption("Input::debug");
 }
 catch (char const* exceptionString){
  std::cerr << " exception = " << exceptionString << std::endl;
 }
 std::cout << ">>>>> input::debug  " << debug  << std::endl;  
 ///==== debug flag (end) ==== 
 
 
 
 
 ///==== program ====
 
 TFile* f = new TFile(inputFileName.c_str(), "READ");    
 treeJetLepVect = (TTree*) f->Get(treeName.c_str());
  
 int totNumEvents = treeJetLepVect->GetEntries(vCut.at(0).c_str());
 int numOutputFiles = totNumEvents / numEventsPerSample;
 std::cout << " numOutputFiles = " << numOutputFiles << std::endl;
 
 treeJetLepVect->SetEntryList(0); 
 treeJetLepVect->Draw(">> myList",vCut.at(0).c_str(),"entrylist");
 TEntryList *myList = (TEntryList*)gDirectory->Get("myList");
 treeJetLepVect->SetEntryList(myList); 
 
 std::cout << " tot = " << myList->GetN() << " = " << totNumEvents << " =? " << treeJetLepVect -> GetEntries () << std::endl;
 
 TH1F* MyHistoCounterMC = (TH1F*) f->Get("AllEvents/totalEvents");
 
 int iEntry = 0;
 for (int iOutFile = 0; iOutFile < numOutputFiles; iOutFile++) {
  std::cout << " iOutFile = " << iOutFile << " : " << numOutputFiles << std::endl;
  TString outputRootFileName = Form("%s/%s_%d.root",outputDirectory.c_str(),OutFileName.c_str(), iOutFile); 
  TFile outputRootFile ( outputRootFileName.Data(), "RECREATE") ;
  outputRootFile.cd () ;
  outputRootFile.mkdir ("ntupleEcalAlignment") ;
  outputRootFile.cd ("ntupleEcalAlignment") ;  
  
  TTree* cloneTree = treeJetLepVect -> CloneTree (0) ;
  
  int countiEntry = 0;
    
  for ( ; iEntry < myList -> GetN () ; iEntry++) { 
//   for ( ; iEntry < treeJetLepVect -> GetEntries () ; iEntry++) {
   countiEntry++;
   treeJetLepVect -> GetEntry (myList->Next());
   cloneTree -> Fill () ;
   if (countiEntry > numEventsPerSample) break;
  }
  cloneTree -> AutoSave () ;

  outputRootFile.cd () ;
  outputRootFile.mkdir ("AllEvents") ;
  outputRootFile.cd ("AllEvents") ;  
  MyHistoCounterMC -> Write();
   
  outputRootFile.Close () ;
 
 }
 
}





