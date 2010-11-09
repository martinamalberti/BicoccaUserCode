/** Monte-Carlo toy Experiment of the population in the different ABCD areas at different
level of integrated luminosity*/
#include "treeReader.h"
#include "hFactory.h"
#include "hFunctions.h"
#include "stdHisto.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TMinuit.h"
#include "Math/GenVector/VectorUtil.h"
#include "TRandom3.h"
#include <time.h>
#include <sstream>
#include "TColor.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TGraphErrors.h"
#include <vector>
#include "ABCD.h"
#include "TRandom3.h"
#include <cstdlib>
#include <math.h>

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#endif

/** function for the search of the maximum value of a distribution and 
the 1-sigma and 2-sigma confidence level*/

std::vector<double> getSigmaBands_FeldmanCousins (TH1D*histo)
{
  ///==== AM Get Neyman Intervals, MPV, +/- 1 sigma ====
  
  int StepY = 1000;
  
  std::vector<double> result (5, 0.) ;
  
  double maxY = histo->GetMaximum();
  int totBins = histo->GetNbinsX();
  int iBinCenter = histo->GetMaximumBin();
  int iBinMin = iBinCenter;
  int iBinMax = iBinMin;
  int iBinMin_cycle = iBinCenter;
  int iBinMax_cycle = iBinMin_cycle;
  double totalEntries = histo->GetEffectiveEntries();
  double integral = histo->GetBinContent(iBinCenter);
  double area = integral / totalEntries;
  double DeltaY = maxY / StepY;
  double PositionY = maxY;
  
  double ValueSx = maxY;
  double ValueDx = maxY;
  
  bool doContinue68 = true;
  int increasePerformance = 0;
  
  if (totalEntries != 0) { ///==== if 0 entries return 0,0,0,0,0
    while (doContinue68 && PositionY>0){ /// && PositionY>0 test for infinite-loop stop
      PositionY = PositionY - DeltaY;
      ///==== look left ====
      for (int iBinSx = (iBinCenter-iBinMin_cycle); iBinSx < iBinCenter; iBinSx++){
	ValueSx = histo->GetBinContent(iBinCenter-iBinSx);
	if (ValueSx <= PositionY) {
	  iBinMin = iBinCenter-iBinSx;
	  break;
	}
	else {
	  if (iBinSx == (iBinCenter-1)){
	    iBinMin = iBinSx;
	  }
	}
      }
      ///==== look right ====
      for (int iBinDx = (iBinMax_cycle-iBinCenter); iBinDx <= (totBins - iBinCenter); iBinDx++){
	ValueDx = histo->GetBinContent(iBinCenter+iBinDx);
	if (ValueDx <= PositionY) {
	  iBinMax = iBinCenter+iBinDx;
	  break;
	}
	else {
	  if (iBinDx == ((totBins - iBinCenter)-1)){
	    iBinMax = iBinDx;
	  }
	}
      }
      
      integral = histo->Integral(iBinMin,iBinMax);
      area = integral / totalEntries;
      // std::cerr << " area 68[" << iBinMin << ":" << iBinCenter << ":" << iBinMax << "] [" << PositionY << ":" << maxY << "] = " << area << " = " << integral << " / " << totalEntries << std::endl;
      if (area > 0.68) {
	if (increasePerformance == 0){
	  increasePerformance = 1;
	  PositionY = PositionY + DeltaY;
	  DeltaY = DeltaY / 10.;
	}
	else {
	  doContinue68 = false;
	}
      }
      else {
	iBinMin_cycle = iBinMin;
	iBinMax_cycle = iBinMax;
      }
    }
    
    ///=== 68% and mean ===
    result.at(1) = histo->GetBinCenter(iBinMin);
    result.at(2) = histo->GetBinCenter(iBinCenter);
    result.at(3) = histo->GetBinCenter(iBinMax);
    ///====================
    
    
    bool doContinue95 = true;
    increasePerformance = 0;
    
    while (doContinue95 && PositionY>0){ /// && PositionY>0 test for infinite-loop stop
      PositionY = PositionY - DeltaY;
      ///==== look left ====
      for (int iBinSx = (iBinCenter-iBinMin_cycle); iBinSx < iBinCenter; iBinSx++){
	ValueSx = histo->GetBinContent(iBinCenter-iBinSx);
	// std::cerr << " ValueSx[" << iBinSx << "] = " << ValueSx << " " ;
	if (ValueSx <= PositionY) {
	  // std::cerr << "found SX " << std::endl;
	  iBinMin = iBinCenter-iBinSx;
	  break;
	}
	else {
	  if (iBinSx == (iBinCenter-1)){
	    iBinMin = iBinSx;
	  }
	}
      }
      ///==== look right ====
      for (int iBinDx = (iBinMax_cycle-iBinCenter); iBinDx <= (totBins - iBinCenter); iBinDx++){
	ValueDx = histo->GetBinContent(iBinCenter+iBinDx);
	// std::cerr << " ValueDx[" << iBinDx << "] = " << ValueDx << " " ;
	if (ValueDx <= PositionY) {
	  // std::cerr << "found DX " << std::endl;
	  iBinMax = iBinCenter+iBinDx;
	  break;
	}
	else {
	  if (iBinDx == ((totBins - iBinCenter)-1)){
	    iBinMax = iBinDx;
	  }
	}
      }
      
      integral = histo->Integral(iBinMin,iBinMax);
      area = integral / totalEntries;
      // std::cerr << " area 95[" << iBinMin << ":" << iBinCenter << ":" << iBinMax << "] [" << PositionY << ":" << maxY << "] = " << area << " = " << integral << " / " << totalEntries << " SX : " << iBinCenter-iBinMin_cycle << " : " << iBinCenter << " DX : " << (iBinMax_cycle-iBinCenter) << " : " << (totBins - iBinCenter) << std::endl;
      if (area > 0.95) {
	if (increasePerformance == 0){
	  increasePerformance = 1;
	  PositionY = PositionY + DeltaY;
	  DeltaY = DeltaY / 10.;
	}
	else {
	  doContinue95 = false;
	}
      }
      else {
	iBinMin_cycle = iBinMin;
	iBinMax_cycle = iBinMax;
      }
  }
  
  ///=== 95% ===
  result.at(0) = histo->GetBinCenter(iBinMin);
  result.at(4) = histo->GetBinCenter(iBinMax);
  ///===========
}

return result ;
}





