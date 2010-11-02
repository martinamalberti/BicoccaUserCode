//==== parameter include ====
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


//---- std include ----
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>




#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"

#include "TFile.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TH1F.h"

#include "TRandom.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TROOT.h"

#include "TEntryList.h"

#include "TF1.h"


std::string treeNameMC;


void ConvertStdVectDouble(std::vector<double> &vET_data,Double_t * V1, int Num){
 vET_data.clear();
 for (int iN = 0; iN < Num; iN++) {
  vET_data.push_back(V1[iN]);
 }
}



int numEntriesMC;
int numSelectedData;

int numberDATA;

TFile* fileInDATA;
TFile* fileInMC;
TFile* outFile;

TTree* myTreeChi2;
TTree* myTreeChi2_Result;
TTree* myTreeNewChi2_Result;
TTree* myTreeLL_Result;


Double_t ScaleTrue = -0.03;
TCanvas* temp = new TCanvas("temp","temp",20,20);

double MinScan = -0.3;
double MaxScan = 0.3;
double MinScanRange = -0.3;
double MaxScanRange = 0.3;
unsigned int iNoSteps = 1000;
unsigned int iPar_NoBG = 0;

std::string variableName;
int numBINS;
double minBINS;
double maxBINS;

int maxIter = 1000;
Int_t numEvents = 100;
int nIter = 0;

Double_t DELTA_CHI2 = 1.0;
Double_t DELTA_LL = 0.5;

std::vector<double> vET_data;

int NBINTemplate = 100;
double MinTemplate = 0.0;
double MaxTemplate = 100.0;
double Delta = (MaxTemplate - MinTemplate) / NBINTemplate;

TString AdditionalCut;
double minET = 30; ///==== used in generation of sample

int EEEB = 0;
///==== 0 = EE+EB
///==== 1 = EE
///==== 2 = EB


int Data_or_MC;


TF1* fitMin = new TF1("fitMin","pol2");


///====================================================
///================== FUNCTIONS =======================

double LLFunc(const double *xx ){
 const Double_t scale = xx[0];

 ///==== Likelihood ==== 
 fileInMC->cd();
 TTree* MyTreeMC = (TTree*) fileInMC->Get(treeNameMC.c_str());
 TString NameMCGr = Form("grMC_%.5f",scale);
 TString NameMC = Form("hMC_%.5f",scale);
 TH1F* hMC;
// TGraphErrors* tempGrLL;
 if (!gROOT->FindObject(NameMC.Data())){
  hMC = new TH1F(NameMC,NameMC,NBINTemplate,MinTemplate,MaxTemplate);
  hMC->Reset();
  
  MyTreeMC->SetEntryList(0); 
  MyTreeMC->Draw(">> myListMCTot",(AdditionalCut + Form(" && (ET * (1+(%f)))>%f",scale,minET)).Data(),"entrylist");
  TEntryList *mylistMCTot = (TEntryList*)gDirectory->Get("myListMCTot");
  MyTreeMC->SetEntryList(mylistMCTot);
  
  TString DrawMC = Form("(%s * (1+(%f)))>>%s",variableName.c_str(),scale,NameMC.Data());
  std::cerr << " DrawMC = " << DrawMC.Data() << std::endl;
//   std::cerr << " LL CUT = " << (AdditionalCut+Form("&& (ET * (1+(%f)))>%f",scale,minET)).Data() << std::endl;
// MyTreeMC->Draw(DrawMC,(AdditionalCut+Form("&& (ET * (1+(%f)))>%f",scale,minET)).Data());
  MyTreeMC->Draw(DrawMC);
  hMC->Scale(10./numEvents);
  outFile->cd();
  hMC->Write();
//  tempGrLL = new TGraphErrors(buildGEfromH (*hMC));
//  tempGrLL->SetName(NameMCGr);
//  tempGrLL->Write();
 }
 else {
  std::cerr << " LL old " << NameMC.Data() << std::endl;
  hMC = (TH1F*) outFile->Get(NameMC.Data());
//  tempGrLL = (TGraphErrors*) outFile->Get(NameMCGr.Data());
 }
 numberDATA = vET_data.size();
 double result = 1.;
 
 std::cerr << " numberDATA = " << numberDATA << " hMC->GetEntries() = " << hMC->GetEntries() << std::endl;
 for (int iEvt = 0; iEvt < numberDATA; iEvt ++){
  double ET = vET_data.at(iEvt);
  int bin = ( ET - MinTemplate ) / Delta;
  if (bin > 0 && bin <= NBINTemplate){
  std::cerr << " ok here " << std::endl;
//   result *= (tempGrLL->Eval(hMC->GetBinCenter(bin)));
//    std::cerr << " hMC->GetBinContent(" << bin << ") = " << hMC->GetBinContent(bin) << " result = " << result << std::endl;
   if (hMC->GetBinContent(bin) != 0) {
    result -= log(hMC->GetBinContent(bin));
   }
   else {
    result = numberDATA * numEvents;
    std::cerr << " >>>>>>>>>>>>>>>> it's ZERO !!!! " << std::endl; //====== > too verbose!
    return result;
   }
//    if (hMC->GetBinContent(bin) == 0) {
//     std::cerr << " result = " << result << " hMC.GetBinContent(" << bin << ":" << NBINTemplate << ") = " << hMC.GetBinContent(bin) << " scale = " << scale << std::endl;
//    }
  }
 }
//  outFile->cd();
//  hMC.Write();
 
//  if (result != 0) result = -log(result);
//  else {
//   result = numberDATA * numEvents;
//   std::cerr << " it's ZERO !!!! " << std::endl;
//  }
 
 ///==== end Likelihood ====    

 return result;
}







