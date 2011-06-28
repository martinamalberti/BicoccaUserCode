#include <iomanip>
#include <algorithm>
 
void EE_Compare_Alignment(TString nameOld, TString nameNew){
 
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
//   gStyle->SetOptStat(1);
 gStyle->SetOptStat("MR");
 gStyle->SetOptFit(0012);
 
//  gStyle->SetOptStat(0);
 gStyle->SetStatColor(kWhite);
 gStyle->SetStatFont(42);
 gStyle->SetStatFontSize(0.025);
 gStyle->SetStatTextColor(1);
 gStyle->SetStatFormat("6.4g");
 gStyle->SetStatBorderSize(1);
 //   gStyle->SetStatBorderSize(0);
//  gStyle->SetStatH(.1);
//  gStyle->SetStatW(.15);
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
 
 Double_t DX_SM_Mean[10];
 Double_t DX_SM_RMS[10];
 Double_t DY_SM_Mean[10];
 Double_t DY_SM_RMS[10];
 Double_t DZ_SM_Mean[10];
 Double_t DZ_SM_RMS[10];
 
 Double_t DTHETAe_SM_Mean[10];
 Double_t DTHETAe_SM_RMS[10];
 Double_t DPSIe_SM_Mean[10];
 Double_t DPSIe_SM_RMS[10];
 Double_t DPHIe_SM_Mean[10];
 Double_t DPHIe_SM_RMS[10];

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
    line >> DX_SM_Mean[iSM_After];      std::cout << " " << DX_SM_Mean[iSM_After] << " cm " ;
    line >> DY_SM_Mean[iSM_After];      std::cout << " " << DY_SM_Mean[iSM_After] << " cm " ;
    line >> DZ_SM_Mean[iSM_After];      std::cout << " " << DZ_SM_Mean[iSM_After] << " cm " ;

//     DX_SM_Mean[iSM_After] *= -1;
//     DY_SM_Mean[iSM_After] *= -1;
//     DZ_SM_Mean[iSM_After] *= -1;
    
    std::cout << std::endl;
    iSM_After++;
  }
 }
 
 
 ///===========================================================================

