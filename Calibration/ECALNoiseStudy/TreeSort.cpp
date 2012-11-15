
// compile: g++ -Wall -o TreeSort `root-config --glibs --cflags` Sorter.h TreeSort.cpp
// To run : ./TreeSort <inputFile> <ouptutPath> <OutputFile> <Variable to sort> <type of variable>

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




int main ( int argc, char** argv){
  
  if(argc!=6) { std::cerr<<" Error: input on command line not correct "<<std::endl; return 1; }

  std::string InputFile        = argv[1];
  std::string OutputPath       = argv[2]; 
  std::string OutputFile       = argv[3];
  std::string SortVarName      = argv[4];
  std::string SortVarType      = argv[5];

  TFile *Input = new TFile(InputFile.c_str(),"READ");
  TTree* tree= (TTree*) Input->Get("simpleNtupleEoverP/SimpleNtupleEoverP");

  std::string Name = OutputPath + OutputFile;
  TFile* out = new TFile(Name.c_str(),"RECREATE"); // Output File definition

  int nEntries = tree -> GetEntries();

  TTree *cloneTree = (TTree*) tree->CloneTree(0); // Clone the chain

  std::cout << "DATA: " <<  nEntries  << " entries in " << InputFile << std::endl;
 
  std::cout<<" **************************** "<<std::endl;
  std::cout<<" ********* SORT ************* "<<std::endl;

  int nSavePts = 0;

  if(SortVarType=="int"){
 
   int SortVariable; tree->SetBranchAddress(SortVarName.c_str(),&SortVariable);
  
   std::vector< Sorter<int> >  sortedEntries; // in order to sort the entries

  
   for(int iEntry = 0; iEntry < nEntries; ++iEntry){
    if( (iEntry%100000 == 0) ) std::cout << "reading saved entry " << iEntry << std::endl;
    tree -> GetEntry(iEntry);
    Sorter<int> dummy; 
    dummy.sortVar = SortVariable; dummy.entry   = iEntry; sortedEntries.push_back(dummy);
    nSavePts++; 
  }

  std::sort(sortedEntries.begin(),sortedEntries.end(),Sorter<int>());
  std::cout << "DATA: " << std::setw(8) << nSavePts << " entries in " << InputFile << std::endl;
  
  // Clone tree                                                                                                                                                   
  std::cout << std::endl;
  std::cout<<" **************************** "<<std::endl;
  std::cout << "******* Clone tree ********" << std::endl;

  for(int iSaved = 0; iSaved < nSavePts; ++iSaved){

    if( (iSaved%10000 == 0) ) std::cout << "reading saved entry " << iSaved << std::endl;

    tree -> GetEntry(sortedEntries[iSaved].entry);
    cloneTree -> Fill();
  }
 }

 if(SortVarType=="float"){

    float SortVariable; tree->SetBranchAddress(SortVarName.c_str(),&SortVariable);

    std::vector< Sorter<float> >  sortedEntries; // in order to sort the entries                                                                                
                                                                                                                                                                  
      for(int iEntry = 0; iEntry < nEntries; ++iEntry){
	if( (iEntry%100000 == 0) ) std::cout << "reading saved entry " << iEntry << std::endl;
	tree -> GetEntry(iEntry);
	Sorter<float> dummy ;    dummy.sortVar = SortVariable; dummy.entry   = iEntry; sortedEntries.push_back(dummy);
	nSavePts++;
      }

      std::sort(sortedEntries.begin(),sortedEntries.end(),Sorter<float>());
      std::cout << "DATA: " << std::setw(8) << nSavePts << " entries in " << InputFile << std::endl;

      // Clone tree 
                                                                                                                                                               
      std::cout << std::endl;
      std::cout<<" **************************** "<<std::endl;
      std::cout << "******* Clone tree ********" << std::endl;

      for(int iSaved = 0; iSaved < nSavePts; ++iSaved){

	  if( (iSaved%10000 == 0) ) std::cout << "reading saved entry " << iSaved << std::endl;

	  tree -> GetEntry(sortedEntries[iSaved].entry);
	  cloneTree -> Fill();
      }


 }



 if(SortVarType=="double"){

    double SortVariable; tree->SetBranchAddress(SortVarName.c_str(),&SortVariable);

    std::vector< Sorter<double> >  sortedEntries; // in order to sort the entries                                                                                       
    for(int iEntry = 0; iEntry < nEntries; ++iEntry){
      if( (iEntry%100000 == 0) ) std::cout << "reading saved entry " << iEntry << std::endl;
      tree -> GetEntry(iEntry);
      Sorter<double> dummy ;    dummy.sortVar = SortVariable; dummy.entry   = iEntry; sortedEntries.push_back(dummy);
      nSavePts++;
    }

    std::sort(sortedEntries.begin(),sortedEntries.end(),Sorter<double>());
    std::cout << "DATA: " << std::setw(8) << nSavePts << " entries in " << InputFile << std::endl;


    // Clone tree                                                                                                                                                   
    std::cout << std::endl;
    std::cout<<" **************************** "<<std::endl;
    std::cout << "******* Clone tree ********" << std::endl;

    for(int iSaved = 0; iSaved < nSavePts; ++iSaved){

      if( (iSaved%10000 == 0) ) std::cout << "reading saved entry " << iSaved << std::endl;

      tree -> GetEntry(sortedEntries[iSaved].entry);
      cloneTree -> Fill();
    }


  }

  if(SortVarType=="string"){

    std::string SortVariable; tree->SetBranchAddress(SortVarName.c_str(),&SortVariable);

    std::vector< Sorter<std::string> >  sortedEntries; // in order to sort the entries                                                                                       

    for(int iEntry = 0; iEntry < nEntries; ++iEntry){
      if( (iEntry%100000 == 0) ) std::cout << "reading saved entry " << iEntry << std::endl;
      tree -> GetEntry(iEntry);
      Sorter<std::string> dummy ;    dummy.sortVar = SortVariable; dummy.entry   = iEntry; sortedEntries.push_back(dummy);
      nSavePts++;
    }

    std::sort(sortedEntries.begin(),sortedEntries.end(),Sorter<std::string>());
    std::cout << "DATA: " << std::setw(8) << nSavePts << " entries in " << InputFile << std::endl;


    // Clone tree                                                                                                                                                   
    std::cout << std::endl;
    std::cout<<" **************************** "<<std::endl;
    std::cout << "******* Clone tree ********" << std::endl;

    for(int iSaved = 0; iSaved < nSavePts; ++iSaved){

      if( (iSaved%10000 == 0) ) std::cout << "reading saved entry " << iSaved << std::endl;

      tree -> GetEntry(sortedEntries[iSaved].entry);
      cloneTree -> Fill();
    }

  }

  out -> Close();
  
  return 0; 
  
}


