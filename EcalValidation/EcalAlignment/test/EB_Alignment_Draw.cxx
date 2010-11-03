{
 double R = 1290;
 
 
 ///===========================================================================
 std::string buffer;
 
 //==== kRed
 
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

//  std::ifstream file ("Survey.txt"); 
 
//  std::ifstream file ("myEBAlignment_2010_Ieri.txt"); 
// std::ifstream file ("myEBAlignment_2010_Andrea.txt"); 
 std::ifstream file ("myEBAlignment_2010_30Oct.txt"); 
// std::ifstream file ("myEBAlignment_2010_Plus.txt"); 
//std::ifstream file ("myEBAlignment_2010_Old.txt"); 
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

// //==== if survey
/*    if (iSM_After<18){
   DZ_SM_Mean[iSM_After] -= +1.64;
  }
   else {
   DZ_SM_Mean[iSM_After] -= +2.84;
  }    
   DX_SM_Mean[iSM_After] /= 10;
   DY_SM_Mean[iSM_After] /= 10;
   DZ_SM_Mean[iSM_After] /= 10;*/
    
    
   std::cout << std::endl;
   iSM_After++;
  }
 }
 
 
 ///===========================================================================

//==== kBlue

 std::cerr << " New File ... " << std::endl;
 
//  std::ifstream fileEle ("myEBAlignment_2010_Minus.txt"); 
 std::ifstream fileEle ("myEBAlignment_2010.txt"); 