//==== kBlue

 std::cerr << " New File ... " << std::endl;
 
 std::string nameFileBlue (nameNew.Data());
 
 
 std::ifstream fileEle (nameFileBlue.c_str()); 
 
 Double_t DX_SM_Mean_After[10];
 Double_t DX_SM_RMS_After[10];
 Double_t DY_SM_Mean_After[10];
 Double_t DY_SM_RMS_After[10];
 Double_t DZ_SM_Mean_After[10];
 Double_t DZ_SM_RMS_After[10];
 
 Double_t DTHETAe_SM_Mean_After[10];
 Double_t DTHETAe_SM_RMS_After[10];
 Double_t DPSIe_SM_Mean_After[10];
 Double_t DPSIe_SM_RMS_After[10];
 Double_t DPHIe_SM_Mean_After[10];
 Double_t DPHIe_SM_RMS_After[10];

 Double_t iSM_SM_Ele[10];
 Double_t iSM_SM_Ele_RMS[10];
    
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
  TGraphErrors* grDZ_EEMinus_Scatter = new TGraphErrors(2,DZ_SM_Mean_After+2,DZ_SM_Mean+2,DZ_SM_RMS_After+2,DZ_SM_RMS+2);
  grDZ_EEMinus_Scatter->SetTitle("EE- #DeltaZ #DeltaZ");
  grDZ_EEMinus_Scatter->SetMarkerColor(kRed);
  grDZ_EEMinus_Scatter->SetLineColor(kRed);
  grDZ_EEMinus_Scatter->SetMarkerStyle(20);
  grDZ_EEMinus_Scatter->SetMarkerSize(1);
  grDZ_EEMinus_Scatter->SetLineWidth(1);
  grDZ_EEMinus_Scatter->GetXaxis()->SetTitle("#DeltaZ After");
  grDZ_EEMinus_Scatter->GetYaxis()->SetTitle("#DeltaZ Before");
  grDZ_EEMinus_Scatter->Draw("AP");
  line1->Draw("same");
  gPad->SetGrid();
  
  cSM_DX_DY_DZ_Scatter->cd(2);
  TGraphErrors* grDZ_EEPlus_Scatter = new TGraphErrors(2,DZ_SM_Mean_After,DZ_SM_Mean,DZ_SM_RMS_After,DZ_SM_RMS);
  grDZ_EEPlus_Scatter->SetTitle("EE+ #DeltaZ #DeltaZ");
  grDZ_EEPlus_Scatter->SetMarkerColor(kRed);
  grDZ_EEPlus_Scatter->SetLineColor(kRed);
  grDZ_EEPlus_Scatter->SetMarkerStyle(20);
  grDZ_EEPlus_Scatter->SetMarkerSize(1);
  grDZ_EEPlus_Scatter->SetLineWidth(1);
  grDZ_EEPlus_Scatter->GetXaxis()->SetTitle("#DeltaZ After");
  grDZ_EEPlus_Scatter->GetYaxis()->SetTitle("#DeltaZ Before");
  grDZ_EEPlus_Scatter->Draw("AP");
  line1->Draw("same");
  gPad->SetGrid();
  
  ///===================================
  cSM_DX_DY_DZ_Scatter->cd(3);
  TGraphErrors* grDY_EEMinus_Scatter = new TGraphErrors(2,DY_SM_Mean_After+2,DY_SM_Mean+2,DY_SM_RMS_After+2,DY_SM_RMS+2);
  grDY_EEMinus_Scatter->SetTitle("EE- #DeltaY #DeltaY");
  grDY_EEMinus_Scatter->SetMarkerColor(kRed);
  grDY_EEMinus_Scatter->SetLineColor(kRed);
  grDY_EEMinus_Scatter->SetMarkerStyle(20);
  grDY_EEMinus_Scatter->SetMarkerSize(1);
  grDY_EEMinus_Scatter->SetLineWidth(1);
  grDY_EEMinus_Scatter->GetXaxis()->SetTitle("#DeltaY After");
  grDY_EEMinus_Scatter->GetYaxis()->SetTitle("#DeltaY Before");
  grDY_EEMinus_Scatter->Draw("AP");
  line1->Draw("same");
  gPad->SetGrid();
  
  cSM_DX_DY_DZ_Scatter->cd(4);
  TGraphErrors* grDY_EEPlus_Scatter = new TGraphErrors(2,DY_SM_Mean_After,DY_SM_Mean,DY_SM_RMS_After,DY_SM_RMS);
  grDY_EEPlus_Scatter->SetTitle("EE+ #DeltaY #DeltaY");
  grDY_EEPlus_Scatter->SetMarkerColor(kRed);
  grDY_EEPlus_Scatter->SetLineColor(kRed);
  grDY_EEPlus_Scatter->SetMarkerStyle(20);
  grDY_EEPlus_Scatter->SetMarkerSize(1);
  grDY_EEPlus_Scatter->SetLineWidth(1);
  grDY_EEPlus_Scatter->GetXaxis()->SetTitle("#DeltaY After");
  grDY_EEPlus_Scatter->GetYaxis()->SetTitle("#DeltaY Before");
  grDY_EEPlus_Scatter->Draw("AP");
  line1->Draw("same");
  gPad->SetGrid();
  
  ///===================================
  cSM_DX_DY_DZ_Scatter->cd(5);
  TGraphErrors* grDX_EEMinus_Scatter = new TGraphErrors(2,DX_SM_Mean_After+2,DX_SM_Mean+2,DX_SM_RMS_After+2,DX_SM_RMS+2);
  grDX_EEMinus_Scatter->SetTitle("EE- #DeltaX #DeltaX");
  grDX_EEMinus_Scatter->SetMarkerColor(kRed);
  grDX_EEMinus_Scatter->SetLineColor(kRed);
  grDX_EEMinus_Scatter->SetMarkerStyle(20);
  grDX_EEMinus_Scatter->SetMarkerSize(1);
  grDX_EEMinus_Scatter->SetLineWidth(1);
  grDX_EEMinus_Scatter->GetXaxis()->SetTitle("#DeltaX After");
  grDX_EEMinus_Scatter->GetYaxis()->SetTitle("#DeltaX Before");
  grDX_EEMinus_Scatter->Draw("AP");
  line1->Draw("same");
  gPad->SetGrid();
  
  cSM_DX_DY_DZ_Scatter->cd(6);
  TGraphErrors* grDX_EEPlus_Scatter = new TGraphErrors(2,DX_SM_Mean_After,DX_SM_Mean,DX_SM_RMS_After,DX_SM_RMS);
  grDX_EEPlus_Scatter->SetTitle("EE+ #DeltaX #DeltaX");
  grDX_EEPlus_Scatter->SetMarkerColor(kRed);
  grDX_EEPlus_Scatter->SetLineColor(kRed);
  grDX_EEPlus_Scatter->SetMarkerStyle(20);
  grDX_EEPlus_Scatter->SetMarkerSize(1);
  grDX_EEPlus_Scatter->SetLineWidth(1);
  grDX_EEPlus_Scatter->GetXaxis()->SetTitle("#DeltaX After");
  grDX_EEPlus_Scatter->GetYaxis()->SetTitle("#DeltaX Before");
  grDX_EEPlus_Scatter->Draw("AP");
  line1->Draw("same");
  gPad->SetGrid();
  
  
  
  ///===========================================================================
  ///===========================================================================
  ///===========================================================================
  TCanvas* cSM_DX_DY_DZ = new TCanvas ("cSM_DX_DY_DZ","cSM_DX_DY_DZ",800,800);
  cSM_DX_DY_DZ->Divide(2,3);
  
  
  ///===================================
  cSM_DX_DY_DZ->cd(1);
  TGraphErrors* grDZ_EEMinus_Ele = new TGraphErrors(2,iSM_SM_Ele+2,DZ_SM_Mean+2,iSM_SM_Ele_RMS+2,DZ_SM_RMS+2);
  grDZ_EEMinus_Ele->SetTitle("EE- #DeltaZ");
  grDZ_EEMinus_Ele->SetMarkerColor(kRed);
  grDZ_EEMinus_Ele->SetLineColor(kRed);
  grDZ_EEMinus_Ele->SetMarkerStyle(20);
  grDZ_EEMinus_Ele->SetMarkerSize(1);
  grDZ_EEMinus_Ele->SetLineWidth(1);
  grDZ_EEMinus_Ele->GetXaxis()->SetTitle("#phi");
  grDZ_EEMinus_Ele->GetYaxis()->SetTitle("#DeltaZ (cm)");
//   grDZ_EEMinus_Ele->Draw("AP");
//   gPad->SetGrid();
  
  TGraphErrors* grDZ_EEMinus_After = new TGraphErrors(2,iSM_SM_Ele+2,DZ_SM_Mean_After+2,iSM_SM_Ele_RMS+2,DZ_SM_RMS_After+2);
  grDZ_EEMinus_After->SetMarkerColor(kBlue);
  grDZ_EEMinus_After->SetLineColor(kBlue);
  grDZ_EEMinus_After->SetMarkerStyle(21);
  grDZ_EEMinus_After->SetMarkerSize(1);
  grDZ_EEMinus_After->SetLineWidth(1);
  grDZ_EEMinus_After->GetXaxis()->SetTitle("#phi");
  grDZ_EEMinus_After->GetYaxis()->SetTitle("#DeltaZ (cm)");
  grDZ_EEMinus_Ele->GetYaxis()->SetRangeUser(std::min(grDZ_EEMinus_Ele->GetMinimum(),grDZ_EEMinus_After->GetMinimum()),std::max(grDZ_EEMinus_Ele->GetMaximum(),grDZ_EEMinus_After->GetMaximum()));
