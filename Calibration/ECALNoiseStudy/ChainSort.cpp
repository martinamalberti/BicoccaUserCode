
// compile: g++ -Wall -o ChainSort `root-config --glibs --cflags` Sorter.h ChainSort.cpp
// To run : ./TreeSort <inputFileList> <ouptutPath> <OutputFile> <Variable to sort> <type of variable>

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
#include "Sorter.h"

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


bool FillChain(TChain& chain, const std::string& inputFileList);


int main ( int argc, char** argv){
  
  if(argc!=6) { std::cerr<<" Error: input on command line not correct "<<std::endl; return 1; }

  std::string InputFileList    = argv[1];
  std::string OutputPath       = argv[2]; 
  std::string OutputFile       = argv[3];
  std::string SortVarName      = argv[4];
  std::string SortVarType      = argv[5];

  TChain* chain = new TChain("simpleNtupleEoverP/SimpleNtupleEoverP");
  FillChain(*chain,InputFileList);

  std::string Name = OutputPath + OutputFile;
  TFile* out = new TFile(Name.c_str(),"RECREATE"); // Output File definition

  int nEntries = chain -> GetEntries();

  TChain *cloneChain = (TChain*) chain->CloneTree(0); // Clone the chain
  TTree  *cloneTree  = cloneChain->GetTree();

  std::cout << "DATA: " <<  nEntries  << " entries in " << InputFileList << std::endl;
 
  std::cout<<" **************************** "<<std::endl;
  std::cout<<" ********* SORT ************* "<<std::endl;

  int nSavePts = 0;

  if(SortVarType=="int"){
 
   int SortVariable; chain->SetBranchAddress(SortVarName.c_str(),&SortVariable);
  
   std::vector< Sorter<int> >  sortedEntries; // in order to sort the entries

  
   for(int iEntry = 0; iEntry < nEntries; ++iEntry){
    if( (iEntry%100000 == 0) ) std::cout << "reading saved entry " << iEntry << std::endl;
    chain -> GetEntry(iEntry);
    Sorter<int> dummy; 
    dummy.sortVar = SortVariable; dummy.entry   = iEntry; sortedEntries.push_back(dummy);
    nSavePts++; 
  }

  std::sort(sortedEntries.begin(),sortedEntries.end(),Sorter<int>());
  std::cout << "DATA: " << std::setw(8) << nSavePts << " entries in " << InputFileList << std::endl;
  
  // Clone tree                                                                                                                                                   
  std::cout << std::endl;
  std::cout<<" **************************** "<<std::endl;
  std::cout << "******* Clone tree ********" << std::endl;

  for(int iSaved = 0; iSaved < nSavePts; ++iSaved){

    if( (iSaved%10000 == 0) ) std::cout << "reading saved entry " << iSaved << std::endl;

    if( (iSaved%100 == 0) ) chain -> GetEntry(sortedEntries[iSaved].entry);
    std::cout<<" Entry "<<iSaved<<" Value "<<SortVariable<<std::endl;
    cloneTree -> Fill();
  }
 }

 if(SortVarType=="float"){

    float SortVariable; chain->SetBranchAddress(SortVarName.c_str(),&SortVariable);

    std::vector< Sorter<float> >  sortedEntries; // in order to sort the entries                                                                                
                                                                                                                                                                  
      for(int iEntry = 0; iEntry < nEntries; ++iEntry){
	if( (iEntry%100000 == 0) ) std::cout << "reading saved entry " << iEntry << std::endl;
	chain -> GetEntry(iEntry);
	Sorter<float> dummy ;    dummy.sortVar = SortVariable; dummy.entry   = iEntry; sortedEntries.push_back(dummy);
	nSavePts++;
      }

      std::sort(sortedEntries.begin(),sortedEntries.end(),Sorter<float>());
      std::cout << "DATA: " << std::setw(8) << nSavePts << " entries in " << InputFileList << std::endl;

      // Clone tree 
                                                                                                                                                               
      std::cout << std::endl;
      std::cout<<" **************************** "<<std::endl;
      std::cout << "******* Clone tree ********" << std::endl;

      for(int iSaved = 0; iSaved < nSavePts; ++iSaved){

	  if( (iSaved%10000 == 0) ) std::cout << "reading saved entry " << iSaved << std::endl;

	  chain -> GetEntry(sortedEntries[iSaved].entry);
	  cloneTree -> Fill();
      }


 }



 if(SortVarType=="double"){

    double SortVariable; chain->SetBranchAddress(SortVarName.c_str(),&SortVariable);

    std::vector< Sorter<double> >  sortedEntries; // in order to sort the entries                                                                                       
    for(int iEntry = 0; iEntry < nEntries; ++iEntry){
      if( (iEntry%100000 == 0) ) std::cout << "reading saved entry " << iEntry << std::endl;
      chain -> GetEntry(iEntry);
      Sorter<double> dummy ;    dummy.sortVar = SortVariable; dummy.entry   = iEntry; sortedEntries.push_back(dummy);
      nSavePts++;
    }

    std::sort(sortedEntries.begin(),sortedEntries.end(),Sorter<double>());
    std::cout << "DATA: " << std::setw(8) << nSavePts << " entries in " << InputFileList << std::endl;


    // Clone tree                                                                                                                                                   
    std::cout << std::endl;
    std::cout<<" **************************** "<<std::endl;
    std::cout << "******* Clone tree ********" << std::endl;

    for(int iSaved = 0; iSaved < nSavePts; ++iSaved){

      if( (iSaved%10000 == 0) ) std::cout << "reading saved entry " << iSaved << std::endl;

      chain -> GetEntry(sortedEntries[iSaved].entry);
      cloneTree -> Fill();
    }


  }

  if(SortVarType=="string"){

    std::string SortVariable; chain->SetBranchAddress(SortVarName.c_str(),&SortVariable);

    std::vector< Sorter<std::string> >  sortedEntries; // in order to sort the entries                                                                                       

    for(int iEntry = 0; iEntry < nEntries; ++iEntry){
      if( (iEntry%100000 == 0) ) std::cout << "reading saved entry " << iEntry << std::endl;
      chain -> GetEntry(iEntry);
      Sorter<std::string> dummy ;    dummy.sortVar = SortVariable; dummy.entry   = iEntry; sortedEntries.push_back(dummy);
      nSavePts++;
    }

    std::sort(sortedEntries.begin(),sortedEntries.end(),Sorter<std::string>());
    std::cout << "DATA: " << std::setw(8) << nSavePts << " entries in " << InputFileList << std::endl;


    // Clone tree                                                                                                                                                   
    std::cout << std::endl;
    std::cout<<" **************************** "<<std::endl;
    std::cout << "******* Clone tree ********" << std::endl;

    for(int iSaved = 0; iSaved < nSavePts; ++iSaved){

      if( (iSaved%10000 == 0) ) std::cout << "reading saved entry " << iSaved << std::endl;

      chain -> GetEntry(sortedEntries[iSaved].entry);
      cloneTree -> Fill();
    }

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
