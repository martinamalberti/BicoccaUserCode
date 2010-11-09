# ifndef ABCD_h
# define ABCD_h
# include <iostream>
# include <cstdlib>
# include <string>
# include <vector>
# include <cmath>
# include "TH2D.h"
# include "TFile.h"
# include "TMath.h"
# include "TTree.h"
# include "ConfigParser.h"
# include "TString.h"
# include "TCanvas.h"
# include "TMinuit.h"

class ABCD
{ 
 public:
  /** ABCD class constructor*/
  ABCD();
  ABCD(TTree* tree_in,std::string& output,std::string& varY,double cutY,std::string&  varX,double cutX,std::string&  gen_cut);
  ABCD(TTree* tree_in);
  
  /** Destructor*/
  ~ABCD();
  
  
  /** Initialization of histos. Called by creator */
  void InitializeHisto();
  
  
  /** Creator of the ABCD space subdivision*/
  void CreateHisto(const int& nbinX,const double& X_MIN,const double& X_MAX,const int & nbinY,const double& Y_MIN,const double& Y_MAX);
  void CreateHisto(int& nbinX,int& nbinY);
  void CreateHisto();
  
  /** Get a pointer to the histogram the ABCD space or for each region*/
  TH2D* GetHisto(char letter);
  TH2D* GetHisto(int num);
  TH2D* GetTotalHisto();
  
  /** method for the automatic displacement of the binning */
  void BinCorrectionX(int& nbin,double& MIN,double& MAX);
  void BinCorrectionY(int& nbin,double& MIN,double& MAX);
  
  /** Get Entries and errors in each region*/
  double GetEntries(char region);
  double GetEntriesError(char region);
  double GetEntries(int number);
  double GetEntriesError(int number);
  
  /** method for effective entries and error*/
  double GetEffectiveEntries(char region);
  double GetEffectiveEntriesError(char region);
  double GetEffectiveEntries(int number);
  double GetEffectiveEntriesError(int number);
  
  /** Get number of bin fixed after a automatic scaling of events in each one along x and y axis*/
  int GetBinXY();
  
  /** Ratio between the entries in two different region
  return(-1)-> wrong region, return(-2)-> infinite ratio
  */
  double GetRatio (char region1, char region2);
  double GetRatioError(char region1, char region2);
  double GetEffectiveRatio (char region1, char region2);
  double GetEffectiveRatioError(char region1, char region2);
  
  /** Double ratio
  return(-1)-> wrong region, return(-2)-> infinite ratio*/
  double GetDoubleRatio(char region1, char region2, char region3, char region4);
  double GetDoubleRatioError(char region1,char region2,char region3,char region4);
  
  double GetEffectiveDoubleRatio(char region1, char region2, char region3, char region4);
  double GetEffectiveDoubleRatioError(char region1, char region2, char region3, char region4);
  
  /**Correlation coefficient and probability on a t-Student distribution P(|r|<|r_{0}|)*/
  double GetCorrelation();
  double GetCorrelationProbability();
  
  /**these methods return the one-dimensional distribution of X or Y variable
  before or after the application of the general cut*/
  void GetDistributionX();
  void GetDistributionXCut();
  void GetDistributionY();
  void GetDistributionYCut();
  
  TH1D* GetDistribution(const char & variable,const std::string& region);
  
  
  
  
  /** Get maximum or minimum value of a fixed variable */
  double GetMaxVariable(std::string& Variable);
  double GetMinVariable(std::string& Variable);
  
  /**Kolmogorv-Smirnov Test given the variable on which we want to project the ABCD distribution*/
  double KolmogorovTest(std::string & Var);
  
  /** ABCD configuration*/
  void Print();
  
  /** Reset of the histograms generated in the ABCD analysis*/
  void ResetHisto();
  
  /** methods for save the output of the analysis*/
  void Save();
  void Save(std::string& outFile);
  void SaveDistribution(std::string outFile);
  
  /** Set Functions*/
  void SetTree(TTree* tree_in);
  void SetVarX(std::string &VarX);
  void SetVarX(TString VarX);
  void SetVarY(std::string &VarY);
  void SetVarY(TString VarY);
  void SetGeneralCut(std::string & GenCut);
  void SetGeneralCut(TString GenCut);
  void SetCutX(double CutX);
  void SetCutY(double CutY);
  void SetBinX(int nbinX);
  void SetBinY(int nbinY);
  void SetMinEntriesPerBin(int minEntriesPerBin);
  
  
  
  
  
 private: 
  TTree *tree_in_p;
  TFile* outFile_p;
  std::string output_p;
  std::string varY_p;
  double cutY_p;
  std::string varX_p;
  double cutX_p;
  std::string gen_cut_p;
  std::vector <TH2D*> VetHisto_p;
//   std::map <char, TH1D*> vH1D_;
  TH2D* Htot_p;
  TH1D* Kolmogorov_pA;
  TH1D* Kolmogorov_pB;
  TH1D* Kolmogorov_pC;
  TH1D* Kolmogorov_pD;
  TH1D* Kolmogorov_p1;
  TH1D* Kolmogorov_p2;
  int nbinX_p;
  int nbinY_p;
  double X_MIN_;
  double X_MAX_;
  double Y_MIN_;
  double Y_MAX_;  
  TH1D*histoX_p;
  TH1D*histoY_p;
  TH1D*histoXCut_p;
  TH1D*histoYCut_p;
  
  int minEntriesPerBin_;
  
  
}; 

# endif