//   grDZ_EEMinus_After->Draw("P");
//   gPad->SetGrid();
  
  
  cSM_DX_DY_DZ->cd(2);
  TGraphErrors* grDZ_EEPlus_Ele = new TGraphErrors(2,iSM_SM_Ele,DZ_SM_Mean,iSM_SM_Ele_RMS,DZ_SM_RMS);
  grDZ_EEPlus_Ele->SetTitle("EE+ #DeltaZ");
  grDZ_EEPlus_Ele->SetMarkerColor(kRed);
  grDZ_EEPlus_Ele->SetLineColor(kRed);
  grDZ_EEPlus_Ele->SetMarkerStyle(20);
  grDZ_EEPlus_Ele->SetMarkerSize(1);
  grDZ_EEPlus_Ele->SetLineWidth(1);
  grDZ_EEPlus_Ele->GetXaxis()->SetTitle("#phi");
  grDZ_EEPlus_Ele->GetYaxis()->SetTitle("#DeltaZ (cm)");
//   grDZ_EEPlus_Ele->Draw("AP");
//   gPad->SetGrid();
  
  TGraphErrors* grDZ_EEPlus_After = new TGraphErrors(2,iSM_SM_Ele,DZ_SM_Mean_After,iSM_SM_Ele_RMS,DZ_SM_RMS_After);
  grDZ_EEPlus_After->SetMarkerColor(kBlue);
  grDZ_EEPlus_After->SetLineColor(kBlue);
  grDZ_EEPlus_After->SetMarkerStyle(21);
  grDZ_EEPlus_After->SetMarkerSize(1);
  grDZ_EEPlus_After->SetLineWidth(1);
  grDZ_EEPlus_After->GetXaxis()->SetTitle("#phi");
  grDZ_EEPlus_After->GetYaxis()->SetTitle("#DeltaZ (cm)");
  grDZ_EEPlus_Ele->GetYaxis()->SetRangeUser(std::min(grDZ_EEPlus_Ele->GetMinimum(),grDZ_EEPlus_After->GetMinimum()),std::max(grDZ_EEPlus_Ele->GetMaximum(),grDZ_EEPlus_After->GetMaximum()));
//   grDZ_EEPlus_After->Draw("P");
//   gPad->SetGrid();
  
 

  ///===================================
  cSM_DX_DY_DZ->cd(3);
  TGraphErrors* grDY_EEMinus_Ele = new TGraphErrors(2,iSM_SM_Ele+2,DY_SM_Mean+2,iSM_SM_Ele_RMS+2,DY_SM_RMS+2);
  grDY_EEMinus_Ele->SetTitle("EE- #DeltaY");
  grDY_EEMinus_Ele->SetMarkerColor(kRed);
  grDY_EEMinus_Ele->SetLineColor(kRed);
  grDY_EEMinus_Ele->SetMarkerStyle(20);
  grDY_EEMinus_Ele->SetMarkerSize(1);
  grDY_EEMinus_Ele->SetLineWidth(1);
  grDY_EEMinus_Ele->GetXaxis()->SetTitle("#phi");
  grDY_EEMinus_Ele->GetYaxis()->SetTitle("#DeltaY (cm)");
//   grDY_EEMinus_Ele->Draw("AP");
//   gPad->SetGrid();
  
  TGraphErrors* grDY_EEMinus_After = new TGraphErrors(2,iSM_SM_Ele+2,DY_SM_Mean_After+2,iSM_SM_Ele_RMS+2,DY_SM_RMS_After+2);
  grDY_EEMinus_After->SetMarkerColor(kBlue);
  grDY_EEMinus_After->SetLineColor(kBlue);
  grDY_EEMinus_After->SetMarkerStyle(21);
  grDY_EEMinus_After->SetMarkerSize(1);
  grDY_EEMinus_After->SetLineWidth(1);
  grDY_EEMinus_After->GetXaxis()->SetTitle("#phi");
  grDY_EEMinus_After->GetYaxis()->SetTitle("#DeltaY (cm)");
  grDY_EEMinus_Ele->GetYaxis()->SetRangeUser(std::min(grDY_EEMinus_Ele->GetMinimum(),grDY_EEMinus_After->GetMinimum()),std::max(grDY_EEMinus_Ele->GetMaximum(),grDY_EEMinus_After->GetMaximum()));
//   grDY_EEMinus_After->Draw("P");
//   gPad->SetGrid();
  
  
  cSM_DX_DY_DZ->cd(4);
  TGraphErrors* grDY_EEPlus_Ele = new TGraphErrors(2,iSM_SM_Ele,DY_SM_Mean,iSM_SM_Ele_RMS,DY_SM_RMS);
  grDY_EEPlus_Ele->SetTitle("EE+ #DeltaY");
  grDY_EEPlus_Ele->SetMarkerColor(kRed);
  grDY_EEPlus_Ele->SetLineColor(kRed);
  grDY_EEPlus_Ele->SetMarkerStyle(20);
  grDY_EEPlus_Ele->SetMarkerSize(1);
  grDY_EEPlus_Ele->SetLineWidth(1);
  grDY_EEPlus_Ele->GetXaxis()->SetTitle("#phi");
  grDY_EEPlus_Ele->GetYaxis()->SetTitle("#DeltaY (cm)");