double Chi2F(const double *xx ){
 const Double_t scale = xx[0];
 fileInMC->cd();
 TTree* MyTreeMC = (TTree*) fileInMC->Get(treeNameMC.c_str());
 outFile->cd();
 TString nameDATA = Form("hDATA_%d_%d_%.5f",Data_or_MC,nIter,ScaleTrue);
 TH1F* hDATA = (TH1F*) outFile->Get(nameDATA);
 TString NameMC = Form("hMC_Chi2_%.5f",scale);
 
 //std::cerr << " NameMC = "  << NameMC.Data() << " => " << scale << std::endl;
 
 TH1F* hMC;
 if (!gROOT->FindObject(NameMC.Data())){
  hMC = new TH1F(NameMC,NameMC,numBINS,minBINS,maxBINS);
  hMC->Reset();

//  std::cerr << " AdditionalCut.Data() = " << AdditionalCut.Data() << std::endl;
  
  MyTreeMC->SetEntryList(0); 
  MyTreeMC->Draw(">> myListMCTot",(AdditionalCut + Form(" && (ET * (1+(%f)))>%f",scale,minET)).Data(),"entrylist");
  TEntryList *mylistMCTot = (TEntryList*)gDirectory->Get("myListMCTot");
  MyTreeMC->SetEntryList(mylistMCTot);
  
  TString DrawMC = Form("(%s * (1+(%f)))>>%s",variableName.c_str(),scale,NameMC.Data());
//  std::cerr << " DrawMC = " << DrawMC.Data() << std::endl;
  MyTreeMC->Draw(DrawMC);
//   MyTreeMC->Draw(DrawMC,(AdditionalCut+Form("&& (ET * (1+(%f)))>%f",scale,minET)).Data());
  hMC->Sumw2();
  hMC->Scale(hDATA->GetEffectiveEntries()/hMC->GetEffectiveEntries());
  outFile->cd();
  hMC->Write();
 }
 else {
//   std::cerr << " KM old "  << NameMC.Data() << std::endl;
  hMC = (TH1F*) outFile->Get(NameMC.Data());
 }
 
 outFile->cd();
//  hDATA.Write();
//  hMC.Write();
 
 double result = hMC->KolmogorovTest(hDATA,"M");
// double result = - hMC->KolmogorovTest(hDATA,"X");
//  double result = hMC->Chi2Test(&hDATA,"CHI2/NDF");
//=========> E' QUESTO! ==> double result = hMC->Chi2Test(hDATA,"CHI2");
 //double result = - hMC.Chi2Test(&hDATA,""); ///==== http://root.cern.ch/root/html/TH1.html#TH1:Chi2Test
 return result;
}


double myChi2(TH1F* hMC, TH1F* hDATA){
 double result = 0;
 int numBin = hMC->GetNbinsX();
 for (int iBin=0; iBin<numBin; iBin++){
  double mc = hMC->GetBinContent(iBin+1);
  double data = hDATA->GetBinContent(iBin+1);
  if (mc!=0) result += ((mc - data) * (mc - data) / mc);
 }
 return result;
}


double NewChi2Func(const double *xx ){
 const Double_t scale = xx[0];
 fileInMC->cd();
 TTree* MyTreeMC = (TTree*) fileInMC->Get(treeNameMC.c_str());
 outFile->cd();
 TString nameDATA = Form("hDATA_%d_%d_%.5f",Data_or_MC,nIter,ScaleTrue);
 TH1F* hDATA = (TH1F*) outFile->Get(nameDATA);
 
 TString NameMC = Form("hMC_Chi2_%.5f",scale);
 TH1F* hMC;
 if (!gROOT->FindObject(NameMC.Data())){
  hMC = new TH1F(NameMC,NameMC,numBINS,minBINS,maxBINS);
  hMC->Reset();
  
  MyTreeMC->SetEntryList(0); 
  MyTreeMC->Draw(">> myListMCTot",(AdditionalCut + Form(" && (ET * (1+(%f)))>%f",scale,minET)).Data(),"entrylist");
  TEntryList *mylistMCTot = (TEntryList*)gDirectory->Get("myListMCTot");
  MyTreeMC->SetEntryList(mylistMCTot);
  
  TString DrawMC = Form("(%s * (1+(%f)))>>%s",variableName.c_str(),scale,NameMC.Data());
  MyTreeMC->Draw(DrawMC);
//   MyTreeMC->Draw(DrawMC,(AdditionalCut+Form("&& (ET * (1+(%f)))>%f",scale,minET)).Data());
  hMC->Sumw2();
  hMC->Scale(hDATA->GetEffectiveEntries()/hMC->GetEffectiveEntries());
  outFile->cd();
  hMC->Write();
 } 
 else {
//   std::cerr << " NewChi2 old " << NameMC.Data() << std::endl;
  hMC = (TH1F*) outFile->Get(NameMC.Data());
 }
 
 double result = myChi2(hMC,hDATA);
 return result;
}

///================== END FUNCTIONS =======================
///========================================================











///=================== FOR MC ===================


///=====================
double a;
double b;
double c;

double pT;
double ET;
double MT;
double EoP;
double eta;

double eleES;
double eleFBrem;
double E5x5;
double p;

ROOT::Math::Minimizer* minuit = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");
ROOT::Math::Functor functorChi2(&Chi2F,1); 
ROOT::Math::Functor functorLL(&LLFunc,1); 
ROOT::Math::Functor functorNewChi2(&NewChi2Func,1); 

double Alpha;
double Chi2;
double NewChi2;
double LL;
 
double AlphaMean;
double AlphaMinus;
double AlphaPlus;
double AlphaMean_Fit;
double AlphaMinus_Fit;
double AlphaPlus_Fit;

///=====================


