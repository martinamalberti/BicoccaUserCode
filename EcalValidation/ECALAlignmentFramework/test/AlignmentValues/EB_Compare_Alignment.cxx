#include <iomanip>
#include <algorithm>
void EB_Compare_Alignment(TString nameOld, TString nameNew){
 
 
 
 ///===========================================================================
 
 //  gROOT->Reset();
 //  gROOT->SetStyle("Plain");
 //  gStyle->SetPalette(1); 
 //  gStyle->SetOptStat(1110);
 //  gStyle->SetPadTickX(1);
 //  gStyle->SetPadTickY(1);
 //  gStyle->SetTitleBorderSize(0);
 
 // style:
 
 //For the canvas:
 gStyle->SetCanvasBorderMode(0);
 gStyle->SetCanvasColor(kWhite);
 gStyle->SetCanvasDefH(800); //600
 //Height of canvas
 gStyle->SetCanvasDefW(800);//800 //Width of canvas
 gStyle->SetCanvasDefX(0);   //POsition on screen
 gStyle->SetCanvasDefY(0);
 
 // For the Pad:
 gStyle->SetPadBorderMode(0);
 // gStyle->SetPadBorderSize(Width_t size = 1);
 gStyle->SetPadColor(kWhite);
 gStyle->SetPadGridX(false);
 gStyle->SetPadGridY(false);
 gStyle->SetGridColor(0);
 gStyle->SetGridStyle(3);
 gStyle->SetGridWidth(1);
 
 //For the frame:
 gStyle->SetFrameBorderMode(0);
 gStyle->SetFrameBorderSize(1);
 gStyle->SetFrameFillColor(0);
 gStyle->SetFrameFillStyle(0);
 gStyle->SetFrameLineColor(1);
 gStyle->SetFrameLineStyle(1);
 gStyle->SetFrameLineWidth(1);
 
 // For the histo:
 gStyle->SetHistLineColor(1);
 gStyle->SetHistLineStyle(0);
 gStyle->SetHistLineWidth(2);
 gStyle->SetEndErrorSize(2);
 //  gStyle->SetErrorMarker(20);
 //gStyle->SetErrorX(0.);
 gStyle->SetMarkerStyle(8);
 
 
 // For the statistics box:
 gStyle->SetOptFile(0);
//  gStyle->SetOptStat(0);
 gStyle->SetOptStat("MR");
 gStyle->SetOptFit(0012);
 
//  gStyle->SetOptStat(100001100);
//  gStyle->SetOptStat(0);
 gStyle->SetStatColor(kWhite);
 gStyle->SetStatFont(42);
 gStyle->SetStatFontSize(0.025);
 gStyle->SetStatTextColor(1);
 gStyle->SetStatFormat("6.4g");
 gStyle->SetStatBorderSize(1);
 //   gStyle->SetStatBorderSize(0);
 gStyle->SetStatH(.3);
 gStyle->SetStatW(.25);
 
 //  gStyle->SetStatX(.9);
 // gStyle->SetStatY(.9);
 
 // For the Global title:
 gStyle->SetOptTitle(0);
 //   gStyle->SetOptTitle(1);
 gStyle->SetTitleFont(42);
 gStyle->SetTitleColor(1);
 gStyle->SetTitleTextColor(1);
 gStyle->SetTitleFillColor(10);
 gStyle->SetTitleFontSize(0.05);
 
 // For the axis titles:
 gStyle->SetTitleColor(1, "XYZ");
 gStyle->SetTitleFont(42, "XYZ");
 //  gStyle->SetTitleSize(0.06, "XYZ");
 gStyle->SetTitleSize(0.04, "XYZ");
 gStyle->SetTitleXOffset(0.9);
 // gStyle->SetTitleYOffset(1.25);
 //gStyle->SetTitleXOffset(0.5);
 gStyle->SetTitleYOffset(1.0);
 
 // For the axis labels:
 gStyle->SetLabelColor(1, "XYZ");
 gStyle->SetLabelFont(42, "XYZ");
 gStyle->SetLabelOffset(0.007, "XYZ");
 
//  gStyle->SetLabelSize(0.04, "XYZ");
 gStyle->SetLabelSize(0.05, "XYZ");
 //  gStyle->SetLabelSize(0.02, "XYZ");
 
 // For the axis:
 gStyle->SetAxisColor(1, "XYZ");
 gStyle->SetStripDecimals(kTRUE);
 gStyle->SetTickLength(0.03, "XYZ");
 gStyle->SetNdivisions(510, "XYZ");
 gStyle->SetPadTickX(0);  // To get tick marks on the opposite side of the frame
 gStyle->SetPadTickY(1);
 // // Postscript options:
 //   //gStyle->SetPaperSize(20.,20.);
 
 // CC style  
 gStyle->SetTitleXOffset(0.8);
 //   gStyle->SetTitleYOffset(0.8);
 //Inv Mass Presel
 gStyle->SetTitleYOffset(1.0);
 
 gStyle->SetLabelOffset(0.005, "XYZ");
 gStyle->SetTitleSize(0.07, "XYZ");
 gStyle->SetTitleFont(22,"X");
 gStyle->SetTitleFont(22,"Y");
 /*  gStyle->SetPadBottomMargin(0.13);
  * gStyle->SetPadTopMargin(0.23);
  * gStyle->SetPadLeftMargin(0.15);
  * gStyle->SetPadRightMargin(0.21);*/
 /*  //PlotPt4Lep=====================
  * gStyle->SetPadBottomMargin(0.15);
  * gStyle->SetPadTopMargin(0.05);
  * gStyle->SetPadLeftMargin(0.15);
  * gStyle->SetPadRightMargin(0.05);*/
 /*  //PlotPrePreSelInvMass=====================
  * gStyle->SetPadBottomMargin(0.15);
  * gStyle->SetPadTopMargin(0.10);
  * gStyle->SetPadLeftMargin(0.15);
  * gStyle->SetPadRightMargin(0.10);*/
 /*  //PlotPrePreSelEff=====================
  * gStyle->SetPadBottomMargin(0.15);
  * gStyle->SetPadTopMargin(0.15);
  * gStyle->SetPadLeftMargin(0.15);
  * gStyle->SetPadRightMargin(0.15);*/
 //PlotInvMass=====================
 gStyle->SetPadBottomMargin(0.15);
 gStyle->SetPadTopMargin(0.07);
 gStyle->SetPadLeftMargin(0.15);
 gStyle->SetPadRightMargin(0.15);
 /*  //PlotEffvsEff=====================
  * gStyle->SetPadBottomMargin(0.15);
  * gStyle->SetPadTopMargin(0.05);
  * gStyle->SetPadLeftMargin(0.15);
  * gStyle->SetPadRightMargin(0.05);*/
 /*  //JetMET=====================
  * gStyle->SetPadBottomMargin(0.15);
  * gStyle->SetPadTopMargin(0.05);
  * gStyle->SetPadLeftMargin(0.15);
  * gStyle->SetPadRightMargin(0.05);*/
 //   //Met2dEff=====================
 //   gStyle->SetPadBottomMargin(0.15);
 //   gStyle->SetPadTopMargin(0.05);
 //   gStyle->SetPadLeftMargin(0.15);
 //   gStyle->SetPadRightMargin(0.15);
 //   //Dalitz+PtVsMET=====================
 //   gStyle->SetPadBottomMargin(0.15);
 //   gStyle->SetPadTopMargin(0.10);
 //   gStyle->SetPadLeftMargin(0.15);
 //   gStyle->SetPadRightMargin(0.15);
 
 /*  gStyle->SetPadBottomMargin(0.13);
  * gStyle->SetPadLeftMargin(0.15);*/
 //  gStyle->SetHistLineWidth(3);
 gStyle->SetHistLineWidth(2);
 gStyle->SetLineStyleString(5,"20 12 4 12");
 gStyle->SetLineStyleString(6,"20 12 4 12 4 12 4 12");
 gStyle->SetLineStyleString(7,"20 20");
 gStyle->SetLineStyleString(8,"20 12 4 12 4 12");
 gStyle->SetLineStyleString(9,"80 20");
 
 
 ///---- somewhere in the code:
 ///---- Axis with x10^XXX
//  TGaxis::SetMaxDigits(1);
 
 gStyle->cd(); 
 
 ///===========================================================================
 
 
 ///===========================================================================
 std::string buffer;
 
 //==== kRed
 
 std::cerr << " 4Apr File ... " << std::endl;
 
 
 double DX_SM_Mean[36];
 double DX_SM_RMS[36];
 double DY_SM_Mean[36];
 double DY_SM_RMS[36];
 double DZ_SM_Mean[36];
 double DZ_SM_RMS[36];
 
 double DTHETAe_SM_Mean[36];
 double DTHETAe_SM_RMS[36];
 double DPSIe_SM_Mean[36];
 double DPSIe_SM_RMS[36];
 double DPHIe_SM_Mean[36];
 double DPHIe_SM_RMS[36];


 
 std::string nameFileRed (nameOld.Data());
 
 
 
 
 std::ifstream file (nameFileRed.c_str()); 

 if (!file.is_open()){
  std::cerr << "noooo" << std::endl;
 }
 int iSM_After = 0;
 while(!file.eof()){
  getline(file,buffer);
//   std::cout << "buffer = " << buffer; // << std::endl;
  if (buffer != ""){ ///---> save from empty line at the end!
    std::stringstream line( buffer );       
    line >> DTHETAe_SM_Mean[iSM_After]; std::cout << " " << DTHETAe_SM_Mean[iSM_After];
    line >> DPSIe_SM_Mean[iSM_After];   std::cout << " " << DPSIe_SM_Mean[iSM_After];
    line >> DPHIe_SM_Mean[iSM_After];   std::cout << " " << DPHIe_SM_Mean[iSM_After];
    line >> DX_SM_Mean[iSM_After];      std::cout << " " << DX_SM_Mean[iSM_After];
    line >> DY_SM_Mean[iSM_After];      std::cout << " " << DY_SM_Mean[iSM_After];
    line >> DZ_SM_Mean[iSM_After];      std::cout << " " << DZ_SM_Mean[iSM_After];

//     DX_SM_Mean[iSM_After] *= -1;
//     DY_SM_Mean[iSM_After] *= -1;
//     DZ_SM_Mean[iSM_After] *= -1;
   
    
//     DX_SM_Mean[iSM_After] += Prev_DX_SM_Mean[iSM_After];
//     DY_SM_Mean[iSM_After] += Prev_DY_SM_Mean[iSM_After];
//     DZ_SM_Mean[iSM_After] += Prev_DZ_SM_Mean[iSM_After];
    
    std::cout << std::endl;
    iSM_After++;
  }
 }
 
 
 ///===========================================================================

//==== kBlue

 std::cerr << " New File ... " << std::endl;
 
 std::string nameFileBlue (nameNew.Data());
 
 
 std::ifstream fileEle (nameFileBlue.c_str()); 
  
 double DX_SM_Mean_After[36];
 double DX_SM_RMS_After[36];
 double DY_SM_Mean_After[36];
 double DY_SM_RMS_After[36];
 double DZ_SM_Mean_After[36];
 double DZ_SM_RMS_After[36];
 
 double DTHETAe_SM_Mean_After[36];
 double DTHETAe_SM_RMS_After[36];
 double DPSIe_SM_Mean_After[36];
 double DPSIe_SM_RMS_After[36];
 double DPHIe_SM_Mean_After[36];
 double DPHIe_SM_RMS_After[36];

 double iSM_SM_Ele[36];
 double iSM_SM_Ele_RMS[38];
    
 if (!fileEle.is_open()){
  std::cerr << "noooo" << std::endl;
 }
 iSM_After = 0;
 while(!fileEle.eof()){
  getline(fileEle,buffer);
  if (buffer != ""){ ///---> save from empty line at the end!
    std::stringstream line( buffer );       
    iSM_SM_Ele[iSM_After] = iSM_After;
    iSM_SM_Ele_RMS[iSM_After] = 0;
    line >> DTHETAe_SM_Mean_After[iSM_After]; std::cout << " " << DTHETAe_SM_Mean_After[iSM_After];
    line >> DPSIe_SM_Mean_After[iSM_After];   std::cout << " " << DPSIe_SM_Mean_After[iSM_After];
    line >> DPHIe_SM_Mean_After[iSM_After];   std::cout << " " << DPHIe_SM_Mean_After[iSM_After];
    line >> DX_SM_Mean_After[iSM_After];      std::cout << " " << DX_SM_Mean_After[iSM_After];
    line >> DY_SM_Mean_After[iSM_After];      std::cout << " " << DY_SM_Mean_After[iSM_After];
    line >> DZ_SM_Mean_After[iSM_After];      std::cout << " " << DZ_SM_Mean_After[iSM_After];
    std::cout << std::endl;
   
    iSM_After++;
  }
 }
  
  ///===========================================================================
  ///===========================================================================
  ///===========================================================================
  
  ///===========================================================================
  TCanvas* cSM_DX_DY_DZ_Scatter = new TCanvas ("cSM_DX_DY_DZ_Scatter","cSM_DX_DY_DZ_Scatter",1000,800);
  cSM_DX_DY_DZ_Scatter->Divide(2,3);
  
  TF1* line1 = new TF1("line1","x",-2,2);
  line1->SetLineColor(kGreen);
  
  ///===================================
  cSM_DX_DY_DZ_Scatter->cd(1);
  TGraphErrors* grDZ_EBMinus_Scatter = new TGraphErrors(18,DZ_SM_Mean_After+18,DZ_SM_Mean+18,DZ_SM_RMS_After+18,DZ_SM_RMS+18);
  grDZ_EBMinus_Scatter->SetTitle("EB- #DeltaZ #DeltaZ");
  grDZ_EBMinus_Scatter->SetMarkerColor(kRed);
  grDZ_EBMinus_Scatter->SetLineColor(kRed);
  grDZ_EBMinus_Scatter->SetMarkerStyle(20);
  grDZ_EBMinus_Scatter->SetMarkerSize(1);
  grDZ_EBMinus_Scatter->SetLineWidth(1);
  grDZ_EBMinus_Scatter->GetXaxis()->SetTitle("#DeltaZ After");
  grDZ_EBMinus_Scatter->GetYaxis()->SetTitle("#DeltaZ Before");
  grDZ_EBMinus_Scatter->Draw("AP");
  line1->Draw("same");
  gPad->SetGrid();
  
  cSM_DX_DY_DZ_Scatter->cd(2);
  TGraphErrors* grDZ_EBPlus_Scatter = new TGraphErrors(18,DZ_SM_Mean_After,DZ_SM_Mean,DZ_SM_RMS_After,DZ_SM_RMS);
  grDZ_EBPlus_Scatter->SetTitle("EB+ #DeltaZ #DeltaZ");
  grDZ_EBPlus_Scatter->SetMarkerColor(kRed);
  grDZ_EBPlus_Scatter->SetLineColor(kRed);
  grDZ_EBPlus_Scatter->SetMarkerStyle(20);
  grDZ_EBPlus_Scatter->SetMarkerSize(1);
  grDZ_EBPlus_Scatter->SetLineWidth(1);
  grDZ_EBPlus_Scatter->GetXaxis()->SetTitle("#DeltaZ After");
  grDZ_EBPlus_Scatter->GetYaxis()->SetTitle("#DeltaZ Before");
  grDZ_EBPlus_Scatter->Draw("AP");
  line1->Draw("same");
  gPad->SetGrid();
  
  ///===================================
  cSM_DX_DY_DZ_Scatter->cd(3);
  TGraphErrors* grDY_EBMinus_Scatter = new TGraphErrors(18,DY_SM_Mean_After+18,DY_SM_Mean+18,DY_SM_RMS_After+18,DY_SM_RMS+18);
  grDY_EBMinus_Scatter->SetTitle("EB- #DeltaY #DeltaY");
  grDY_EBMinus_Scatter->SetMarkerColor(kRed);
  grDY_EBMinus_Scatter->SetLineColor(kRed);
  grDY_EBMinus_Scatter->SetMarkerStyle(20);
  grDY_EBMinus_Scatter->SetMarkerSize(1);
  grDY_EBMinus_Scatter->SetLineWidth(1);
  grDY_EBMinus_Scatter->GetXaxis()->SetTitle("#DeltaY After");
  grDY_EBMinus_Scatter->GetYaxis()->SetTitle("#DeltaY Before");
  grDY_EBMinus_Scatter->Draw("AP");
  line1->Draw("same");
  gPad->SetGrid();
  
  cSM_DX_DY_DZ_Scatter->cd(4);
  TGraphErrors* grDY_EBPlus_Scatter = new TGraphErrors(18,DY_SM_Mean_After,DY_SM_Mean,DY_SM_RMS_After,DY_SM_RMS);
  grDY_EBPlus_Scatter->SetTitle("EB+ #DeltaY #DeltaY");
  grDY_EBPlus_Scatter->SetMarkerColor(kRed);
  grDY_EBPlus_Scatter->SetLineColor(kRed);
  grDY_EBPlus_Scatter->SetMarkerStyle(20);
  grDY_EBPlus_Scatter->SetMarkerSize(1);
  grDY_EBPlus_Scatter->SetLineWidth(1);
  grDY_EBPlus_Scatter->GetXaxis()->SetTitle("#DeltaY After");
  grDY_EBPlus_Scatter->GetYaxis()->SetTitle("#DeltaY Before");
  grDY_EBPlus_Scatter->Draw("AP");
  line1->Draw("same");
  gPad->SetGrid();
  
  ///===================================
  cSM_DX_DY_DZ_Scatter->cd(5);
  TGraphErrors* grDX_EBMinus_Scatter = new TGraphErrors(18,DX_SM_Mean_After+18,DX_SM_Mean+18,DX_SM_RMS_After+18,DX_SM_RMS+18);
  grDX_EBMinus_Scatter->SetTitle("EB- #DeltaX #DeltaX");
  grDX_EBMinus_Scatter->SetMarkerColor(kRed);
  grDX_EBMinus_Scatter->SetLineColor(kRed);
  grDX_EBMinus_Scatter->SetMarkerStyle(20);
  grDX_EBMinus_Scatter->SetMarkerSize(1);
  grDX_EBMinus_Scatter->SetLineWidth(1);
  grDX_EBMinus_Scatter->GetXaxis()->SetTitle("#DeltaX After");
  grDX_EBMinus_Scatter->GetYaxis()->SetTitle("#DeltaX Before");
  grDX_EBMinus_Scatter->Draw("AP");
  line1->Draw("same");
  gPad->SetGrid();
  
  cSM_DX_DY_DZ_Scatter->cd(6);
  TGraphErrors* grDX_EBPlus_Scatter = new TGraphErrors(18,DX_SM_Mean_After,DX_SM_Mean,DX_SM_RMS_After,DX_SM_RMS);
  grDX_EBPlus_Scatter->SetTitle("EB+ #DeltaX #DeltaX");
  grDX_EBPlus_Scatter->SetMarkerColor(kRed);
  grDX_EBPlus_Scatter->SetLineColor(kRed);
  grDX_EBPlus_Scatter->SetMarkerStyle(20);
  grDX_EBPlus_Scatter->SetMarkerSize(1);
  grDX_EBPlus_Scatter->SetLineWidth(1);
  grDX_EBPlus_Scatter->GetXaxis()->SetTitle("#DeltaX After");
  grDX_EBPlus_Scatter->GetYaxis()->SetTitle("#DeltaX Before");
  grDX_EBPlus_Scatter->Draw("AP");
  line1->Draw("same");
  gPad->SetGrid();
  
  
  
  ///===========================================================================
  ///===========================================================================
  ///===========================================================================
  TCanvas* cSM_DX_DY_DZ = new TCanvas ("cSM_DX_DY_DZ","cSM_DX_DY_DZ",800,800);
  cSM_DX_DY_DZ->Divide(2,3);
  
  
  ///===================================
  cSM_DX_DY_DZ->cd(1);
  TGraphErrors* grDZ_EBMinus_Ele = new TGraphErrors(18,iSM_SM_Ele+18,DZ_SM_Mean+18,iSM_SM_Ele_RMS+18,DZ_SM_RMS+18);
  grDZ_EBMinus_Ele->SetTitle("EB- #DeltaZ");
  grDZ_EBMinus_Ele->SetMarkerColor(kRed);
  grDZ_EBMinus_Ele->SetLineColor(kRed);
  grDZ_EBMinus_Ele->SetMarkerStyle(20);
  grDZ_EBMinus_Ele->SetMarkerSize(1);
  grDZ_EBMinus_Ele->SetLineWidth(1);
  grDZ_EBMinus_Ele->GetXaxis()->SetTitle("iSM");
  grDZ_EBMinus_Ele->GetYaxis()->SetTitle("#DeltaZ (cm)");
//   grDZ_EBMinus_Ele->Draw("AP");
//   gPad->SetGrid();
  
  TGraphErrors* grDZ_EBMinus_After = new TGraphErrors(18,iSM_SM_Ele+18,DZ_SM_Mean_After+18,iSM_SM_Ele_RMS,DZ_SM_RMS_After+18);
  grDZ_EBMinus_After->SetMarkerColor(kBlue);
  grDZ_EBMinus_After->SetLineColor(kBlue);
  grDZ_EBMinus_After->SetMarkerStyle(21);
  grDZ_EBMinus_After->SetMarkerSize(1);
  grDZ_EBMinus_After->SetLineWidth(1);
  grDZ_EBMinus_After->GetXaxis()->SetTitle("iSM");
  grDZ_EBMinus_After->GetYaxis()->SetTitle("#DeltaZ (cm)");
  grDZ_EBMinus_Ele->GetYaxis()->SetRangeUser(std::min(grDZ_EBMinus_Ele->GetMinimum(),grDZ_EBMinus_After->GetMinimum()),std::max(grDZ_EBMinus_Ele->GetMaximum(),grDZ_EBMinus_After->GetMaximum()));
//   grDZ_EBMinus_After->Draw("P");
//   gPad->SetGrid();
  
  
  cSM_DX_DY_DZ->cd(2);
  TGraphErrors* grDZ_EBPlus_Ele = new TGraphErrors(18,iSM_SM_Ele,DZ_SM_Mean,iSM_SM_Ele_RMS,DZ_SM_RMS);
  grDZ_EBPlus_Ele->SetTitle("EB+ #DeltaZ");
  grDZ_EBPlus_Ele->SetMarkerColor(kRed);
  grDZ_EBPlus_Ele->SetLineColor(kRed);
  grDZ_EBPlus_Ele->SetMarkerStyle(20);
  grDZ_EBPlus_Ele->SetMarkerSize(1);
  grDZ_EBPlus_Ele->SetLineWidth(1);
  grDZ_EBPlus_Ele->GetXaxis()->SetTitle("iSM");
  grDZ_EBPlus_Ele->GetYaxis()->SetTitle("#DeltaZ (cm)");
//   grDZ_EBPlus_Ele->Draw("AP");
//   gPad->SetGrid();
  
  TGraphErrors* grDZ_EBPlus_After = new TGraphErrors(18,iSM_SM_Ele,DZ_SM_Mean_After,iSM_SM_Ele_RMS,DZ_SM_RMS_After);
  grDZ_EBPlus_After->SetMarkerColor(kBlue);
  grDZ_EBPlus_After->SetLineColor(kBlue);
  grDZ_EBPlus_After->SetMarkerStyle(21);
  grDZ_EBPlus_After->SetMarkerSize(1);
  grDZ_EBPlus_After->SetLineWidth(1);
  grDZ_EBPlus_After->GetXaxis()->SetTitle("iSM");
  grDZ_EBPlus_After->GetYaxis()->SetTitle("#DeltaZ (cm)");
  grDZ_EBPlus_Ele->GetYaxis()->SetRangeUser(std::min(grDZ_EBPlus_Ele->GetMinimum(),grDZ_EBPlus_After->GetMinimum()),std::max(grDZ_EBPlus_Ele->GetMaximum(),grDZ_EBPlus_After->GetMaximum()));
//   grDZ_EBPlus_After->Draw("P");
//   gPad->SetGrid();
  
 

  ///===================================
  cSM_DX_DY_DZ->cd(3);
  TGraphErrors* grDY_EBMinus_Ele = new TGraphErrors(18,iSM_SM_Ele+18,DY_SM_Mean+18,iSM_SM_Ele_RMS+18,DY_SM_RMS+18);
  grDY_EBMinus_Ele->SetTitle("EB- #DeltaY");
  grDY_EBMinus_Ele->SetMarkerColor(kRed);
  grDY_EBMinus_Ele->SetLineColor(kRed);
  grDY_EBMinus_Ele->SetMarkerStyle(20);
  grDY_EBMinus_Ele->SetMarkerSize(1);
  grDY_EBMinus_Ele->SetLineWidth(1);
  grDY_EBMinus_Ele->GetXaxis()->SetTitle("iSM");
  grDY_EBMinus_Ele->GetYaxis()->SetTitle("#DeltaY (cm)");
//   grDY_EBMinus_Ele->Draw("AP");
//   gPad->SetGrid();
  
  TGraphErrors* grDY_EBMinus_After = new TGraphErrors(18,iSM_SM_Ele+18,DY_SM_Mean_After+18,iSM_SM_Ele_RMS+18,DY_SM_RMS_After+18);
  grDY_EBMinus_After->SetMarkerColor(kBlue);
  grDY_EBMinus_After->SetLineColor(kBlue);
  grDY_EBMinus_After->SetMarkerStyle(21);
  grDY_EBMinus_After->SetMarkerSize(1);
  grDY_EBMinus_After->SetLineWidth(1);
  grDY_EBMinus_After->GetXaxis()->SetTitle("iSM");
  grDY_EBMinus_After->GetYaxis()->SetTitle("#DeltaY (cm)");
  grDY_EBMinus_Ele->GetYaxis()->SetRangeUser(std::min(grDY_EBMinus_Ele->GetMinimum(),grDY_EBMinus_After->GetMinimum()),std::max(grDY_EBMinus_Ele->GetMaximum(),grDY_EBMinus_After->GetMaximum()));
//   grDY_EBMinus_After->Draw("P");
//   gPad->SetGrid();
  
  
  cSM_DX_DY_DZ->cd(4);
  TGraphErrors* grDY_EBPlus_Ele = new TGraphErrors(18,iSM_SM_Ele,DY_SM_Mean,iSM_SM_Ele_RMS,DY_SM_RMS);
  grDY_EBPlus_Ele->SetTitle("EB+ #DeltaY");
  grDY_EBPlus_Ele->SetMarkerColor(kRed);
  grDY_EBPlus_Ele->SetLineColor(kRed);
  grDY_EBPlus_Ele->SetMarkerStyle(20);
  grDY_EBPlus_Ele->SetMarkerSize(1);
  grDY_EBPlus_Ele->SetLineWidth(1);
  grDY_EBPlus_Ele->GetXaxis()->SetTitle("iSM");
  grDY_EBPlus_Ele->GetYaxis()->SetTitle("#DeltaY (cm)");
//   grDY_EBPlus_Ele->Draw("AP");
//   gPad->SetGrid();
  
  TGraphErrors* grDY_EBPlus_After = new TGraphErrors(18,iSM_SM_Ele,DY_SM_Mean_After,iSM_SM_Ele_RMS,DY_SM_RMS_After);
  grDY_EBPlus_After->SetMarkerColor(kBlue);
  grDY_EBPlus_After->SetLineColor(kBlue);
  grDY_EBPlus_After->SetMarkerStyle(21);
  grDY_EBPlus_After->SetMarkerSize(1);
  grDY_EBPlus_After->SetLineWidth(1);
  grDY_EBPlus_After->GetXaxis()->SetTitle("iSM");
  grDY_EBPlus_After->GetYaxis()->SetTitle("#DeltaY (cm)");
  grDY_EBPlus_Ele->GetYaxis()->SetRangeUser(std::min(grDY_EBPlus_Ele->GetMinimum(),grDY_EBPlus_After->GetMinimum()),std::max(grDY_EBPlus_Ele->GetMaximum(),grDY_EBPlus_After->GetMaximum()));
//   grDY_EBPlus_After->Draw("P");
//   gPad->SetGrid();
  
  ///===================================
  cSM_DX_DY_DZ->cd(5);
  TGraphErrors* grDX_EBMinus_Ele = new TGraphErrors(18,iSM_SM_Ele+18,DX_SM_Mean+18,iSM_SM_Ele_RMS+18,DX_SM_RMS+18);
  grDX_EBMinus_Ele->SetTitle("EB- #DeltaX");
  grDX_EBMinus_Ele->SetMarkerColor(kRed);
  grDX_EBMinus_Ele->SetLineColor(kRed);
  grDX_EBMinus_Ele->SetMarkerStyle(20);
  grDX_EBMinus_Ele->SetMarkerSize(1);
  grDX_EBMinus_Ele->SetLineWidth(1);
  grDX_EBMinus_Ele->GetXaxis()->SetTitle("iSM");
  grDX_EBMinus_Ele->GetYaxis()->SetTitle("#DeltaX (cm)");
//   grDX_EBMinus_Ele->Draw("AP");
//   gPad->SetGrid();
  
  TGraphErrors* grDX_EBMinus_After = new TGraphErrors(18,iSM_SM_Ele+18,DX_SM_Mean_After+18,iSM_SM_Ele_RMS+18,DX_SM_RMS_After+18);
  grDX_EBMinus_After->SetMarkerColor(kBlue);
  grDX_EBMinus_After->SetLineColor(kBlue);
  grDX_EBMinus_After->SetMarkerStyle(21);
  grDX_EBMinus_After->SetMarkerSize(1);
  grDX_EBMinus_After->SetLineWidth(1);
  grDX_EBMinus_After->GetXaxis()->SetTitle("iSM");
  grDX_EBMinus_After->GetYaxis()->SetTitle("#DeltaX (cm)");
  grDX_EBMinus_Ele->GetYaxis()->SetRangeUser(std::min(grDX_EBMinus_Ele->GetMinimum(),grDX_EBMinus_After->GetMinimum()),std::max(grDX_EBMinus_Ele->GetMaximum(),grDX_EBMinus_After->GetMaximum()));
//   grDX_EBMinus_After->Draw("P");
//   gPad->SetGrid();
  
  
  cSM_DX_DY_DZ->cd(6);
  TGraphErrors* grDX_EBPlus_Ele = new TGraphErrors(18,iSM_SM_Ele,DX_SM_Mean,iSM_SM_Ele_RMS,DX_SM_RMS);
  grDX_EBPlus_Ele->SetTitle("EB+ #DeltaX");
  grDX_EBPlus_Ele->SetMarkerColor(kRed);
  grDX_EBPlus_Ele->SetLineColor(kRed);
  grDX_EBPlus_Ele->SetMarkerStyle(20);
  grDX_EBPlus_Ele->SetMarkerSize(1);
  grDX_EBPlus_Ele->SetLineWidth(1);
  grDX_EBPlus_Ele->GetXaxis()->SetTitle("iSM");
  grDX_EBPlus_Ele->GetYaxis()->SetTitle("#DeltaX (cm)");
//   grDX_EBPlus_Ele->Draw("AP");
//   gPad->SetGrid();
  
  TGraphErrors* grDX_EBPlus_After = new TGraphErrors(18,iSM_SM_Ele,DX_SM_Mean_After,iSM_SM_Ele_RMS,DX_SM_RMS_After);
  grDX_EBPlus_After->SetMarkerColor(kBlue);
  grDX_EBPlus_After->SetLineColor(kBlue);
  grDX_EBPlus_After->SetMarkerStyle(21);
  grDX_EBPlus_After->SetMarkerSize(1);
  grDX_EBPlus_After->SetLineWidth(1);
  grDX_EBPlus_After->GetXaxis()->SetTitle("iSM");
  grDX_EBPlus_After->GetYaxis()->SetTitle("#DeltaX (cm)");
  grDX_EBPlus_Ele->GetYaxis()->SetRangeUser(std::min(grDX_EBPlus_Ele->GetMinimum(),grDX_EBPlus_After->GetMinimum()),std::max(grDX_EBPlus_Ele->GetMaximum(),grDX_EBPlus_After->GetMaximum()));
//   grDX_EBPlus_After->Draw("P");
//   gPad->SetGrid();
  
  
  
  
  
  
  
   ///===========================================================================
  ///===========================================================================
  ///===========================================================================
  TCanvas* cSM_DTHETAe_DPSIe_DPHIe = new TCanvas ("cSM_DTHETAe_DPSIe_DPHIe","cSM_DTHETAe_DPSIe_DPHIe",800,800);
  cSM_DTHETAe_DPSIe_DPHIe->Divide(2,3);
  
  
  ///===================================
  cSM_DTHETAe_DPSIe_DPHIe->cd(1);
  TGraphErrors* grDPHIe_EBMinus_Ele = new TGraphErrors(18,iSM_SM_Ele+18,DPHIe_SM_Mean+18,iSM_SM_Ele_RMS+18,DPHIe_SM_RMS+18);
  grDPHIe_EBMinus_Ele->SetTitle("EB- #Delta#phi Euler");
  grDPHIe_EBMinus_Ele->SetMarkerColor(kRed);
  grDPHIe_EBMinus_Ele->SetLineColor(kRed);
  grDPHIe_EBMinus_Ele->SetMarkerStyle(20);
  grDPHIe_EBMinus_Ele->SetMarkerSize(1);
  grDPHIe_EBMinus_Ele->SetLineWidth(1);
  grDPHIe_EBMinus_Ele->GetXaxis()->SetTitle("#phi");
  grDPHIe_EBMinus_Ele->GetYaxis()->SetTitle("#Delta#phi Euler");
  grDPHIe_EBMinus_Ele->Draw("AP");
  gPad->SetGrid();
  
  TGraphErrors* grDPHIe_EBMinus_After = new TGraphErrors(18,iSM_SM_Ele+18,DPHIe_SM_Mean_After+18,iSM_SM_Ele_RMS,DPHIe_SM_RMS_After+18);
  grDPHIe_EBMinus_After->SetMarkerColor(kBlue);
  grDPHIe_EBMinus_After->SetLineColor(kBlue);
  grDPHIe_EBMinus_After->SetMarkerStyle(21);
  grDPHIe_EBMinus_After->SetMarkerSize(1);
  grDPHIe_EBMinus_After->SetLineWidth(1);
  grDPHIe_EBMinus_After->GetXaxis()->SetTitle("#phi");
  grDPHIe_EBMinus_After->GetYaxis()->SetTitle("#Delta#phi Euler");
  grDPHIe_EBMinus_After->Draw("P");
  gPad->SetGrid();
  
  
  cSM_DTHETAe_DPSIe_DPHIe->cd(2);
  TGraphErrors* grDPHIe_EBPlus_Ele = new TGraphErrors(18,iSM_SM_Ele,DPHIe_SM_Mean,iSM_SM_Ele_RMS,DPHIe_SM_RMS);
  grDPHIe_EBPlus_Ele->SetTitle("EB+ #Delta#phi Euler");
  grDPHIe_EBPlus_Ele->SetMarkerColor(kRed);
  grDPHIe_EBPlus_Ele->SetLineColor(kRed);
  grDPHIe_EBPlus_Ele->SetMarkerStyle(20);
  grDPHIe_EBPlus_Ele->SetMarkerSize(1);
  grDPHIe_EBPlus_Ele->SetLineWidth(1);
  grDPHIe_EBPlus_Ele->GetXaxis()->SetTitle("#phi");
  grDPHIe_EBPlus_Ele->GetYaxis()->SetTitle("#Delta#phi Euler");
  grDPHIe_EBPlus_Ele->Draw("AP");
  gPad->SetGrid();
  
  TGraphErrors* grDPHIe_EBPlus_After = new TGraphErrors(18,iSM_SM_Ele,DPHIe_SM_Mean_After,iSM_SM_Ele_RMS,DPHIe_SM_RMS_After);
  grDPHIe_EBPlus_After->SetMarkerColor(kBlue);
  grDPHIe_EBPlus_After->SetLineColor(kBlue);
  grDPHIe_EBPlus_After->SetMarkerStyle(21);
  grDPHIe_EBPlus_After->SetMarkerSize(1);
  grDPHIe_EBPlus_After->SetLineWidth(1);
  grDPHIe_EBPlus_After->GetXaxis()->SetTitle("#phi");
  grDPHIe_EBPlus_After->GetYaxis()->SetTitle("#Delta#phi Euler");
  grDPHIe_EBPlus_After->Draw("P");
  gPad->SetGrid();
  
 

  ///===================================
  cSM_DTHETAe_DPSIe_DPHIe->cd(3);
  TGraphErrors* grDPSIe_EBMinus_Ele = new TGraphErrors(18,iSM_SM_Ele+18,DPSIe_SM_Mean+18,iSM_SM_Ele_RMS+18,DPSIe_SM_RMS+18);
  grDPSIe_EBMinus_Ele->SetTitle("EB- #Delta#psi Euler");
  grDPSIe_EBMinus_Ele->SetMarkerColor(kRed);
  grDPSIe_EBMinus_Ele->SetLineColor(kRed);
  grDPSIe_EBMinus_Ele->SetMarkerStyle(20);
  grDPSIe_EBMinus_Ele->SetMarkerSize(1);
  grDPSIe_EBMinus_Ele->SetLineWidth(1);
  grDPSIe_EBMinus_Ele->GetXaxis()->SetTitle("#phi");
  grDPSIe_EBMinus_Ele->GetYaxis()->SetTitle("#Delta#psi Euler");
  grDPSIe_EBMinus_Ele->Draw("AP");
  gPad->SetGrid();
  
  TGraphErrors* grDPSIe_EBMinus_After = new TGraphErrors(18,iSM_SM_Ele+18,DPSIe_SM_Mean_After+18,iSM_SM_Ele_RMS+18,DPSIe_SM_RMS_After+18);
  grDPSIe_EBMinus_After->SetMarkerColor(kBlue);
  grDPSIe_EBMinus_After->SetLineColor(kBlue);
  grDPSIe_EBMinus_After->SetMarkerStyle(21);
  grDPSIe_EBMinus_After->SetMarkerSize(1);
  grDPSIe_EBMinus_After->SetLineWidth(1);
  grDPSIe_EBMinus_After->GetXaxis()->SetTitle("#phi");
  grDPSIe_EBMinus_After->GetYaxis()->SetTitle("#Delta#psi Euler");
  grDPSIe_EBMinus_After->Draw("P");
  gPad->SetGrid();
  
  
  cSM_DTHETAe_DPSIe_DPHIe->cd(4);
  TGraphErrors* grDPSIe_EBPlus_Ele = new TGraphErrors(18,iSM_SM_Ele,DPSIe_SM_Mean,iSM_SM_Ele_RMS,DPSIe_SM_RMS);
  grDPSIe_EBPlus_Ele->SetTitle("EB+ #Delta#psi Euler");
  grDPSIe_EBPlus_Ele->SetMarkerColor(kRed);
  grDPSIe_EBPlus_Ele->SetLineColor(kRed);
  grDPSIe_EBPlus_Ele->SetMarkerStyle(20);
  grDPSIe_EBPlus_Ele->SetMarkerSize(1);
  grDPSIe_EBPlus_Ele->SetLineWidth(1);
  grDPSIe_EBPlus_Ele->GetXaxis()->SetTitle("#phi");
  grDPSIe_EBPlus_Ele->GetYaxis()->SetTitle("#Delta#psi Euler");
  grDPSIe_EBPlus_Ele->Draw("AP");
  gPad->SetGrid();
  
  TGraphErrors* grDPSIe_EBPlus_After = new TGraphErrors(18,iSM_SM_Ele,DPSIe_SM_Mean_After,iSM_SM_Ele_RMS,DPSIe_SM_RMS_After);
  grDPSIe_EBPlus_After->SetMarkerColor(kBlue);
  grDPSIe_EBPlus_After->SetLineColor(kBlue);
  grDPSIe_EBPlus_After->SetMarkerStyle(21);
  grDPSIe_EBPlus_After->SetMarkerSize(1);
  grDPSIe_EBPlus_After->SetLineWidth(1);
  grDPSIe_EBPlus_After->GetXaxis()->SetTitle("#phi");
  grDPSIe_EBPlus_After->GetYaxis()->SetTitle("#Delta#psi Euler");
  grDPSIe_EBPlus_After->Draw("P");
  gPad->SetGrid();
  
  ///===================================
  cSM_DTHETAe_DPSIe_DPHIe->cd(5);
  TGraphErrors* grDTHETAe_EBMinus_Ele = new TGraphErrors(18,iSM_SM_Ele+18,DTHETAe_SM_Mean+18,iSM_SM_Ele_RMS+18,DTHETAe_SM_RMS+18);
  grDTHETAe_EBMinus_Ele->SetTitle("EB- #Delta#theta Euler");
  grDTHETAe_EBMinus_Ele->SetMarkerColor(kRed);
  grDTHETAe_EBMinus_Ele->SetLineColor(kRed);
  grDTHETAe_EBMinus_Ele->SetMarkerStyle(20);
  grDTHETAe_EBMinus_Ele->SetMarkerSize(1);
  grDTHETAe_EBMinus_Ele->SetLineWidth(1);
  grDTHETAe_EBMinus_Ele->GetXaxis()->SetTitle("#phi");
  grDTHETAe_EBMinus_Ele->GetYaxis()->SetTitle("#Delta#theta Euler");
  grDTHETAe_EBMinus_Ele->Draw("AP");
  gPad->SetGrid();
  
  TGraphErrors* grDTHETAe_EBMinus_After = new TGraphErrors(18,iSM_SM_Ele+18,DTHETAe_SM_Mean_After+18,iSM_SM_Ele_RMS+18,DTHETAe_SM_RMS_After+18);
  grDTHETAe_EBMinus_After->SetMarkerColor(kBlue);
  grDTHETAe_EBMinus_After->SetLineColor(kBlue);
  grDTHETAe_EBMinus_After->SetMarkerStyle(21);
  grDTHETAe_EBMinus_After->SetMarkerSize(1);
  grDTHETAe_EBMinus_After->SetLineWidth(1);
  grDTHETAe_EBMinus_After->GetXaxis()->SetTitle("#phi");
  grDTHETAe_EBMinus_After->GetYaxis()->SetTitle("#Delta#theta Euler");
  grDTHETAe_EBMinus_After->Draw("P");
  gPad->SetGrid();
  
  
  cSM_DTHETAe_DPSIe_DPHIe->cd(6);
  TGraphErrors* grDTHETAe_EBPlus_Ele = new TGraphErrors(18,iSM_SM_Ele,DTHETAe_SM_Mean,iSM_SM_Ele_RMS,DTHETAe_SM_RMS);
  grDTHETAe_EBPlus_Ele->SetTitle("EB+ #Delta#theta Euler");
  grDTHETAe_EBPlus_Ele->SetMarkerColor(kRed);
  grDTHETAe_EBPlus_Ele->SetLineColor(kRed);
  grDTHETAe_EBPlus_Ele->SetMarkerStyle(20);
  grDTHETAe_EBPlus_Ele->SetMarkerSize(1);
  grDTHETAe_EBPlus_Ele->SetLineWidth(1);
  grDTHETAe_EBPlus_Ele->GetXaxis()->SetTitle("#phi");
  grDTHETAe_EBPlus_Ele->GetYaxis()->SetTitle("#Delta#theta Euler");
  grDTHETAe_EBPlus_Ele->Draw("AP");
  gPad->SetGrid();
  
  TGraphErrors* grDTHETAe_EBPlus_After = new TGraphErrors(18,iSM_SM_Ele,DTHETAe_SM_Mean_After,iSM_SM_Ele_RMS,DTHETAe_SM_RMS_After);
  grDTHETAe_EBPlus_After->SetMarkerColor(kBlue);
  grDTHETAe_EBPlus_After->SetLineColor(kBlue);
  grDTHETAe_EBPlus_After->SetMarkerStyle(21);
  grDTHETAe_EBPlus_After->SetMarkerSize(1);
  grDTHETAe_EBPlus_After->SetLineWidth(1);
  grDTHETAe_EBPlus_After->GetXaxis()->SetTitle("#phi");
  grDTHETAe_EBPlus_After->GetYaxis()->SetTitle("#Delta#theta Euler");
  grDTHETAe_EBPlus_After->Draw("P");
  gPad->SetGrid();
  
 
  ///==== draw ====
  cSM_DX_DY_DZ->cd(1);
  TMultiGraph* mgr_DZ_EBMinus = new TMultiGraph();
  mgr_DZ_EBMinus->Add(grDZ_EBMinus_Ele);
  mgr_DZ_EBMinus->Add(grDZ_EBMinus_After);
  mgr_DZ_EBMinus->Draw("AP");
  mgr_DZ_EBMinus->GetXaxis()->SetTitle("iSM");
  mgr_DZ_EBMinus->GetYaxis()->SetTitle("#DeltaZ");
  gPad->SetGrid();
  
  cSM_DX_DY_DZ->cd(3);
  TMultiGraph* mgr_DY_EBMinus = new TMultiGraph();
  mgr_DY_EBMinus->Add(grDY_EBMinus_Ele);
  mgr_DY_EBMinus->Add(grDY_EBMinus_After);
  mgr_DY_EBMinus->Draw("AP");
  mgr_DY_EBMinus->GetXaxis()->SetTitle("iSM");
  mgr_DY_EBMinus->GetYaxis()->SetTitle("#DeltaY");
  gPad->SetGrid();
  
  cSM_DX_DY_DZ->cd(5);
  TMultiGraph* mgr_DX_EBMinus = new TMultiGraph();
  mgr_DX_EBMinus->Add(grDX_EBMinus_Ele);
  mgr_DX_EBMinus->Add(grDX_EBMinus_After);
  mgr_DX_EBMinus->Draw("AP");
  mgr_DX_EBMinus->GetXaxis()->SetTitle("iSM");
  mgr_DX_EBMinus->GetYaxis()->SetTitle("#DeltaX");
  gPad->SetGrid();
  
  
  cSM_DX_DY_DZ->cd(2);
  TMultiGraph* mgr_DZ_EBPlus = new TMultiGraph();
  mgr_DZ_EBPlus->Add(grDZ_EBPlus_Ele);
  mgr_DZ_EBPlus->Add(grDZ_EBPlus_After);
  mgr_DZ_EBPlus->Draw("AP");
  mgr_DZ_EBPlus->GetXaxis()->SetTitle("iSM");
  mgr_DZ_EBPlus->GetYaxis()->SetTitle("#DeltaZ");
  gPad->SetGrid();
  
  cSM_DX_DY_DZ->cd(4);
  TMultiGraph* mgr_DY_EBPlus = new TMultiGraph();
  mgr_DY_EBPlus->Add(grDY_EBPlus_Ele);
  mgr_DY_EBPlus->Add(grDY_EBPlus_After);
  mgr_DY_EBPlus->Draw("AP");
  mgr_DY_EBPlus->GetXaxis()->SetTitle("iSM");
  mgr_DY_EBPlus->GetYaxis()->SetTitle("#DeltaY");
  gPad->SetGrid();
  
  cSM_DX_DY_DZ->cd(6);
  TMultiGraph* mgr_DX_EBPlus = new TMultiGraph();
  mgr_DX_EBPlus->Add(grDX_EBPlus_Ele);
  mgr_DX_EBPlus->Add(grDX_EBPlus_After);
  mgr_DX_EBPlus->Draw("AP");
  mgr_DX_EBPlus->GetXaxis()->SetTitle("iSM");
  mgr_DX_EBPlus->GetYaxis()->SetTitle("#DeltaX");
  gPad->SetGrid();
  
  
  
  ///===========================================================================
  ///===========================================================================
  ///===========================================================================
  TCanvas* cDifference = new TCanvas ("cDifference","cDifference",900,350);
  cDifference->Divide(3,1);
  
  TH1F *hDXDYDZ = new TH1F("hDXDYDZ","#DeltaX#DeltaY#DeltaZ",40,-0.5,0.5);
  for (int i = 0; i<36; i++) hDXDYDZ->Fill(DX_SM_Mean_After[i] - DX_SM_Mean[i]);
  for (int i = 0; i<36; i++) hDXDYDZ->Fill(DY_SM_Mean_After[i] - DY_SM_Mean[i]);
  for (int i = 0; i<36; i++) hDXDYDZ->Fill(DZ_SM_Mean_After[i] - DZ_SM_Mean[i]);
  
  
  TH1F *hDX = new TH1F("hDX","#DeltaX",40,-0.5,0.5);
  for (int i = 0; i<36; i++) hDX->Fill(DX_SM_Mean_After[i] - DX_SM_Mean[i]);

  TH1F *hDY = new TH1F("hDY","#DeltaY",40,-0.5,0.5);
  for (int i = 0; i<36; i++) hDY->Fill(DY_SM_Mean_After[i] - DY_SM_Mean[i]);

  TH1F *hDZ = new TH1F("hDZ","#DeltaZ",40,-0.5,0.5);
  for (int i = 0; i<36; i++) hDZ->Fill(DZ_SM_Mean_After[i] - DZ_SM_Mean[i]);
 
  cDifference->cd(1);
  hDX->SetLineColor(kRed);
  hDX->SetFillColor(kRed);
  hDX->SetFillStyle(3002);
  hDX->Draw();
  gPad->SetGrid();
    
  cDifference->cd(2);
  hDY->SetLineColor(kRed);
  hDY->SetFillColor(kRed);
  hDY->SetFillStyle(3002);
  hDY->Draw();
  gPad->SetGrid();
  
  cDifference->cd(3);
  hDZ->SetLineColor(kRed);
  hDZ->SetFillColor(kRed);
  hDZ->SetFillStyle(3002);
  hDZ->Draw();
  gPad->SetGrid();  
  
  
  TF1* funz = new TF1("gaus","gaus",-0.5, 0.5);
  funz->SetParName(1,"peak");
  funz->SetParName(2,"#sigma gauss");
  
  TCanvas* cDXDYDZ_EB = new TCanvas ("cDXDYDZ_EB","cDXDYDZ_EB",700,700);
  hDXDYDZ->SetLineColor(kRed);
  hDXDYDZ->SetFillColor(kRed);
  hDXDYDZ->SetFillStyle(3002);
  hDXDYDZ->Draw();
  funz->SetParameter(0,0);
  hDXDYDZ->Fit("gaus","RME");
  funz->FixParameter(0,funz->GetParameter(0));
  hDXDYDZ->GetXaxis()->SetTitle("cm");
  gPad->SetGrid();
  
  TCanvas* cDX_EB = new TCanvas ("cDX_EB","cDX_EB",700,700);
  hDX->SetLineColor(kRed);
  hDX->SetFillColor(kRed);
  hDX->SetFillStyle(3002);
  hDX->Draw();
  funz->SetParameter(0,20);
  hDX->Fit("gaus","RME");
  funz->SetParameter(0,funz->GetParameter(0));
  hDX->Fit("gaus","RME");
  funz->FixParameter(0,funz->GetParameter(0));
  hDX->GetXaxis()->SetTitle("#Deltax (cm)");
  gPad->SetGrid();
  
  TCanvas* cDY_EB = new TCanvas ("cDY_EB","cDY_EB",700,700);
  hDY->SetLineColor(kRed);
  hDY->SetFillColor(kRed);
  hDY->SetFillStyle(3002);
  hDY->Draw();
  funz->SetParameter(0,0);
  hDY->Fit("gaus","RME");
  funz->FixParameter(0,funz->GetParameter(0));
  hDY->GetXaxis()->SetTitle("#Deltay (cm)");
  gPad->SetGrid();
  
  TCanvas* cDZ_EB = new TCanvas ("cDZ_EB","cDZ_EB",700,700);
  hDZ->SetLineColor(kRed);
  hDZ->SetFillColor(kRed);
  hDZ->SetFillStyle(3002);
  hDZ->Draw();
  funz->SetParameter(0,0);
  hDZ->Fit("gaus","RME");
  hDZ->GetXaxis()->SetTitle("#Deltaz (cm)");
  gPad->SetGrid();
  
}