//   grDY_EEPlus_Ele->Draw("AP");
//   gPad->SetGrid();
  
  TGraphErrors* grDY_EEPlus_After = new TGraphErrors(2,iSM_SM_Ele,DY_SM_Mean_After,iSM_SM_Ele_RMS,DY_SM_RMS_After);
  grDY_EEPlus_After->SetMarkerColor(kBlue);
  grDY_EEPlus_After->SetLineColor(kBlue);
  grDY_EEPlus_After->SetMarkerStyle(21);
  grDY_EEPlus_After->SetMarkerSize(1);
  grDY_EEPlus_After->SetLineWidth(1);
  grDY_EEPlus_After->GetXaxis()->SetTitle("#phi");
  grDY_EEPlus_After->GetYaxis()->SetTitle("#DeltaY (cm)");
  grDY_EEPlus_Ele->GetYaxis()->SetRangeUser(std::min(grDY_EEPlus_Ele->GetMinimum(),grDY_EEPlus_After->GetMinimum()),std::max(grDY_EEPlus_Ele->GetMaximum(),grDY_EEPlus_After->GetMaximum()));
//   grDY_EEPlus_After->Draw("P");
//   gPad->SetGrid();
  
  ///===================================
  cSM_DX_DY_DZ->cd(5);
  TGraphErrors* grDX_EEMinus_Ele = new TGraphErrors(2,iSM_SM_Ele+2,DX_SM_Mean+2,iSM_SM_Ele_RMS+2,DX_SM_RMS+2);
  grDX_EEMinus_Ele->SetTitle("EE- #DeltaX");
  grDX_EEMinus_Ele->SetMarkerColor(kRed);
  grDX_EEMinus_Ele->SetLineColor(kRed);
  grDX_EEMinus_Ele->SetMarkerStyle(20);
  grDX_EEMinus_Ele->SetMarkerSize(1);
  grDX_EEMinus_Ele->SetLineWidth(1);
  grDX_EEMinus_Ele->GetXaxis()->SetTitle("#phi");
  grDX_EEMinus_Ele->GetYaxis()->SetTitle("#DeltaX (cm)");
//   grDX_EEMinus_Ele->Draw("AP");
//   gPad->SetGrid();
  
  TGraphErrors* grDX_EEMinus_After = new TGraphErrors(2,iSM_SM_Ele+2,DX_SM_Mean_After+2,iSM_SM_Ele_RMS+2,DX_SM_RMS_After+2);
  grDX_EEMinus_After->SetMarkerColor(kBlue);
  grDX_EEMinus_After->SetLineColor(kBlue);
  grDX_EEMinus_After->SetMarkerStyle(21);
  grDX_EEMinus_After->SetMarkerSize(1);
  grDX_EEMinus_After->SetLineWidth(1);
  grDX_EEMinus_After->GetXaxis()->SetTitle("#phi");
  grDX_EEMinus_After->GetYaxis()->SetTitle("#DeltaX (cm)");
  grDX_EEMinus_Ele->GetYaxis()->SetRangeUser(std::min(grDX_EEMinus_Ele->GetMinimum(),grDX_EEMinus_After->GetMinimum()),std::max(grDX_EEMinus_Ele->GetMaximum(),grDX_EEMinus_After->GetMaximum()));
//   grDX_EEMinus_After->Draw("P");
//   gPad->SetGrid();
  
  
  cSM_DX_DY_DZ->cd(6);
  TGraphErrors* grDX_EEPlus_Ele = new TGraphErrors(2,iSM_SM_Ele,DX_SM_Mean,iSM_SM_Ele_RMS,DX_SM_RMS);
  grDX_EEPlus_Ele->SetTitle("EE+ #DeltaX");
  grDX_EEPlus_Ele->SetMarkerColor(kRed);
  grDX_EEPlus_Ele->SetLineColor(kRed);
  grDX_EEPlus_Ele->SetMarkerStyle(20);
  grDX_EEPlus_Ele->SetMarkerSize(1);
  grDX_EEPlus_Ele->SetLineWidth(1);
  grDX_EEPlus_Ele->GetXaxis()->SetTitle("#phi");
  grDX_EEPlus_Ele->GetYaxis()->SetTitle("#DeltaX (cm)");
//   grDX_EEPlus_Ele->Draw("AP");
//   gPad->SetGrid();
  
  TGraphErrors* grDX_EEPlus_After = new TGraphErrors(2,iSM_SM_Ele,DX_SM_Mean_After,iSM_SM_Ele_RMS,DX_SM_RMS_After);
  grDX_EEPlus_After->SetMarkerColor(kBlue);
  grDX_EEPlus_After->SetLineColor(kBlue);
  grDX_EEPlus_After->SetMarkerStyle(21);
  grDX_EEPlus_After->SetMarkerSize(1);
  grDX_EEPlus_After->SetLineWidth(1);
  grDX_EEPlus_After->GetXaxis()->SetTitle("#phi");
  grDX_EEPlus_After->GetYaxis()->SetTitle("#DeltaX (cm)");
  grDX_EEPlus_Ele->GetYaxis()->SetRangeUser(std::min(grDX_EEPlus_Ele->GetMinimum(),grDX_EEPlus_After->GetMinimum()),std::max(grDX_EEPlus_Ele->GetMaximum(),grDX_EEPlus_After->GetMaximum()));
