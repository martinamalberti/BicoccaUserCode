#include "histoFunc.h"
#include "ntupleUtils.h"
#include "ntupleUtils.cc"

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

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>
#include <map>
#include <algorithm>
#include <math.h>
#include <vector>

TH1F* templateHisto;
TF1* templateFunc;
std::vector<double>* mydata;



void timeSort(const std::string& inFile, const std::string& outFile)
{
  TFile* f = TFile::Open(inFile.c_str(),"READ");
  TTree* tree = (TTree*)(f->Get("ntu"));
  
  TFile* o = new TFile(outFile.c_str(),"RECREATE");
  TTree* clone = tree -> CloneTree(0);
  
  std::cout << "DATA: " << std::setw(8) << tree->GetEntries() << " entries in " << inFile << std::endl;
  
  
  
  // Set branch addresses
  int timeStampHigh;
  tree -> SetBranchAddress("timeStampHigh", &timeStampHigh);
  
  
  
  // Loop and sort events
  std::cout << std::endl;
  std::cout << "***** Sort events *****" << std::endl;
  
  int nEntries = tree -> GetEntries(); 
  std::vector<Sorter> sortedEntries;
  
  int nSavePts = 0;
  for(int iEntry = 0; iEntry < nEntries; ++iEntry)
  {
    if( (iEntry%100000 == 0) ) std::cout << "reading saved entry " << iEntry << std::endl;
    
    tree -> GetEntry(iEntry);
  
    Sorter dummy;
    dummy.time = timeStampHigh;
    dummy.entry = iEntry;
    sortedEntries.push_back(dummy);
    
    nSavePts++; 
  }
  std::sort(sortedEntries.begin(),sortedEntries.end(),Sorter());
  std::cout << "DATA: " << std::setw(8) << nSavePts << " entries in " << inFile << std::endl;
  
  
  
  // Clone tree
  std::cout << std::endl;
  std::cout << "***** Clone tree *****" << std::endl;
    
  for(int iSaved = 0; iSaved < nSavePts; ++iSaved)
  {
    if( (iSaved%10000 == 0) ) std::cout << "reading saved entry " << iSaved << std::endl;
    
    tree -> GetEntry(sortedEntries[iSaved].entry);
    
    clone -> Fill();
  }
  
  clone -> AutoSave();
  o -> Close();
}

