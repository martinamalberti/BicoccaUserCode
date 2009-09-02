#ifndef NTupleUtils_h
#define NTupleUtils_h

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "CaloOnlineTools/EcalTools/interface/EcalCosmicsTreeContent.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <string>
#include <sstream>

#include "TROOT.h"
#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TClonesArray.h"
#include "TString.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3F.h"
#include "TText.h"
#include "TLegend.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TApplication.h"


double calcTheta (double eta) ;
double calcEta (double theta) ;
double getEcalXLenght (const TVector3& G1,const TVector3& G2) ;
double get3DAlpha (const TVector3& G1,const TVector3& G2) ;
double get2DAlphaXY (const TVector3& G1,const TVector3& G2) ;
double get2DAlphaYZ (const TVector3& G1,const TVector3& G2) ;
bool isBefore (const TVector3& pOld) ;
double where (double deltaAlpha, const TVector3& G1,const TVector3& G2, double alphaOld);
std::pair<TVector3,TVector3> getEcalCross (const TVector3& G1,const TVector3& G2, double ECALrad) ;
std::pair<double,double> getAlphaEcalCross (const TVector3& G1,const TVector3& G2, double ECALrad) ;
double get2DThickness (double z);
void setVectorOnECAL (TVector3 & vector, double eta, double phi, double radius = 129.) ;
void  setMuonTkAtECAL (GlobalPoint & AtEcal, int MUindex, const EcalCosmicsTreeContent & event);
double superGausCumCauda (double *x, double *par);
std::pair<double,double> fitdEdx (TH1F* dEdx);
std::pair<double,double> LangausFit (TH1F* histo);
void DrawBetheBloch();
void DrawBetheBloch(TGraph* BetheBloch);
double FindBetheBlochValue(TGraph* BetheBloch, float& muonP);
void DrawBetheBlochResiduals(TGraphAsymmErrors& BetheBloch_exp, TGraph& BetheBloch_th, TGraphErrors& BetheBloch_residuals);
bool IsInsideEcal(GlobalPoint& point);
void LangausFit(TF1** langaus, TH1D* histo, double* startValues, double xMin, double xMax);



template <class THisto>
void BinLogX(THisto& h)
{
  TAxis* axis = h.GetXaxis () ;
  int bins = axis -> GetNbins () ;
  
  Axis_t from = axis -> GetXmin () ;
  Axis_t to = axis -> GetXmax () ;
  Axis_t width = (to - from) / bins ;
  Axis_t* new_bins = new Axis_t[bins + 1] ;
  
  for (int i = 0; i <= bins; i++)
    new_bins[i] = pow(10, from + i * width) ;
  
  axis -> Set (bins, new_bins) ;
  delete new_bins ;
  
  // std::cout << std::endl;
  // for (int bin = 0 ; bin <= h.GetNbinsX () +1 ; ++bin)
  // {
  //  std::cout << "BIN: " << bin
  //            << "   lowEdge: " << h.GetBinLowEdge (bin)
  //            << "   higEdge: " << h.GetBinLowEdge (bin) + h.GetBinWidth (bin)
  //            << std::endl ;
  // }
}



template <class THisto>
void BinLogX_lastBin(THisto& h)
{
  TAxis* axis = h.GetXaxis () ;
  int bins = axis -> GetNbins () ;
  
  Axis_t from = axis -> GetXmin () ;
  Axis_t to = axis -> GetXmax () ;
  Axis_t width = (to - from) / bins ;
  Axis_t* new_bins = new Axis_t[bins + 1] ;
  
  for (int i = 0; i <= bins; i++)
    new_bins[i] = pow(10, from + i * width) ;
  
  new_bins[bins] = pow(10, from + bins * width + 1.);
  
  axis -> Set (bins, new_bins) ;
  delete new_bins ;
  
  // std::cout << std::endl;
  // for (int bin = 0 ; bin <= h.GetNbinsX () +1 ; ++bin)
  // {
  //  std::cout << "BIN: " << bin
  //            << "   lowEdge: " << h.GetBinLowEdge (bin)
  //            << "   higEdge: " << h.GetBinLowEdge (bin) + h.GetBinWidth (bin)
  //            << std::endl ;
  // }
}



template <class THisto>
void BinLogX(THisto* h)
{
  TAxis* axis = h -> GetXaxis () ;
  int bins = axis -> GetNbins () ;
  
  Axis_t from = axis -> GetXmin () ;
  Axis_t to = axis -> GetXmax () ;
  Axis_t width = (to - from) / bins ;
  Axis_t* new_bins = new Axis_t[bins + 1] ;
  
  for (int i = 0; i <= bins; i++)
    new_bins[i] = pow(10, from + i * width) ;
  
  axis -> Set (bins, new_bins) ;
  delete new_bins ;
  
  // std::cout << std::endl;
  // for (int bin = 0 ; bin <= h.GetNbinsX () +1 ; ++bin)
  // {
  //  std::cout << "BIN: " << bin
  //            << "   lowEdge: " << h.GetBinLowEdge (bin)
  //            << "   higEdge: " << h.GetBinLowEdge (bin) + h.GetBinWidth (bin)
  //            << std::endl ;
  // }
}

#endif