//   grDX_EEPlus_After->Draw("P");
//   gPad->SetGrid();
  
  
  
  ///==== draw ====
  cSM_DX_DY_DZ->cd(1);
  TMultiGraph* mgr_DZ_EEMinus = new TMultiGraph();
  mgr_DZ_EEMinus->Add(grDZ_EEMinus_Ele);
  mgr_DZ_EEMinus->Add(grDZ_EEMinus_After);
  mgr_DZ_EEMinus->Draw("AP");
  mgr_DZ_EEMinus->GetXaxis()->SetTitle("iSM");
  mgr_DZ_EEMinus->GetYaxis()->SetTitle("#DeltaZ");
  gPad->SetGrid();
  
  cSM_DX_DY_DZ->cd(3);
  TMultiGraph* mgr_DY_EEMinus = new TMultiGraph();
  mgr_DY_EEMinus->Add(grDY_EEMinus_Ele);
  mgr_DY_EEMinus->Add(grDY_EEMinus_After);
  mgr_DY_EEMinus->Draw("AP");
  mgr_DY_EEMinus->GetXaxis()->SetTitle("iSM");
  mgr_DY_EEMinus->GetYaxis()->SetTitle("#DeltaY");
  gPad->SetGrid();
  
  cSM_DX_DY_DZ->cd(5);
  TMultiGraph* mgr_DX_EEMinus = new TMultiGraph();
  mgr_DX_EEMinus->Add(grDX_EEMinus_Ele);
  mgr_DX_EEMinus->Add(grDX_EEMinus_After);
  mgr_DX_EEMinus->Draw("AP");
  mgr_DX_EEMinus->GetXaxis()->SetTitle("iSM");
  mgr_DX_EEMinus->GetYaxis()->SetTitle("#DeltaX");
  gPad->SetGrid();
  
  
  cSM_DX_DY_DZ->cd(2);
  TMultiGraph* mgr_DZ_EEPlus = new TMultiGraph();
  mgr_DZ_EEPlus->Add(grDZ_EEPlus_Ele);
  mgr_DZ_EEPlus->Add(grDZ_EEPlus_After);
  mgr_DZ_EEPlus->Draw("AP");
  mgr_DZ_EEPlus->GetXaxis()->SetTitle("iSM");
  mgr_DZ_EEPlus->GetYaxis()->SetTitle("#DeltaZ");
  gPad->SetGrid();
  
  cSM_DX_DY_DZ->cd(4);
  TMultiGraph* mgr_DY_EEPlus = new TMultiGraph();
  mgr_DY_EEPlus->Add(grDY_EEPlus_Ele);
  mgr_DY_EEPlus->Add(grDY_EEPlus_After);
  mgr_DY_EEPlus->Draw("AP");
  mgr_DY_EEPlus->GetXaxis()->SetTitle("iSM");
  mgr_DY_EEPlus->GetYaxis()->SetTitle("#DeltaY");
  gPad->SetGrid();
  
  cSM_DX_DY_DZ->cd(6);
  TMultiGraph* mgr_DX_EEPlus = new TMultiGraph();
  mgr_DX_EEPlus->Add(grDX_EEPlus_Ele);
  mgr_DX_EEPlus->Add(grDX_EEPlus_After);
  mgr_DX_EEPlus->Draw("AP");
  mgr_DX_EEPlus->GetXaxis()->SetTitle("iSM");
  mgr_DX_EEPlus->GetYaxis()->SetTitle("#DeltaX");
  gPad->SetGrid();
  
  
  
  ///===========================================================================
  ///===========================================================================
  ///===========================================================================
  TCanvas* cSM_DTHETAe_DPSIe_DPHIe = new TCanvas ("cSM_DTHETAe_DPSIe_DPHIe","cSM_DTHETAe_DPSIe_DPHIe",800,800);
  cSM_DTHETAe_DPSIe_DPHIe->Divide(2,3);
  
  
  ///===================================
  cSM_DTHETAe_DPSIe_DPHIe->cd(1);
  TGraphErrors* grDPHIe_EEMinus_Ele = new TGraphErrors(2,iSM_SM_Ele+2,DPHIe_SM_Mean+2,iSM_SM_Ele_RMS+2,DPHIe_SM_RMS+2);
  grDPHIe_EEMinus_Ele->SetTitle("EE- #Delta#phi Euler");
  grDPHIe_EEMinus_Ele->SetMarkerColor(kRed);
  grDPHIe_EEMinus_Ele->SetLineColor(kRed);
  grDPHIe_EEMinus_Ele->SetMarkerStyle(20);
  grDPHIe_EEMinus_Ele->SetMarkerSize(1);
  grDPHIe_EEMinus_Ele->SetLineWidth(1);
  grDPHIe_EEMinus_Ele->GetXaxis()->SetTitle("#phi");
  grDPHIe_EEMinus_Ele->GetYaxis()->SetTitle("#Delta#phi Euler ");
//   grDPHIe_EEMinus_Ele->Draw("AP");
//   gPad->SetGrid();
  
  TGraphErrors* grDPHIe_EEMinus_After = new TGraphErrors(2,iSM_SM_Ele+2,DPHIe_SM_Mean_After+2,iSM_SM_Ele_RMS,DPHIe_SM_RMS_After+2);
  grDPHIe_EEMinus_After->SetMarkerColor(kBlue);
  grDPHIe_EEMinus_After->SetLineColor(kBlue);
  grDPHIe_EEMinus_After->SetMarkerStyle(21);
  grDPHIe_EEMinus_After->SetMarkerSize(1);
  grDPHIe_EEMinus_After->SetLineWidth(1);
  grDPHIe_EEMinus_After->GetXaxis()->SetTitle("#phi");
  grDPHIe_EEMinus_After->GetYaxis()->SetTitle("#Delta#phi Euler ");
