# include <iostream>
# include <cstdlib>
# include "TH2D.h"
# include "TFile.h"
# include <fstream>
# include <cmath>
# include <TCanvas.h>
# include <TPaveStats.h>

/// MAcros for visualize the results of the independent pair reserch 

void Funz()
{
  int event = gPad->GetEvent();
  if (event != kButton1Up) return;
  int px = gPad->GetEventX();
  int py = gPad->GetEventY();
  TObject *select = gPad->GetSelected();
  if (!select) return;
  if (select->InheritsFrom("TH2")) {
    TH2 *h = (TH2*)select;
    Float_t xx = gPad->AbsPixeltoX(px);
    Float_t x  = gPad->PadtoX(xx);
    Int_t binx = h->GetXaxis()->FindBin(x);
    
    Float_t yy = gPad->AbsPixeltoY(py);
    Float_t y  = gPad->PadtoY(yy);
    Int_t biny = h->GetYaxis()->FindBin(y);
    gStyle->SetOptStat(1);
    TString CommandToROOTSize = Form(".x test/Print_Histo_Cut.cxx(\"%s\",\"%s\")",h->GetXaxis()->GetBinLabel(binx),h->GetYaxis()->GetBinLabel(biny));
    std::cerr<<"<<<<<<<<<"<<CommandToROOTSize.Data()<<std::endl;
    gROOT->ProcessLine(CommandToROOTSize);
  }
  
}




int Print_Variable()
{ 
  /// Open the ABCD analysis results for each pair considered
  TFile* imput_cut=TFile::Open("output/Variable_Histogram.root");

  TH2D* Correlation=(TH2D*)imput_cut->Get("CorrelationHistogramm");
  TH2D* CorrelationProbability=(TH2D*)imput_cut->Get("CorrelationProbabilityHistogramm");
  TH2D* DoubleRatio=(TH2D*)imput_cut->Get("DoubleRatioHistogramm");
  TH2D* Kolmogorov=(TH2D*)imput_cut->Get("KolmogorovHistogramm");
  TH2D* KolmogorovProduct=(TH2D*)imput_cut->Get("KolmogorovProductHistogramm");

  gStyle->SetPaintTextFormat("5.2f");
  
  TCanvas * c1=new TCanvas("c1","Correlation Factor",1);
  gStyle->SetOptStat(0);

  Correlation->GetXaxis()->SetLabelSize(0.028);
  Correlation->GetYaxis()->SetLabelSize(0.028);
  Correlation->Draw("colz");
  Correlation->Draw("textSAME");
  
  TCanvas * c2=new TCanvas("c2","Correlation Probability",1);
  gStyle->SetOptStat(0);
  CorrelationProbability->GetXaxis()->SetLabelSize(0.028);
  CorrelationProbability->GetYaxis()->SetLabelSize(0.028);
  
  CorrelationProbability->Draw("colz");
  CorrelationProbability->Draw("textSAME");
  
  TCanvas * c3=new TCanvas("c3","DoubleRatio",1);
  gStyle->SetOptStat(0);
  DoubleRatio->GetXaxis()->SetLabelSize(0.028);
  DoubleRatio->GetYaxis()->SetLabelSize(0.028);
  c3->SetGrid();
  DoubleRatio->Draw("colz");
  DoubleRatio->Draw("textSAME");
  
  TCanvas * c4=new TCanvas("c4","Kolmogorov",1);
  gStyle->SetOptStat(0);
  Kolmogorov->GetXaxis()->SetLabelSize(0.028);
  Kolmogorov->GetYaxis()->SetLabelSize(0.028);
  c4->SetGrid();
  Kolmogorov->Draw("colz");
  Kolmogorov->Draw("textSAME");
  


 
 c4->AddExec("ciccio","Funz()");
 c1->AddExec("ciccio1","Funz()");
 c2->AddExec("ciccio2","Funz()");
 c3->AddExec("ciccio3","Funz()");

   

  return 0;
 
}

