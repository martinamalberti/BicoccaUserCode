#ifndef ntupleUtils_h
#define ntupleUtils_h

#include <iostream>
#include <fstream>
#include <string>

#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TF1.h"

#ifdef _MAKECINT_
#pragma link C++ class vector<Sorter>+;
#pragma link C++ class vector<SorterLC>+;
#endif



/*** likelihood for unbinned fit ***/
void mylike(int& /*npar*/, double* /*gin*/, double& fval, double* par, int /*iflag*/);



/*** fill a chain from a list of files ***/
bool FillChain(TChain* chain, const std::string& inputFileList);


/*** time sort a tree ***/
struct Sorter
{
  int time;
  int entry;
  
  bool operator() (const Sorter& s1, const Sorter& s2)
  {
    return s1.time < s2.time;
  }
};

/*** time sort a tree ***/
struct SorterLC
{
  float laserCorr;
  int entry;
  
  bool operator() (const SorterLC& s1, const SorterLC& s2)
  {
    return s1.laserCorr < s2.laserCorr;
  }
};



/*** double crystall ball ***/
double crystalBallLowHigh(double* x, double* par);

#endif