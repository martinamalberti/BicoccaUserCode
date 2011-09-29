#include "histoFunc.h"

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


void printEvent(const std::string& inFile)
{
  TFile* f = TFile::Open(inFile.c_str(),"READ");
  TTree* tree = (TTree*)(f->Get("ntu"));
  
  std::cout << "DATA: " << std::setw(8) << tree->GetEntries() << " entries in " << inFile << std::endl;
  
  
  
  // Set branch addresses
  int runId,lumiId,eventId;
  int isW,timeStampHigh;
  int ele1_isEB;
  float ele1_seedLaserCorr,ele1_seedLaserAlpha,ele1_seedE;
  float ele1_scE,ele1_scERaw;
  tree->SetBranchAddress("runId", &runId);
  tree->SetBranchAddress("lumiId", &lumiId);
  tree->SetBranchAddress("eventId", &eventId);
  tree->SetBranchAddress("isW", &isW);
  tree->SetBranchAddress("timeStampHigh", &timeStampHigh);
  tree->SetBranchAddress("ele1_isEB", &ele1_isEB);
  tree->SetBranchAddress("ele1_seedLaserCorr", &ele1_seedLaserCorr);
  tree->SetBranchAddress("ele1_seedLaserAlpha", &ele1_seedLaserAlpha);
  tree->SetBranchAddress("ele1_seedE", &ele1_seedE);
  tree->SetBranchAddress("ele1_scE", &ele1_scE);
  tree->SetBranchAddress("ele1_scERaw", &ele1_scERaw);
  
  
  
  
  
  
  // Loop on the events
  std::cout << std::endl;
  std::cout << "***** Loop on the events *****" << std::endl;
  
  int nEntries = tree -> GetEntries(); 
  for(int iEntry = 0; iEntry < nEntries; ++iEntry)
  {
    if( (iEntry%100000 == 0) ) std::cout << "reading saved entry " << iEntry << std::endl;
    
    tree -> GetEntry(iEntry);
    
    if( runId > 163375 ) continue;
    if( isW != 1 ) continue;
    if( ele1_isEB == 1 ) continue;
    if(ele1_seedLaserCorr < 1.1 ) continue;
        
    //if( runId   != 163255 )    continue;
    //if( lumiId  != 839 )       continue;
    //if( eventId != 531928437 ) continue;

    //if( runId   != 163270 )    continue;
    //if( lumiId  != 513 )       continue;
    //if( eventId != 311377955 ) continue;

    if( runId   != 163374 )    continue;
    if( lumiId  != 776 )       continue;
    if( eventId != 401839700 ) continue;    
    
    
    
    std::cout << "*** Run: " << runId << " *** LS: " << lumiId << " *** Event: " << eventId << " *** TimeStamp: " << timeStampHigh << " ***" << std::endl;
    if( ele1_isEB == 1 )
    {
      std::cout << ">>> electron in EB" << std::endl;
      std::cout << ">>>>>> seed:   E = " << ele1_seedE << "   alpha = " << ele1_seedLaserAlpha << "   laserCorr = " << ele1_seedLaserCorr << std::endl;
      std::cout << ">>>>>>   sc:   E = " << ele1_scE << "   ERaw = " << ele1_scERaw << std::endl;
    }
    else
    {
      std::cout << ">>> electron in EE" << std::endl;
      std::cout << ">>>>>> seed:   E = " << ele1_seedE << "   alpha = " << ele1_seedLaserAlpha << "   laserCorr = " << ele1_seedLaserCorr << std::endl;
      std::cout << ">>>>>>   sc:   E = " << ele1_scE << "   ERaw = " << ele1_scERaw << std::endl;
    }

  }
}