//   grDPHIe_EEMinus_After->Draw("P");
//   gPad->SetGrid();
  
  
  cSM_DTHETAe_DPSIe_DPHIe->cd(2);
  TGraphErrors* grDPHIe_EEPlus_Ele = new TGraphErrors(2,iSM_SM_Ele,DPHIe_SM_Mean,iSM_SM_Ele_RMS,DPHIe_SM_RMS);
  grDPHIe_EEPlus_Ele->SetTitle("EE+ #Delta#phi Euler");
  grDPHIe_EEPlus_Ele->SetMarkerColor(kRed);
  grDPHIe_EEPlus_Ele->SetLineColor(kRed);
  grDPHIe_EEPlus_Ele->SetMarkerStyle(20);
  grDPHIe_EEPlus_Ele->SetMarkerSize(1);
  grDPHIe_EEPlus_Ele->SetLineWidth(1);
  grDPHIe_EEPlus_Ele->GetXaxis()->SetTitle("#phi");
  grDPHIe_EEPlus_Ele->GetYaxis()->SetTitle("#Delta#phi Euler ");
  grDPHIe_EEPlus_Ele->Draw("AP");
  gPad->SetGrid();
  
  TGraphErrors* grDPHIe_EEPlus_After = new TGraphErrors(2,iSM_SM_Ele,DPHIe_SM_Mean_After,iSM_SM_Ele_RMS,DPHIe_SM_RMS_After);
  grDPHIe_EEPlus_After->SetMarkerColor(kBlue);
  grDPHIe_EEPlus_After->SetLineColor(kBlue);
  grDPHIe_EEPlus_After->SetMarkerStyle(21);
  grDPHIe_EEPlus_After->SetMarkerSize(1);
  grDPHIe_EEPlus_After->SetLineWidth(1);
  grDPHIe_EEPlus_After->GetXaxis()->SetTitle("#phi");
  grDPHIe_EEPlus_After->GetYaxis()->SetTitle("#Delta#phi Euler ");
  grDPHIe_EEPlus_After->Draw("P");
  gPad->SetGrid();
  
 

  ///===================================
  cSM_DTHETAe_DPSIe_DPHIe->cd(3);
  TGraphErrors* grDPSIe_EEMinus_Ele = new TGraphErrors(2,iSM_SM_Ele+2,DPSIe_SM_Mean+2,iSM_SM_Ele_RMS+2,DPSIe_SM_RMS+2);
  grDPSIe_EEMinus_Ele->SetTitle("EE- #Delta#phi Euler");
  grDPSIe_EEMinus_Ele->SetMarkerColor(kRed);
  grDPSIe_EEMinus_Ele->SetLineColor(kRed);
  grDPSIe_EEMinus_Ele->SetMarkerStyle(20);
  grDPSIe_EEMinus_Ele->SetMarkerSize(1);
  grDPSIe_EEMinus_Ele->SetLineWidth(1);
  grDPSIe_EEMinus_Ele->GetXaxis()->SetTitle("#phi");
  grDPSIe_EEMinus_Ele->GetYaxis()->SetTitle("#Delta#psi Euler ");
//   grDPSIe_EEMinus_Ele->Draw("AP");
//   gPad->SetGrid();
  
  TGraphErrors* grDPSIe_EEMinus_After = new TGraphErrors(2,iSM_SM_Ele+2,DPSIe_SM_Mean_After+2,iSM_SM_Ele_RMS+2,DPSIe_SM_RMS_After+2);
  grDPSIe_EEMinus_After->SetMarkerColor(kBlue);
  grDPSIe_EEMinus_After->SetLineColor(kBlue);
  grDPSIe_EEMinus_After->SetMarkerStyle(21);
  grDPSIe_EEMinus_After->SetMarkerSize(1);
  grDPSIe_EEMinus_After->SetLineWidth(1);
  grDPSIe_EEMinus_After->GetXaxis()->SetTitle("#phi");
  grDPSIe_EEMinus_After->GetYaxis()->SetTitle("#Delta#psi Euler ");
//   grDPSIe_EEMinus_After->Draw("P");
//   gPad->SetGrid();
  
  
  cSM_DTHETAe_DPSIe_DPHIe->cd(4);
  TGraphErrors* grDPSIe_EEPlus_Ele = new TGraphErrors(2,iSM_SM_Ele,DPSIe_SM_Mean,iSM_SM_Ele_RMS,DPSIe_SM_RMS);
  grDPSIe_EEPlus_Ele->SetTitle("EE+ #Delta#phi Euler");
  grDPSIe_EEPlus_Ele->SetMarkerColor(kRed);
  grDPSIe_EEPlus_Ele->SetLineColor(kRed);
  grDPSIe_EEPlus_Ele->SetMarkerStyle(20);
  grDPSIe_EEPlus_Ele->SetMarkerSize(1);
  grDPSIe_EEPlus_Ele->SetLineWidth(1);
  grDPSIe_EEPlus_Ele->GetXaxis()->SetTitle("#phi");
  grDPSIe_EEPlus_Ele->GetYaxis()->SetTitle("#Delta#psi Euler ");
  grDPSIe_EEPlus_Ele->Draw("AP");
  gPad->SetGrid();
  
  TGraphErrors* grDPSIe_EEPlus_After = new TGraphErrors(2,iSM_SM_Ele,DPSIe_SM_Mean_After,iSM_SM_Ele_RMS,DPSIe_SM_RMS_After);
  grDPSIe_EEPlus_After->SetMarkerColor(kBlue);
  grDPSIe_EEPlus_After->SetLineColor(kBlue);
  grDPSIe_EEPlus_After->SetMarkerStyle(21);
  grDPSIe_EEPlus_After->SetMarkerSize(1);
  grDPSIe_EEPlus_After->SetLineWidth(1);
  grDPSIe_EEPlus_After->GetXaxis()->SetTitle("#phi");
  grDPSIe_EEPlus_After->GetYaxis()->SetTitle("#Delta#psi Euler ");
  grDPSIe_EEPlus_After->Draw("P");
  gPad->SetGrid();
  
  ///===================================
  cSM_DTHETAe_DPSIe_DPHIe->cd(5);
  TGraphErrors* grDTHETAe_EEMinus_Ele = new TGraphErrors(2,iSM_SM_Ele+2,DTHETAe_SM_Mean+2,iSM_SM_Ele_RMS+2,DTHETAe_SM_RMS+2);
  grDTHETAe_EEMinus_Ele->SetTitle("EE- #Delta#theta Euler");
  grDTHETAe_EEMinus_Ele->SetMarkerColor(kRed);
  grDTHETAe_EEMinus_Ele->SetLineColor(kRed);
  grDTHETAe_EEMinus_Ele->SetMarkerStyle(20);
  grDTHETAe_EEMinus_Ele->SetMarkerSize(1);
  grDTHETAe_EEMinus_Ele->SetLineWidth(1);
  grDTHETAe_EEMinus_Ele->GetXaxis()->SetTitle("#phi");
  grDTHETAe_EEMinus_Ele->GetYaxis()->SetTitle("#Delta#theta Euler ");
