void draw_evolution(TString nameInFileList, TString nameOutputDir){
 
 gStyle->SetTitleXOffset(0.8);
 gStyle->SetTitleYOffset(1.0);
 
 gStyle->SetLabelOffset(0.005, "XYZ");
 gStyle->SetTitleSize(0.07, "XYZ");
 gStyle->SetTitleFont(22,"X");
 gStyle->SetTitleFont(22,"Y");
 
 gStyle->SetPadLeftMargin(0.20);
 gStyle->SetPadRightMargin(0.20);
 gStyle->SetPadTopMargin(0.20);
 gStyle->SetPadBottomMargin(0.20);
 gROOT->ForceStyle();
 
 
 TString toDoShell;
 toDoShell = Form("rm -r %s ;",nameOutputDir.Data());
 system(toDoShell.Data());
 toDoShell = Form("mkdir %s ;",nameOutputDir.Data());
 system(toDoShell.Data());
 toDoShell = Form("mkdir %s/plot ;",nameOutputDir.Data());
 system(toDoShell.Data());
 
 
 
 
 std::ifstream indata;
 
 std::cerr << "File " << nameInFileList.Data() << std::endl;
 
 indata.open(nameInFileList.Data()); // opens the file
 if(!indata) { // file couldn't be opened
  std::cerr << "Error: file " << nameInFileList.Data() << " could not be opened" << std::endl;
  gApplication->Terminate(0);
 }
 
 std::vector<std::string> nameRuns;
 std::vector<int> numRuns;
 std::vector<std::string> nameRunsHR;
 
 std::string tempString;
 std::string tempString2;
 int tempRun;
 while ( !indata.eof() ) { // keep reading until end-of-file
  indata >>  tempString;
  indata >> tempRun;
  nameRuns.push_back(tempString);
  numRuns.push_back(tempRun);
  std::cout << " " <<  tempString << " : " << tempRun << " : ";
  indata >>  tempString2;
  nameRunsHR.push_back(tempString2);
  std::cout << " " <<  tempString2 << std::endl;
 }
 indata.close(); 

 
 
 double X[100];
 double errX[100];
 
 
 
 double EEPlus_DATA_Dphi_ePlus[100];
 double EEPlus_DATA_Dphi_eMinus[100];
 double EEPlus_DATA_Deta[100];
 double EEPlus_DATA_err_Dphi_ePlus[100];
 double EEPlus_DATA_err_Dphi_eMinus[100];
 double EEPlus_DATA_err_Deta[100];
 
 
 double EBPlus_DATA_Dphi_ePlus[100];
 double EBPlus_DATA_Dphi_eMinus[100];
 double EBPlus_DATA_Deta[100];
 double EBPlus_DATA_err_Dphi_ePlus[100];
 double EBPlus_DATA_err_Dphi_eMinus[100];
 double EBPlus_DATA_err_Deta[100];
 
 
 double EBMinus_DATA_Dphi_ePlus[100];
 double EBMinus_DATA_Dphi_eMinus[100];
 double EBMinus_DATA_Deta[100];
 double EBMinus_DATA_err_Dphi_ePlus[100];
 double EBMinus_DATA_err_Dphi_eMinus[100];
 double EBMinus_DATA_err_Deta[100];
 
 
 double EEMinus_DATA_Dphi_ePlus[100];
 double EEMinus_DATA_Dphi_eMinus[100];
 double EEMinus_DATA_Deta[100];
 double EEMinus_DATA_err_Dphi_ePlus[100];
 double EEMinus_DATA_err_Dphi_eMinus[100];
 double EEMinus_DATA_err_Deta[100];
 
 
 
 double EEPlus_MC_Dphi_ePlus[100];
 double EEPlus_MC_Dphi_eMinus[100];
 double EEPlus_MC_Deta[100];
 double EEPlus_MC_err_Dphi_ePlus[100];
 double EEPlus_MC_err_Dphi_eMinus[100];
 double EEPlus_MC_err_Deta[100];
 
 
 double EBPlus_MC_Dphi_ePlus[100];
 double EBPlus_MC_Dphi_eMinus[100];
 double EBPlus_MC_Deta[100];
 double EBPlus_MC_err_Dphi_ePlus[100];
 double EBPlus_MC_err_Dphi_eMinus[100];
 double EBPlus_MC_err_Deta[100];
 
 
 double EBMinus_MC_Dphi_ePlus[100];
 double EBMinus_MC_Dphi_eMinus[100];
 double EBMinus_MC_Deta[100];
 double EBMinus_MC_err_Dphi_ePlus[100];
 double EBMinus_MC_err_Dphi_eMinus[100];
 double EBMinus_MC_err_Deta[100];
 
 
 double EEMinus_MC_Dphi_ePlus[100];
 double EEMinus_MC_Dphi_eMinus[100];
 double EEMinus_MC_Deta[100];
 double EEMinus_MC_err_Dphi_ePlus[100];
 double EEMinus_MC_err_Dphi_eMinus[100];
 double EEMinus_MC_err_Deta[100];
 
 for (int i=0; i<nameRuns.size(); i++){
  std::ifstream indata_temp;
  TString tempName = Form ("%s/result.txt",nameRuns.at(i).c_str());
  indata_temp.open(tempName.Data()); // opens the file
  if(!indata_temp) { // file couldn't be opened
  std::cerr << "Error: file " << tempName.Data() << " could not be opened" << std::endl;
  gApplication->Terminate(0);
  }
  std::cout << "tempName = " << tempName.Data() << std::endl;
  
  X[i] = numRuns.at(i);
  errX[i] = 0;
  
  int counter = 0;
  while ( !indata_temp.eof() && counter<24) { // keep reading until end-of-file
   ///~~~~ DATA ~~~~
   if (counter == 0) {
    indata_temp >> EEPlus_DATA_Dphi_ePlus[i]; // std::cout << " EEPlus_DATA_Dphi_ePlus[" << i << "] = " << EEPlus_DATA_Dphi_ePlus[i] << std::endl;
    indata_temp >> EEPlus_DATA_err_Dphi_ePlus[i];
   }
   if (counter == 1) {
    indata_temp >> EEPlus_DATA_Dphi_eMinus[i];
    indata_temp >> EEPlus_DATA_err_Dphi_eMinus[i];
   }
   if (counter == 2) {
    indata_temp >> EEPlus_DATA_Deta[i];
    indata_temp >> EEPlus_DATA_err_Deta[i];
   }

   if (counter == 3) {
    indata_temp >> EBPlus_DATA_Dphi_ePlus[i];
    indata_temp >> EBPlus_DATA_err_Dphi_ePlus[i];
   }
   if (counter == 4) {
    indata_temp >> EBPlus_DATA_Dphi_eMinus[i];
    indata_temp >> EBPlus_DATA_err_Dphi_eMinus[i];
   }
   if (counter == 5) {
    indata_temp >> EBPlus_DATA_Deta[i];
    indata_temp >> EBPlus_DATA_err_Deta[i];
   }
   
   if (counter == 6) {
    indata_temp >> EBMinus_DATA_Dphi_ePlus[i];
    indata_temp >> EBMinus_DATA_err_Dphi_ePlus[i];
   }
   if (counter == 7) {
    indata_temp >> EBMinus_DATA_Dphi_eMinus[i];
    indata_temp >> EBMinus_DATA_err_Dphi_eMinus[i];
   }
   if (counter == 8) {
    indata_temp >> EBMinus_DATA_Deta[i];
    indata_temp >> EBMinus_DATA_err_Deta[i];
   }
   
   if (counter == 9) {
    indata_temp >> EEMinus_DATA_Dphi_ePlus[i];
    indata_temp >> EEMinus_DATA_err_Dphi_ePlus[i];
   }
   if (counter == 10) {
    indata_temp >> EEMinus_DATA_Dphi_eMinus[i];
    indata_temp >> EEMinus_DATA_err_Dphi_eMinus[i];
   }
   if (counter == 11) {
    indata_temp >> EEMinus_DATA_Deta[i];
    indata_temp >> EEMinus_DATA_err_Deta[i];
   }
   
   ///~~~~ MC ~~~~
   if (counter == 12) {
    indata_temp >> EEPlus_MC_Dphi_ePlus[i];
    indata_temp >> EEPlus_MC_err_Dphi_ePlus[i];
   }
   if (counter == 13) {
    indata_temp >> EEPlus_MC_Dphi_eMinus[i];
    indata_temp >> EEPlus_MC_err_Dphi_eMinus[i];
   }
   if (counter == 14) {
    indata_temp >> EEPlus_MC_Deta[i];
    indata_temp >> EEPlus_MC_err_Deta[i];
   }
   
   if (counter == 15) {
    indata_temp >> EBPlus_MC_Dphi_ePlus[i];
    indata_temp >> EBPlus_MC_err_Dphi_ePlus[i];
   }
   if (counter == 16) {
    indata_temp >> EBPlus_MC_Dphi_eMinus[i];
    indata_temp >> EBPlus_MC_err_Dphi_eMinus[i];
   }
   if (counter == 17) {
    indata_temp >> EBPlus_MC_Deta[i];
    indata_temp >> EBPlus_MC_err_Deta[i];
   }
   
   if (counter == 18) {
    indata_temp >> EBMinus_MC_Dphi_ePlus[i];
    indata_temp >> EBMinus_MC_err_Dphi_ePlus[i];
   }
   if (counter == 19) {
    indata_temp >> EBMinus_MC_Dphi_eMinus[i];
    indata_temp >> EBMinus_MC_err_Dphi_eMinus[i];
   }
   if (counter == 20) {
    indata_temp >> EBMinus_MC_Deta[i];
    indata_temp >> EBMinus_MC_err_Deta[i];
   }
   
   if (counter == 21) {
    indata_temp >> EEMinus_MC_Dphi_ePlus[i];
    indata_temp >> EEMinus_MC_err_Dphi_ePlus[i];
   }
   if (counter == 22) {
    indata_temp >> EEMinus_MC_Dphi_eMinus[i];
    indata_temp >> EEMinus_MC_err_Dphi_eMinus[i];
   }
   if (counter == 23) {
    indata_temp >> EEMinus_MC_Deta[i];
    indata_temp >> EEMinus_MC_err_Deta[i];
   }
  counter ++;
  }
  indata_temp.close();  
 }
 
 
 ///==== Graphs ====
 
 int numBin = nameRuns.size();
 std::cout << " numBin = " << numBin << std::endl;
 
 
 TGraphErrors* grEEPlus_DATA_Dphi_ePlus = new TGraphErrors(numBin,X,EEPlus_DATA_Dphi_ePlus,errX, EEPlus_DATA_err_Dphi_ePlus);
 TGraphErrors* grEEPlus_DATA_Dphi_eMinus = new TGraphErrors(numBin,X,EEPlus_DATA_Dphi_eMinus,errX, EEPlus_DATA_err_Dphi_eMinus);
 TGraphErrors* grEEPlus_DATA_Deta = new TGraphErrors(numBin,X,EEPlus_DATA_Deta,errX, EEPlus_DATA_err_Deta);

 TGraphErrors* grEBPlus_DATA_Dphi_ePlus = new TGraphErrors(numBin,X,EBPlus_DATA_Dphi_ePlus,errX, EBPlus_DATA_err_Dphi_ePlus);
 TGraphErrors* grEBPlus_DATA_Dphi_eMinus = new TGraphErrors(numBin,X,EBPlus_DATA_Dphi_eMinus,errX, EBPlus_DATA_err_Dphi_eMinus);
 TGraphErrors* grEBPlus_DATA_Deta = new TGraphErrors(numBin,X,EBPlus_DATA_Deta,errX, EBPlus_DATA_err_Deta);
 
 TGraphErrors* grEBMinus_DATA_Dphi_ePlus = new TGraphErrors(numBin,X,EBMinus_DATA_Dphi_ePlus,errX, EBMinus_DATA_err_Dphi_ePlus);
 TGraphErrors* grEBMinus_DATA_Dphi_eMinus = new TGraphErrors(numBin,X,EBMinus_DATA_Dphi_eMinus,errX, EBMinus_DATA_err_Dphi_eMinus);
 TGraphErrors* grEBMinus_DATA_Deta = new TGraphErrors(numBin,X,EBMinus_DATA_Deta,errX, EBMinus_DATA_err_Deta);
 
 TGraphErrors* grEEMinus_DATA_Dphi_ePlus = new TGraphErrors(numBin,X,EEMinus_DATA_Dphi_ePlus,errX, EEMinus_DATA_err_Dphi_ePlus);
 TGraphErrors* grEEMinus_DATA_Dphi_eMinus = new TGraphErrors(numBin,X,EEMinus_DATA_Dphi_eMinus,errX, EEMinus_DATA_err_Dphi_eMinus);
 TGraphErrors* grEEMinus_DATA_Deta = new TGraphErrors(numBin,X,EEMinus_DATA_Deta,errX, EEMinus_DATA_err_Deta);
 
 
 TGraphErrors* grEEPlus_MC_Dphi_ePlus = new TGraphErrors(numBin,X,EEPlus_MC_Dphi_ePlus,errX, EEPlus_MC_err_Dphi_ePlus);
 TGraphErrors* grEEPlus_MC_Dphi_eMinus = new TGraphErrors(numBin,X,EEPlus_MC_Dphi_eMinus,errX, EEPlus_MC_err_Dphi_eMinus);
 TGraphErrors* grEEPlus_MC_Deta = new TGraphErrors(numBin,X,EEPlus_MC_Deta,errX, EEPlus_MC_err_Deta);
 
 TGraphErrors* grEBPlus_MC_Dphi_ePlus = new TGraphErrors(numBin,X,EBPlus_MC_Dphi_ePlus,errX, EBPlus_MC_err_Dphi_ePlus);
 TGraphErrors* grEBPlus_MC_Dphi_eMinus = new TGraphErrors(numBin,X,EBPlus_MC_Dphi_eMinus,errX, EBPlus_MC_err_Dphi_eMinus);
 TGraphErrors* grEBPlus_MC_Deta = new TGraphErrors(numBin,X,EBPlus_MC_Deta,errX, EBPlus_MC_err_Deta);
 
 TGraphErrors* grEBMinus_MC_Dphi_ePlus = new TGraphErrors(numBin,X,EBMinus_MC_Dphi_ePlus,errX, EBMinus_MC_err_Dphi_ePlus);
 TGraphErrors* grEBMinus_MC_Dphi_eMinus = new TGraphErrors(numBin,X,EBMinus_MC_Dphi_eMinus,errX, EBMinus_MC_err_Dphi_eMinus);
 TGraphErrors* grEBMinus_MC_Deta = new TGraphErrors(numBin,X,EBMinus_MC_Deta,errX, EBMinus_MC_err_Deta);
 
 TGraphErrors* grEEMinus_MC_Dphi_ePlus = new TGraphErrors(numBin,X,EEMinus_MC_Dphi_ePlus,errX, EEMinus_MC_err_Dphi_ePlus);
 TGraphErrors* grEEMinus_MC_Dphi_eMinus = new TGraphErrors(numBin,X,EEMinus_MC_Dphi_eMinus,errX, EEMinus_MC_err_Dphi_eMinus);
 TGraphErrors* grEEMinus_MC_Deta = new TGraphErrors(numBin,X,EEMinus_MC_Deta,errX, EEMinus_MC_err_Deta);
 
 
 TString toDoROOT;
 
 ///==== EE+ ====
 TCanvas* ccEEPlus_Dphi_ePlus = new TCanvas ("EEPlus_Dphi_ePlus","EE+ #Delta#phi e+", 800, 800);
 TMultiGraph* mgr_EEPlus_Dphi_ePlus = new TMultiGraph(); 
 grEEPlus_DATA_Dphi_ePlus->SetMarkerSize(2);
 grEEPlus_DATA_Dphi_ePlus->SetMarkerStyle(20);
 grEEPlus_DATA_Dphi_ePlus->SetLineWidth(2);
 grEEPlus_DATA_Dphi_ePlus->SetMarkerColor(kRed);
 grEEPlus_DATA_Dphi_ePlus->SetLineColor(kRed);
 grEEPlus_DATA_Dphi_ePlus->SetLineStyle(2);
 
 grEEPlus_MC_Dphi_ePlus->SetMarkerSize(1);
 grEEPlus_MC_Dphi_ePlus->SetMarkerStyle(20);
 grEEPlus_MC_Dphi_ePlus->SetLineWidth(1);
 grEEPlus_MC_Dphi_ePlus->SetMarkerColor(kBlue);
 grEEPlus_MC_Dphi_ePlus->SetLineColor(kBlue);
 grEEPlus_MC_Dphi_ePlus->SetLineStyle(1);
 
 mgr_EEPlus_Dphi_ePlus->Add(grEEPlus_DATA_Dphi_ePlus);
 mgr_EEPlus_Dphi_ePlus->Add(grEEPlus_MC_Dphi_ePlus);
 mgr_EEPlus_Dphi_ePlus->Draw("AP");
 mgr_EEPlus_Dphi_ePlus->GetXaxis()->SetTitle(" ");
 mgr_EEPlus_Dphi_ePlus->GetYaxis()->SetTitle("#Delta#phi e+ EE+"); 
 for (int i=0; (i< (numBin-1)); i++) {
  int bin = mgr_EEPlus_Dphi_ePlus->GetXaxis()->FindBin(i+1);
  mgr_EEPlus_Dphi_ePlus->GetXaxis()->SetBinLabel(bin,nameRunsHR.at(i).c_str());
  std::cout << " bin = " << bin << std::endl;
 } 
 gPad->SetGrid();
 toDoROOT = Form("%s/plot/Dphi_EEPlus_eplus.png",nameOutputDir.Data());
 ccEEPlus_Dphi_ePlus->SaveAs(toDoROOT);
 
 TCanvas* ccEEPlus_Dphi_eMinus = new TCanvas ("EEPlus_Dphi_eMinus","EE+ #Delta#phi e-", 800, 800);
 TMultiGraph* mgr_EEPlus_Dphi_eMinus = new TMultiGraph(); 
 grEEPlus_DATA_Dphi_eMinus->SetMarkerSize(2);
 grEEPlus_DATA_Dphi_eMinus->SetMarkerStyle(20);
 grEEPlus_DATA_Dphi_eMinus->SetLineWidth(2);
 grEEPlus_DATA_Dphi_eMinus->SetMarkerColor(kRed);
 grEEPlus_DATA_Dphi_eMinus->SetLineColor(kRed);
 grEEPlus_DATA_Dphi_eMinus->SetLineStyle(2);
 
 grEEPlus_MC_Dphi_eMinus->SetMarkerSize(1);
 grEEPlus_MC_Dphi_eMinus->SetMarkerStyle(20);
 grEEPlus_MC_Dphi_eMinus->SetLineWidth(1);
 grEEPlus_MC_Dphi_eMinus->SetMarkerColor(kBlue);
 grEEPlus_MC_Dphi_eMinus->SetLineColor(kBlue);
 grEEPlus_MC_Dphi_eMinus->SetLineStyle(1);
 
 mgr_EEPlus_Dphi_eMinus->Add(grEEPlus_DATA_Dphi_eMinus);
 mgr_EEPlus_Dphi_eMinus->Add(grEEPlus_MC_Dphi_eMinus);
 mgr_EEPlus_Dphi_eMinus->Draw("AP");
 mgr_EEPlus_Dphi_eMinus->GetXaxis()->SetTitle(" ");
 mgr_EEPlus_Dphi_eMinus->GetYaxis()->SetTitle("#Delta#phi e- EE+");
 for (int i=0; (i< (numBin-1)); i++) {
  int bin = mgr_EEPlus_Dphi_eMinus->GetXaxis()->FindBin(i+1);
  mgr_EEPlus_Dphi_eMinus->GetXaxis()->SetBinLabel(bin,nameRunsHR.at(i).c_str());
 } 
 gPad->SetGrid();
 toDoROOT = Form("%s/plot/Dphi_EEPlus_eminus.png",nameOutputDir.Data());
 ccEEPlus_Dphi_eMinus->SaveAs(toDoROOT);
 
 TCanvas* ccEEPlus_Deta = new TCanvas ("EEPlus_Deta","EE+ #Delta#eta", 800, 800);
 TMultiGraph* mgr_EEPlus_Deta = new TMultiGraph(); 
 grEEPlus_DATA_Deta->SetMarkerSize(2);
 grEEPlus_DATA_Deta->SetMarkerStyle(20);
 grEEPlus_DATA_Deta->SetLineWidth(2);
 grEEPlus_DATA_Deta->SetMarkerColor(kRed);
 grEEPlus_DATA_Deta->SetLineColor(kRed);
 grEEPlus_DATA_Deta->SetLineStyle(2);
 
 grEEPlus_MC_Deta->SetMarkerSize(1);
 grEEPlus_MC_Deta->SetMarkerStyle(20);
 grEEPlus_MC_Deta->SetLineWidth(1);
 grEEPlus_MC_Deta->SetMarkerColor(kBlue);
 grEEPlus_MC_Deta->SetLineColor(kBlue);
 grEEPlus_MC_Deta->SetLineStyle(1);
 
 mgr_EEPlus_Deta->Add(grEEPlus_DATA_Deta);
 mgr_EEPlus_Deta->Add(grEEPlus_MC_Deta);
 mgr_EEPlus_Deta->Draw("AP");
 mgr_EEPlus_Deta->GetXaxis()->SetTitle(" ");
 mgr_EEPlus_Deta->GetYaxis()->SetTitle("#Delta#eta EE+");
 for (int i=0; (i< (numBin-1)); i++) {
  int bin = mgr_EEPlus_Deta->GetXaxis()->FindBin(i+1);
  mgr_EEPlus_Deta->GetXaxis()->SetBinLabel(bin,nameRunsHR.at(i).c_str());
 } 
 gPad->SetGrid();
 toDoROOT = Form("%s/plot/Deta_EEPlus.png",nameOutputDir.Data());
 ccEEPlus_Deta->SaveAs(toDoROOT);
 
 
 
 ///==== EB+ ====
 TCanvas* ccEBPlus_Dphi_ePlus = new TCanvas ("EBPlus_Dphi_ePlus","EB+ #Delta#phi e+", 800, 800);
 TMultiGraph* mgr_EBPlus_Dphi_ePlus = new TMultiGraph(); 
 grEBPlus_DATA_Dphi_ePlus->SetMarkerSize(2);
 grEBPlus_DATA_Dphi_ePlus->SetMarkerStyle(20);
 grEBPlus_DATA_Dphi_ePlus->SetLineWidth(2);
 grEBPlus_DATA_Dphi_ePlus->SetMarkerColor(kRed);
 grEBPlus_DATA_Dphi_ePlus->SetLineColor(kRed);
 grEBPlus_DATA_Dphi_ePlus->SetLineStyle(2);
 
 grEBPlus_MC_Dphi_ePlus->SetMarkerSize(1);
 grEBPlus_MC_Dphi_ePlus->SetMarkerStyle(20);
 grEBPlus_MC_Dphi_ePlus->SetLineWidth(1);
 grEBPlus_MC_Dphi_ePlus->SetMarkerColor(kBlue);
 grEBPlus_MC_Dphi_ePlus->SetLineColor(kBlue);
 grEBPlus_MC_Dphi_ePlus->SetLineStyle(1);
 
 mgr_EBPlus_Dphi_ePlus->Add(grEBPlus_DATA_Dphi_ePlus);
 mgr_EBPlus_Dphi_ePlus->Add(grEBPlus_MC_Dphi_ePlus);
 mgr_EBPlus_Dphi_ePlus->Draw("AP");
 mgr_EBPlus_Dphi_ePlus->GetXaxis()->SetTitle(" ");
 mgr_EBPlus_Dphi_ePlus->GetYaxis()->SetTitle("#Delta#phi e+ EB+");
 for (int i=0; (i< (numBin-1)); i++) {
  int bin = mgr_EBPlus_Dphi_ePlus->GetXaxis()->FindBin(i+1);
  mgr_EBPlus_Dphi_ePlus->GetXaxis()->SetBinLabel(bin,nameRunsHR.at(i).c_str());
 } 
 gPad->SetGrid();
 toDoROOT = Form("%s/plot/Dphi_EBPlus_eplus.png",nameOutputDir.Data());
 ccEBPlus_Dphi_ePlus->SaveAs(toDoROOT);
 
 TCanvas* ccEBPlus_Dphi_eMinus = new TCanvas ("EBPlus_Dphi_eMinus","EB+ #Delta#phi e-", 800, 800);
 TMultiGraph* mgr_EBPlus_Dphi_eMinus = new TMultiGraph(); 
 grEBPlus_DATA_Dphi_eMinus->SetMarkerSize(2);
 grEBPlus_DATA_Dphi_eMinus->SetMarkerStyle(20);
 grEBPlus_DATA_Dphi_eMinus->SetLineWidth(2);
 grEBPlus_DATA_Dphi_eMinus->SetMarkerColor(kRed);
 grEBPlus_DATA_Dphi_eMinus->SetLineColor(kRed);
 grEBPlus_DATA_Dphi_eMinus->SetLineStyle(2);
 
 grEBPlus_MC_Dphi_eMinus->SetMarkerSize(1);
 grEBPlus_MC_Dphi_eMinus->SetMarkerStyle(20);
 grEBPlus_MC_Dphi_eMinus->SetLineWidth(1);
 grEBPlus_MC_Dphi_eMinus->SetMarkerColor(kBlue);
 grEBPlus_MC_Dphi_eMinus->SetLineColor(kBlue);
 grEBPlus_MC_Dphi_eMinus->SetLineStyle(1);
 
 mgr_EBPlus_Dphi_eMinus->Add(grEBPlus_DATA_Dphi_eMinus);
 mgr_EBPlus_Dphi_eMinus->Add(grEBPlus_MC_Dphi_eMinus);
 mgr_EBPlus_Dphi_eMinus->Draw("AP");
 mgr_EBPlus_Dphi_eMinus->GetXaxis()->SetTitle(" ");
 mgr_EBPlus_Dphi_eMinus->GetYaxis()->SetTitle("#Delta#phi e- EB+");
 for (int i=0; (i< (numBin-1)); i++) {
  int bin = mgr_EBPlus_Dphi_eMinus->GetXaxis()->FindBin(i+1);
  mgr_EBPlus_Dphi_eMinus->GetXaxis()->SetBinLabel(bin,nameRunsHR.at(i).c_str());
 } 
 gPad->SetGrid();
 toDoROOT = Form("%s/plot/Dphi_EBPlus_eminus.png",nameOutputDir.Data());
 ccEBPlus_Dphi_eMinus->SaveAs(toDoROOT);
 
 TCanvas* ccEBPlus_Deta = new TCanvas ("EBPlus_Deta","EB+ #Delta#eta", 800, 800);
 TMultiGraph* mgr_EBPlus_Deta = new TMultiGraph(); 
 grEBPlus_DATA_Deta->SetMarkerSize(2);
 grEBPlus_DATA_Deta->SetMarkerStyle(20);
 grEBPlus_DATA_Deta->SetLineWidth(2);
 grEBPlus_DATA_Deta->SetMarkerColor(kRed);
 grEBPlus_DATA_Deta->SetLineColor(kRed);
 grEBPlus_DATA_Deta->SetLineStyle(2);
 
 grEBPlus_MC_Deta->SetMarkerSize(1);
 grEBPlus_MC_Deta->SetMarkerStyle(20);
 grEBPlus_MC_Deta->SetLineWidth(1);
 grEBPlus_MC_Deta->SetMarkerColor(kBlue);
 grEBPlus_MC_Deta->SetLineColor(kBlue);
 grEBPlus_MC_Deta->SetLineStyle(1);
 
 mgr_EBPlus_Deta->Add(grEBPlus_DATA_Deta);
 mgr_EBPlus_Deta->Add(grEBPlus_MC_Deta);
 mgr_EBPlus_Deta->Draw("AP");
 mgr_EBPlus_Deta->GetXaxis()->SetTitle(" ");
 mgr_EBPlus_Deta->GetYaxis()->SetTitle("#Delta#eta EB+");
 for (int i=0; (i< (numBin-1)); i++) {
  int bin = mgr_EBPlus_Deta->GetXaxis()->FindBin(i+1);
  mgr_EBPlus_Deta->GetXaxis()->SetBinLabel(bin,nameRunsHR.at(i).c_str());
 } 
 gPad->SetGrid();
 toDoROOT = Form("%s/plot/Deta_EBPlus.png",nameOutputDir.Data());
 ccEBPlus_Deta->SaveAs(toDoROOT);
 
 
 
 ///==== EB- ====
 TCanvas* ccEBMinus_Dphi_ePlus = new TCanvas ("EBMinus_Dphi_ePlus","EB- #Delta#phi e+", 800, 800);
 TMultiGraph* mgr_EBMinus_Dphi_ePlus = new TMultiGraph(); 
 grEBMinus_DATA_Dphi_ePlus->SetMarkerSize(2);
 grEBMinus_DATA_Dphi_ePlus->SetMarkerStyle(20);
 grEBMinus_DATA_Dphi_ePlus->SetLineWidth(2);
 grEBMinus_DATA_Dphi_ePlus->SetMarkerColor(kRed);
 grEBMinus_DATA_Dphi_ePlus->SetLineColor(kRed);
 grEBMinus_DATA_Dphi_ePlus->SetLineStyle(2);
 
 grEBMinus_MC_Dphi_ePlus->SetMarkerSize(1);
 grEBMinus_MC_Dphi_ePlus->SetMarkerStyle(20);
 grEBMinus_MC_Dphi_ePlus->SetLineWidth(1);
 grEBMinus_MC_Dphi_ePlus->SetMarkerColor(kBlue);
 grEBMinus_MC_Dphi_ePlus->SetLineColor(kBlue);
 grEBMinus_MC_Dphi_ePlus->SetLineStyle(1);
 
 mgr_EBMinus_Dphi_ePlus->Add(grEBMinus_DATA_Dphi_ePlus);
 mgr_EBMinus_Dphi_ePlus->Add(grEBMinus_MC_Dphi_ePlus);
 mgr_EBMinus_Dphi_ePlus->Draw("AP");
 mgr_EBMinus_Dphi_ePlus->GetXaxis()->SetTitle(" ");
 mgr_EBMinus_Dphi_ePlus->GetYaxis()->SetTitle("#Delta#phi e+ EB-");
 for (int i=0; (i< (numBin-1)); i++) {
  int bin = mgr_EBMinus_Dphi_ePlus->GetXaxis()->FindBin(i+1);
  mgr_EBMinus_Dphi_ePlus->GetXaxis()->SetBinLabel(bin,nameRunsHR.at(i).c_str());
 } 
 gPad->SetGrid();
 toDoROOT = Form("%s/plot/Dphi_EBMinus_eplus.png",nameOutputDir.Data());
 ccEBMinus_Dphi_ePlus->SaveAs(toDoROOT);
 
 TCanvas* ccEBMinus_Dphi_eMinus = new TCanvas ("EBMinus_Dphi_eMinus","EB- #Delta#phi e-", 800, 800);
 TMultiGraph* mgr_EBMinus_Dphi_eMinus = new TMultiGraph(); 
 grEBMinus_DATA_Dphi_eMinus->SetMarkerSize(2);
 grEBMinus_DATA_Dphi_eMinus->SetMarkerStyle(20);
 grEBMinus_DATA_Dphi_eMinus->SetLineWidth(2);
 grEBMinus_DATA_Dphi_eMinus->SetMarkerColor(kRed);
 grEBMinus_DATA_Dphi_eMinus->SetLineColor(kRed);
 grEBMinus_DATA_Dphi_eMinus->SetLineStyle(2);
 
 grEBMinus_MC_Dphi_eMinus->SetMarkerSize(1);
 grEBMinus_MC_Dphi_eMinus->SetMarkerStyle(20);
 grEBMinus_MC_Dphi_eMinus->SetLineWidth(1);
 grEBMinus_MC_Dphi_eMinus->SetMarkerColor(kBlue);
 grEBMinus_MC_Dphi_eMinus->SetLineColor(kBlue);
 grEBMinus_MC_Dphi_eMinus->SetLineStyle(1);
 
 mgr_EBMinus_Dphi_eMinus->Add(grEBMinus_DATA_Dphi_eMinus);
 mgr_EBMinus_Dphi_eMinus->Add(grEBMinus_MC_Dphi_eMinus);
 mgr_EBMinus_Dphi_eMinus->Draw("AP");
 mgr_EBMinus_Dphi_eMinus->GetXaxis()->SetTitle(" ");
 mgr_EBMinus_Dphi_eMinus->GetYaxis()->SetTitle("#Delta#phi e- EB-");
 for (int i=0; (i< (numBin-1)); i++) {
  int bin = mgr_EBMinus_Dphi_eMinus->GetXaxis()->FindBin(i+1);
  mgr_EBMinus_Dphi_eMinus->GetXaxis()->SetBinLabel(bin,nameRunsHR.at(i).c_str());
 } 
 gPad->SetGrid();
 toDoROOT = Form("%s/plot/Dphi_EBMinus_eminus.png",nameOutputDir.Data());
 ccEBMinus_Dphi_eMinus->SaveAs(toDoROOT);
 
 TCanvas* ccEBMinus_Deta = new TCanvas ("EBMinus_Deta","EB- #Delta#eta", 800, 800);
 TMultiGraph* mgr_EBMinus_Deta = new TMultiGraph(); 
 grEBMinus_DATA_Deta->SetMarkerSize(2);
 grEBMinus_DATA_Deta->SetMarkerStyle(20);
 grEBMinus_DATA_Deta->SetLineWidth(2);
 grEBMinus_DATA_Deta->SetMarkerColor(kRed);
 grEBMinus_DATA_Deta->SetLineColor(kRed);
 grEBMinus_DATA_Deta->SetLineStyle(2);
 
 grEBMinus_MC_Deta->SetMarkerSize(1);
 grEBMinus_MC_Deta->SetMarkerStyle(20);
 grEBMinus_MC_Deta->SetLineWidth(1);
 grEBMinus_MC_Deta->SetMarkerColor(kBlue);
 grEBMinus_MC_Deta->SetLineColor(kBlue);
 grEBMinus_MC_Deta->SetLineStyle(1);
 
 mgr_EBMinus_Deta->Add(grEBMinus_DATA_Deta);
 mgr_EBMinus_Deta->Add(grEBMinus_MC_Deta);
 mgr_EBMinus_Deta->Draw("AP");
 mgr_EBMinus_Deta->GetXaxis()->SetTitle(" ");
 mgr_EBMinus_Deta->GetYaxis()->SetTitle("#Delta#eta EB-");
 for (int i=0; (i< (numBin-1)); i++) {
  int bin = mgr_EBMinus_Deta->GetXaxis()->FindBin(i+1);
  mgr_EBMinus_Deta->GetXaxis()->SetBinLabel(bin,nameRunsHR.at(i).c_str());
 } 
 gPad->SetGrid();
 toDoROOT = Form("%s/plot/Deta_EBMinus.png",nameOutputDir.Data());
 ccEBMinus_Deta->SaveAs(toDoROOT);
 
 
 
 
 ///==== EE- ====
 TCanvas* ccEEMinus_Dphi_ePlus = new TCanvas ("EEMinus_Dphi_ePlus","EE- #Delta#phi e+", 800, 800);
 TMultiGraph* mgr_EEMinus_Dphi_ePlus = new TMultiGraph(); 
 grEEMinus_DATA_Dphi_ePlus->SetMarkerSize(2);
 grEEMinus_DATA_Dphi_ePlus->SetMarkerStyle(20);
 grEEMinus_DATA_Dphi_ePlus->SetLineWidth(2);
 grEEMinus_DATA_Dphi_ePlus->SetMarkerColor(kRed);
 grEEMinus_DATA_Dphi_ePlus->SetLineColor(kRed);
 grEEMinus_DATA_Dphi_ePlus->SetLineStyle(2);
 
 grEEMinus_MC_Dphi_ePlus->SetMarkerSize(1);
 grEEMinus_MC_Dphi_ePlus->SetMarkerStyle(20);
 grEEMinus_MC_Dphi_ePlus->SetLineWidth(1);
 grEEMinus_MC_Dphi_ePlus->SetMarkerColor(kBlue);
 grEEMinus_MC_Dphi_ePlus->SetLineColor(kBlue);
 grEEMinus_MC_Dphi_ePlus->SetLineStyle(1);
 
 mgr_EEMinus_Dphi_ePlus->Add(grEEMinus_DATA_Dphi_ePlus);
 mgr_EEMinus_Dphi_ePlus->Add(grEEMinus_MC_Dphi_ePlus);
 mgr_EEMinus_Dphi_ePlus->Draw("AP");
 mgr_EEMinus_Dphi_ePlus->GetXaxis()->SetTitle(" ");
 mgr_EEMinus_Dphi_ePlus->GetYaxis()->SetTitle("#Delta#phi e+ EE-");
 for (int i=0; (i< (numBin-1)); i++) {
  int bin = mgr_EEMinus_Dphi_ePlus->GetXaxis()->FindBin(i+1);
  mgr_EEMinus_Dphi_ePlus->GetXaxis()->SetBinLabel(bin,nameRunsHR.at(i).c_str());
 } 
 gPad->SetGrid();
 toDoROOT = Form("%s/plot/Dphi_EEMinus_eplus.png",nameOutputDir.Data());
 ccEEMinus_Dphi_ePlus->SaveAs(toDoROOT);
 
 TCanvas* ccEEMinus_Dphi_eMinus = new TCanvas ("EEMinus_Dphi_eMinus","EE- #Delta#phi e-", 800, 800);
 TMultiGraph* mgr_EEMinus_Dphi_eMinus = new TMultiGraph(); 
 grEEMinus_DATA_Dphi_eMinus->SetMarkerSize(2);
 grEEMinus_DATA_Dphi_eMinus->SetMarkerStyle(20);
 grEEMinus_DATA_Dphi_eMinus->SetLineWidth(2);
 grEEMinus_DATA_Dphi_eMinus->SetMarkerColor(kRed);
 grEEMinus_DATA_Dphi_eMinus->SetLineColor(kRed);
 grEEMinus_DATA_Dphi_eMinus->SetLineStyle(2);
 
 grEEMinus_MC_Dphi_eMinus->SetMarkerSize(1);
 grEEMinus_MC_Dphi_eMinus->SetMarkerStyle(20);
 grEEMinus_MC_Dphi_eMinus->SetLineWidth(1);
 grEEMinus_MC_Dphi_eMinus->SetMarkerColor(kBlue);
 grEEMinus_MC_Dphi_eMinus->SetLineColor(kBlue);
 grEEMinus_MC_Dphi_eMinus->SetLineStyle(1);
 
 mgr_EEMinus_Dphi_eMinus->Add(grEEMinus_DATA_Dphi_eMinus);
 mgr_EEMinus_Dphi_eMinus->Add(grEEMinus_MC_Dphi_eMinus);
 mgr_EEMinus_Dphi_eMinus->Draw("AP");
 mgr_EEMinus_Dphi_eMinus->GetXaxis()->SetTitle(" ");
 mgr_EEMinus_Dphi_eMinus->GetYaxis()->SetTitle("#Delta#phi e- EE-");
 for (int i=0; (i< (numBin-1)); i++) {
  int bin = mgr_EEMinus_Dphi_eMinus->GetXaxis()->FindBin(i+1);
  mgr_EEMinus_Dphi_eMinus->GetXaxis()->SetBinLabel(bin,nameRunsHR.at(i).c_str());
 }
 gPad->SetGrid();
 toDoROOT = Form("%s/plot/Dphi_EEMinus_eminus.png",nameOutputDir.Data());
 ccEEMinus_Dphi_eMinus->SaveAs(toDoROOT);
 
 TCanvas* ccEEMinus_Deta = new TCanvas ("EEMinus_Deta","EE- #Delta#eta", 800, 800);
 TMultiGraph* mgr_EEMinus_Deta = new TMultiGraph(); 
 grEEMinus_DATA_Deta->SetMarkerSize(2);
 grEEMinus_DATA_Deta->SetMarkerStyle(20);
 grEEMinus_DATA_Deta->SetLineWidth(2);
 grEEMinus_DATA_Deta->SetMarkerColor(kRed);
 grEEMinus_DATA_Deta->SetLineColor(kRed);
 grEEMinus_DATA_Deta->SetLineStyle(2);
 
 grEEMinus_MC_Deta->SetMarkerSize(1);
 grEEMinus_MC_Deta->SetMarkerStyle(20);
 grEEMinus_MC_Deta->SetLineWidth(1);
 grEEMinus_MC_Deta->SetMarkerColor(kBlue);
 grEEMinus_MC_Deta->SetLineColor(kBlue);
 grEEMinus_MC_Deta->SetLineStyle(1);
 
 mgr_EEMinus_Deta->Add(grEEMinus_DATA_Deta);
 mgr_EEMinus_Deta->Add(grEEMinus_MC_Deta);
 mgr_EEMinus_Deta->Draw("AP");
 mgr_EEMinus_Deta->GetXaxis()->SetTitle(" ");
 mgr_EEMinus_Deta->GetYaxis()->SetTitle("#Delta#eta EE-");
 for (int i=0; (i< (numBin-1)); i++) {
  int bin = mgr_EEMinus_Deta->GetXaxis()->FindBin(i+1);
  mgr_EEMinus_Deta->GetXaxis()->SetBinLabel(bin,nameRunsHR.at(i).c_str());
 }
 gPad->SetGrid();
 toDoROOT = Form("%s/plot/Deta_EEMinus.png",nameOutputDir.Data());
 ccEEMinus_Deta->SaveAs(toDoROOT);
 
 
 
 
 
 ///========================
 ///==== to create HTML ====
 
 
 toDoShell = Form("cp scripts/html/templateEvolution.html %s/",nameOutputDir.Data());
 system(toDoShell.Data());
 
}