///**** Chi2 ****
void doMC_Chi2(){	
 TTree* MyTreeMC = (TTree*) fileInMC->Get(treeNameMC.c_str());

 for (nIter = 0; nIter<maxIter; nIter++){
  if (!(nIter%1)) std::cerr << ">>> nIter = " << nIter << " : " << maxIter << std::endl;
  vET_data.clear();
  outFile->cd();
  TString nameDATA = Form("hDATA_%d_%d_%.5f",Data_or_MC,nIter,ScaleTrue);
  TH1F hDATA(nameDATA,nameDATA,numBINS,minBINS,maxBINS);
  
  MyTreeMC->Draw(">> myListMC",(AdditionalCut+Form("&& (ET * (1+(%f)))>%f",ScaleTrue,minET)).Data(),"entrylist");
  TEntryList *myListMC = (TEntryList*)gDirectory->Get("myListMC");
  
  MyTreeMC->SetEntryList(0); 
  TEntryList *listMCHere = new TEntryList("listMCHere","listMCHere");    
  for (int iEvt = 0; iEvt < numSelectedData; iEvt ++){
   listMCHere->Enter(myListMC->GetEntry(gRandom->Uniform(0,myListMC->GetN())));
  }
  
  MyTreeMC->SetEntryList(listMCHere);
  MyTreeMC->Draw(Form("(1+%f) * %s >> %s",ScaleTrue,variableName.c_str(),nameDATA.Data()));
  
  ConvertStdVectDouble(vET_data,MyTreeMC->GetV1(),numSelectedData);
  
  ///===== Chi2 ====
  minuit->SetFunction(functorChi2);
  TGraph * grChi2 = new TGraph(iNoSteps);
  minuit->Scan(iPar_NoBG,iNoSteps,grChi2->GetX(),grChi2->GetY(),MinScan,MaxScan);
  grChi2->Draw("AL");
  outFile->cd();
  minuit->PrintResults();

  const double *outParametersTemp = minuit->X();
  const double *errParametersTemp = minuit->Errors();
  
  double *outParameters = new double;
  double *errParameters = new double;
  outParameters[0] = outParametersTemp[0];
  errParameters[0] = errParametersTemp[0];
  
  double minChi2 = grChi2->Eval(outParameters[0]);

  ///===== end Chi2 ====
    
  ///==== Save the whole shape of LL/Chi2 ====
  for (unsigned int ii=0; ii < iNoSteps; ii++){
   double X_ii = (MaxScan - MinScan) / iNoSteps * ii + MinScan;
   
   Alpha   = X_ii;
   Chi2    = grChi2->Eval(X_ii);
   LL      = 0;
   NewChi2 = 0;
   
   myTreeChi2->Fill();
  }
  

  ///===== Look for minima =====
  double a;
  double b;
  double c;
  
  ///===== Chi2 ====
 std::cerr << " === Chi2 === " << std::endl;
//  std::cerr << "==== min Scan = " << minChi2 << std::endl;
  double errX_low = -9999;
  double errX_up = 9999;
  int err_low = 0;
  int err_up = 0;
  for (unsigned int ii=0; ii < iNoSteps; ii++){
   double X_ii = (MaxScan - MinScan) / iNoSteps * ii + MinScan;
   double here = grChi2->Eval(X_ii);
   if (err_low == 0){
    if (here < (minChi2 + DELTA_CHI2)){
     errX_low = X_ii;
     err_low = 1;
    }
   }
   else if (err_up == 0 && here > (minChi2 + DELTA_CHI2) && X_ii > outParameters[0]){
    errX_up = X_ii; 
    err_up = 1;
   }
  }
  
  AlphaMean = outParameters[0];
  AlphaMinus = errX_low;
  AlphaPlus = errX_up;
 
  grChi2->Fit("fitMin","RMQ");
  c = fitMin->GetParameter(0);
  b = fitMin->GetParameter(1);
  a = fitMin->GetParameter(2);
  AlphaMean_Fit  = -b / (2*a);
  AlphaMinus_Fit = (-b + 2 * sqrt(a)) / (2*a);  ///==== delta Chi2 = 1 
  AlphaPlus_Fit  = (-b - 2 * sqrt(a)) / (2*a);  ///==== delta Chi2 = 1   

  myTreeChi2_Result->Fill();
 
//  delete listMCHere;
  
 }
}


///**** LL ****
void doMC_LL(){
 TTree* MyTreeMC = (TTree*) fileInMC->Get(treeNameMC.c_str());

 for (nIter = 0; nIter<maxIter; nIter++){
  if (!(nIter%1)) std::cerr << ">>> nIter = " << nIter << " : " << maxIter << std::endl;
  vET_data.clear();
  outFile->cd();
  TString nameDATA = Form("hDATA_%d_%d_%.5f",Data_or_MC,nIter,ScaleTrue);
  TH1F hDATA(nameDATA,nameDATA,numBINS,minBINS,maxBINS);
  
  MyTreeMC->Draw(">> myListMC",(AdditionalCut+Form("&& (ET * (1+(%f)))>%f",ScaleTrue,minET)).Data(),"entrylist");
  TEntryList *myListMC = (TEntryList*)gDirectory->Get("myListMC");
  
  MyTreeMC->SetEntryList(0); 
  TEntryList *listMCHere = new TEntryList("listMCHere","listMCHere");    
  for (int iEvt = 0; iEvt < numSelectedData; iEvt ++){
   listMCHere->Enter(myListMC->GetEntry(gRandom->Uniform(0,myListMC->GetN())));
  }
  
  MyTreeMC->SetEntryList(listMCHere);
  MyTreeMC->Draw(Form("(1+%f) * %s >> %s",ScaleTrue,variableName.c_str(),nameDATA.Data()));
  
  ConvertStdVectDouble(vET_data,MyTreeMC->GetV1(),numSelectedData);
  
  ///==== likelihood ====
 std::cerr << " === LL === " << std::endl;
 std::cerr << " === pseudo vET_data.size() = " << vET_data.size() << std::endl;
 
 
  minuit->SetFunction(functorLL); 
  TGraph * grLL_temp = new TGraph(iNoSteps);
  minuit->Scan(iPar_NoBG,iNoSteps,grLL_temp->GetX(),grLL_temp->GetY(),MinScan,MaxScan);
  TGraph * grLL = new TGraph();
  int nPointLL = 0;
 for (unsigned int iStep = 0; iStep < iNoSteps; iStep++){
   double x = MinScan + (MaxScan - MinScan) / iNoSteps * (iStep+0.5);
   double y = LLFunc(&x);
   if (y != numberDATA * numEvents) {
    grLL->SetPoint(nPointLL,x,y);
    nPointLL++;
   }
  }
  grLL->Draw("AL");
  outFile->cd();
  minuit->PrintResults();
  const double *outParametersTemp2 = minuit->X();
  const double *errParametersTemp2 = minuit->Errors();
  
  double *outParametersLL = new double;
  double *errParametersLL = new double;
  outParametersLL[0] = outParametersTemp2[0];
  errParametersLL[0] = errParametersTemp2[0];
  
  std::cerr << " nPointLL = " << nPointLL << std::endl;
  
  double minLL = grLL->Eval(outParametersLL[0]);
  ///==== end likelihood ====
  
    ///==== Save the whole shape of LL/Chi2 ====
  for (unsigned int ii=0; ii < iNoSteps; ii++){
   double X_ii = (MaxScan - MinScan) / iNoSteps * ii + MinScan;
   
   Alpha   = X_ii;
   Chi2    = 0;
   LL      = grLL->Eval(X_ii);
   NewChi2 = 0;
   
   myTreeChi2->Fill();
  }

  ///===== Look for minima =====
  double a;
  double b;
  double c;
  
  
  double errX_low = -9999;
  double errX_up = 9999;
  int err_low = 0;
  int err_up = 0;
  for (unsigned int ii=0; ii < iNoSteps; ii++){
   double X_ii = (MaxScan - MinScan) / iNoSteps * ii + MinScan;
   double here = grLL->Eval(X_ii);
   if (err_low == 0){
    if (here < (minLL + DELTA_LL)){
     errX_low = X_ii;
     err_low = 1;
    }
   }
   else if (err_up == 0 && here > (minLL + DELTA_LL) && X_ii > outParametersLL[0]){
    errX_up = X_ii; 
    err_up = 1;
   }
  }
  
  AlphaMean = outParametersLL[0];
  AlphaMinus = errX_low;
  AlphaPlus = errX_up;
 
  grLL->Fit("fitMin","RMQ");
  c = fitMin->GetParameter(0);
  b = fitMin->GetParameter(1);
  a = fitMin->GetParameter(2);
  AlphaMean_Fit  = -b / (2*a);
  AlphaMinus_Fit = (-b + sqrt(2*a)) / (2*a);  ///==== delta LL = 0.5
  AlphaPlus_Fit  = (-b - sqrt(2*a)) / (2*a);  ///==== delta LL = 0.5   
  
  myTreeLL_Result->Fill();
 
  grLL->Write();
  
  //delete listMCHere;
  
 }
}