//   grDTHETAe_EEMinus_Ele->Draw("AP");
//   gPad->SetGrid();
  
  TGraphErrors* grDTHETAe_EEMinus_After = new TGraphErrors(2,iSM_SM_Ele+2,DTHETAe_SM_Mean_After+2,iSM_SM_Ele_RMS+2,DTHETAe_SM_RMS_After+2);
  grDTHETAe_EEMinus_After->SetMarkerColor(kBlue);
  grDTHETAe_EEMinus_After->SetLineColor(kBlue);
  grDTHETAe_EEMinus_After->SetMarkerStyle(21);
  grDTHETAe_EEMinus_After->SetMarkerSize(1);
  grDTHETAe_EEMinus_After->SetLineWidth(1);
  grDTHETAe_EEMinus_After->GetXaxis()->SetTitle("#phi");
  grDTHETAe_EEMinus_After->GetYaxis()->SetTitle("#Delta#theta Euler ");
//   grDTHETAe_EEMinus_After->Draw("P");
//   gPad->SetGrid();
  
  
  cSM_DTHETAe_DPSIe_DPHIe->cd(6);
  TGraphErrors* grDTHETAe_EEPlus_Ele = new TGraphErrors(2,iSM_SM_Ele,DTHETAe_SM_Mean,iSM_SM_Ele_RMS,DTHETAe_SM_RMS);
  grDTHETAe_EEPlus_Ele->SetTitle("EE+ #Delta#theta Euler");
  grDTHETAe_EEPlus_Ele->SetMarkerColor(kRed);
  grDTHETAe_EEPlus_Ele->SetLineColor(kRed);
  grDTHETAe_EEPlus_Ele->SetMarkerStyle(20);
  grDTHETAe_EEPlus_Ele->SetMarkerSize(1);
  grDTHETAe_EEPlus_Ele->SetLineWidth(1);
  grDTHETAe_EEPlus_Ele->GetXaxis()->SetTitle("#phi");
  grDTHETAe_EEPlus_Ele->GetYaxis()->SetTitle("#Delta#theta Euler ");
  grDTHETAe_EEPlus_Ele->Draw("AP");
  gPad->SetGrid();
  
  TGraphErrors* grDTHETAe_EEPlus_After = new TGraphErrors(2,iSM_SM_Ele,DTHETAe_SM_Mean_After,iSM_SM_Ele_RMS,DTHETAe_SM_RMS_After);
  grDTHETAe_EEPlus_After->SetMarkerColor(kBlue);
  grDTHETAe_EEPlus_After->SetLineColor(kBlue);
  grDTHETAe_EEPlus_After->SetMarkerStyle(21);
  grDTHETAe_EEPlus_After->SetMarkerSize(1);
  grDTHETAe_EEPlus_After->SetLineWidth(1);
  grDTHETAe_EEPlus_After->GetXaxis()->SetTitle("#phi");
  grDTHETAe_EEPlus_After->GetYaxis()->SetTitle("#Delta#theta Euler ");
  grDTHETAe_EEPlus_After->Draw("P");
  gPad->SetGrid();
  
  
  
  ///==== draw ====
  cSM_DTHETAe_DPSIe_DPHIe->cd(1);
  TMultiGraph* mgr_DPHIe_EEMinus = new TMultiGraph();
  mgr_DPHIe_EEMinus->Add(grDPHIe_EEMinus_Ele);
  mgr_DPHIe_EEMinus->Add(grDPHIe_EEMinus_After);
  mgr_DPHIe_EEMinus->Draw("AP");
  mgr_DPHIe_EEMinus->GetXaxis()->SetTitle("iSM");
  mgr_DPHIe_EEMinus->GetYaxis()->SetTitle("#Delta#phi Euler ");
  gPad->SetGrid();
  
  cSM_DTHETAe_DPSIe_DPHIe->cd(3);
  TMultiGraph* mgr_DPSIe_EEMinus = new TMultiGraph();
  mgr_DPSIe_EEMinus->Add(grDPSIe_EEMinus_Ele);
  mgr_DPSIe_EEMinus->Add(grDPSIe_EEMinus_After);
  mgr_DPSIe_EEMinus->Draw("AP");
  mgr_DPSIe_EEMinus->GetXaxis()->SetTitle("iSM");
  mgr_DPSIe_EEMinus->GetYaxis()->SetTitle("#Delta#psi Euler ");
  gPad->SetGrid();
  
  cSM_DTHETAe_DPSIe_DPHIe->cd(5);
  TMultiGraph* mgr_DTHETAe_EEMinus = new TMultiGraph();
  mgr_DTHETAe_EEMinus->Add(grDTHETAe_EEMinus_Ele);
  mgr_DTHETAe_EEMinus->Add(grDTHETAe_EEMinus_After);
  mgr_DTHETAe_EEMinus->Draw("AP");
  mgr_DTHETAe_EEMinus->GetXaxis()->SetTitle("iSM");
  mgr_DTHETAe_EEMinus->GetYaxis()->SetTitle("#Delta#theta Euler ");
  gPad->SetGrid();
  
  
