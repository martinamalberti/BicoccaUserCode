# ifndef ABCDMultiDimensional_h
# define ABCDMultiDimensional_h

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

class ABCDMultiDimensional
{ 
 public:
  /** ABCDMultiDimensional class constructor*/
  ABCDMultiDimensional(TTree* tree_in,
       const std::string & output,
       const std::vector<std::string> & varY,
       const std::vector<std::string> & MinMajY,
       const std::vector<double> & cutY,
       const std::vector<std::string> & varX,
       const std::vector<std::string> & MinMajX,
       const std::vector<double> & cutX,
       const std::string &  gen_cut
       );
  
  /** Destructor*/
  ~ABCDMultiDimensional();
  
  /** Change sign of selections */
  std::vector<std::string> OppositeSign(const std::vector<std::string>&);
  
  /** Initialization of histos. Called by creator */
  void InitializeHisto();
  
  /** Creator of the ABCDMultiDimensional space subdivision*/
  void CreateHisto();
  
  /** Get a pointer to the histogram the ABCDMultiDimensional space or for each region*/
  TH2D* GetHisto(char letter);
  TH2D* GetHisto(int num);
  TH2D* GetTotalHisto();
  
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
  
  TH1D* GetDistribution(const char & variable,const std::string& region);
  
  /** ABCDMultiDimensional configuration*/
  void Print();
  
  /** Set Functions*/
  void SetTree(TTree* tree_in);
  
  /** Reset of the histograms generated in the ABCDMultiDimensional analysis*/
  void ResetHisto();
  
  /** methods for save the output of the analysis*/
  void Save();
  void Save(std::string& outFile);
  void SaveDistribution(std::string outFile);
  
  /** Set Functions*/
  
  
 private: 
  std::vector<char> vRegionName;
  
  std::string output_;
  std::vector<std::string> varY_;
  std::vector<std::string> MinMajY_;
  std::vector<std::string> OppositeMinMajY_;
  std::vector<double> cutY_;
  std::vector<std::string> varX_;
  std::vector<std::string> MinMajX_;
  std::vector<std::string> OppositeMinMajX_;
  std::vector<double> cutX_;
  std::string gen_cut_;
  
  TTree *tree_in_;
  TFile* outFile_;
  TH2D* Htot_;
  std::vector <TH2D*> VetHisto_; ///==== A, B, C, D
//  std::vector <TH1D*> VetHistoABCD_; ///==== AB, CD, AC, BD
  
}; 

# endif