/** MAIN FUNCTION*/

int main(int argc, char** argv)
{ 
 ///=========================
 gROOT->Reset();
 gROOT->SetStyle("Plain");
 gStyle->SetPalette(1);
 ///=========================
 
  if(argc != 2)
  {
    std::cerr << ">>>>> analysis.cpp::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  /** read and acquire the information put in the cfg file*/
  
  parseConfigFile (argv[1]) ;
  
  std::string treeName  = gConfigParser -> readStringOption("Input::treeName");
  std::string inputFile = gConfigParser -> readStringOption("Input::inputFile");
  std::string treeName2 = gConfigParser -> readStringOption("Input::treeName2"); 
  
  /** open the tree of data*/
  
  TChain *chain = new TChain(treeName.c_str());
  chain->Add(inputFile.c_str());
  TTree*tree_in=(TTree*)chain;
  
  TChain *chain2 = new TChain(treeName2.c_str());
  chain2->Add(inputFile.c_str());
  TTree*tree_in_eff=(TTree*)chain2;
  
  double preselection_efficiency;
  int numEntriesBefore;
  double XSection;
  
  tree_in_eff->SetBranchAddress("preselection_efficiency",&preselection_efficiency);
  tree_in_eff->SetBranchAddress("numEntriesBefore",&numEntriesBefore);
  tree_in_eff->SetBranchAddress("XSection",&XSection); 
  
  
  
  /** output file*/
  std::string OutFileName    = gConfigParser -> readStringOption("Output::outFileName");
  std::cout << ">>>>> Output::outFileName  " << OutFileName  << std::endl;
  
  
  double start, end;  
  double step;
  start = clock();
  
  /// Luminosity values chosen for the analysis
  
  std::string inputLuminosity  = gConfigParser -> readStringOption("Input::luminosity");
  std::ifstream File(inputLuminosity.c_str());
  std::vector <double> lum;
  std::string buffer;
  while(!File.eof())
  {
    getline(File,buffer);
    std::stringstream line(buffer);
    
    if (buffer != "")
    {
      if (buffer.at(0) != '#')
      {
	line>>step;
	lum.push_back(step);
	
      }
    }
  }  
  
  
  
  ///==== number of Toy MC to be performed ====
  int numToyMC = gConfigParser -> readIntOption("Options::numToyMC");
  
  ///==== definition of signal region ====
  char signalRegion = (static_cast<std::string>(gConfigParser -> readStringOption("Input::signalRegion"))).at(0);
  std::cout << " signalRegion = " << signalRegion << std::endl;
  
  /// read the ABCD Variables 
  
  std::string inputVariable    = gConfigParser -> readStringOption("Input::inputVariable");
  std::ifstream Variable(inputVariable.c_str());
  
  std::string VariableX;
  std::string VariableY;
  double CutX;
  double CutY;
  
  
  while(!Variable.eof())
  {
    getline(Variable,buffer);
    std::stringstream line( buffer );
    if (buffer != "")
    {
      if (buffer.at(0) != '#')
      {
	line >> VariableX;
	line >> CutX;
	line >> VariableY;
	line >> CutY;
      }
    }
  }
  
  
  /// Optimal Selection on variables
  
  std::string inputFileCUT    = gConfigParser -> readStringOption("Input::inputFileCUT");
  std::ifstream inFile(inputFileCUT.c_str());
  std::vector <std::string> Var;
  std::vector <double> Cut;
  std::vector <std::string> vMinMaj;
  std::string G_Cut,stringA;
  double doubleA;
  std::string MinMaj;
  int flag=0;
  
  while(!inFile.eof())
  { 
   getline(inFile,buffer);
   std::stringstream line( buffer );
   if(buffer == "[Variable]") continue;
   if (buffer!="" && flag==0 && buffer!="[General_Cut]")
   { 
    if (buffer.at(0) != '#')
    {
     line >> stringA;
     line >> MinMaj;
     line >> doubleA;
     Var.push_back(stringA);
     Cut.push_back(doubleA);
     vMinMaj.push_back(MinMaj);
    }
   }
   if (buffer != ""&&flag==1)
   { 
    if (buffer.at(0) != '#')
    { 
     G_Cut = buffer; //==== all the buffer "string" is the general cut
    } 
   }
   if(buffer=="[General_Cut]") 
   {
    flag=1;  //==== next time will be the input of the General_Cut
   }  
  } 
  
  std::cout << " Input:" << std::endl;
  std::cout << "   General_Cut: " << G_Cut << std::endl;
  for(int iVar=0; iVar<Var.size(); iVar++){
   std::cout << "   Variable[" << iVar << "] : " << Var[iVar] << " " <<  vMinMaj[iVar] << " " << Cut[iVar] << std::endl;
  }
  
  /// General cut
  
  TString Variable_Cut;
  TString Complete_Cut;
  
  
  for(int j=0;j<Var.size();j++){
   if (Var[j] == VariableX) continue;
   if (Var[j] == VariableY) continue;
    
   Variable_Cut = Variable_Cut + Form("%s %s %f && ",Var[j].c_str(),vMinMaj[j].c_str(),Cut[j]);
  }
  Complete_Cut = Variable_Cut+Form("%s",G_Cut.c_str());
  
  /// ABCD analysis on the Monte-Carlo Data
  
  ABCD Analisi(tree_in);
  Analisi.SetVarX(VariableX);
  Analisi.SetVarY(VariableY);
  Analisi.SetCutX(CutX);
  Analisi.SetCutY(CutY);
  Analisi.SetGeneralCut(Complete_Cut);
  Analisi.CreateHisto();
  
  
  std::vector <TH1D*> HistogrammExpected;
  std::vector <TH1D*> HistogrammABCD;
  std::vector <TH1D*> HistogrammRelativeError;


  
  int EndA = Analisi.GetEntries('A');
  int EndB = Analisi.GetEntries('B');
  int EndC = Analisi.GetEntries('C');
  int EndD = Analisi.GetEntries('D');
  double TotalMC = EndA + EndB + EndC + EndD;
  double epsilonA = EndA / TotalMC;
  double epsilonB = (EndA + EndB) / TotalMC;
  double epsilonC = (EndA + EndB + EndC) / TotalMC;
  

  double vectEventsABCC[4];
  double EventsA;
  double EventsB;
  double EventsC;
  double EventsD;
  double EventsABCD;
  double RelativeError;

  int TotaleEventi;
  int IntEventsABCD=0;
  
  int A = 0;
  int B = 0;
  int C = 0;
  int D = 0;
  
  TH1D* HistoExpected;
  TH1D* HistoABCD;
  TH1D* HistoRelativeError;

  TRandom3* Rand=new TRandom3();
  TRandom3* Multirand=new TRandom3();
  
  tree_in_eff->GetEntry(0);
  std::cout << "Xsection = " << XSection << " preselection = " << preselection_efficiency << " Number = " << numEntriesBefore << std::endl;
  
  for(int k=0;k<lum.size();k++)
  {   
   std::cout << " simulation of Lumi = " << lum.at(k) << " pb^{-1}   " << k << " : " << lum.size() << std::endl;
    /// loop over all luminosity values set
    std::stringstream Num;
    Num << lum[k];
    std::string nameExpected = "HistoExpected" + Num.str();
    std::string nameABCD = "HistoABCD" + Num.str();
    std::string nameRelativeError = "HistoRelativeError" + Num.str();

    /// Number of events at a fixed L
    EventsA = lum[k] * XSection * preselection_efficiency * EndA / numEntriesBefore;
    EventsB = lum[k] * XSection * preselection_efficiency * EndB / numEntriesBefore;
    EventsC = lum[k] * XSection * preselection_efficiency * EndC / numEntriesBefore;
    EventsD = lum[k] * XSection * preselection_efficiency * EndD / numEntriesBefore;

    int ExpEventsSig;
    if ( signalRegion == 'A' ) {
     ExpEventsSig = EventsA;
    }
    if ( signalRegion == 'B' ) {
     ExpEventsSig = EventsB;
    }
    if ( signalRegion == 'C' ) {
     ExpEventsSig = EventsC;
    }
    if ( signalRegion == 'D' ) {
     ExpEventsSig = EventsD;
    }
    
    HistoExpected      = new TH1D(nameExpected.c_str(),"Event in Signal region", numToyMC / 300, 0, ExpEventsSig * 10);
    HistoABCD          = new TH1D(nameABCD.c_str(),"Event ABCD in Signal region", numToyMC / 300, 0, ExpEventsSig * 10);
    HistoRelativeError = new TH1D(nameRelativeError.c_str(),"Relative Error", numToyMC / 300,-6,6);
    
    /// approximation to the nearest integer value
    if(( EventsA + EventsB + EventsC + EventsD - int(EventsA + EventsB + EventsC + EventsD))<0.5)
    { 
     TotaleEventi = floor(EventsA + EventsB + EventsC + EventsD);
    }
    else
    { 
     TotaleEventi = ceil(EventsA + EventsB + EventsC + EventsD);
    }
    
    /// Toy Experiment
    for(int j=0; j<numToyMC; j++)
    { 
      /// loop for the number of pseudo-generation at a fixed L
      
      int RandNumExpected=Rand->Poisson(TotaleEventi);
      for(int i=0;i<RandNumExpected;i++)
      {	   
	   /// loop for the division of the RandNumExpected events in the ABCD areas according to a quadrinomial distribution
	  
	   double RandPoint=Multirand->Uniform(0,1);
             
	   if(RandPoint>=0 && RandPoint<=epsilonA)	
	      A++;
	   if(RandPoint>epsilonA && RandPoint<=epsilonB)
	     B++;
	   if(RandPoint>epsilonB && RandPoint<=epsilonC)
	      C++;
	   if(RandPoint>epsilonC && RandPoint<=1)
	     D++;
	   
      }

      EventsA = A;
      EventsB = B;
      EventsC = C;
      EventsD = D;
	  
      vectEventsABCC[0] = A;
      vectEventsABCC[1] = B;
      vectEventsABCC[2] = C;
      vectEventsABCC[3] = D;
      
      /// ABCD method application
      
      if(EventsA!=0 && EventsC!=0 && EventsD!=0 && EventsB!=0) 
      {
       int ExpEventsSig;
       if ( signalRegion == 'A' ) {
	EventsABCD = EventsB * EventsC / EventsD;
	ExpEventsSig = EventsA;
       }
       if ( signalRegion == 'B' ) {
	EventsABCD = EventsA * EventsD / EventsC;
	ExpEventsSig = EventsB;
       }
       if ( signalRegion == 'C' ) {
	EventsABCD = EventsA * EventsD / EventsB;
	ExpEventsSig = EventsC;
       }
       if ( signalRegion == 'D' ) {
	EventsABCD = EventsB * EventsC / EventsA;
	ExpEventsSig = EventsD;
       }
	
	if((EventsABCD-int(EventsABCD))<=0.5)
	/// integer approximation
	{ 
	 IntEventsABCD = floor(EventsABCD);
	}
	else
        {
	 IntEventsABCD = ceil(EventsABCD);
	}
        /// Fill the distribution used to study the performance of the ABCD method
	
	HistoABCD -> Fill (IntEventsABCD);
	
	HistoExpected -> Fill (ExpEventsSig);
	RelativeError = (IntEventsABCD - ExpEventsSig) / (ExpEventsSig);
	HistoRelativeError -> Fill (RelativeError);
	
      }
      
      A = 0;
      B = 0;
      C = 0;
      D = 0;
    }
    
    /// Put the distributions in a vector in which each entry is referred to a fixed value of luminosity
    HistogrammExpected.push_back(HistoExpected);
    HistogrammABCD.push_back(HistoABCD);
    HistogrammRelativeError.push_back(HistoRelativeError);

    std::cout<<"<<<<<<<<<<<<>"<<" Luminosità Integrata"<<std::endl;
  }


/** Make the dependencies of the ABCD method estimators as function of L */
 system("mkdir output");
 system("mkdir output/ToyMC");
 
 TFile output (OutFileName.c_str(),"RECREATE");
 output.cd();
 
 TGraphAsymmErrors*Mean = new TGraphAsymmErrors();
 TGraphAsymmErrors*MeanABCD_1sigma = new TGraphAsymmErrors();
 TGraphAsymmErrors*MeanExpected = new TGraphAsymmErrors();
 TGraphAsymmErrors* RmsABCD = new TGraphAsymmErrors();
 TGraphAsymmErrors* SystematicBiasBands = new TGraphAsymmErrors();
 TGraphAsymmErrors* SystematicBias = new TGraphAsymmErrors();
 TGraphAsymmErrors* SystematicBiasError = new TGraphAsymmErrors();
 TGraphAsymmErrors* BiasMean = new TGraphAsymmErrors();


 
 for (int i=0;i<lum.size();i++)
 {
  std::cout << " simulation of Lumi = " << lum.at(i) << " pb^{-1}   " << i << " : " << lum.size() << std::endl;
  /// save each distribution obtained as function of L
   HistogrammExpected[i]->Write();
   HistogrammABCD[i]->Write();
   HistogrammRelativeError[i]->Write();

   
   /// peak analysis of each distributions
   std::vector<double> MeanABCDBands = getSigmaBands_FeldmanCousins(HistogrammABCD[i]);
   std::vector<double> SystematicBands = getSigmaBands_FeldmanCousins(HistogrammRelativeError[i]);

   
   /// sigma_RMS is equal for each distributions
   double sigma_Rms = 1/(sqrt(2*(HistogrammABCD[i]->GetEntries()-1)));
   
  
   /// peak of ABCD distributions as function of L with 1 sigma-band of error
   MeanABCD_1sigma->SetPoint(i,lum[i],MeanABCDBands.at(2));
   MeanABCD_1sigma->SetPointError(i,0,0,fabs(MeanABCDBands.at(2)-MeanABCDBands.at(1))/2,fabs(MeanABCDBands.at(3)-MeanABCDBands.at(2))/2);
   
   /// mean of each ABCD distributions as function of L with a band of error given by the RMS
   Mean->SetPoint(i,lum[i],HistogrammABCD[i]->GetMean());
   Mean->SetPointError(i,0,0,fabs(HistogrammABCD[i]->GetRMS()),fabs(HistogrammABCD[i]->GetRMS()));
   
   /// RMS of the ABCD distributions as function of L with an error band given by sigma_RMS
   RmsABCD->SetPoint(i,lum[i],HistogrammABCD[i]->GetRMS());
   RmsABCD->SetPointError(i,0,0,sigma_Rms,sigma_Rms);
   
   /// peak of relative error distributions as function of L, the error band is given by the 1-sigma uncertainty
   SystematicBiasBands->SetPoint(i,lum[i],fabs(SystematicBands.at(2)));
   SystematicBiasBands->SetPointError(i,0,0,fabs(SystematicBands.at(1)-SystematicBands.at(2))/2,fabs(SystematicBands.at(3)-SystematicBands.at(2))/2);
  
   /// mean of relative error distributions as function of L, the error band is given by the RMS on the mean value
   SystematicBias->SetPoint(i,lum[i],fabs(HistogrammRelativeError[i]->GetMean()));
   SystematicBias->SetPointError(i,0,0,HistogrammRelativeError[i]->GetRMS()/(sqrt(HistogrammRelativeError[i]->GetEntries())),HistogrammRelativeError[i]->GetMean()/(sqrt(HistogrammRelativeError[i]->GetEntries())));
   
   /// RMS of relative error distributions as function of L, the error band is given by the sigma_RMS 
   SystematicBiasError->SetPoint(i,lum[i],HistogrammRelativeError[i]->GetRMS());
   SystematicBiasError->SetPointError(i,0,0,1/(sqrt(2*(HistogrammRelativeError[i]->GetEntries()-1))),1/(sqrt(2*(HistogrammRelativeError[i]->GetEntries()-1))));
   
   /// mean of relative error distributions as function of L, the error band is given by the RMS of the distributions
   BiasMean->SetPoint(i,lum[i],fabs(HistogrammRelativeError[i]->GetMean()));
   BiasMean->SetPointError(i,0,0,HistogrammRelativeError[i]->GetRMS()/2,HistogrammRelativeError[i]->GetRMS()/2);

 }
  
  /// Save this TGraph results in a TCanvas
  TString xAxisNameMean = Form("%s ^{ABCD}_{Mean}",VariableX.c_str());
  TString xAxisNamePeak = Form("%s ^{ABCD}_{Peak}",VariableX.c_str());
  
  TCanvas*ABCDMean=new TCanvas("ABCDMean","",1);
  ABCDMean->cd();
  ABCDMean->SetGrid();
  Mean->SetMarkerColor(kBlue);
  Mean->SetMarkerStyle(8);
  Mean->SetMarkerSize(0.7);
  Mean->SetLineWidth(2);
  Mean->SetFillColor(kRed);
  Mean->SetFillStyle(3002);
  Mean->GetXaxis()->SetTitle("L[pb^{-1}]");
  Mean->GetYaxis()->SetTitle(xAxisNameMean);
  MeanExpected->SetMarkerColor(kRed);
  MeanExpected->SetMarkerStyle(8);
  MeanExpected->SetMarkerSize(0.7);
  MeanExpected->SetLineWidth(2);
  MeanExpected->SetFillColor(kRed);
  MeanExpected->SetFillStyle(3002);
  Mean->Draw("3APC");
  MeanExpected->Draw("same");
  ABCDMean->Write();
  
  
  
  
  TCanvas* ABCDMeanBands= new TCanvas("ABCDMeanBands","",1);
  ABCDMeanBands->cd();
  ABCDMeanBands->SetGrid();
  MeanABCD_1sigma->SetMarkerColor(kBlue);
  MeanABCD_1sigma->SetMarkerStyle(8);
  MeanABCD_1sigma->SetMarkerSize(0.7);
  MeanABCD_1sigma->SetLineWidth(2);
  MeanABCD_1sigma->SetFillColor(kRed);
  MeanABCD_1sigma->SetFillStyle(3002);
  MeanABCD_1sigma->GetXaxis()->SetTitle("L[pb^{-1}]");
  MeanABCD_1sigma->GetYaxis()->SetTitle(xAxisNamePeak);
  MeanABCD_1sigma->Draw("3APC");
  ABCDMeanBands->Write();

  
  TCanvas* ABCDsigma= new TCanvas("ABCDsigma","",1);
  ABCDsigma->cd();
  ABCDsigma->SetGrid();
  RmsABCD->SetMarkerColor(kBlue);
  RmsABCD->SetMarkerStyle(8);
  RmsABCD->SetMarkerSize(0.7);
  RmsABCD->SetLineWidth(2);
  RmsABCD->SetFillColor(kRed);
  RmsABCD->SetFillStyle(3002);
  RmsABCD->GetXaxis()->SetTitle("L[pb^{-1}]");
  RmsABCD->GetYaxis()->SetTitle("#sigma^{ABCD}");
  RmsABCD->Draw("3APC");

  
  ABCDsigma->Write();
  
  TCanvas* SystematicBiasABCD= new TCanvas("SystematicBiasABCD","",1);
  SystematicBiasABCD->cd();
  SystematicBiasABCD->SetGrid();
  SystematicBias->SetMarkerColor(kBlue);
  SystematicBias->SetMarkerStyle(8);
  SystematicBias->SetMarkerSize(0.7);
  SystematicBias->SetLineWidth(2);
  SystematicBias->SetFillColor(kRed);
  SystematicBias->SetFillStyle(3002);
  SystematicBias->GetXaxis()->SetTitle("L[pb^{-1}]");
  SystematicBias->GetYaxis()->SetTitle("<r>");
  SystematicBias->Draw("3APC");
  
  SystematicBiasABCD->Write();
  
  TCanvas* SystematicBiasBandsABCD= new TCanvas("SystematicBiasBandsABCD","",1);
  SystematicBiasBandsABCD->cd();
  SystematicBiasBandsABCD->SetGrid();
  SystematicBiasBands->SetMarkerColor(kBlue);
  SystematicBiasBands->SetMarkerStyle(8);
  SystematicBiasBands->SetMarkerSize(0.7);
  SystematicBiasBands->SetLineWidth(2);
  SystematicBiasBands->SetFillColor(kRed);
  SystematicBiasBands->SetFillStyle(3002);
  SystematicBiasBands->GetXaxis()->SetTitle("L[pb^{-1}]");
  SystematicBiasBands->GetYaxis()->SetTitle("r_{peak}");
  SystematicBiasBands->Draw("3APC");
  
  SystematicBiasBandsABCD->Write();
  
  
  
  
  TCanvas* SystematicBiasRMS= new TCanvas("SystematicBiasRMS","",1);
  SystematicBiasRMS->cd();
  SystematicBiasRMS->SetGrid();
  SystematicBiasError->SetMarkerColor(kBlue);
  SystematicBiasError->SetMarkerStyle(8);
  SystematicBiasError->SetMarkerSize(0.7);
  SystematicBiasError->SetLineWidth(2);
  SystematicBiasError->SetFillColor(kRed);
  SystematicBiasError->SetFillStyle(3002);
  SystematicBiasError->GetXaxis()->SetTitle("L[pb^{-1}]");
  SystematicBiasError->GetYaxis()->SetTitle("#sigma_{r}");
  SystematicBiasError->Draw("3APC");
  

  
  SystematicBiasRMS->Write();
  
  TCanvas* BiasTotMean= new TCanvas("BiasTotMean","",1);
  BiasTotMean->cd();
  BiasTotMean->SetGrid();
  BiasMean->SetMarkerColor(kBlue);
  BiasMean->SetMarkerStyle(8);
  BiasMean->SetMarkerSize(0.7);
  BiasMean->SetLineWidth(2);
  BiasMean->SetFillColor(kRed);
  BiasMean->SetFillStyle(3002);
  BiasMean->GetXaxis()->SetTitle("L[pb^{-1}]");
  BiasMean->GetYaxis()->SetTitle("<r>");
  BiasMean->Draw("3APC");
  
  
  BiasTotMean->Write();
  
  

  
  end = clock();
  std::cout <<"Time = " <<  ((double) (end - start)) << " (a.u.)" << std::endl;
  
  return(0); 
} 