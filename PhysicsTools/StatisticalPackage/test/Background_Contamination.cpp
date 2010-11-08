/** Programm used for the application of the ABCD analysis for an independent pair chosen
at a fixed level of L for signal and all its related background.
This is useful to evaluate the background contamination in the controlled regions
in order to establish the right pair for a correct application of the ABCD method.*/

#include "treeReader.h"
#include "hFactory.h"
#include "hFunctions.h"
#include "stdHisto.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "TPaveStats.h"
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
#include "ABCD.h"
#include "TSystem.h"
#include "TLegend.h"
#include "TLine.h"
#include "TPaveText.h"

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#endif

int main(int argc, char** argv)
{ 
 
 ///=========================
 gROOT->Reset();
 gROOT->SetStyle("Plain");
 gStyle->SetPalette(1);
 ///=========================
 
  /** control of the initial topics*/
  if(argc != 2)
  {
    std::cerr << ">>>>> analysis.cpp::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  /** acquisition of the information contained in the cfg file*/
  
  parseConfigFile (argv[1]) ;
  
  std::string treeName  = gConfigParser -> readStringOption("Input::treeName");
  std::string treeName2 = gConfigParser -> readStringOption("Input::treeName2");
  std::cout << ">>>>> input::treeName  " << treeName  << std::endl; 
  std::cout << ">>>>> input::treeName2  " << treeName  << std::endl;

  
//   int entryMAX = gConfigParser -> readIntOption("Input::entryMAX");
//   int entryMIN = gConfigParser -> readIntOption("Input::entryMIN");
//   int entryMOD = gConfigParser -> readIntOption("Input::entryMOD");
//   int nbinX= gConfigParser -> readIntOption("Input::nbinX");
//   int nbinY= gConfigParser -> readIntOption("Input::nbinY");
//   std::cout << ">>>>> input::entryMIN  " << entryMIN  << std::endl;  
//   std::cout << ">>>>> input::entryMAX  " << entryMAX  << std::endl;  
//   std::cout << ">>>>> input::entryMOD  " << entryMOD  << std::endl;  

  int nbinX =    gConfigParser -> readIntOption   ("Settings::nbinX");
  double X_MIN = gConfigParser -> readDoubleOption("Settings::XMIN");
  double X_MAX = gConfigParser -> readDoubleOption("Settings::XMAX");
  
  int nbinY =    gConfigParser -> readIntOption   ("Settings::nbinY");
  double Y_MIN = gConfigParser -> readDoubleOption("Settings::YMIN");
  double Y_MAX = gConfigParser -> readDoubleOption("Settings::YMAX");
  
  

  
  /** read the output files*/
  
  std::string OutFileName    = gConfigParser -> readStringOption("Output::outFileName");
  std::cout << ">>>>> Output::outFileName  " << OutFileName  << std::endl;

  
  
  double start, end;
  start = clock();

  /** read the pair on which apply the ABCD analysis and the cuts on both variables.*/ 

  std::string inputVariable    = gConfigParser -> readStringOption("Input::inputVariable");
  std::ifstream Variable(inputVariable.c_str());
  std::string buffer;
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

  std::cout<<"<<<<<<<<<<<<<>"<<VariableX<<"<<<<<<<<<<<<>"<<VariableY<<std::endl;
  
  
  /** acquisition of variables and their selection contained in a txt file.
  Leave spaces between fields!
  */
  
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
  
  
 
 /** acquisition of the trees for signal and backgrounds*/

 std::vector<TTree*> Signal_Background;
 std::vector<TTree*> TreeEffVect;
 std::vector<std::string> NameSample;
 std::vector<std::string> NameObject;
 std::vector<int> Color;
 std::string NameFile;
 std::string Obj;
 int col;

 std::string inputFile    = gConfigParser -> readStringOption("Input::inputFile");
 std::ifstream File(inputFile.c_str());

 int TotalBackground = 0;
 TFile* file_vect[1000];
 
 double preselection_efficiency;
 double XSection;
 int numEntriesBefore;
 
 while(!File.eof())
 {
   getline(File,buffer);
   if (buffer != "")
   {
     if (buffer.at(0) != '#')
     {
       std::stringstream line( buffer );      
       line >>NameFile;
       NameSample.push_back(NameFile);
       std::cout<<"<<<<<<<<<"<<NameSample[TotalBackground]<<std::endl;
       
       line>>Obj;
       NameObject.push_back(Obj);
   
       line>>col;
       Color.push_back(col);
       
       file_vect[TotalBackground]= new TFile(NameFile.c_str(), "READ");
       TreeEffVect.push_back((TTree*) file_vect[TotalBackground]->Get("outTreeSelections"));   

       TreeEffVect[TotalBackground]->SetBranchAddress("preselection_efficiency",&preselection_efficiency);
       TreeEffVect[TotalBackground]->SetBranchAddress("numEntriesBefore",&numEntriesBefore);
       TreeEffVect[TotalBackground]->SetBranchAddress("XSection",&XSection);
       
       Signal_Background.push_back((TTree*) file_vect[TotalBackground]->Get("outTreeJetLep"));

       TotalBackground++;
     }
   }

 }


 gSystem->Exec("mkdir output");
 gSystem->Exec("mkdir output/background/");


/** ABCD anlysis on signal and Backgrounds*/ 
 
 TFile* outFileTemp=new TFile("outFileTemp_____.root","RECREATE");
 TString Variable_Cut;
 TString Complete_Cut;
 
 double lum = gConfigParser -> readDoubleOption("Input::luminosity");
 
 std::vector<TH1D*> Histogram;

 TString nameStack = Form ("%s %s distribution",VariableX.c_str(),VariableY.c_str());
 THStack* Distribution=new THStack("Distribution",nameStack);
 
 THStack* hsKolmogorov_pAB = new THStack("hsKolmogorov_pAB","AB");
 THStack* hsKolmogorov_pCD = new THStack("hsKolmogorov_pCD","CD");
 THStack* hsKolmogorov_pAC = new THStack("hsKolmogorov_pAC","AC");
 THStack* hsKolmogorov_pBD = new THStack("hsKolmogorov_pBD","BD");
 
 double ttA=0,ttB=0,ttC=0,ttD=0;
 double BackgroundA=0,BackgroundB=0,BackgroundC=0,BackgroundD=0;
 std::string A="A";
 std::string B="B";
 std::string C="C";
 std::string D="D";

 
 for(int i=0;i<Signal_Background.size();i++)
 { 
   outFileTemp->cd();
  
   if(i==0)
   {  
     /** construction of the general cut. This is done only one times because it's equal for all samples*/
   
    for(int j=0;j<Var.size();j++)
    {
     if(Var[j]==VariableX) continue;
     if(Var[j]==VariableY) continue;
    
    Variable_Cut = Variable_Cut + Form("%s %s %f && ",Var[j].c_str(),vMinMaj[j].c_str(),Cut[j]);
    } 
    
    Complete_Cut = Variable_Cut + Form ("( %s )",G_Cut.c_str());

   }
   /** ABCD analysis for a given sample*/
   ABCD Analysis(Signal_Background.at(i));
   Analysis.SetVarX(VariableX);
   Analysis.SetVarY(VariableY);
   Analysis.SetCutX(CutX);
   Analysis.SetCutY(CutY);
   Analysis.SetGeneralCut(Complete_Cut);
   ///==== All samples with the same bin/bin width so that a meaningful (and readable) Stack can be create
   Analysis.CreateHisto(nbinX,X_MIN,X_MAX,nbinY,Y_MIN,Y_MAX);
//    Analysis.CreateHisto();

   
   double EndA = Analysis.GetEntries('A');
   double EndB = Analysis.GetEntries('B');
   double EndC = Analysis.GetEntries('C');
   double EndD = Analysis.GetEntries('D');

   TreeEffVect[i]->GetEntry(0);
   
   /** Events in each region expected at a fixed integrated luminosity*/
   
   double True_EventsA = lum * XSection * preselection_efficiency * (EndA/numEntriesBefore);
   double True_EventsB = lum * XSection * preselection_efficiency * (EndB/numEntriesBefore);
   double True_EventsC = lum * XSection * preselection_efficiency * (EndC/numEntriesBefore);
   double True_EventsD = lum * XSection * preselection_efficiency * (EndD/numEntriesBefore);
//    std::cout<<NameSample[i]<<std::endl;
//    std::cout << " " <<std::endl;
//    std::cout << " = " << lum << " * " << XSection << " * " << preselection_efficiency << " / " << numEntriesBefore << " >> " << NameSample[i] << std::endl;
//    std::cout<<"      A      "<<True_EventsA<<"      B     "<<True_EventsB<<"     C      "<<True_EventsC<<"      D     "<<True_EventsD<<std::endl;
//    std::cout<<""<<std::endl;
   
   TString NameHisto=Form("%s",NameObject[i].c_str());
   
   /** Histograms for the events in each region for each samples*/
   TH1D* Event = new TH1D(NameHisto.Data(),"Event",4,0,4);
   
   Event->SetBinContent(1,True_EventsA);
   Event->SetBinContent(2,True_EventsB);
   Event->SetBinContent(3,True_EventsC);
   Event->SetBinContent(4,True_EventsD);
   Event->SetFillColor(Color[i]);
   Event->SetLineColor(Color[i]);

   Event->GetXaxis()->SetBinLabel(1,A.c_str());
   Event->GetXaxis()->SetBinLabel(2,B.c_str());
   Event->GetXaxis()->SetBinLabel(3,C.c_str());
   Event->GetXaxis()->SetBinLabel(4,D.c_str());
   
   Histogram.push_back(Event);
   
   /** Stack for the evaluation of background contamination*/
   Distribution->Add(Event);

   TH1D* Kolmogorov_pAB = (TH1D*) Analysis.GetDistribution('Y',"AB");
   TH1D* Kolmogorov_pCD = (TH1D*) Analysis.GetDistribution('Y',"CD");
   TH1D* Kolmogorov_pAC = (TH1D*) Analysis.GetDistribution('X',"AC");
   TH1D* Kolmogorov_pBD = (TH1D*) Analysis.GetDistribution('X',"BD");
   
   Kolmogorov_pAB->SetName(Form("AB_%s",NameObject[i].c_str()));
   Kolmogorov_pCD->SetName(Form("CD_%s",NameObject[i].c_str()));
   Kolmogorov_pAC->SetName(Form("AC_%s",NameObject[i].c_str()));
   Kolmogorov_pBD->SetName(Form("BD_%s",NameObject[i].c_str()));
   
   Kolmogorov_pAB->Scale(lum * XSection * preselection_efficiency /numEntriesBefore);
   Kolmogorov_pCD->Scale(lum * XSection * preselection_efficiency /numEntriesBefore);
   Kolmogorov_pAC->Scale(lum * XSection * preselection_efficiency /numEntriesBefore);
   Kolmogorov_pBD->Scale(lum * XSection * preselection_efficiency /numEntriesBefore);
   
   Kolmogorov_pAB->SetFillColor(Color[i]);
   Kolmogorov_pAB->SetLineColor(Color[i]);
   Kolmogorov_pCD->SetFillColor(Color[i]);
   Kolmogorov_pCD->SetLineColor(Color[i]);
   Kolmogorov_pAC->SetFillColor(Color[i]);
   Kolmogorov_pAC->SetLineColor(Color[i]);
   Kolmogorov_pBD->SetFillColor(Color[i]);
   Kolmogorov_pBD->SetLineColor(Color[i]);
   
//    std::cout << " bin = " << Kolmogorov_pAB->GetNbinsX() << " " << Kolmogorov_pCD->GetNbinsX() << " " << Kolmogorov_pAC->GetNbinsX() << " " << Kolmogorov_pBD->GetNbinsX() << std::endl;
   hsKolmogorov_pAB->Add(Kolmogorov_pAB);
   hsKolmogorov_pCD->Add(Kolmogorov_pCD);
   hsKolmogorov_pAC->Add(Kolmogorov_pAC);
   hsKolmogorov_pBD->Add(Kolmogorov_pBD);
   
   
   /** save the distribution for x and y variable chosen before and after cuts*/
   Analysis.GetDistributionX();
   Analysis.GetDistributionY();
   Analysis.GetDistributionXCut();
   Analysis.GetDistributionYCut();
   std::string Name = "output/background/" + NameObject[i] + "_" + VariableX + "_" + VariableY + ".root";
   Analysis.SaveDistribution(Name);
   std::string Name2 = "output/background/HistoABCD_" + NameObject[i] + "_" + VariableX + "_" + VariableY + ".root";
   Analysis.Save(Name2);  
   
 
   if(NameObject[i]=="TTbarJets") {
    ttA = True_EventsA;
    ttB = True_EventsB;
    ttC = True_EventsC;
    ttD = True_EventsD;
   }
   else{
    BackgroundA = BackgroundA + True_EventsA;
    BackgroundB = BackgroundB + True_EventsB;
    BackgroundC = BackgroundC + True_EventsC;
    BackgroundD = BackgroundD + True_EventsD;
   }   
 }
 
 /**  save  the obtained results */

 TLegend *leg = new TLegend(0.12, 0.65, 0.4, 0.9);

 TFile output (OutFileName.c_str(),"RECREATE");
 output.cd();

 for(int i=0; i<Histogram.size();i++)
 { 
   Histogram[i]->Write();
   leg->AddEntry(Histogram[i],NameObject[i].c_str());
 }
 
 
 TCanvas* c_AB = new TCanvas("c_AB","c_AB",1);
 hsKolmogorov_pAB->Draw();
 leg->SetFillColor(10);
 leg->Draw();
 gStyle->SetOptStat(0); 
 gPad->SetGrid();
 c_AB->Write();
 
 TCanvas* c_CD = new TCanvas("c_CD","c_CD",1);
 hsKolmogorov_pCD->Draw();
 leg->SetFillColor(10);
 leg->Draw();
 gStyle->SetOptStat(0); 
 gPad->SetGrid();
 c_CD->Write();
 
 TCanvas* c_AC = new TCanvas("c_AC","c_AC",1);
 hsKolmogorov_pAC->Draw();
 leg->SetFillColor(10);
 leg->Draw();
 gStyle->SetOptStat(0); 
 gPad->SetGrid();
 c_AC->Write();
 
 TCanvas* c_BD = new TCanvas("c_BD","c_BD",1);
 hsKolmogorov_pBD->Draw();
 leg->SetFillColor(10);
 leg->Draw();
 gStyle->SetOptStat(0); 
 gPad->SetGrid();
 c_BD->Write();
  
 
 /** Distribution of the relative frequency of the chosen background in the ABCD regions*/
 
 TCanvas* Estimator=new TCanvas("Estimator","",1);
 Estimator->cd();
 TH1D* Efficiency=new TH1D("Efficiency","",4,0,4);

 Efficiency->GetXaxis()->SetBinLabel(1,A.c_str());
 Efficiency->GetXaxis()->SetBinLabel(2,B.c_str());
 Efficiency->GetXaxis()->SetBinLabel(3,C.c_str());
 Efficiency->GetXaxis()->SetBinLabel(4,D.c_str());
 
 Efficiency->SetBinContent(1,ttA/(ttA+BackgroundA));
 Efficiency->SetBinContent(2,ttB/(ttB+BackgroundB));
 Efficiency->SetBinContent(3,ttC/(ttC+BackgroundC));
 Efficiency->SetBinContent(4,ttD/(ttD+BackgroundD));
 
 
 Efficiency->Draw();
 
 Estimator->Write();
 
 /** Results for signal and all background in a single stack*/
 
 TCanvas* cHstack = new TCanvas("cHstack","Background Contamination",1);
 Distribution->Draw(); 
 leg->SetFillColor(10);
 leg->Draw();
 cHstack->Write();

 output.Write();
 
 end = clock();
 std::cout <<"Time = " <<  ((double) (end - start)) << " (a.u.)" << std::endl;
 
 delete outFileTemp;
 
 gSystem->Exec("rm outFileTemp_____.root");
 
 
  return(0);
}