///**** NewChi2 ****
void doMC_NewChi2(){
 TTree* MyTreeMC = (TTree*) fileInMC->Get(treeNameMC.c_str());
 for (nIter = 0; nIter<maxIter; nIter++){
  if (!(nIter%1)) std::cerr << ">>> nIter = " << nIter << " : " << maxIter << std::endl;
  vET_data.clear();
  outFile->cd();
  TString nameDATA = Form("hDATA_%d_%d_%.5f",Data_or_MC,nIter,ScaleTrue);
  TH1F hDATA(nameDATA,nameDATA,numBINS,minBINS,maxBINS);
  
  MyTreeMC->Draw(">> myListMC",(AdditionalCut+Form("&& (ET * (1+(%f)))>%f",ScaleTrue,minET)).Data(),"entrylist");
  TEntryList *myListMC = (TEntryList*)gDirectory->Get("myListMC");

  MyTreeMC->SetEntryList(0); 
  TEntryList *listMCHere = new TEntryList("listMCHere","listMCHere");    
  for (int iEvt = 0; iEvt < numSelectedData; iEvt ++){
   listMCHere->Enter(myListMC->GetEntry(gRandom->Uniform(0,myListMC->GetN())));
  }
 
 MyTreeMC->SetEntryList(listMCHere);
 MyTreeMC->Draw(Form("(1+%f) * %s >> %s",ScaleTrue,variableName.c_str(),nameDATA.Data()));
  
 ConvertStdVectDouble(vET_data,MyTreeMC->GetV1(),numSelectedData);
   
  ///==== newChi2 ====
  minuit->SetFunction(functorNewChi2);
  TGraph * grNewChi2 = new TGraph(iNoSteps);
  minuit->Scan(iPar_NoBG,iNoSteps,grNewChi2->GetX(),grNewChi2->GetY(),MinScan,MaxScan);
  grNewChi2->Draw("AL");
  outFile->cd();
  minuit->PrintResults();
  const double *outParametersNewChi2 = minuit->X();
  const double *errParametersNewChi2 = minuit->Errors();
 
  double minNewChi2 = grNewChi2->Eval(outParametersNewChi2[0]);
  ///==== end newChi2 ====
 
  ///==== Save the whole shape of LL/Chi2 ====
  for (unsigned int ii=0; ii < iNoSteps; ii++){
   double X_ii = (MaxScan - MinScan) / iNoSteps * ii + MinScan;
   
   Alpha   = X_ii;
   Chi2    = 0;
   LL      = 0;
   NewChi2 = grNewChi2->Eval(X_ii);
   
   myTreeChi2->Fill();
  }
  

  ///===== Look for minima =====
  double a;
  double b;
  double c;
  
  double errX_low = -9999;
  double errX_up = 9999;
  int err_low = 0;
  int err_up = 0;
  for (unsigned int ii=0; ii < iNoSteps; ii++){
   double X_ii = (MaxScan - MinScan) / iNoSteps * ii + MinScan;
   double here = grNewChi2->Eval(X_ii);
   if (err_low == 0){
    if (here < (minNewChi2 + DELTA_CHI2)){
     errX_low = X_ii;
     err_low = 1;
    }
   }
   else if (err_up == 0 && here > (minNewChi2 + DELTA_CHI2) && X_ii > outParametersNewChi2[0]){
    errX_up = X_ii; 
    err_up = 1;
   }
  }
  
  AlphaMean = outParametersNewChi2[0];
  AlphaMinus = errX_low;
  AlphaPlus = errX_up;
  
  
  grNewChi2->Fit("fitMin","RMQ");
  c = fitMin->GetParameter(0);
  b = fitMin->GetParameter(1);
  a = fitMin->GetParameter(2);
  AlphaMean_Fit  = -b / (2*a);
  AlphaMinus_Fit = (-b + 2 * sqrt(a)) / (2*a);  ///==== delta Chi2 = 1 
  AlphaPlus_Fit  = (-b - 2 * sqrt(a)) / (2*a);  ///==== delta Chi2 = 1   
  
  myTreeNewChi2_Result->Fill();
  
 // delete listMCHere;
 }
 
}




