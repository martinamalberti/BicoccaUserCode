
// compile: g++ -Wall -o TreeSort `root-config --glibs --cflags` TreeSort.cpp
// To run : ./TreeSort <inputFileList> <OutputFile> <Variable to sort>

#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
#include "TPaveStats.h"
#include "TLegend.h"
#include "TTree.h"
#include "TVirtualFitter.h"
#include "TChain.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>
#include <map>
#include <algorithm>
#include <math.h>
#include <vector>
#include <fstream>

struct Sorter{
  int sortVar;
  int entry;

  bool operator() (const Sorter& s1, const Sorter& s2)
  {
    return s1.sortVar < s2.sortVar;
  }
};

bool FillChain(TChain& chain, const std::string& inputFileList);



int main ( int argc, char** argv){
  
  if(argc!=5) { std::cerr<<" Error: input on command line not correct "<<std::endl; return 1; }

  std::string InputList        = argv[1];
  std::string OutputPath       = argv[2]; 
  std::string OutputFile       = argv[3];
  std::string SortVarName      = argv[4];

  TChain* chain = new TChain("simpleNtupleEoverP/SimpleNtupleEoverP"); // Input list of File
  FillChain(*chain,InputList);

  std::string Name = OutputPath + OutputFile;
  TFile* out = new TFile(Name.c_str(),"RECREATE"); // Output File definition

  // Set branch addresses                                                                                                                                    
  int SortVariable;
  chain -> SetBranchAddress(SortVarName.c_str(), &SortVariable);

  int nEntries = chain -> GetEntries();

  TChain *cloneChain = (TChain*)chain->CloneTree(0); // Clone the chain

  TTree* clone = cloneChain -> GetTree(); // Get a Tree

  std::cout << "DATA: " <<  nEntries  << " entries in " << InputList << std::endl;
  
    
  // Loop and sort events
  std::cout << std::endl;
  std::cout << "***** Sort events *****" << std::endl;
  
  std::vector<Sorter> sortedEntries; // in order to sort the entries
  
  int nSavePts = 0;

  for(int iEntry = 0; iEntry < nEntries; ++iEntry){

    if( (iEntry%100000 == 0) ) std::cout << "reading saved entry " << iEntry << std::endl;
    
    chain -> GetEntry(iEntry);
    //    std::cout<<SortVariable<<std::endl;  
    Sorter dummy;
    dummy.sortVar = SortVariable;
    dummy.entry   = iEntry;
    sortedEntries.push_back(dummy);
    
    nSavePts++; 
  }

  std::sort(sortedEntries.begin(),sortedEntries.end(),Sorter());
  std::cout << "DATA: " << std::setw(8) << nSavePts << " entries in " << InputList << std::endl;
  
    
  // Clone tree
  std::cout << std::endl;
  std::cout << "***** Clone tree *****" << std::endl;
  
  for(int iSaved = 0; iSaved < nSavePts; ++iSaved){

   if( (iSaved%10000 == 0) ) std::cout << "reading saved entry " << iSaved << std::endl;
    
      chain -> GetEntry(sortedEntries[iSaved].entry);
    
      clone -> Fill();
  }
  
   out -> Close();

  return 0; 
  
}


bool FillChain(TChain& chain, const std::string& inputFileList)
{
  std::ifstream inFile(inputFileList.c_str());
  std::string buffer;

  if(!inFile.is_open())
    {
      std::cerr << "** ERROR: Can't open '" << inputFileList << "' for input" << std::endl;
      return false;
    }

  while(1)
    {
      inFile >> buffer;
      if(!inFile.good()) break;
      if( buffer.at(0) == '#' ) continue;
      chain.Add(buffer.c_str());
      std::cout << ">>> ntupleUtils::FillChain - treeName = " << chain.GetName() << " from file " << buffer << std::endl;
    }

  return true;
}
