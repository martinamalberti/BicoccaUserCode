#ifndef ntupleUtils_h
#define ntupleUtils_h

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>


#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TF1.h"

#ifdef _MAKECINT_
#pragma link C++ class vector<Sorter>+;
#pragma link C++ class vector<SorterLC>+;
#pragma link C++ class map<int,TH1F*>+;
#pragma link C++ class map<string,TH1F*>+;
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
struct myEvent
{
  int runId;
  int timeStampHigh;

  int region;
  
  float scE;
  float P;

  float scLaserCorr;
  float seedLaserAlpha;
  
  bool operator() (const myEvent& s1, const myEvent& s2)
  {
    return s1.timeStampHigh < s2.timeStampHigh;
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



/*** breit-wigner ***/
double breitWigner(double* x, double* par);

/*** crystall ball with low tail ***/
double crystalBallLow(double* x, double* par);

/*** double crystall ball ***/
double crystalBallLowHigh(double* x, double* par);

/*** breit-wigner convoluted with crystalBall ***/
double breitWigner_crystalBallLow(double* x, double* par);


#endif
