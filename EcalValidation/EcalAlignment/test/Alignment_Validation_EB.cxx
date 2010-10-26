
void Alignment_Validation_EB(TString nameFileBefore, TString nameFileAfter){
 
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
 TTree* myTreeBefore = (TTree*) FileBefore->Get("myTree");
 myTreeBefore->SetName("myTreeBefore");
 
 TFile* FileAfter = new TFile(nameFileAfter,"READ");
 TTree* myTreeAfter = (TTree*) FileAfter->Get("myTree");
 myTreeAfter->SetName("myTreeAfter");
 
 
 TPaveStats *stEB[8];
 
 int NBIN_PHI_EB = 36;
 int NBIN_ETA_EB = 30;
  
 double MINPHI = -3.14159265;
 double MAXPHI = +3.14159265;
 
 double MINETA = -1.5;
 double MAXETA = +1.5;
 
 int NBIN_DETA = 100;
 int NBIN_DPHI = 100;
 
 double MINDETA = -0.01;
 double MAXDETA = +0.01;
 
 double MINDPHI = -0.05;
 double MAXDPHI = +0.05;
 
 TString draw;
 
 ///========================
 ///========== EB ==========
 
 TString genCut_EB = Form("%s && abs(etaSC)<1.5",genCut.Data());
 
 ///==== Before ====
 
 TH2F* hDPhi_vs_Phi_EB_Before = new TH2F("hDPhi_vs_Phi_EB_Before","#Delta#phi vs #phi EB Before",NBIN_PHI_EB,MINPHI,MAXPHI,NBIN_DPHI,MINDPHI,MAXDPHI);
 
 TH2F* hDPhi_vs_Eta_EB_Before = new TH2F("hDPhi_vs_Eta_EB_Before","#Delta#phi vs #eta EB Before",NBIN_PHI_EB,MINETA,MAXETA,NBIN_DPHI,MINDPHI,MAXDPHI);
 
 TH2F* hDEta_vs_Phi_EB_Before = new TH2F("hDEta_vs_Phi_EB_Before","#Delta#eta vs #phi EB Before",NBIN_PHI_EB,MINPHI,MAXPHI,NBIN_DETA,MINDETA,MAXDETA);
 
 TH2F* hDEta_vs_Eta_EB_Before = new TH2F("hDEta_vs_Eta_EB_Before","#Delta#eta vs #eta EB Before",NBIN_PHI_EB,MINETA,MAXETA,NBIN_DETA,MINDETA,MAXDETA);
 
 
 draw = Form("DeltaPhiIn:phiSC >> hDPhi_vs_Phi_EB_Before");
 myTreeBefore->Draw(draw,genCut_EB,"goff");
 draw = Form("DeltaPhiIn:etaSC >> hDPhi_vs_Eta_EB_Before");
 myTreeBefore->Draw(draw,genCut_EB,"goff");
 
 draw = Form("DeltaEtaIn:phiSC >> hDEta_vs_Phi_EB_Before");
 myTreeBefore->Draw(draw,genCut_EB,"goff");
 draw = Form("DeltaEtaIn:etaSC >> hDEta_vs_Eta_EB_Before");
 myTreeBefore->Draw(draw,genCut_EB,"goff");
 
 ///==== After ====
  
 TH2F* hDPhi_vs_Phi_EB_After = new TH2F("hDPhi_vs_Phi_EB_After","#Delta#phi vs #phi EB After",NBIN_PHI_EB,MINPHI,MAXPHI,NBIN_DPHI,MINDPHI,MAXDPHI);
 
 TH2F* hDPhi_vs_Eta_EB_After = new TH2F("hDPhi_vs_Eta_EB_After","#Delta#phi vs #eta EB After",NBIN_PHI_EB,MINETA,MAXETA,NBIN_DPHI,MINDPHI,MAXDPHI);
 
 TH2F* hDEta_vs_Phi_EB_After = new TH2F("hDEta_vs_Phi_EB_After","#Delta#eta vs #phi EB After",NBIN_PHI_EB,MINPHI,MAXPHI,NBIN_DETA,MINDETA,MAXDETA);
 
 TH2F* hDEta_vs_Eta_EB_After = new TH2F("hDEta_vs_Eta_EB_After","#Delta#eta vs #eta EB After",NBIN_PHI_EB,MINETA,MAXETA,NBIN_DETA,MINDETA,MAXDETA);
 
 
 draw = Form("DeltaPhiIn:phiSC >> hDPhi_vs_Phi_EB_After");
 myTreeAfter->Draw(draw,genCut_EB,"goff");
 draw = Form("DeltaPhiIn:etaSC >> hDPhi_vs_Eta_EB_After");
 myTreeAfter->Draw(draw,genCut_EB,"goff");
 
 draw = Form("DeltaEtaIn:phiSC >> hDEta_vs_Phi_EB_After");
 myTreeAfter->Draw(draw,genCut_EB,"goff");
 draw = Form("DeltaEtaIn:etaSC >> hDEta_vs_Eta_EB_After");
 myTreeAfter->Draw(draw,genCut_EB,"goff");
 
 
 ///==== Draw ====
 TCanvas* cDETA_DPHI_vs_Eta_EB = new TCanvas ("cDETA_DPHI_vs_Eta_EB","cDETA_DPHI_vs_Eta_EB",1000,1000);
 cDETA_DPHI_vs_Eta_EB->Divide(2,2);
  
 cDETA_DPHI_vs_Eta_EB->cd(1);
 hDPhi_vs_Eta_EB_Before->Draw("colz");
 gPad->SetGrid();
 
 cDETA_DPHI_vs_Eta_EB->cd(2);
 hDEta_vs_Eta_EB_Before->Draw("colz");
 gPad->SetGrid();
 
 cDETA_DPHI_vs_Eta_EB->cd(3);
 hDPhi_vs_Eta_EB_After->Draw("colz");
 gPad->SetGrid();
 
 cDETA_DPHI_vs_Eta_EB->cd(4);
 hDEta_vs_Eta_EB_After->Draw("colz");
 gPad->SetGrid();
 
 
 TCanvas* cDETA_DPHI_vs_Phi_EB = new TCanvas ("cDETA_DPHI_vs_Phi_EB","cDETA_DPHI_vs_Phi_EB",1000,1000);
 cDETA_DPHI_vs_Phi_EB->Divide(2,2);
 gPad->SetGrid();
 
 cDETA_DPHI_vs_Phi_EB->cd(1);
 hDPhi_vs_Phi_EB_Before->Draw("colz");
 gPad->SetGrid();
 
 cDETA_DPHI_vs_Phi_EB->cd(2);
 hDEta_vs_Phi_EB_Before->Draw("colz");
 gPad->SetGrid();
 
 cDETA_DPHI_vs_Phi_EB->cd(3);
 hDPhi_vs_Phi_EB_After->Draw("colz");
 gPad->SetGrid();
 
 cDETA_DPHI_vs_Phi_EB->cd(4);
 hDEta_vs_Phi_EB_After->Draw("colz");
 gPad->SetGrid();
 
 
 
 
 
 ///==== Profile ====
 
 TProfile * profile_hDPhi_vs_Eta_EB_Before = hDPhi_vs_Eta_EB_Before->ProfileX ("_DPhi_vs_Eta_EB_Before") ;
 profile_hDPhi_vs_Eta_EB_Before->SetMarkerColor(kRed);
 profile_hDPhi_vs_Eta_EB_Before->SetLineColor(kRed);
 profile_hDPhi_vs_Eta_EB_Before->SetMarkerStyle(20);
 profile_hDPhi_vs_Eta_EB_Before->SetMarkerSize(1);
 profile_hDPhi_vs_Eta_EB_Before->SetLineWidth(1);

 TProfile * profile_hDPhi_vs_Phi_EB_Before = hDPhi_vs_Phi_EB_Before->ProfileX ("_DPhi_vs_Phi_EB_Before") ;
 profile_hDPhi_vs_Phi_EB_Before->SetMarkerColor(kRed);
 profile_hDPhi_vs_Phi_EB_Before->SetLineColor(kRed);
 profile_hDPhi_vs_Phi_EB_Before->SetMarkerStyle(20);
 profile_hDPhi_vs_Phi_EB_Before->SetMarkerSize(1);
 profile_hDPhi_vs_Phi_EB_Before->SetLineWidth(1);
 
 TProfile * profile_hDEta_vs_Eta_EB_Before = hDEta_vs_Eta_EB_Before->ProfileX ("_DEta_vs_Eta_EB_Before") ;
 profile_hDEta_vs_Eta_EB_Before->SetMarkerColor(kRed);
 profile_hDEta_vs_Eta_EB_Before->SetLineColor(kRed);
 profile_hDEta_vs_Eta_EB_Before->SetMarkerStyle(20);
 profile_hDEta_vs_Eta_EB_Before->SetMarkerSize(1);
 profile_hDEta_vs_Eta_EB_Before->SetLineWidth(1);
 
 TProfile * profile_hDEta_vs_Phi_EB_Before = hDEta_vs_Phi_EB_Before->ProfileX ("_DEta_vs_Phi_EB_Before") ;
 profile_hDEta_vs_Phi_EB_Before->SetMarkerColor(kRed);
 profile_hDEta_vs_Phi_EB_Before->SetLineColor(kRed);
 profile_hDEta_vs_Phi_EB_Before->SetMarkerStyle(20);
 profile_hDEta_vs_Phi_EB_Before->SetMarkerSize(1);
 profile_hDEta_vs_Phi_EB_Before->SetLineWidth(1);
 
 TProfile * profile_hDPhi_vs_Eta_EB_After = hDPhi_vs_Eta_EB_After->ProfileX ("_DPhi_vs_Eta_EB_After") ;
 profile_hDPhi_vs_Eta_EB_After->SetMarkerColor(kBlue);
 profile_hDPhi_vs_Eta_EB_After->SetLineColor(kBlue);
 profile_hDPhi_vs_Eta_EB_After->SetMarkerStyle(20);
 profile_hDPhi_vs_Eta_EB_After->SetMarkerSize(1);
 profile_hDPhi_vs_Eta_EB_After->SetLineWidth(1);
 
 TProfile * profile_hDPhi_vs_Phi_EB_After = hDPhi_vs_Phi_EB_After->ProfileX ("_DPhi_vs_Phi_EB_After") ;
 profile_hDPhi_vs_Phi_EB_After->SetMarkerColor(kBlue);
 profile_hDPhi_vs_Phi_EB_After->SetLineColor(kBlue);
 profile_hDPhi_vs_Phi_EB_After->SetMarkerStyle(20);
 profile_hDPhi_vs_Phi_EB_After->SetMarkerSize(1);
 profile_hDPhi_vs_Phi_EB_After->SetLineWidth(1);
 
 TProfile * profile_hDEta_vs_Eta_EB_After = hDEta_vs_Eta_EB_After->ProfileX ("_DEta_vs_Eta_EB_After") ;
 profile_hDEta_vs_Eta_EB_After->SetMarkerColor(kBlue);
 profile_hDEta_vs_Eta_EB_After->SetLineColor(kBlue);
 profile_hDEta_vs_Eta_EB_After->SetMarkerStyle(20);
 profile_hDEta_vs_Eta_EB_After->SetMarkerSize(1);
 profile_hDEta_vs_Eta_EB_After->SetLineWidth(1);
 
 TProfile * profile_hDEta_vs_Phi_EB_After = hDEta_vs_Phi_EB_After->ProfileX ("_DEta_vs_Phi_EB_After") ;
 profile_hDEta_vs_Phi_EB_After->SetMarkerColor(kBlue);
 profile_hDEta_vs_Phi_EB_After->SetLineColor(kBlue);
 profile_hDEta_vs_Phi_EB_After->SetMarkerStyle(20);
 profile_hDEta_vs_Phi_EB_After->SetMarkerSize(1);
 profile_hDEta_vs_Phi_EB_After->SetLineWidth(1);
 
 TCanvas* cProfile_DETA_DPHI_vs_Eta_Phi_EB = new TCanvas ("cProfile_DETA_DPHI_vs_Eta_Phi_EB","cProfile_DETA_DPHI_vs_Eta_Phi_EB",1000,1000);
 cProfile_DETA_DPHI_vs_Eta_Phi_EB->Divide(2,2);
 
 cProfile_DETA_DPHI_vs_Eta_Phi_EB->cd(1);
 profile_hDPhi_vs_Eta_EB_Before->Draw("PL");
 profile_hDPhi_vs_Eta_EB_After->Draw("same");
 gPad->BuildLegend();
 gPad->SetGrid();
 
 cProfile_DETA_DPHI_vs_Eta_Phi_EB->cd(2);
 profile_hDEta_vs_Eta_EB_Before->Draw("PL");
 profile_hDEta_vs_Eta_EB_After->Draw("same");
 gPad->BuildLegend();
 gPad->SetGrid();
  
 cProfile_DETA_DPHI_vs_Eta_Phi_EB->cd(3);
 profile_hDPhi_vs_Phi_EB_Before->Draw("PL");
 profile_hDPhi_vs_Phi_EB_After->Draw("same");
 gPad->BuildLegend();
 gPad->SetGrid();
 
 cProfile_DETA_DPHI_vs_Eta_Phi_EB->cd(4);
 profile_hDEta_vs_Phi_EB_Before->Draw("PL");
 profile_hDEta_vs_Phi_EB_After->Draw("same");
 gPad->BuildLegend();
 gPad->SetGrid();
 
 
 
 
 ///==== 1D ====
 TString genCut_EBPlus = Form("%s && etaSC>0 && abs(etaSC)<1.5",genCut.Data());
 TString genCut_EBMinus = Form("%s && etaSC<0 && abs(etaSC)<1.5",genCut.Data());
 
 TH1F* hDEta_EBPlus_Before = new TH1F("hDEta_EBPlus_Before","#Delta#eta EB+ Before",NBIN_DETA,MINDETA,MAXDETA);
 myTreeBefore->Draw("DeltaEtaIn >> hDEta_EBPlus_Before",genCut_EBPlus,"goff");
 hDEta_EBPlus_Before->SetMarkerColor(kRed);
 hDEta_EBPlus_Before->SetLineColor(kRed);
 hDEta_EBPlus_Before->SetMarkerStyle(20);
 hDEta_EBPlus_Before->SetMarkerSize(1);
 hDEta_EBPlus_Before->SetLineWidth(1);
 
 TH1F* hDEta_EBPlus_After = new TH1F("hDEta_EBPlus_After","#Delta#eta EB+ After",NBIN_DETA,MINDETA,MAXDETA);
 myTreeAfter->Draw("DeltaEtaIn >> hDEta_EBPlus_After",genCut_EBPlus,"goff");
 hDEta_EBPlus_After->SetMarkerColor(kBlue);
 hDEta_EBPlus_After->SetLineColor(kBlue);
 hDEta_EBPlus_After->SetMarkerStyle(20);
 hDEta_EBPlus_After->SetMarkerSize(1);
 hDEta_EBPlus_After->SetLineWidth(1);
 
 TH1F* hDEta_EBMinus_Before = new TH1F("hDEta_EBMinus_Before","#Delta#eta EB- Before",NBIN_DETA,MINDETA,MAXDETA);
 myTreeBefore->Draw("DeltaEtaIn >> hDEta_EBMinus_Before",genCut_EBMinus,"goff");
 hDEta_EBMinus_Before->SetMarkerColor(kRed);
 hDEta_EBMinus_Before->SetLineColor(kRed);
 hDEta_EBMinus_Before->SetMarkerStyle(20);
 hDEta_EBMinus_Before->SetMarkerSize(1);
 hDEta_EBMinus_Before->SetLineWidth(1);
 
 TH1F* hDEta_EBMinus_After = new TH1F("hDEta_EBMinus_After","#Delta#eta EB- After",NBIN_DETA,MINDETA,MAXDETA);
 myTreeAfter->Draw("DeltaEtaIn >> hDEta_EBMinus_After",genCut_EBMinus,"goff");
 hDEta_EBMinus_After->SetMarkerColor(kBlue);
 hDEta_EBMinus_After->SetLineColor(kBlue);
 hDEta_EBMinus_After->SetMarkerStyle(20);
 hDEta_EBMinus_After->SetMarkerSize(1);
 hDEta_EBMinus_After->SetLineWidth(1);
 
 
 TH1F* hDPhi_EBPlus_Before = new TH1F("hDPhi_EBPlus_Before","#Delta#phi EB+ Before",NBIN_DPHI,MINDPHI,MAXDPHI);
 myTreeBefore->Draw("DeltaPhiIn >> hDPhi_EBPlus_Before",genCut_EBPlus,"goff");
 hDPhi_EBPlus_Before->SetMarkerColor(kRed);
 hDPhi_EBPlus_Before->SetLineColor(kRed);
 hDPhi_EBPlus_Before->SetMarkerStyle(20);
 hDPhi_EBPlus_Before->SetMarkerSize(1);
 hDPhi_EBPlus_Before->SetLineWidth(1);
 
 TH1F* hDPhi_EBPlus_After = new TH1F("hDPhi_EBPlus_After","#Delta#phi EB+ After",NBIN_DPHI,MINDPHI,MAXDPHI);
 myTreeAfter->Draw("DeltaPhiIn >> hDPhi_EBPlus_After",genCut_EBPlus,"goff");
 hDPhi_EBPlus_After->SetMarkerColor(kBlue);
 hDPhi_EBPlus_After->SetLineColor(kBlue);
 hDPhi_EBPlus_After->SetMarkerStyle(20);
 hDPhi_EBPlus_After->SetMarkerSize(1);
 hDPhi_EBPlus_After->SetLineWidth(1);
 
 TH1F* hDPhi_EBMinus_Before = new TH1F("hDPhi_EBMinus_Before","#Delta#phi EB- Before",NBIN_DPHI,MINDPHI,MAXDPHI);
 myTreeBefore->Draw("DeltaPhiIn >> hDPhi_EBMinus_Before",genCut_EBMinus,"goff");
 hDPhi_EBMinus_Before->SetMarkerColor(kRed);
 hDPhi_EBMinus_Before->SetLineColor(kRed);
 hDPhi_EBMinus_Before->SetMarkerStyle(20);
 hDPhi_EBMinus_Before->SetMarkerSize(1);
 hDPhi_EBMinus_Before->SetLineWidth(1);
 
 TH1F* hDPhi_EBMinus_After = new TH1F("hDPhi_EBMinus_After","#Delta#phi EB- After",NBIN_DPHI,MINDPHI,MAXDPHI);
 myTreeAfter->Draw("DeltaPhiIn >> hDPhi_EBMinus_After",genCut_EBMinus,"goff");
 hDPhi_EBMinus_After->SetMarkerColor(kBlue);
 hDPhi_EBMinus_After->SetLineColor(kBlue);
 hDPhi_EBMinus_After->SetMarkerStyle(20);
 hDPhi_EBMinus_After->SetMarkerSize(1);
 hDPhi_EBMinus_After->SetLineWidth(1);
 
 ///==== Draw ====
 TCanvas* cDETA_DPHI_EB = new TCanvas ("cDETA_DPHI_EB","cDETA_DPHI_EB",1000,1000);
 cDETA_DPHI_EB->Divide(2,2);
 
 cDETA_DPHI_EB->cd(1);
 hDEta_EBMinus_After->Draw("PLS");
 hDEta_EBMinus_After->Scale(hDEta_EBMinus_Before->GetEntries()/hDEta_EBMinus_After->GetEntries());
 gPad->Update();
 stEB[0]= (TPaveStats*)(hDEta_EBMinus_After->GetListOfFunctions()->FindObject("stats"));
 stEB[0]->SetY1NDC(0.85); //new x start position
 stEB[0]->SetY2NDC(1.00); //new x end position
 stEB[0]->SetTextColor(kBlue);
 stEB[0]->Draw();
 hDEta_EBMinus_Before->Draw("LPsameS");
 hDEta_EBMinus_Before->GetXaxis()->SetTitle("#Delta#eta");
 gPad->Update();
 stEB[1]= (TPaveStats*)(hDEta_EBMinus_Before->GetListOfFunctions()->FindObject("stats"));
 stEB[1]->SetY1NDC(0.72); //new x start position
 stEB[1]->SetY2NDC(0.85); //new x end position
 stEB[1]->SetTextColor(kRed);
 stEB[1]->Draw(); 
 gPad->SetGrid();
 gPad->BuildLegend();
 gPad->Update();

 
 cDETA_DPHI_EB->cd(2);
 hDEta_EBPlus_After->Draw("PLS");
 hDEta_EBPlus_After->Scale(hDEta_EBPlus_Before->GetEntries()/hDEta_EBPlus_After->GetEntries());
 gPad->Update();
 stEB[2]= (TPaveStats*)(hDEta_EBPlus_After->GetListOfFunctions()->FindObject("stats"));
 stEB[2]->SetY1NDC(0.85); //new x start position
 stEB[2]->SetY2NDC(1.00); //new x end position
 stEB[2]->SetTextColor(kBlue);
 stEB[2]->Draw();
 hDEta_EBPlus_Before->Draw("LPsameS"); 
 hDEta_EBPlus_Before->GetXaxis()->SetTitle("#Delta#eta");
 gPad->Update();
 stEB[3]= (TPaveStats*)(hDEta_EBPlus_Before->GetListOfFunctions()->FindObject("stats"));
 stEB[3]->SetY1NDC(0.72); //new x start position
 stEB[3]->SetY2NDC(0.85); //new x end position
 stEB[3]->SetTextColor(kRed);
 stEB[3]->Draw(); 
 gPad->SetGrid();
 gPad->BuildLegend();
 gPad->Update();
 
 cDETA_DPHI_EB->cd(3);
 hDPhi_EBMinus_After->Draw("PLS");
 hDPhi_EBMinus_After->Scale(hDPhi_EBMinus_Before->GetEntries()/hDPhi_EBMinus_After->GetEntries());
 gPad->Update();
 stEB[4]= (TPaveStats*)(hDPhi_EBMinus_After->GetListOfFunctions()->FindObject("stats"));
 stEB[4]->SetY1NDC(0.85); //new x start position
 stEB[4]->SetY2NDC(1.00); //new x end position
 stEB[4]->SetTextColor(kBlue);
 stEB[4]->Draw();
 hDPhi_EBMinus_Before->Draw("LPsameS"); 
 hDPhi_EBMinus_Before->GetXaxis()->SetTitle("#Delta#phi");
 gPad->Update();
 stEB[5]= (TPaveStats*)(hDPhi_EBMinus_Before->GetListOfFunctions()->FindObject("stats"));
 stEB[5]->SetY1NDC(0.72); //new x start position
 stEB[5]->SetY2NDC(0.85); //new x end position
 stEB[5]->SetTextColor(kRed);
 stEB[5]->Draw(); 
 gPad->SetGrid();
 gPad->BuildLegend();
 gPad->Update();
 
 cDETA_DPHI_EB->cd(4);
 hDPhi_EBPlus_After->Draw("PLS");
 hDPhi_EBPlus_After->Scale(hDPhi_EBPlus_Before->GetEntries()/hDPhi_EBPlus_After->GetEntries());
 gPad->Update();
 stEB[6]= (TPaveStats*)(hDPhi_EBPlus_After->GetListOfFunctions()->FindObject("stats"));
 stEB[6]->SetY1NDC(0.85); //new x start position
 stEB[6]->SetY2NDC(1.00); //new x end position
 stEB[6]->SetTextColor(kBlue);
 stEB[6]->Draw();
 hDPhi_EBPlus_Before->Draw("LPsameS"); 
 hDPhi_EBPlus_Before->GetXaxis()->SetTitle("#Delta#phi");
 gPad->Update();
 stEB[7]= (TPaveStats*)(hDPhi_EBPlus_Before->GetListOfFunctions()->FindObject("stats"));
 stEB[7]->SetY1NDC(0.72); //new x start position
 stEB[7]->SetY2NDC(0.85); //new x end position
 stEB[7]->SetTextColor(kRed);
 stEB[7]->Draw(); 
 gPad->SetGrid();
 gPad->BuildLegend();
 gPad->Update();

}