//   std::ifstream fileEle ("myEBAlignment_2010_Andrea.txt"); 
  
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
 grDZ_EBMinus_Ele->GetXaxis()->SetTitle("#phi");
 grDZ_EBMinus_Ele->GetYaxis()->SetTitle("#DeltaZ (cm)");
 grDZ_EBMinus_Ele->Draw("AP");
 gPad->SetGrid();
  
 TGraphErrors* grDZ_EBMinus_After = new TGraphErrors(18,iSM_SM_Ele+18,DZ_SM_Mean_After+18,iSM_SM_Ele_RMS,DZ_SM_RMS_After+18);
 grDZ_EBMinus_After->SetMarkerColor(kBlue);
 grDZ_EBMinus_After->SetLineColor(kBlue);
 grDZ_EBMinus_After->SetMarkerStyle(21);
 grDZ_EBMinus_After->SetMarkerSize(1);
 grDZ_EBMinus_After->SetLineWidth(1);
 grDZ_EBMinus_After->GetXaxis()->SetTitle("#phi");
 grDZ_EBMinus_After->GetYaxis()->SetTitle("#DeltaZ (cm)");
 grDZ_EBMinus_After->Draw("P");
 gPad->SetGrid();
  
  
 cSM_DX_DY_DZ->cd(2);
 TGraphErrors* grDZ_EBPlus_Ele = new TGraphErrors(18,iSM_SM_Ele,DZ_SM_Mean,iSM_SM_Ele_RMS,DZ_SM_RMS);
 grDZ_EBPlus_Ele->SetTitle("EB+ #DeltaZ");
 grDZ_EBPlus_Ele->SetMarkerColor(kRed);
 grDZ_EBPlus_Ele->SetLineColor(kRed);
 grDZ_EBPlus_Ele->SetMarkerStyle(20);
 grDZ_EBPlus_Ele->SetMarkerSize(1);
 grDZ_EBPlus_Ele->SetLineWidth(1);
 grDZ_EBPlus_Ele->GetXaxis()->SetTitle("#phi");
 grDZ_EBPlus_Ele->GetYaxis()->SetTitle("#DeltaZ (cm)");
 grDZ_EBPlus_Ele->Draw("AP");
 gPad->SetGrid();
  
 TGraphErrors* grDZ_EBPlus_After = new TGraphErrors(18,iSM_SM_Ele,DZ_SM_Mean_After,iSM_SM_Ele_RMS,DZ_SM_RMS_After);
 grDZ_EBPlus_After->SetMarkerColor(kBlue);
 grDZ_EBPlus_After->SetLineColor(kBlue);
 grDZ_EBPlus_After->SetMarkerStyle(21);
 grDZ_EBPlus_After->SetMarkerSize(1);
 grDZ_EBPlus_After->SetLineWidth(1);
 grDZ_EBPlus_After->GetXaxis()->SetTitle("#phi");
 grDZ_EBPlus_After->GetYaxis()->SetTitle("#DeltaZ (cm)");
 grDZ_EBPlus_After->Draw("P");
 gPad->SetGrid();
  
 

  ///===================================
 cSM_DX_DY_DZ->cd(3);
 TGraphErrors* grDY_EBMinus_Ele = new TGraphErrors(18,iSM_SM_Ele+18,DY_SM_Mean+18,iSM_SM_Ele_RMS+18,DY_SM_RMS+18);
 grDY_EBMinus_Ele->SetTitle("EB- #DeltaY");
 grDY_EBMinus_Ele->SetMarkerColor(kRed);
 grDY_EBMinus_Ele->SetLineColor(kRed);
 grDY_EBMinus_Ele->SetMarkerStyle(20);
 grDY_EBMinus_Ele->SetMarkerSize(1);
 grDY_EBMinus_Ele->SetLineWidth(1);
 grDY_EBMinus_Ele->GetXaxis()->SetTitle("#phi");
 grDY_EBMinus_Ele->GetYaxis()->SetTitle("#DeltaY (cm)");
 grDY_EBMinus_Ele->Draw("AP");
 gPad->SetGrid();
  
 TGraphErrors* grDY_EBMinus_After = new TGraphErrors(18,iSM_SM_Ele+18,DY_SM_Mean_After+18,iSM_SM_Ele_RMS+18,DY_SM_RMS_After+18);
 grDY_EBMinus_After->SetMarkerColor(kBlue);
 grDY_EBMinus_After->SetLineColor(kBlue);
 grDY_EBMinus_After->SetMarkerStyle(21);
 grDY_EBMinus_After->SetMarkerSize(1);
 grDY_EBMinus_After->SetLineWidth(1);
 grDY_EBMinus_After->GetXaxis()->SetTitle("#phi");
 grDY_EBMinus_After->GetYaxis()->SetTitle("#DeltaY (cm)");
 grDY_EBMinus_After->Draw("P");
 gPad->SetGrid();
  
  
 cSM_DX_DY_DZ->cd(4);
 TGraphErrors* grDY_EBPlus_Ele = new TGraphErrors(18,iSM_SM_Ele,DY_SM_Mean,iSM_SM_Ele_RMS,DY_SM_RMS);
 grDY_EBPlus_Ele->SetTitle("EB+ #DeltaY");
 grDY_EBPlus_Ele->SetMarkerColor(kRed);
 grDY_EBPlus_Ele->SetLineColor(kRed);
 grDY_EBPlus_Ele->SetMarkerStyle(20);
 grDY_EBPlus_Ele->SetMarkerSize(1);
 grDY_EBPlus_Ele->SetLineWidth(1);
 grDY_EBPlus_Ele->GetXaxis()->SetTitle("#phi");
 grDY_EBPlus_Ele->GetYaxis()->SetTitle("#DeltaY (cm)");
 grDY_EBPlus_Ele->Draw("AP");
 gPad->SetGrid();
  
 TGraphErrors* grDY_EBPlus_After = new TGraphErrors(18,iSM_SM_Ele,DY_SM_Mean_After,iSM_SM_Ele_RMS,DY_SM_RMS_After);
 grDY_EBPlus_After->SetMarkerColor(kBlue);
 grDY_EBPlus_After->SetLineColor(kBlue);
 grDY_EBPlus_After->SetMarkerStyle(21);
 grDY_EBPlus_After->SetMarkerSize(1);
 grDY_EBPlus_After->SetLineWidth(1);
 grDY_EBPlus_After->GetXaxis()->SetTitle("#phi");
 grDY_EBPlus_After->GetYaxis()->SetTitle("#DeltaY (cm)");
 grDY_EBPlus_After->Draw("P");
 gPad->SetGrid();
  
  ///===================================
 cSM_DX_DY_DZ->cd(5);
 TGraphErrors* grDX_EBMinus_Ele = new TGraphErrors(18,iSM_SM_Ele+18,DX_SM_Mean+18,iSM_SM_Ele_RMS+18,DX_SM_RMS+18);
 grDX_EBMinus_Ele->SetTitle("EB- #DeltaX");
 grDX_EBMinus_Ele->SetMarkerColor(kRed);
 grDX_EBMinus_Ele->SetLineColor(kRed);
 grDX_EBMinus_Ele->SetMarkerStyle(20);
 grDX_EBMinus_Ele->SetMarkerSize(1);
 grDX_EBMinus_Ele->SetLineWidth(1);
 grDX_EBMinus_Ele->GetXaxis()->SetTitle("#phi");
 grDX_EBMinus_Ele->GetYaxis()->SetTitle("#DeltaX (cm)");
 grDX_EBMinus_Ele->Draw("AP");
 gPad->SetGrid();
  
 TGraphErrors* grDX_EBMinus_After = new TGraphErrors(18,iSM_SM_Ele+18,DX_SM_Mean_After+18,iSM_SM_Ele_RMS+18,DX_SM_RMS_After+18);
 grDX_EBMinus_After->SetMarkerColor(kBlue);
 grDX_EBMinus_After->SetLineColor(kBlue);
 grDX_EBMinus_After->SetMarkerStyle(21);
 grDX_EBMinus_After->SetMarkerSize(1);
 grDX_EBMinus_After->SetLineWidth(1);
 grDX_EBMinus_After->GetXaxis()->SetTitle("#phi");
 grDX_EBMinus_After->GetYaxis()->SetTitle("#DeltaX (cm)");
 grDX_EBMinus_After->Draw("P");
 gPad->SetGrid();
  
  
 cSM_DX_DY_DZ->cd(6);
 TGraphErrors* grDX_EBPlus_Ele = new TGraphErrors(18,iSM_SM_Ele,DX_SM_Mean,iSM_SM_Ele_RMS,DX_SM_RMS);
 grDX_EBPlus_Ele->SetTitle("EB+ #DeltaX");
 grDX_EBPlus_Ele->SetMarkerColor(kRed);
 grDX_EBPlus_Ele->SetLineColor(kRed);
 grDX_EBPlus_Ele->SetMarkerStyle(20);
 grDX_EBPlus_Ele->SetMarkerSize(1);
 grDX_EBPlus_Ele->SetLineWidth(1);
 grDX_EBPlus_Ele->GetXaxis()->SetTitle("#phi");
 grDX_EBPlus_Ele->GetYaxis()->SetTitle("#DeltaX (cm)");
 grDX_EBPlus_Ele->Draw("AP");
 gPad->SetGrid();
  
 TGraphErrors* grDX_EBPlus_After = new TGraphErrors(18,iSM_SM_Ele,DX_SM_Mean_After,iSM_SM_Ele_RMS,DX_SM_RMS_After);
 grDX_EBPlus_After->SetMarkerColor(kBlue);
 grDX_EBPlus_After->SetLineColor(kBlue);
 grDX_EBPlus_After->SetMarkerStyle(21);
 grDX_EBPlus_After->SetMarkerSize(1);
 grDX_EBPlus_After->SetLineWidth(1);
 grDX_EBPlus_After->GetXaxis()->SetTitle("#phi");
 grDX_EBPlus_After->GetYaxis()->SetTitle("#DeltaX (cm)");
 grDX_EBPlus_After->Draw("P");
 gPad->SetGrid();
  
  
  
  
  
  
  
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
  
  
  
  
}