//   cSM_DTHETAe_DPSIe_DPHIe->cd(2);
//   TMultiGraph* mgr_DPHIe_EEPlus = new TMultiGraph();
//   mgr_DPHIe_EEPlus->Add(grDPHIe_EEPlus_Ele);
//   mgr_DPHIe_EEPlus->Add(grDPHIe_EEPlus_After);
//   mgr_DPHIe_EEPlus->Draw("AP");
//   mgr_DPHIe_EEPlus->GetXaxis()->SetTitle("iSM");
//   mgr_DPHIe_EEPlus->GetYaxis()->SetTitle("#Delta#Phi");
//   gPad->SetGrid();
  
//   cSM_DTHETAe_DPSIe_DPHIe->cd(4);
//   TMultiGraph* mgr_DPSIe_EEPlus = new TMultiGraph();
//   mgr_DPSIe_EEPlus->Add(grDPSIe_EEPlus_Ele);
//   mgr_DPSIe_EEPlus->Add(grDPSIe_EEPlus_After);
//   mgr_DPSIe_EEPlus->Draw("AP");
//   mgr_DPSIe_EEPlus->GetXaxis()->SetTitle("iSM");
//   mgr_DPSIe_EEPlus->GetYaxis()->SetTitle("#Delta#Psi");
//   gPad->SetGrid();
  
//   cSM_DTHETAe_DPSIe_DPHIe->cd(6);
//   TMultiGraph* mgr_DTHETAe_EEPlus = new TMultiGraph();
//   mgr_DTHETAe_EEPlus->Add(grDTHETAe_EEPlus_Ele);
//   mgr_DTHETAe_EEPlus->Add(grDTHETAe_EEPlus_After);
//   mgr_DTHETAe_EEPlus->Draw("AP");
//   mgr_DTHETAe_EEPlus->GetXaxis()->SetTitle("iSM");
//   mgr_DTHETAe_EEPlus->GetYaxis()->SetTitle("#Delta#Theta");
//   gPad->SetGrid();
  
  
  ///===========================================================================
  ///===========================================================================
  ///===========================================================================
  TCanvas* cDifference = new TCanvas ("cDifference","cDifference",900,350);
  cDifference->Divide(3,1);
  
  
  TH1F *hDXDYDZ = new TH1F("hDXDYDZ","#DeltaX#DeltaY#DeltaZ",300,-1.5,1.5);
  for (int i = 0; i<4; i++) hDXDYDZ->Fill(DX_SM_Mean_After[i] - DX_SM_Mean[i]);
  for (int i = 0; i<4; i++) hDXDYDZ->Fill(DY_SM_Mean_After[i] - DY_SM_Mean[i]);
  for (int i = 0; i<4; i++) hDXDYDZ->Fill(DZ_SM_Mean_After[i] - DZ_SM_Mean[i]);
  
  for (int i = 0; i<4; i++) {
   std::cout << " ~~~~~~~~~~~~~~~~~~ " << std::endl;
   std::cout << "  " << DX_SM_Mean_After[i] << " ~~~ " << DX_SM_Mean[i] << std::endl;
   std::cout << "  " << DY_SM_Mean_After[i] << " ~~~ " << DY_SM_Mean[i] << std::endl;
   std::cout << "  " << DZ_SM_Mean_After[i] << " ~~~ " << DZ_SM_Mean[i] << std::endl;
  }
  
  TH1F *hDX = new TH1F("hDX","#DeltaX",20,-1.5,1.5);
  for (int i = 0; i<4; i++) hDX->Fill(DX_SM_Mean_After[i] - DX_SM_Mean[i]);

  TH1F *hDY = new TH1F("hDY","#DeltaY",20,-1.5,1.5);
  for (int i = 0; i<4; i++) hDY->Fill(DY_SM_Mean_After[i] - DY_SM_Mean[i]);

  TH1F *hDZ = new TH1F("hDZ","#DeltaZ",20,-1.5,1.5);
  for (int i = 0; i<4; i++) hDZ->Fill(DZ_SM_Mean_After[i] - DZ_SM_Mean[i]);
 
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
  
  TCanvas* cDXDYDZ_EE = new TCanvas ("cDXDYDZ_EE","cDXDYDZ_EE",700,700);
  hDXDYDZ->SetLineColor(kRed);
  hDXDYDZ->SetFillColor(kRed);
  hDXDYDZ->SetFillStyle(3002);
  hDXDYDZ->Draw();
  funz->SetParameter(0,0);
  hDXDYDZ->Fit("gaus","RME");
  funz->FixParameter(0,funz->GetParameter(0));
  hDXDYDZ->Fit("gaus","RME");
  hDXDYDZ->GetXaxis()->SetTitle("cm");
  gPad->SetGrid();
  
  TCanvas* cDX_EE = new TCanvas ("cDX_EE","cDX_EE",700,700);
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
  
  TCanvas* cDY_EE = new TCanvas ("cDY_EE","cDY_EE",700,700);
  hDY->SetLineColor(kRed);
  hDY->SetFillColor(kRed);
  hDY->SetFillStyle(3002);
  hDY->Draw();
  funz->SetParameter(0,0);
  hDY->Fit("gaus","RME");
  funz->FixParameter(0,funz->GetParameter(0));
  hDY->GetXaxis()->SetTitle("#Deltay (cm)");
  gPad->SetGrid();
  
  TCanvas* cDZ_EE = new TCanvas ("cDZ_EE","cDZ_EE",700,700);
  hDZ->SetLineColor(kRed);
  hDZ->SetFillColor(kRed);
  hDZ->SetFillStyle(3002);
  hDZ->Draw();
  funz->SetParameter(0,0);
  hDZ->Fit("gaus","RME");
  hDZ->GetXaxis()->SetTitle("#Deltaz (cm)");
  gPad->SetGrid();
}


