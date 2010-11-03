{
 double Z = 3.205;

 
 
 ///===========================================================================
 std::string buffer;
 
 //==== kRed
 
 double DX_SM_Mean[4];
 double DX_SM_RMS[4];
 double DY_SM_Mean[4];
 double DY_SM_RMS[4];
 double DZ_SM_Mean[4];
 double DZ_SM_RMS[4];
 
 double DTHETAe_SM_Mean[4];
 double DTHETAe_SM_RMS[4];
 double DPSIe_SM_Mean[4];
 double DPSIe_SM_RMS[4];
 double DPHIe_SM_Mean[4];
 double DPHIe_SM_RMS[4];

//  std::ifstream file ("myEEAlignment_2010_Ieri.txt"); 
// std::ifstream file ("myEEAlignment_2010_Andrea.txt"); 
 std::ifstream file ("myEEAlignment_2010_30Oct.txt"); 
//  std::ifstream file ("myEEAlignment_2010_Plus.txt"); 
//std::ifstream file ("myEEAlignment_2010_Old.txt"); 
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

   std::cout << std::endl;
   iSM_After++;
  }
 }
 
 
 ///===========================================================================

//==== kBlue

 std::cerr << " New File ... " << std::endl;
 
//  std::ifstream fileEle ("myEEAlignment_2010_Minus.txt"); 
 std::ifstream fileEle ("myEEAlignment_2010.txt"); 
  
 double DX_SM_Mean_After[4];
 double DX_SM_RMS_After[4];
 double DY_SM_Mean_After[4];
 double DY_SM_RMS_After[4];
 double DZ_SM_Mean_After[4];
 double DZ_SM_RMS_After[4];
 
 double DTHETAe_SM_Mean_After[4];
 double DTHETAe_SM_RMS_After[4];
 double DPSIe_SM_Mean_After[4];
 double DPSIe_SM_RMS_After[4];
 double DPHIe_SM_Mean_After[4];
 double DPHIe_SM_RMS_After[4];

 double iSM_SM_Ele[4];
 double iSM_SM_Ele_RMS[38];
    
 if (!fileEle.is_open()){
  std::cerr << "noooo" << std::endl;
 }
 iSM_After = 0;
 while(!fileEle.eof()){
  getline(fileEle,buffer);
//   std::cout << "buffer = " << buffer; // << std::endl;
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
 grDZ_EEMinus_Ele->Draw("AP");
 gPad->SetGrid();
  
 TGraphErrors* grDZ_EEMinus_After = new TGraphErrors(2,iSM_SM_Ele+2,DZ_SM_Mean_After+2,iSM_SM_Ele_RMS,DZ_SM_RMS_After+2);
 grDZ_EEMinus_After->SetMarkerColor(kBlue);
 grDZ_EEMinus_After->SetLineColor(kBlue);
 grDZ_EEMinus_After->SetMarkerStyle(21);
 grDZ_EEMinus_After->SetMarkerSize(1);
 grDZ_EEMinus_After->SetLineWidth(1);
 grDZ_EEMinus_After->GetXaxis()->SetTitle("#phi");
 grDZ_EEMinus_After->GetYaxis()->SetTitle("#DeltaZ (cm)");
 grDZ_EEMinus_After->Draw("P");
 gPad->SetGrid();
  
  
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
 grDZ_EEPlus_Ele->Draw("AP");
 gPad->SetGrid();
  
 TGraphErrors* grDZ_EEPlus_After = new TGraphErrors(2,iSM_SM_Ele,DZ_SM_Mean_After,iSM_SM_Ele_RMS,DZ_SM_RMS_After);
 grDZ_EEPlus_After->SetMarkerColor(kBlue);
 grDZ_EEPlus_After->SetLineColor(kBlue);
 grDZ_EEPlus_After->SetMarkerStyle(21);
 grDZ_EEPlus_After->SetMarkerSize(1);
 grDZ_EEPlus_After->SetLineWidth(1);
 grDZ_EEPlus_After->GetXaxis()->SetTitle("#phi");
 grDZ_EEPlus_After->GetYaxis()->SetTitle("#DeltaZ (cm)");
 grDZ_EEPlus_After->Draw("P");
 gPad->SetGrid();
  
 

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
 grDY_EEMinus_Ele->Draw("AP");
 gPad->SetGrid();
  
 TGraphErrors* grDY_EEMinus_After = new TGraphErrors(2,iSM_SM_Ele+2,DY_SM_Mean_After+2,iSM_SM_Ele_RMS+2,DY_SM_RMS_After+2);
 grDY_EEMinus_After->SetMarkerColor(kBlue);
 grDY_EEMinus_After->SetLineColor(kBlue);
 grDY_EEMinus_After->SetMarkerStyle(21);
 grDY_EEMinus_After->SetMarkerSize(1);
 grDY_EEMinus_After->SetLineWidth(1);
 grDY_EEMinus_After->GetXaxis()->SetTitle("#phi");
 grDY_EEMinus_After->GetYaxis()->SetTitle("#DeltaY (cm)");
 grDY_EEMinus_After->Draw("P");
 gPad->SetGrid();
  
  
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
 grDY_EEPlus_Ele->Draw("AP");
 gPad->SetGrid();
  
 TGraphErrors* grDY_EEPlus_After = new TGraphErrors(2,iSM_SM_Ele,DY_SM_Mean_After,iSM_SM_Ele_RMS,DY_SM_RMS_After);
 grDY_EEPlus_After->SetMarkerColor(kBlue);
 grDY_EEPlus_After->SetLineColor(kBlue);
 grDY_EEPlus_After->SetMarkerStyle(21);
 grDY_EEPlus_After->SetMarkerSize(1);
 grDY_EEPlus_After->SetLineWidth(1);
 grDY_EEPlus_After->GetXaxis()->SetTitle("#phi");
 grDY_EEPlus_After->GetYaxis()->SetTitle("#DeltaY (cm)");
 grDY_EEPlus_After->Draw("P");
 gPad->SetGrid();
  
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
 grDX_EEMinus_Ele->Draw("AP");
 gPad->SetGrid();
  
 TGraphErrors* grDX_EEMinus_After = new TGraphErrors(2,iSM_SM_Ele+2,DX_SM_Mean_After+2,iSM_SM_Ele_RMS+2,DX_SM_RMS_After+2);
 grDX_EEMinus_After->SetMarkerColor(kBlue);
 grDX_EEMinus_After->SetLineColor(kBlue);
 grDX_EEMinus_After->SetMarkerStyle(21);
 grDX_EEMinus_After->SetMarkerSize(1);
 grDX_EEMinus_After->SetLineWidth(1);
 grDX_EEMinus_After->GetXaxis()->SetTitle("#phi");
 grDX_EEMinus_After->GetYaxis()->SetTitle("#DeltaX (cm)");
 grDX_EEMinus_After->Draw("P");
 gPad->SetGrid();
  
  
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
 grDX_EEPlus_Ele->Draw("AP");
 gPad->SetGrid();
  
 TGraphErrors* grDX_EEPlus_After = new TGraphErrors(2,iSM_SM_Ele,DX_SM_Mean_After,iSM_SM_Ele_RMS,DX_SM_RMS_After);
 grDX_EEPlus_After->SetMarkerColor(kBlue);
 grDX_EEPlus_After->SetLineColor(kBlue);
 grDX_EEPlus_After->SetMarkerStyle(21);
 grDX_EEPlus_After->SetMarkerSize(1);
 grDX_EEPlus_After->SetLineWidth(1);
 grDX_EEPlus_After->GetXaxis()->SetTitle("#phi");
 grDX_EEPlus_After->GetYaxis()->SetTitle("#DeltaX (cm)");
 grDX_EEPlus_After->Draw("P");
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
 grDPHIe_EEMinus_Ele->Draw("AP");
 gPad->SetGrid();
  
 TGraphErrors* grDPHIe_EEMinus_After = new TGraphErrors(2,iSM_SM_Ele+2,DPHIe_SM_Mean_After+2,iSM_SM_Ele_RMS,DPHIe_SM_RMS_After+2);
 grDPHIe_EEMinus_After->SetMarkerColor(kBlue);
 grDPHIe_EEMinus_After->SetLineColor(kBlue);
 grDPHIe_EEMinus_After->SetMarkerStyle(21);
 grDPHIe_EEMinus_After->SetMarkerSize(1);
 grDPHIe_EEMinus_After->SetLineWidth(1);
 grDPHIe_EEMinus_After->GetXaxis()->SetTitle("#phi");
 grDPHIe_EEMinus_After->GetYaxis()->SetTitle("#Delta#phi Euler ");
 grDPHIe_EEMinus_After->Draw("P");
 gPad->SetGrid();
  
  
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
 grDPSIe_EEMinus_Ele->GetYaxis()->SetTitle("#Delta#phi Euler ");
 grDPSIe_EEMinus_Ele->Draw("AP");
 gPad->SetGrid();
  
 TGraphErrors* grDPSIe_EEMinus_After = new TGraphErrors(2,iSM_SM_Ele+2,DPSIe_SM_Mean_After+2,iSM_SM_Ele_RMS+2,DPSIe_SM_RMS_After+2);
 grDPSIe_EEMinus_After->SetMarkerColor(kBlue);
 grDPSIe_EEMinus_After->SetLineColor(kBlue);
 grDPSIe_EEMinus_After->SetMarkerStyle(21);
 grDPSIe_EEMinus_After->SetMarkerSize(1);
 grDPSIe_EEMinus_After->SetLineWidth(1);
 grDPSIe_EEMinus_After->GetXaxis()->SetTitle("#phi");
 grDPSIe_EEMinus_After->GetYaxis()->SetTitle("#Delta#phi Euler ");
 grDPSIe_EEMinus_After->Draw("P");
 gPad->SetGrid();
  
  
 cSM_DTHETAe_DPSIe_DPHIe->cd(4);
 TGraphErrors* grDPSIe_EEPlus_Ele = new TGraphErrors(2,iSM_SM_Ele,DPSIe_SM_Mean,iSM_SM_Ele_RMS,DPSIe_SM_RMS);
 grDPSIe_EEPlus_Ele->SetTitle("EE+ #Delta#phi Euler");
 grDPSIe_EEPlus_Ele->SetMarkerColor(kRed);
 grDPSIe_EEPlus_Ele->SetLineColor(kRed);
 grDPSIe_EEPlus_Ele->SetMarkerStyle(20);
 grDPSIe_EEPlus_Ele->SetMarkerSize(1);
 grDPSIe_EEPlus_Ele->SetLineWidth(1);
 grDPSIe_EEPlus_Ele->GetXaxis()->SetTitle("#phi");
 grDPSIe_EEPlus_Ele->GetYaxis()->SetTitle("#Delta#phi Euler ");
 grDPSIe_EEPlus_Ele->Draw("AP");
 gPad->SetGrid();
  
 TGraphErrors* grDPSIe_EEPlus_After = new TGraphErrors(2,iSM_SM_Ele,DPSIe_SM_Mean_After,iSM_SM_Ele_RMS,DPSIe_SM_RMS_After);
 grDPSIe_EEPlus_After->SetMarkerColor(kBlue);
 grDPSIe_EEPlus_After->SetLineColor(kBlue);
 grDPSIe_EEPlus_After->SetMarkerStyle(21);
 grDPSIe_EEPlus_After->SetMarkerSize(1);
 grDPSIe_EEPlus_After->SetLineWidth(1);
 grDPSIe_EEPlus_After->GetXaxis()->SetTitle("#phi");
 grDPSIe_EEPlus_After->GetYaxis()->SetTitle("#Delta#phi Euler ");
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
 grDTHETAe_EEMinus_Ele->Draw("AP");
 gPad->SetGrid();
  
 TGraphErrors* grDTHETAe_EEMinus_After = new TGraphErrors(2,iSM_SM_Ele+2,DTHETAe_SM_Mean_After+2,iSM_SM_Ele_RMS+2,DTHETAe_SM_RMS_After+2);
 grDTHETAe_EEMinus_After->SetMarkerColor(kBlue);
 grDTHETAe_EEMinus_After->SetLineColor(kBlue);
 grDTHETAe_EEMinus_After->SetMarkerStyle(21);
 grDTHETAe_EEMinus_After->SetMarkerSize(1);
 grDTHETAe_EEMinus_After->SetLineWidth(1);
 grDTHETAe_EEMinus_After->GetXaxis()->SetTitle("#phi");
 grDTHETAe_EEMinus_After->GetYaxis()->SetTitle("#Delta#theta Euler ");
 grDTHETAe_EEMinus_After->Draw("P");
 gPad->SetGrid();
  
  
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
  
  
  
}