int main(int argc, char** argv){
 
 ///=======================================================================================
 ///==== Calculate Scale from Data (minimization Chi2 and more)
 ///==== then perform toy MC to see the error on THAT scale with THAT statistics available
 ///======================================================================================= 

 if(argc != 2)
 {
  std::cerr << ">>>>> analysis.cpp::usage: " << argv[0] << " configFileName" << std::endl ;
  return 1;
 }
 
 // Parse the config file                                                                                                                                                          
 
 std::string fileName (argv[1]) ;
 boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile(fileName) ;
 boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet () ;

 edm::ParameterSet subPSetInput = parameterSet->getParameter<edm::ParameterSet> ("inputTree") ;
 std::string treeNameDATA  = subPSetInput.getParameter<std::string> ("treeNameDATA") ;
 std::string inputFileDATA = subPSetInput.getParameter<std::string> ("inputFileDATA") ;
 treeNameMC = subPSetInput.getParameter<std::string> ("treeNameMC") ;
 std::string inputFileMC = subPSetInput.getParameter<std::string> ("inputFileMC") ;
  
 std::cout << ">>>>> InputDATA::treeName  " << treeNameDATA  << std::endl;  
 std::cout << ">>>>> InputDATA::inputFile  " << inputFileDATA  << std::endl;  
 std::cout << ">>>>> InputMC::treeName  " << treeNameMC  << std::endl;  
 std::cout << ">>>>> InputMC::inputFile  " << inputFileMC  << std::endl;  


 edm::ParameterSet subPSetOutput = parameterSet->getParameter<edm::ParameterSet> ("outputTree") ;
 std::string outputFile = subPSetOutput.getParameter<std::string> ("outputFile") ;

 std::cout << ">>>>> Output::outputFile  " << outputFile  << std::endl;  


 edm::ParameterSet subPSetOptions = parameterSet->getParameter<edm::ParameterSet> ("options") ;
 MinScan  = subPSetOptions.getParameter<double> ("MinScan") ;
 MaxScan  = subPSetOptions.getParameter<double> ("MaxScan") ;
 iNoSteps = subPSetOptions.getParameter<int>    ("iNoSteps") ;
  
 std::cout << ">>>>> Options::MinScan  " << MinScan  << std::endl;  
 std::cout << ">>>>> Options::MaxScan  " << MaxScan  << std::endl;  
 std::cout << ">>>>> Options::iNoSteps " << iNoSteps << std::endl;  

 MinScanRange  = subPSetOptions.getParameter<double> ("MinScanRange") ;
 MaxScanRange  = subPSetOptions.getParameter<double> ("MaxScanRange") ;
 
 std::cout << ">>>>> Options::MinScanRange  " << MinScanRange  << std::endl;  
 std::cout << ">>>>> Options::MaxScanRange  " << MaxScanRange  << std::endl;  

 minBINS = subPSetOptions.getParameter<double> ("minBINS") ;
 maxBINS = subPSetOptions.getParameter<double> ("maxBINS") ;
 numBINS = subPSetOptions.getParameter<int>    ("numBINS") ;

 std::cout << ">>>>> Options::numBINS   " << numBINS  << std::endl;  
 std::cout << ">>>>> Options::minBINS   " << minBINS  << std::endl;
 std::cout << ">>>>> Options::maxBINS   " << maxBINS  << std::endl;


 NBINTemplate = 10 * numBINS;
 MinTemplate = minBINS;
 MaxTemplate = maxBINS;
 Delta = (MaxTemplate - MinTemplate) / NBINTemplate;



 variableName = subPSetOptions.getParameter<std::string> ("variableName") ;
 std::cout << ">>>>> Options::variableName " << variableName.c_str() << std::endl;
 
 double minET = subPSetOptions.getParameter<double> ("minET") ;
 std::cout << ">>>>> Options::minET " << minET << std::endl;
 
 
 std::string temp_cut = subPSetOptions.getParameter<std::string> ("cut") ;
 std::cout << ">>>>> Options::cut " << temp_cut.c_str() << std::endl;
 AdditionalCut = Form("%s",temp_cut.c_str());
 std::cout << ">>>>> Options::AdditionalCut " << AdditionalCut.Data() << std::endl;
 
 
 EEEB = subPSetOptions.getParameter<int> ("EEorEB");
 std::cout << ">>>>> Options::EEEB " << EEEB << std::endl;
///==== 0 = EE+EB
///==== 1 = EE
///==== 2 = EB
///==== 3 = EE+
///==== 4 = EE-

 if (EEEB == 1) { ///==== EE
  AdditionalCut = Form("%s && (eta > 1.5 || eta < -1.5)",AdditionalCut.Data());
 }

 if (EEEB == 2) { ///==== EB
  AdditionalCut = Form("%s && (eta < 1.5 && eta > -1.5)",AdditionalCut.Data());
 }
 if (EEEB == 3) { ///==== EE+
  AdditionalCut = Form("%s && (eta > 1.5)",AdditionalCut.Data());
 }
 if (EEEB == 4) { ///==== EE-
  AdditionalCut = Form("%s && (eta < -1.5)",AdditionalCut.Data());
 }
 if (EEEB == 5) { ///==== EB mod 1
  AdditionalCut = Form("%s && (abs(eta) < 0.435)",AdditionalCut.Data());
 }
 if (EEEB == 6) { ///==== EB mod 2
  AdditionalCut = Form("%s && (abs(eta) < 0.783 && abs(eta) > 0.435)",AdditionalCut.Data());
 }
 if (EEEB == 7) { ///==== EB mod 3
  AdditionalCut = Form("%s && (abs(eta) < 1.131 && abs(eta) > 0.783)",AdditionalCut.Data());
 }
 if (EEEB == 8) { ///==== EB mod 4
  AdditionalCut = Form("%s && (abs(eta) < 1.479 && abs(eta) > 1.131)",AdditionalCut.Data());
 }
 if (EEEB == 9) { ///==== EE No ES
  AdditionalCut = Form("%s && (abs(eta) > 2.5)",AdditionalCut.Data());
 }
 if (EEEB == 10) { ///==== EE + ES
 AdditionalCut = Form("%s && (abs(eta) < 2.5 && abs(eta) > 1.5)",AdditionalCut.Data());
 }
 if (EEEB == 11) { ///==== EE- + ES
  AdditionalCut = Form("%s && (eta > -2.5 && eta < -1.5)",AdditionalCut.Data());
 }
 if (EEEB == 12) { ///==== EE+ + ES
  AdditionalCut = Form("%s && (eta < 2.5 && eta > 1.5)",AdditionalCut.Data());
 }
 
 std::cout << ">>>>>        :: " << AdditionalCut.Data() << std::endl;

 maxIter = subPSetOptions.getParameter<int> ("numToyMC") ;
 std::cout << ">>>>> Options::numToyMC  " << maxIter  << std::endl;  

 ///==== DATA ====
 fileInDATA = new TFile(inputFileDATA.c_str(),"READ");
 
 ///==== W ====
 fileInMC = new TFile(inputFileMC.c_str(),"READ");

 ///==== output ====
 outFile = new TFile(outputFile.c_str(),"RECREATE");
 outFile->cd();
 

 double ScaleTrue_Chi2;
 double ScaleTrue_Chi2_Fit;
 double ScaleTrue_LL;
 double ScaleTrue_LL_Fit;
 double ScaleTrue_NewChi2;
 double ScaleTrue_NewChi2_Fit;


 ///==== Prepare output trees ====

 myTreeChi2 = new TTree("myTreeChi2","myTreeChi2");
 myTreeChi2->Branch("Data_or_MC",&Data_or_MC,"Data_or_MC/I");
 myTreeChi2->Branch("Alpha",&Alpha,"Alpha/D");
 myTreeChi2->Branch("Chi2",&Chi2,"Chi2/D");
 myTreeChi2->Branch("NewChi2",&NewChi2,"NewChi2/D");
 myTreeChi2->Branch("LL",&LL,"LL/D");
 myTreeChi2->Branch("ScaleTrue",&ScaleTrue,"ScaleTrue/D");

 myTreeChi2_Result = new TTree("myTreeChi2_Result","myTreeChi2_Result");

 myTreeChi2_Result->Branch("Data_or_MC",&Data_or_MC,"Data_or_MC/I");
 myTreeChi2_Result->Branch("AlphaMean",&AlphaMean,"AlphaMean/D");
 myTreeChi2_Result->Branch("AlphaMinus",&AlphaMinus,"AlphaMinus/D");
 myTreeChi2_Result->Branch("AlphaPlus",&AlphaPlus,"AlphaPlus/D");
 myTreeChi2_Result->Branch("AlphaMean_Fit",&AlphaMean_Fit,"AlphaMean_Fit/D");
 myTreeChi2_Result->Branch("AlphaMinus_Fit",&AlphaMinus_Fit,"AlphaMinus_Fit/D");
 myTreeChi2_Result->Branch("AlphaPlus_Fit",&AlphaPlus_Fit,"AlphaPlus_Fit/D");
 myTreeChi2_Result->Branch("ScaleTrue",&ScaleTrue,"ScaleTrue/D");

 myTreeLL_Result = new TTree("myTreeLL_Result","myTreeLL_Result");
 myTreeLL_Result->Branch("Data_or_MC",&Data_or_MC,"Data_or_MC/I");
 myTreeLL_Result->Branch("AlphaMean",&AlphaMean,"AlphaMean/D");
 myTreeLL_Result->Branch("AlphaMinus",&AlphaMinus,"AlphaMinus/D");
 myTreeLL_Result->Branch("AlphaPlus",&AlphaPlus,"AlphaPlus/D");
 myTreeLL_Result->Branch("AlphaMean_Fit",&AlphaMean_Fit,"AlphaMean_Fit/D");
 myTreeLL_Result->Branch("AlphaMinus_Fit",&AlphaMinus_Fit,"AlphaMinus_Fit/D");
 myTreeLL_Result->Branch("AlphaPlus_Fit",&AlphaPlus_Fit,"AlphaPlus_Fit/D");
 myTreeLL_Result->Branch("ScaleTrue",&ScaleTrue,"ScaleTrue/D");

 myTreeNewChi2_Result = new TTree("myTreeNewChi2_Result","myTreeNewChi2_Result");
 myTreeNewChi2_Result->Branch("Data_or_MC",&Data_or_MC,"Data_or_MC/I");
 myTreeNewChi2_Result->Branch("AlphaMean",&AlphaMean,"AlphaMean/D");
 myTreeNewChi2_Result->Branch("AlphaMinus",&AlphaMinus,"AlphaMinus/D");
 myTreeNewChi2_Result->Branch("AlphaPlus",&AlphaPlus,"AlphaPlus/D");
 myTreeNewChi2_Result->Branch("AlphaMean_Fit",&AlphaMean_Fit,"AlphaMean_Fit/D");
 myTreeNewChi2_Result->Branch("AlphaMinus_Fit",&AlphaMinus_Fit,"AlphaMinus_Fit/D");
 myTreeNewChi2_Result->Branch("AlphaPlus_Fit",&AlphaPlus_Fit,"AlphaPlus_Fit/D");
 myTreeNewChi2_Result->Branch("ScaleTrue",&ScaleTrue,"ScaleTrue/D");


 ///==== Prepare input trees ====
 TTree* MyTreeDATA = (TTree*) fileInDATA->Get(treeNameDATA.c_str());

 MyTreeDATA->SetBranchAddress("pT",&pT);
 MyTreeDATA->SetBranchAddress("ET",&ET);
 MyTreeDATA->SetBranchAddress("MT",&MT);
 MyTreeDATA->SetBranchAddress("EoP",&EoP);
 MyTreeDATA->SetBranchAddress("eta",&eta);

 MyTreeDATA->SetBranchAddress("E5x5",&E5x5);
 MyTreeDATA->SetBranchAddress("p",&p);
 MyTreeDATA->SetBranchAddress("eleES",&eleES);
 MyTreeDATA->SetBranchAddress("eleFBrem",&eleFBrem);
 
 TTree* MyTreeMC = (TTree*) fileInMC->Get(treeNameMC.c_str());
 MyTreeMC->SetBranchAddress("pT",&pT);
 MyTreeMC->SetBranchAddress("ET",&ET);
 MyTreeMC->SetBranchAddress("MT",&MT);
 MyTreeMC->SetBranchAddress("EoP",&EoP);
 MyTreeMC->SetBranchAddress("eta",&eta);
 
 MyTreeMC->SetBranchAddress("E5x5",&E5x5);
 MyTreeMC->SetBranchAddress("p",&p);
 MyTreeMC->SetBranchAddress("eleES",&eleES);
 MyTreeMC->SetBranchAddress("eleFBrem",&eleFBrem);
 
 numEntriesMC = MyTreeMC->GetEntries();
 
 ///==== prepare minuit ====

 fitMin->SetRange(MinScanRange,MaxScanRange); 
 
 double step[1] = {0.001};
 double variable[1] = {0.0};
 minuit->SetLimitedVariable(0,"Scale" , variable[0]  , step[0] , MinScan  , MaxScan );
 

///===========================
///==== DATA Scale search ====
 ScaleTrue = -1000; ///==== default
 Data_or_MC = 1; ///=== 1 = Data;  0 = MC;
 numEvents = MyTreeDATA->GetEntries(); //==== number of events in Data sample
 outFile->cd();
 vET_data.clear();
 nIter = 1000000000; ///==== less than 1000000000 iterations at the end !!!
 TString nameDATA = Form("hDATA_%d_%d_%.5f",Data_or_MC,nIter,ScaleTrue);
 TH1F hDATA(nameDATA,nameDATA,numBINS,minBINS,maxBINS);
 
 MyTreeDATA->Draw(">> myList",(AdditionalCut + Form(" && ET > %f",minET)).Data(),"entrylist");
 TEntryList *mylist = (TEntryList*)gDirectory->Get("myList");
 MyTreeDATA->SetEntryList(mylist);
 
 MyTreeDATA->Draw(Form("%s >> %s",variableName.c_str(),nameDATA.Data()));
 ConvertStdVectDouble(vET_data,MyTreeDATA->GetV1(),mylist->GetN());
 
 hDATA.Write();
  
 std::cerr << "... I'm minimizing ... DATA analysis" << std::endl;
 std::cerr << ">>>>>>> numEvents = " << numEvents << " => " << vET_data.size() << " selected (=" << mylist->GetN() << ")" << std::endl;
 numSelectedData = vET_data.size();
 
 
 ///===== Chi2 ====
 std::cerr << " === Chi2 === " << std::endl;
 minuit->SetFunction(functorChi2);

 TGraph * grChi2 = new TGraph(iNoSteps);
 minuit->Scan(iPar_NoBG,iNoSteps,grChi2->GetX(),grChi2->GetY(),MinScan,MaxScan);

// TGraph * grChi2 = new TGraph();
// for (int iStep = 0; iStep < iNoSteps; iStep++){
//  double x = MinScan + (MaxScan - MinScan) / iNoSteps * (iStep+0.5);
//  double y = Chi2F(&x);
//  grChi2->SetPoint(iStep+1,x,y);
// }
 grChi2->Draw("AL");
 outFile->cd();
 minuit->PrintResults();
 outFile->cd();
 grChi2->SetTitle("grChi2");
 grChi2->Write();
 const double *outParametersTemp = minuit->X();
 const double *errParametersTemp = minuit->Errors();
  
 double *outParameters = new double;
 double *errParameters = new double;
 outParameters[0] = outParametersTemp[0];
 errParameters[0] = errParametersTemp[0];
   
 double minChi2 = grChi2->Eval(outParameters[0]);
 std::cerr << " numEvents = " << numEvents << " Scale = " << outParameters[0] << " +/- " << errParameters[0] << std::endl;
 ///===== end Chi2 ====
  
 ///==== likelihood ====
 std::cerr << " === LL === " << std::endl;
 minuit->SetFunction(functorLL);

 TGraph * grLL_temp = new TGraph(iNoSteps);
 minuit->Scan(iPar_NoBG,iNoSteps,grLL_temp->GetX(),grLL_temp->GetY(),MinScan,MaxScan);
 TGraph * grLL = new TGraph();
 grLL->SetName("grLL");
 int nPointLL = 0;
 for (unsigned int iStep = 0; iStep < iNoSteps; iStep++){
  double x = MinScan + (MaxScan - MinScan) / iNoSteps * (iStep+0.5);
  double y = LLFunc(&x);
//  std::cerr << " y = " << y << std::endl;
  if (y != numberDATA * numEvents) {
   std::cerr << " Ok y = " << y << std::endl;
   grLL->SetPoint(nPointLL,x,y);
   nPointLL++;
  }
 }
 
 
 std::cerr << " finito " << std::endl;
 
 grLL->Draw("AL");
 outFile->cd();
 minuit->PrintResults();
 outFile->cd();
 grLL->SetTitle("grLL");
 grLL->Write();
 
 std::cerr << " done " << std::endl;

 const double *outParametersTemp2 = minuit->X();
 const double *errParametersTemp2 = minuit->Errors();

 std::cerr << " done 2 " << std::endl;
  
 double *outParametersLL = new double;
 double *errParametersLL = new double;
 outParametersLL[0] = outParametersTemp2[0];
 errParametersLL[0] = errParametersTemp2[0];
 
 double minLL = grLL->Eval(outParametersLL[0]);
 std::cerr << " numEvents = " << numEvents << " Scale = " << outParametersLL[0] << " +/- " << errParametersLL[0] << std::endl;
 ///==== end likelihood ====
 
 
 ///==== newChi2 ====
 std::cerr << " === newChi2 === " << std::endl;
 minuit->SetFunction(functorNewChi2);
 TGraph * grNewChi2 = new TGraph(iNoSteps);
 minuit->Scan(iPar_NoBG,iNoSteps,grNewChi2->GetX(),grNewChi2->GetY(),MinScan,MaxScan);
 grNewChi2->Draw("AL");
 outFile->cd();
 minuit->PrintResults();
 outFile->cd();
 grNewChi2->SetTitle("grNewChi2");
 grNewChi2->Write();
 const double *outParametersNewChi2 = minuit->X();
 const double *errParametersNewChi2 = minuit->Errors();
 double minNewChi2 = grNewChi2->Eval(outParametersNewChi2[0]);
 std::cerr << " numEvents = " << numEvents << " Scale = " << outParametersNewChi2[0] << " +/- " << errParametersNewChi2[0] << std::endl;
 ///==== end newChi2 ====
  
 
 
 std::cerr << "... Minimized with all methods ..." << std::endl;

 ///==== Save the whole shape of LL/Chi2 ====
 for (unsigned int ii=0; ii < iNoSteps; ii++){
  double X_ii = (MaxScan - MinScan) / iNoSteps * ii + MinScan;
  
  Alpha   = X_ii;
  Chi2    = grChi2->Eval(X_ii);
  LL      = grLL->Eval(X_ii);
  NewChi2 = grNewChi2->Eval(X_ii);
  myTreeChi2->Fill();
 }
  
 ///===== Look for minima =====
  
 ///===== Chi2 ====
 std::cerr << " === Chi2 === " << std::endl;
 std::cerr << "==== min Scan = " << minChi2 << std::endl;
 double errX_low = -9999;
 double errX_up = 9999;
 int err_low = 0;
 int err_up = 0;
 for (unsigned int ii=0; ii < iNoSteps; ii++){
  double X_ii = (MaxScan - MinScan) / iNoSteps * ii + MinScan;
  double here = grChi2->Eval(X_ii);
  if (err_low == 0){
   if (here < (minChi2 + DELTA_CHI2)){
    errX_low = X_ii;
    err_low = 1;
   }
  }
  else if (err_up == 0 && here > (minChi2 + DELTA_CHI2) && X_ii > outParameters[0]){
   errX_up = X_ii; 
   err_up = 1;
  }
 }
 
 AlphaMean = outParameters[0];
 AlphaMinus = errX_low;
 AlphaPlus = errX_up;

 grChi2->Fit("fitMin","RMQ");
 c = fitMin->GetParameter(0);
 b = fitMin->GetParameter(1);
 a = fitMin->GetParameter(2);
 AlphaMean_Fit  = -b / (2*a);
 AlphaMinus_Fit = (-b + 2 * sqrt(a)) / (2*a);  ///==== delta Chi2 = 1 
 AlphaPlus_Fit  = (-b - 2 * sqrt(a)) / (2*a);  ///==== delta Chi2 = 1   
 myTreeChi2_Result->Fill();
  
 ScaleTrue_Chi2 = AlphaMean;
 ScaleTrue_Chi2_Fit = AlphaMean_Fit;

 ///===== LogLikelihood ====
 std::cerr << " === LL === " << std::endl;
 std::cerr << "==== min Scan = " << minLL << std::endl;
 errX_low = -9999;
 errX_up = 9999;
 err_low = 0;
 err_up = 0;
 for (unsigned int ii=0; ii < iNoSteps; ii++){
  double X_ii = (MaxScan - MinScan) / iNoSteps * ii + MinScan;
  double here = grLL->Eval(X_ii);
  if (err_low == 0){
   if (here < (minLL + DELTA_LL)){
    errX_low = X_ii;
    err_low = 1;
   }
  }
  else if (err_up == 0 && here > (minLL + DELTA_LL) && X_ii > outParametersLL[0]){
   errX_up = X_ii; 
   err_up = 1;
  }
 }
 
 AlphaMean = outParametersLL[0];
 AlphaMinus = errX_low;
 AlphaPlus = errX_up;

 grLL->Fit("fitMin","RMQ");
 c = fitMin->GetParameter(0);
 b = fitMin->GetParameter(1);
 a = fitMin->GetParameter(2);
 AlphaMean_Fit  = -b / (2*a);
 AlphaMinus_Fit = (-b + sqrt(2*a)) / (2*a);  ///==== delta LL = 0.5
 AlphaPlus_Fit  = (-b - sqrt(2*a)) / (2*a);  ///==== delta LL = 0.5   
 
 myTreeLL_Result->Fill();
 
 ScaleTrue_LL = AlphaMean;
 ScaleTrue_LL_Fit = AlphaMean_Fit;

 
 ///===== NewChi2 ====
 std::cerr << " === NewChi2 === " << std::endl;
 std::cerr << "==== min Scan = " << minNewChi2 << std::endl;
 errX_low = -9999;
 errX_up = 9999;
 err_low = 0;
 err_up = 0;
 for (unsigned int ii=0; ii < iNoSteps; ii++){
  double X_ii = (MaxScan - MinScan) / iNoSteps * ii + MinScan;
  double here = grNewChi2->Eval(X_ii);
  if (err_low == 0){
   if (here < (minNewChi2 + DELTA_CHI2)){
    errX_low = X_ii;
    err_low = 1;
   }
  }
  else if (err_up == 0 && here > (minNewChi2 + DELTA_CHI2) && X_ii > outParametersNewChi2[0]){
   errX_up = X_ii; 
   err_up = 1;
  }
 }
  
 AlphaMean = outParametersNewChi2[0];
 AlphaMinus = errX_low;
 AlphaPlus = errX_up;
 
 
 grNewChi2->Fit("fitMin","RMQ");
 c = fitMin->GetParameter(0);
 b = fitMin->GetParameter(1);
 a = fitMin->GetParameter(2);
 AlphaMean_Fit  = -b / (2*a);
 AlphaMinus_Fit = (-b + 2 * sqrt(a)) / (2*a);  ///==== delta Chi2 = 1 
 AlphaPlus_Fit  = (-b - 2 * sqrt(a)) / (2*a);  ///==== delta Chi2 = 1   
 
 myTreeNewChi2_Result->Fill();
  
 ScaleTrue_NewChi2 = AlphaMean;
 ScaleTrue_NewChi2_Fit = AlphaMean_Fit;

 std::cerr << " ================ End DATA Scale search ================ " << std::endl;


///==== MC analysis ==== Scale search ====
 Data_or_MC = 0; ///=== 1 = Data;  0 = MC; -1 = MC Fit
 std::cerr << " ==== MC Scale search ==== " << std::endl; 
 ///==== cycle on number of Toy MC experiments ====
 ///=== Chi2 ===
 std::cerr << "======================= Chi2 " << ScaleTrue_Chi2 << " =====================" << std::endl;
//  ScaleTrue = ScaleTrue_Chi2;
//  doMC_Chi2();

///=== LogLikelihood ===
 std::cerr << "======================= LL " << ScaleTrue_LL << " =====================" << std::endl;
//  ScaleTrue = ScaleTrue_LL;
//  doMC_LL();

///=== NewChi2 ===
 std::cerr << "======================= NewChi2 " << ScaleTrue_NewChi2 << " =====================" << std::endl;
//  ScaleTrue = ScaleTrue_NewChi2;
//  doMC_NewChi2();


 Data_or_MC = -1; ///=== 1 = Data;  0 = MC; -1 = MC Fit
 std::cerr << " ==== MC Scale search ==== " << std::endl; 
 ///==== cycle on number of Toy MC experiments ====
 ///=== Chi2 ===
 std::cerr << "======================= Chi2 FIT " << ScaleTrue_Chi2_Fit << " =====================" << std::endl;
//  ScaleTrue = ScaleTrue_Chi2_Fit;
//  doMC_Chi2();

///=== LogLikelihood ===
 std::cerr << "======================= LL FIT " << ScaleTrue_LL_Fit << " =====================" << std::endl;
 ScaleTrue = ScaleTrue_LL_Fit;
 doMC_LL();

///=== NewChi2 ===
 std::cerr << "======================= NewChi2 FIT " << ScaleTrue_NewChi2_Fit << " =====================" << std::endl;
//  ScaleTrue = ScaleTrue_NewChi2_Fit;
//  doMC_NewChi2();

 
  ///----------------------
  ///---- Plot results ----
  ///----------------------
  outFile->cd();
  myTreeChi2->Write();
  myTreeLL_Result->Write();
  myTreeChi2_Result->Write();
  myTreeNewChi2_Result->Write();
  delete fitMin;
  
}



