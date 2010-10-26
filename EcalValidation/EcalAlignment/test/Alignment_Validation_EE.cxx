
void Alignment_Validation_EE(TString nameFileBefore, TString nameFileAfter){
 ///===========================================================================
 
 gROOT->Reset();
 gROOT->SetStyle("Plain");
 gStyle->SetPalette(1); 
 gStyle->SetOptStat(1110);
 gStyle->SetPadTickX(1);
 gStyle->SetPadTickY(1);
 gStyle->SetTitleBorderSize(0);
 
 ///===========================================================================
 
 //  TString genCut = Form("(eleFBrem<0.8 && eleCharge<0)");
//  TString genCut = Form("(eleFBrem<0.8)");
 TString genCut = Form("(1)");
  
 ///===========================================================================
 ///===========================================================================
 ///===========================================================================
 
 TFile* FileBefore = new TFile(nameFileBefore,"READ");
 TTree* myTreeBefore = (TTree*) FileBefore->Get("ntupleEcalAlignment/myTree");
 myTreeBefore->SetName("myTreeBefore");
 
 TFile* FileAfter = new TFile(nameFileAfter,"READ");
 TTree* myTreeAfter = (TTree*) FileAfter->Get("ntupleEcalAlignment/myTree");
 myTreeAfter->SetName("myTreeAfter");
 
 
 TPaveStats *stEE[8];
 
 int NBIN_PHI_EE = 36;
 int NBIN_ETA_EE = 30;
 
 double MINPHI = -3.14159265;
 double MAXPHI = +3.14159265;
 
 double MINETA = -3.5;
 double MAXETA = +3.5;
 
 int NBIN_DETA = 100;
 int NBIN_DPHI = 100;
 
 double MINDETA = -0.01;
 double MAXDETA = +0.01;
 
 double MINDPHI = -0.05;
 double MAXDPHI = +0.05;
 
 TString draw;
 
 ///========================
 ///========== EE ==========
 
 TString genCut_EE = Form("%s && abs(etaSC)>1.5",genCut.Data());
 
 ///==== Before ====
 
 TH2F* hDPhi_vs_Phi_EE_Before = new TH2F("hDPhi_vs_Phi_EE_Before","#Delta#phi vs #phi EE Before",NBIN_PHI_EE,MINPHI,MAXPHI,NBIN_DPHI,MINDPHI,MAXDPHI);
 
 TH2F* hDPhi_vs_Eta_EE_Before = new TH2F("hDPhi_vs_Eta_EE_Before","#Delta#phi vs #eta EE Before",NBIN_PHI_EE,MINETA,MAXETA,NBIN_DPHI,MINDPHI,MAXDPHI);
 
 TH2F* hDEta_vs_Phi_EE_Before = new TH2F("hDEta_vs_Phi_EE_Before","#Delta#eta vs #phi EE Before",NBIN_PHI_EE,MINPHI,MAXPHI,NBIN_DETA,MINDETA,MAXDETA);
 
 TH2F* hDEta_vs_Eta_EE_Before = new TH2F("hDEta_vs_Eta_EE_Before","#Delta#eta vs #eta EE Before",NBIN_PHI_EE,MINETA,MAXETA,NBIN_DETA,MINDETA,MAXDETA);
 
 
 draw = Form("DeltaPhiIn:phiSC >> hDPhi_vs_Phi_EE_Before");
 myTreeBefore->Draw(draw,genCut_EE,"goff");
 draw = Form("DeltaPhiIn:etaSC >> hDPhi_vs_Eta_EE_Before");
 myTreeBefore->Draw(draw,genCut_EE,"goff");
 
 draw = Form("DeltaEtaIn:phiSC >> hDEta_vs_Phi_EE_Before");
 myTreeBefore->Draw(draw,genCut_EE,"goff");
 draw = Form("DeltaEtaIn:etaSC >> hDEta_vs_Eta_EE_Before");
 myTreeBefore->Draw(draw,genCut_EE,"goff");
 
 ///==== After ====
 
 TH2F* hDPhi_vs_Phi_EE_After = new TH2F("hDPhi_vs_Phi_EE_After","#Delta#phi vs #phi EE After",NBIN_PHI_EE,MINPHI,MAXPHI,NBIN_DPHI,MINDPHI,MAXDPHI);
 
 TH2F* hDPhi_vs_Eta_EE_After = new TH2F("hDPhi_vs_Eta_EE_After","#Delta#phi vs #eta EE After",NBIN_PHI_EE,MINETA,MAXETA,NBIN_DPHI,MINDPHI,MAXDPHI);
 
 TH2F* hDEta_vs_Phi_EE_After = new TH2F("hDEta_vs_Phi_EE_After","#Delta#eta vs #phi EE After",NBIN_PHI_EE,MINPHI,MAXPHI,NBIN_DETA,MINDETA,MAXDETA);
 
 TH2F* hDEta_vs_Eta_EE_After = new TH2F("hDEta_vs_Eta_EE_After","#Delta#eta vs #eta EE After",NBIN_PHI_EE,MINETA,MAXETA,NBIN_DETA,MINDETA,MAXDETA);
 
 
 draw = Form("DeltaPhiIn:phiSC >> hDPhi_vs_Phi_EE_After");
 myTreeAfter->Draw(draw,genCut_EE,"goff");
 draw = Form("DeltaPhiIn:etaSC >> hDPhi_vs_Eta_EE_After");
 myTreeAfter->Draw(draw,genCut_EE,"goff");
 
 draw = Form("DeltaEtaIn:phiSC >> hDEta_vs_Phi_EE_After");
 myTreeAfter->Draw(draw,genCut_EE,"goff");
 draw = Form("DeltaEtaIn:etaSC >> hDEta_vs_Eta_EE_After");
 myTreeAfter->Draw(draw,genCut_EE,"goff");
 
 
 ///==== Draw ====
 TCanvas* cDETA_DPHI_vs_Eta_EE = new TCanvas ("cDETA_DPHI_vs_Eta_EE","cDETA_DPHI_vs_Eta_EE",1000,1000);
 cDETA_DPHI_vs_Eta_EE->Divide(2,2);
 
 cDETA_DPHI_vs_Eta_EE->cd(1);
 hDPhi_vs_Eta_EE_Before->Draw("colz");
 gPad->SetGrid();
 
 cDETA_DPHI_vs_Eta_EE->cd(2);
 hDEta_vs_Eta_EE_Before->Draw("colz");
 gPad->SetGrid();
 
 cDETA_DPHI_vs_Eta_EE->cd(3);
 hDPhi_vs_Eta_EE_After->Draw("colz");
 gPad->SetGrid();
 
 cDETA_DPHI_vs_Eta_EE->cd(4);
 hDEta_vs_Eta_EE_After->Draw("colz");
 gPad->SetGrid();
 
 
 TCanvas* cDETA_DPHI_vs_Phi_EE = new TCanvas ("cDETA_DPHI_vs_Phi_EE","cDETA_DPHI_vs_Phi_EE",1000,1000);
 cDETA_DPHI_vs_Phi_EE->Divide(2,2);
 gPad->SetGrid();
 
 cDETA_DPHI_vs_Phi_EE->cd(1);
 hDPhi_vs_Phi_EE_Before->Draw("colz");
 gPad->SetGrid();
 
 cDETA_DPHI_vs_Phi_EE->cd(2);
 hDEta_vs_Phi_EE_Before->Draw("colz");
 gPad->SetGrid();
 
 cDETA_DPHI_vs_Phi_EE->cd(3);
 hDPhi_vs_Phi_EE_After->Draw("colz");
 gPad->SetGrid();
 
 cDETA_DPHI_vs_Phi_EE->cd(4);
 hDEta_vs_Phi_EE_After->Draw("colz");
 gPad->SetGrid();
 
 
 
 
 
 ///==== Profile ====
 
 TProfile * profile_hDPhi_vs_Eta_EE_Before = hDPhi_vs_Eta_EE_Before->ProfileX ("_DPhi_vs_Eta_EE_Before") ;
 profile_hDPhi_vs_Eta_EE_Before->SetMarkerColor(kRed);
 profile_hDPhi_vs_Eta_EE_Before->SetLineColor(kRed);
 profile_hDPhi_vs_Eta_EE_Before->SetMarkerStyle(20);
 profile_hDPhi_vs_Eta_EE_Before->SetMarkerSize(1);
 profile_hDPhi_vs_Eta_EE_Before->SetLineWidth(1);
 
 TProfile * profile_hDPhi_vs_Phi_EE_Before = hDPhi_vs_Phi_EE_Before->ProfileX ("_DPhi_vs_Phi_EE_Before") ;
 profile_hDPhi_vs_Phi_EE_Before->SetMarkerColor(kRed);
 profile_hDPhi_vs_Phi_EE_Before->SetLineColor(kRed);
 profile_hDPhi_vs_Phi_EE_Before->SetMarkerStyle(20);
 profile_hDPhi_vs_Phi_EE_Before->SetMarkerSize(1);
 profile_hDPhi_vs_Phi_EE_Before->SetLineWidth(1);
 
 TProfile * profile_hDEta_vs_Eta_EE_Before = hDEta_vs_Eta_EE_Before->ProfileX ("_DEta_vs_Eta_EE_Before") ;
 profile_hDEta_vs_Eta_EE_Before->SetMarkerColor(kRed);
 profile_hDEta_vs_Eta_EE_Before->SetLineColor(kRed);
 profile_hDEta_vs_Eta_EE_Before->SetMarkerStyle(20);
 profile_hDEta_vs_Eta_EE_Before->SetMarkerSize(1);
 profile_hDEta_vs_Eta_EE_Before->SetLineWidth(1);
 
 TProfile * profile_hDEta_vs_Phi_EE_Before = hDEta_vs_Phi_EE_Before->ProfileX ("_DEta_vs_Phi_EE_Before") ;
 profile_hDEta_vs_Phi_EE_Before->SetMarkerColor(kRed);
 profile_hDEta_vs_Phi_EE_Before->SetLineColor(kRed);
 profile_hDEta_vs_Phi_EE_Before->SetMarkerStyle(20);
 profile_hDEta_vs_Phi_EE_Before->SetMarkerSize(1);
 profile_hDEta_vs_Phi_EE_Before->SetLineWidth(1);
 
 TProfile * profile_hDPhi_vs_Eta_EE_After = hDPhi_vs_Eta_EE_After->ProfileX ("_DPhi_vs_Eta_EE_After") ;
 profile_hDPhi_vs_Eta_EE_After->SetMarkerColor(kBlue);
 profile_hDPhi_vs_Eta_EE_After->SetLineColor(kBlue);
 profile_hDPhi_vs_Eta_EE_After->SetMarkerStyle(20);
 profile_hDPhi_vs_Eta_EE_After->SetMarkerSize(1);
 profile_hDPhi_vs_Eta_EE_After->SetLineWidth(1);
 
 TProfile * profile_hDPhi_vs_Phi_EE_After = hDPhi_vs_Phi_EE_After->ProfileX ("_DPhi_vs_Phi_EE_After") ;
 profile_hDPhi_vs_Phi_EE_After->SetMarkerColor(kBlue);
 profile_hDPhi_vs_Phi_EE_After->SetLineColor(kBlue);
 profile_hDPhi_vs_Phi_EE_After->SetMarkerStyle(20);
 profile_hDPhi_vs_Phi_EE_After->SetMarkerSize(1);
 profile_hDPhi_vs_Phi_EE_After->SetLineWidth(1);
 
 TProfile * profile_hDEta_vs_Eta_EE_After = hDEta_vs_Eta_EE_After->ProfileX ("_DEta_vs_Eta_EE_After") ;
 profile_hDEta_vs_Eta_EE_After->SetMarkerColor(kBlue);
 profile_hDEta_vs_Eta_EE_After->SetLineColor(kBlue);
 profile_hDEta_vs_Eta_EE_After->SetMarkerStyle(20);
 profile_hDEta_vs_Eta_EE_After->SetMarkerSize(1);
 profile_hDEta_vs_Eta_EE_After->SetLineWidth(1);
 
 TProfile * profile_hDEta_vs_Phi_EE_After = hDEta_vs_Phi_EE_After->ProfileX ("_DEta_vs_Phi_EE_After") ;
 profile_hDEta_vs_Phi_EE_After->SetMarkerColor(kBlue);
 profile_hDEta_vs_Phi_EE_After->SetLineColor(kBlue);
 profile_hDEta_vs_Phi_EE_After->SetMarkerStyle(20);
 profile_hDEta_vs_Phi_EE_After->SetMarkerSize(1);
 profile_hDEta_vs_Phi_EE_After->SetLineWidth(1);
 
 TCanvas* cProfile_DETA_DPHI_vs_Eta_Phi_EE = new TCanvas ("cProfile_DETA_DPHI_vs_Eta_Phi_EE","cProfile_DETA_DPHI_vs_Eta_Phi_EE",1000,1000);
 cProfile_DETA_DPHI_vs_Eta_Phi_EE->Divide(2,2);
 
 cProfile_DETA_DPHI_vs_Eta_Phi_EE->cd(1);
 profile_hDPhi_vs_Eta_EE_Before->Draw("PL");
 profile_hDPhi_vs_Eta_EE_After->Draw("same");
 gPad->BuildLegend();
 gPad->SetGrid();
 
 cProfile_DETA_DPHI_vs_Eta_Phi_EE->cd(2);
 profile_hDEta_vs_Eta_EE_Before->Draw("PL");
 profile_hDEta_vs_Eta_EE_After->Draw("same");
 gPad->BuildLegend();
 gPad->SetGrid();
 
 cProfile_DETA_DPHI_vs_Eta_Phi_EE->cd(3);
 profile_hDPhi_vs_Phi_EE_Before->Draw("PL");
 profile_hDPhi_vs_Phi_EE_After->Draw("same");
 gPad->BuildLegend();
 gPad->SetGrid();
 
 cProfile_DETA_DPHI_vs_Eta_Phi_EE->cd(4);
 profile_hDEta_vs_Phi_EE_Before->Draw("PL");
 profile_hDEta_vs_Phi_EE_After->Draw("same");
 gPad->BuildLegend();
 gPad->SetGrid();
 
 
 
 
 ///==== 1D ====
 TString genCut_EEPlus = Form("%s && etaSC>0 && abs(etaSC)>1.5",genCut.Data());
 TString genCut_EEMinus = Form("%s && etaSC<0 && abs(etaSC)>1.5",genCut.Data());
 
 TH1F* hDEta_EEPlus_Before = new TH1F("hDEta_EEPlus_Before","#Delta#eta EE+ Before",NBIN_DETA,MINDETA,MAXDETA);
 myTreeBefore->Draw("DeltaEtaIn >> hDEta_EEPlus_Before",genCut_EEPlus,"goff");
 hDEta_EEPlus_Before->SetMarkerColor(kRed);
 hDEta_EEPlus_Before->SetLineColor(kRed);
 hDEta_EEPlus_Before->SetMarkerStyle(20);
 hDEta_EEPlus_Before->SetMarkerSize(1);
 hDEta_EEPlus_Before->SetLineWidth(1);
 
 TH1F* hDEta_EEPlus_After = new TH1F("hDEta_EEPlus_After","#Delta#eta EE+ After",NBIN_DETA,MINDETA,MAXDETA);
 myTreeAfter->Draw("DeltaEtaIn >> hDEta_EEPlus_After",genCut_EEPlus,"goff");
 hDEta_EEPlus_After->SetMarkerColor(kBlue);
 hDEta_EEPlus_After->SetLineColor(kBlue);
 hDEta_EEPlus_After->SetMarkerStyle(20);
 hDEta_EEPlus_After->SetMarkerSize(1);
 hDEta_EEPlus_After->SetLineWidth(1);
 
 TH1F* hDEta_EEMinus_Before = new TH1F("hDEta_EEMinus_Before","#Delta#eta EE- Before",NBIN_DETA,MINDETA,MAXDETA);
 myTreeBefore->Draw("DeltaEtaIn >> hDEta_EEMinus_Before",genCut_EEMinus,"goff");
 hDEta_EEMinus_Before->SetMarkerColor(kRed);
 hDEta_EEMinus_Before->SetLineColor(kRed);
 hDEta_EEMinus_Before->SetMarkerStyle(20);
 hDEta_EEMinus_Before->SetMarkerSize(1);
 hDEta_EEMinus_Before->SetLineWidth(1);
 
 TH1F* hDEta_EEMinus_After = new TH1F("hDEta_EEMinus_After","#Delta#eta EE- After",NBIN_DETA,MINDETA,MAXDETA);
 myTreeAfter->Draw("DeltaEtaIn >> hDEta_EEMinus_After",genCut_EEMinus,"goff");
 hDEta_EEMinus_After->SetMarkerColor(kBlue);
 hDEta_EEMinus_After->SetLineColor(kBlue);
 hDEta_EEMinus_After->SetMarkerStyle(20);
 hDEta_EEMinus_After->SetMarkerSize(1);
 hDEta_EEMinus_After->SetLineWidth(1);
 
 
 TH1F* hDPhi_EEPlus_Before = new TH1F("hDPhi_EEPlus_Before","#Delta#phi EE+ Before",NBIN_DPHI,MINDPHI,MAXDPHI);
 myTreeBefore->Draw("DeltaPhiIn >> hDPhi_EEPlus_Before",genCut_EEPlus,"goff");
 hDPhi_EEPlus_Before->SetMarkerColor(kRed);
 hDPhi_EEPlus_Before->SetLineColor(kRed);
 hDPhi_EEPlus_Before->SetMarkerStyle(20);
 hDPhi_EEPlus_Before->SetMarkerSize(1);
 hDPhi_EEPlus_Before->SetLineWidth(1);
 
 TH1F* hDPhi_EEPlus_After = new TH1F("hDPhi_EEPlus_After","#Delta#phi EE+ After",NBIN_DPHI,MINDPHI,MAXDPHI);
 myTreeAfter->Draw("DeltaPhiIn >> hDPhi_EEPlus_After",genCut_EEPlus,"goff");
 hDPhi_EEPlus_After->SetMarkerColor(kBlue);
 hDPhi_EEPlus_After->SetLineColor(kBlue);
 hDPhi_EEPlus_After->SetMarkerStyle(20);
 hDPhi_EEPlus_After->SetMarkerSize(1);
 hDPhi_EEPlus_After->SetLineWidth(1);
 
 TH1F* hDPhi_EEMinus_Before = new TH1F("hDPhi_EEMinus_Before","#Delta#phi EE- Before",NBIN_DPHI,MINDPHI,MAXDPHI);
 myTreeBefore->Draw("DeltaPhiIn >> hDPhi_EEMinus_Before",genCut_EEMinus,"goff");
 hDPhi_EEMinus_Before->SetMarkerColor(kRed);
 hDPhi_EEMinus_Before->SetLineColor(kRed);
 hDPhi_EEMinus_Before->SetMarkerStyle(20);
 hDPhi_EEMinus_Before->SetMarkerSize(1);
 hDPhi_EEMinus_Before->SetLineWidth(1);
 
 TH1F* hDPhi_EEMinus_After = new TH1F("hDPhi_EEMinus_After","#Delta#phi EE- After",NBIN_DPHI,MINDPHI,MAXDPHI);
 myTreeAfter->Draw("DeltaPhiIn >> hDPhi_EEMinus_After",genCut_EEMinus,"goff");
 hDPhi_EEMinus_After->SetMarkerColor(kBlue);
 hDPhi_EEMinus_After->SetLineColor(kBlue);
 hDPhi_EEMinus_After->SetMarkerStyle(20);
 hDPhi_EEMinus_After->SetMarkerSize(1);
 hDPhi_EEMinus_After->SetLineWidth(1);
 
 ///==== Draw ====
 TCanvas* cDETA_DPHI_EE = new TCanvas ("cDETA_DPHI_EE","cDETA_DPHI_EE",1000,1000);
 cDETA_DPHI_EE->Divide(2,2);
 
 cDETA_DPHI_EE->cd(1);
 hDEta_EEMinus_After->Draw("PLS");
 hDEta_EEMinus_After->Scale(hDEta_EEMinus_Before->GetEntries()/hDEta_EEMinus_After->GetEntries());
 gPad->Update();
 stEE[0]= (TPaveStats*)(hDEta_EEMinus_After->GetListOfFunctions()->FindObject("stats"));
 stEE[0]->SetY1NDC(0.85); //new x start position
 stEE[0]->SetY2NDC(1.00); //new x end position
 stEE[0]->SetTextColor(kBlue);
 stEE[0]->Draw();
 hDEta_EEMinus_Before->Draw("LPsameS");
 hDEta_EEMinus_Before->GetXaxis()->SetTitle("#Delta#eta");
 gPad->Update();
 stEE[1]= (TPaveStats*)(hDEta_EEMinus_Before->GetListOfFunctions()->FindObject("stats"));
 stEE[1]->SetY1NDC(0.72); //new x start position
 stEE[1]->SetY2NDC(0.85); //new x end position
 stEE[1]->SetTextColor(kRed);
 stEE[1]->Draw(); 
 gPad->SetGrid();
 gPad->BuildLegend();
 gPad->Update();
 
 
 cDETA_DPHI_EE->cd(2);
 hDEta_EEPlus_After->Draw("PLS");
 hDEta_EEPlus_After->Scale(hDEta_EEPlus_Before->GetEntries()/hDEta_EEPlus_After->GetEntries());
 gPad->Update();
 stEE[2]= (TPaveStats*)(hDEta_EEPlus_After->GetListOfFunctions()->FindObject("stats"));
 stEE[2]->SetY1NDC(0.85); //new x start position
 stEE[2]->SetY2NDC(1.00); //new x end position
 stEE[2]->SetTextColor(kBlue);
 stEE[2]->Draw();
 hDEta_EEPlus_Before->Draw("LPsameS"); 
 hDEta_EEPlus_Before->GetXaxis()->SetTitle("#Delta#eta");
 gPad->Update();
 stEE[3]= (TPaveStats*)(hDEta_EEPlus_Before->GetListOfFunctions()->FindObject("stats"));
 stEE[3]->SetY1NDC(0.72); //new x start position
 stEE[3]->SetY2NDC(0.85); //new x end position
 stEE[3]->SetTextColor(kRed);
 stEE[3]->Draw(); 
 gPad->SetGrid();
 gPad->BuildLegend();
 gPad->Update();
 
 cDETA_DPHI_EE->cd(3);
 hDPhi_EEMinus_After->Draw("PLS");
 hDPhi_EEMinus_After->Scale(hDPhi_EEMinus_Before->GetEntries()/hDPhi_EEMinus_After->GetEntries());
 gPad->Update();
 stEE[4]= (TPaveStats*)(hDPhi_EEMinus_After->GetListOfFunctions()->FindObject("stats"));
 stEE[4]->SetY1NDC(0.85); //new x start position
 stEE[4]->SetY2NDC(1.00); //new x end position
 stEE[4]->SetTextColor(kBlue);
 stEE[4]->Draw();
 hDPhi_EEMinus_Before->Draw("LPsameS"); 
 hDPhi_EEMinus_Before->GetXaxis()->SetTitle("#Delta#phi");
 gPad->Update();
 stEE[5]= (TPaveStats*)(hDPhi_EEMinus_Before->GetListOfFunctions()->FindObject("stats"));
 stEE[5]->SetY1NDC(0.72); //new x start position
 stEE[5]->SetY2NDC(0.85); //new x end position
 stEE[5]->SetTextColor(kRed);
 stEE[5]->Draw(); 
 gPad->SetGrid();
 gPad->BuildLegend();
 gPad->Update();
 
 cDETA_DPHI_EE->cd(4);
 hDPhi_EEPlus_After->Draw("PLS");
 hDPhi_EEPlus_After->Scale(hDPhi_EEPlus_Before->GetEntries()/hDPhi_EEPlus_After->GetEntries());
 gPad->Update();
 stEE[6]= (TPaveStats*)(hDPhi_EEPlus_After->GetListOfFunctions()->FindObject("stats"));
 stEE[6]->SetY1NDC(0.85); //new x start position
 stEE[6]->SetY2NDC(1.00); //new x end position
 stEE[6]->SetTextColor(kBlue);
 stEE[6]->Draw();
 hDPhi_EEPlus_Before->Draw("LPsameS"); 
 hDPhi_EEPlus_Before->GetXaxis()->SetTitle("#Delta#phi");
 gPad->Update();
 stEE[7]= (TPaveStats*)(hDPhi_EEPlus_Before->GetListOfFunctions()->FindObject("stats"));
 stEE[7]->SetY1NDC(0.72); //new x start position
 stEE[7]->SetY2NDC(0.85); //new x end position
 stEE[7]->SetTextColor(kRed);
 stEE[7]->Draw(); 
 gPad->SetGrid();
 gPad->BuildLegend();
 gPad->Update();
 
}



