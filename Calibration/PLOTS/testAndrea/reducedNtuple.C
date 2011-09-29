#include "TFile.h"
#include "TTree.h"

#include <iostream>
#include <iomanip>
#include <string>



void reducedNtuple(const std::string& inFile, const std::string& outFile)
{
  TFile* f = TFile::Open(inFile.c_str(),"READ");
  TTree* tree = (TTree*)(f->Get("ntu"));
  
  TFile* o = new TFile(outFile.c_str(),"RECREATE");
  TTree* newTree = new TTree("ntuReduced","ntu");
  
  std::cout << "DATA: " << std::setw(8) << tree->GetEntries() << " entries in " << inFile << std::endl;
  
  
  
  // Set branch addresses
  int runId, lumiId, eventId;
  int timeStampHigh;
  int isW;
  float seedLaserCorr;
  float scEta;
  tree -> SetBranchAddress("runId",             &runId);
  tree -> SetBranchAddress("lumiId",            &lumiId);
  tree -> SetBranchAddress("eventId",           &eventId);
  tree -> SetBranchAddress("timeStampHigh",     &timeStampHigh);
  tree -> SetBranchAddress("isW",               &isW);
  tree -> SetBranchAddress("ele1_seedLaserCorr",&seedLaserCorr);
  tree -> SetBranchAddress("ele1_scEta",        &scEta);  
  
  newTree -> Branch("runId",        &runId,                "runId/I");
  newTree -> Branch("lumiId",       &lumiId,              "lumiId/I");
  newTree -> Branch("eventId",      &eventId,            "eventId/I");
  newTree -> Branch("timeStampHigh",&timeStampHigh,"timeStampHigh/I");
  newTree -> Branch("isW",          &isW,                    "isW/I");
  newTree -> Branch("seedLaserCorr",&seedLaserCorr,"seedLaserCorr/F");
  newTree -> Branch("scEta",        &scEta,                "scEta/F");
  
  
  // Loop and sort events
  std::cout << std::endl;
  std::cout << "***** Sort events *****" << std::endl;
  
  int nEntries = tree -> GetEntries(); 
  int nSavePts = 0;
  for(int iEntry = 0; iEntry < nEntries; ++iEntry)
  {
    if( (iEntry%100000 == 0) ) std::cout << "reading saved entry " << iEntry << std::endl;
    
    tree -> GetEntry(iEntry);
  
    if( isW != 1 ) continue;
    
    newTree -> Fill();
    nSavePts++; 
  }
  std::cout << "DATA: " << std::setw(8) << nSavePts << " entries in " << inFile << std::endl;
  
  
  
  // Reduced tree
  std::cout << std::endl;
  std::cout << "***** Reduced tree *****" << std::endl;
    
  newTree -> Write();
  o -> Close();
}

