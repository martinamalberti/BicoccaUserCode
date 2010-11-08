# include <iostream>
# include <cstdlib>
# include "TH2D.h"
# include "TFile.h"
# include <fstream>
# include <cmath>
# include <TCanvas.h>
# include <TPaveStats.h>

/// Macros for the visualization of the ABCD distributions obtained

int Print_BackgroundContamination(TString Var1,TString Var2,TString Directory)
{ 
 
 TFile* FileIn = new TFile("output/background/Background_Contamination.root", "READ"); 
 
 TCanvas* cHstack = (TCanvas*) FileIn->Get("cHstack");
 cHstack->Draw();
 
 TCanvas* c_AB = (TCanvas*) FileIn->Get("c_AB");
 c_AB->Draw();
 TCanvas* c_CD = (TCanvas*) FileIn->Get("c_CD");
 c_CD->Draw();
 TCanvas* c_AC = (TCanvas*) FileIn->Get("c_AC");
 c_AC->Draw();
 TCanvas* c_BD = (TCanvas*) FileIn->Get("c_BD");
 c_BD->Draw();
 
 
 
 TString toDo = Form(".x test/Print_Histo_Cut.cxx\(\"%s\",\"%s\",\"%s\"\)",Var1.Data(), Var2.Data(), Directory.Data());
 gROOT->ProcessLine(toDo);
 
 
  
}  