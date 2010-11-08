/** Application of the ABCD class for the search of a pair of independent variables
for a given background */

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
#include "StatisticalPackage.h"
#include "TColor.h"
#include "TStyle.h"
#include <vector>
#include "TSystem.h"
#include "ABCD.h"

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#endif





int main(int argc, char** argv)
{ 
 /** control of the initial topics*/
 
 if(argc != 2)
 {
  std::cerr << ">>>>> analysis.cpp::usage: " << argv[0] << " configFileName" << std::endl ;
  return 1;
 }
 
 /** acquisition of the information contained in the cfg file*/
 
 parseConfigFile (argv[1]) ;
 
 std::string treeName  = gConfigParser -> readStringOption("Input::treeName");
 std::string inputFile = gConfigParser -> readStringOption("Input::inputFile");
 
 
//  int entryMAX = gConfigParser -> readIntOption("Input::entryMAX");
//  int entryMIN = gConfigParser -> readIntOption("Input::entryMIN");
//  int entryMOD = gConfigParser -> readIntOption("Input::entryMOD");
//  int nbinX = gConfigParser -> readIntOption("Input::nbinX");
//  int nbinY = gConfigParser -> readIntOption("Input::nbinY"); 
 
//  std::cout << ">>>>> input::entryMIN  " << entryMIN  << std::endl;  
//  std::cout << ">>>>> input::entryMAX  " << entryMAX  << std::endl;  
//  std::cout << ">>>>> input::entryMOD  " << entryMOD  << std::endl;  
//  std::cout << ">>>>> input::nbinX  " << nbinX  << std::endl; 
//  std::cout << ">>>>> input::nbinY  " << nbinY  << std::endl; 
 
 /** open the tree of data*/
 
 TFile* Input = new TFile(inputFile.c_str(),"READ");
 TTree* tree_in = (TTree*) Input->Get(treeName.c_str());
 
//  TChain *chain = new TChain(treeName.c_str());
//  chain->Add(inputFile.c_str());
//  TTree* tree_in = (TTree*) chain;
 
 //==================================================================
 
 /** read the output files*/
 
 std::string OutFileName    = gConfigParser -> readStringOption("Output::outFileName");
 std::cout << ">>>>> Output::outFileName  " << OutFileName  << std::endl;
 
 /** Clock and entry max*/
 
 double start, end;
 
//  entryMAX = tree_in->GetEntries();
//  std::cout << entryMAX << std::endl;
 
 int step = 0;
 start = clock();
 
 /** acquisition of variables and their selection contained in a txt file.
 Leave spaces between fields!
 */
 
 std::string imputFileCUT    = gConfigParser -> readStringOption("Input::inputFileCUT");
 std::ifstream inFile(imputFileCUT.c_str());
 std::vector <std::string> Var;
 std::vector <double> Cut;
//  std::vector <int> Sign;
 std::vector <std::string> vMinMaj;
 std::string G_Cut,stringA,buffer;
 double doubleA;
 std::string MinMaj;
//  int intA;
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
  
 
 /** Set Branch in the input tree*/
 
 std::string FileName;
 double KolmogorovFactor1;
 double KolmogorovFactor2;
 double Correlation;
 double CorrelationProbability;
 double DoubleRatio;
 double Nvariable=Var.size();
 
 /** Histograms for the ABCD analysis*/
 
 TH2D* CorrelationHistogramm= new TH2D("CorrelationHistogramm","Correlation Factor",Nvariable,0,Nvariable,Nvariable,0,Nvariable);
 TH2D* CorrelationProbabilityHistogramm= new TH2D("CorrelationProbabilityHistogramm","Correlation Probability",Nvariable,0,Nvariable,Nvariable,0,Nvariable);
 TH2D* DoubleRatioHistogramm= new TH2D("DoubleRatioHistogramm","Double Ratio",Nvariable,0,Nvariable,Nvariable,0,Nvariable);
 TH2D* KolmogorovHistogramm= new TH2D("KolmogorovHistogramm","Kolmogorov-Smirnov Test",Nvariable,0,Nvariable,Nvariable,0,Nvariable);
 TH2D* KolmogorovProductHistogramm= new TH2D("KolmogorovProductHistogramm","Kolmogorov Product",Nvariable,0,Nvariable,Nvariable,0,Nvariable);
 
 
 /** Create output directory */
 TString CommandToExec = Form("mkdir output/");
 gSystem->Exec(CommandToExec);  
 CommandToExec = Form("mkdir output/Histogram-Cut/");
 gSystem->Exec(CommandToExec);  
 
 
 /** Create output file*/
 
 TFile outFile(OutFileName.c_str(),"RECREATE");
 outFile.cd();
 
 /** Set tree*/
 
 ABCD Analysis(tree_in);
 
 for(int i=0;i<Var.size();i++)
 { 
  for(int j=i;j<Var.size();j++)
  { 
   if(i==j)
   {
    /** ABCD analysis on the 2-D configuration of the same variable*/
    
    CorrelationHistogramm->GetXaxis()->SetBinLabel(i+1,(Var[i]).c_str());
    CorrelationHistogramm->GetYaxis()->SetBinLabel(j+1,Var[j].c_str());
    CorrelationHistogramm->SetBinContent(i+1,j+1,1);
    
    CorrelationProbabilityHistogramm->GetXaxis()->SetBinLabel(i+1,Var[i].c_str());
    CorrelationProbabilityHistogramm->GetYaxis()->SetBinLabel(j+1,Var[j].c_str());
    CorrelationProbabilityHistogramm->SetBinContent(i+1,j+1,1);
    
    DoubleRatioHistogramm->GetXaxis()->SetBinLabel(i+1,Var[i].c_str());
    DoubleRatioHistogramm->GetYaxis()->SetBinLabel(j+1,Var[j].c_str());
    
    KolmogorovHistogramm->GetXaxis()->SetBinLabel(i+1,Var[i].c_str());
    KolmogorovHistogramm->GetYaxis()->SetBinLabel(j+1,Var[j].c_str());
    KolmogorovProductHistogramm->GetXaxis()->SetBinLabel(i+1,Var[i].c_str());
    KolmogorovProductHistogramm->GetYaxis()->SetBinLabel(j+1,Var[j].c_str());
   }
   
   else{
    /** Construction of the general cut*/
    TString G_Cut_Agg;
    
    for(int k=0;k<Var.size();k++)
    {
     if(k==i) continue;
     if(k==j) continue;
     
     G_Cut_Agg = G_Cut_Agg + Form("%s %s %f && ",Var[k].c_str(),vMinMaj[k].c_str(),Cut[k]);
     
    } 
    TString Complete_Cut = G_Cut_Agg + Form("( %s )",G_Cut.c_str());
    
    /** Statistic tests for the research of independent variables for both the antisymmetric configuration
    of the ABCD space obtained through the exchange between x and y variables.*/ 
    
    FileName = "output/Histogram-Cut/"+Var[i]+"_"+Var[j]+".root"; 
    Analysis.SetVarX(Var[i]);
    Analysis.SetVarY(Var[j]);
    Analysis.SetGeneralCut(G_Cut);
    Analysis.SetCutX(Cut[i]);
    Analysis.SetCutY(Cut[j]);
    Analysis.SetGeneralCut(Complete_Cut);
    Analysis.CreateHisto();
    
    KolmogorovFactor1 = Analysis.KolmogorovTest(Var[i]);
    
    Correlation = Analysis.GetCorrelation();
    CorrelationProbability = Analysis.GetCorrelationProbability();
    DoubleRatio = Analysis.GetDoubleRatio('B','A','D','C');
    
    
    Analysis.Save(FileName);  
    
    CorrelationHistogramm -> SetBinContent(i+1,j+1,Correlation);
    CorrelationProbabilityHistogramm -> SetBinContent(i+1,j+1,CorrelationProbability);
    if(DoubleRatio!=-2 && DoubleRatio!=-1) {
     DoubleRatioHistogramm -> SetBinContent(i+1,j+1,fabs(DoubleRatio));
    }
        
    KolmogorovHistogramm -> SetBinContent(i+1,j+1,KolmogorovFactor1);
    

    FileName="output/Histogram-Cut/"+Var[j]+"_"+Var[i]+".root";
    Analysis.SetVarX(Var[j]);
    Analysis.SetVarY(Var[i]);
    Analysis.SetGeneralCut(Complete_Cut);
    
    Analysis.SetCutX(Cut[j]);
    Analysis.SetCutY(Cut[i]);
    
    Analysis.CreateHisto();
    
    KolmogorovFactor2 = Analysis.KolmogorovTest(Var[j]);
    
    Correlation = Analysis.GetCorrelation();
    CorrelationProbability = Analysis.GetCorrelationProbability();
    
    DoubleRatio = Analysis.GetDoubleRatio('B','A','D','C');
    
    Analysis.Save(FileName);  
    
    CorrelationHistogramm->SetBinContent(j+1,i+1,Correlation);
    CorrelationProbabilityHistogramm->SetBinContent(j+1,i+1,CorrelationProbability);
    
    if(DoubleRatio!=-2 && DoubleRatio!=-1) {
     DoubleRatioHistogramm->SetBinContent(j+1,i+1,fabs(DoubleRatio));
    }
    //else{DoubleRatioHistogramm->SetBinContent(j+1,i+1,2);}
    
    KolmogorovHistogramm->SetBinContent(j+1,i+1,KolmogorovFactor2);
    
    KolmogorovProductHistogramm->SetBinContent(i+1,j+1,KolmogorovFactor2*KolmogorovFactor1);
    KolmogorovProductHistogramm->SetBinContent(j+1,i+1,KolmogorovFactor2*KolmogorovFactor1);
    
    
    CorrelationHistogramm->GetXaxis()->SetBinLabel(i+1,Var[i].c_str());
    CorrelationHistogramm->GetYaxis()->SetBinLabel(j+1,Var[j].c_str());
    CorrelationProbabilityHistogramm->GetXaxis()->SetBinLabel(i+1,Var[i].c_str());
    CorrelationProbabilityHistogramm->GetYaxis()->SetBinLabel(j+1,Var[j].c_str());
    DoubleRatioHistogramm->GetXaxis()->SetBinLabel(i+1,Var[i].c_str());
    DoubleRatioHistogramm->GetYaxis()->SetBinLabel(j+1,Var[j].c_str());
    KolmogorovHistogramm->GetXaxis()->SetBinLabel(i+1,Var[i].c_str());
    KolmogorovHistogramm->GetYaxis()->SetBinLabel(j+1,Var[j].c_str());
    KolmogorovProductHistogramm->GetXaxis()->SetBinLabel(i+1,Var[i].c_str());
    KolmogorovProductHistogramm->GetYaxis()->SetBinLabel(j+1,Var[j].c_str());
    
   }   
  }
 }  
 
 /** Save results in the output file*/
 outFile.cd();
 
 CorrelationHistogramm->Write();
 
 CorrelationProbabilityHistogramm->Write();
 
 DoubleRatioHistogramm->Write();
 
 KolmogorovHistogramm->Write();
 
 KolmogorovProductHistogramm->Write();
 
 outFile.Print();
 outFile.Write();
 
 end = clock();
 std::cout <<"Time = " <<  ((double) (end - start)) << " (a.u.)" << std::endl;
 
 return(0); 
}

