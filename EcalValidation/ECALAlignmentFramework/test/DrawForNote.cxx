#include "test/TDRStyle.cc"

void createHisto(TH1F* hdata, TH1F* hmc, TTree* tdata, TTree* tmc, char* variable, char* title, char* cut) {
 
 TString toDraw;
 TString tcut;
 
 tcut = Form("%s",cut);
 toDraw = Form("%s >> %s",variable,hdata->GetName());
 tdata -> Draw(toDraw.Data(),tcut.Data());
 tcut = Form("(%s) * weight_PU",cut);
 toDraw = Form("%s >> %s",variable,hmc->GetName());
 tmc   -> Draw(toDraw.Data(),tcut.Data());
 
 hdata->GetXaxis()->SetTitle(title);
 hmc  ->GetXaxis()->SetTitle(title);
 
 hdata->SetLineColor(kBlack);
 hdata->SetLineWidth(1);
 hdata->SetFillColor(kWhite);
 
 hmc->SetLineColor(kTeal);
 hmc->SetLineWidth(2);
 hmc->SetFillColor(kTeal);
 hmc->SetFillStyle(3002);
 
 double integralData;
 double integralMC  ;
 
 integralData = hdata->Integral();
 integralMC   = hmc->Integral();
 hmc->Scale(integralData/integralMC);
 
}



void DrawForNote(){
 TDRStyle();
 
 TFile *fileDATA = TFile::Open("data/out_Skimmed_DATA_2011_Tag_GR_R_44_V12_0.root");
 TFile *fileMC   = TFile::Open("data/out_Skimmed_W_Fall11_0.root");
 
 TTree* trDATA = (TTree*) fileDATA-> Get("ntupleEcalAlignment/myTree");
 TTree* trMC   = (TTree*) fileMC  -> Get("ntupleEcalAlignment/myTree");
 

 
 
 ///---- all ----
 TH1F* DPhiDATA = new TH1F("DPhiDATA","#Delta#phi Data",500,-0.04,0.04);
 TH1F* DPhiMC   = new TH1F("DPhiMC",  "#Delta#phi MC"  ,500,-0.04,0.04);

 TH1F* DEtaDATA = new TH1F("DEtaDATA","#Delta#eta Data",500,-0.02,0.02);
 TH1F* DEtaMC   = new TH1F("DEtaMC"  ,"#Delta#eta MC"  ,500,-0.02,0.02);
 
 createHisto(DPhiDATA, DPhiMC, trDATA, trMC, "deltaPhiSuperClusterAtVtx", "#Delta#phi","1");
 createHisto(DEtaDATA, DEtaMC, trDATA, trMC, "deltaEtaSuperClusterAtVtx", "#Delta#eta","1"); 

 ///---- legend (begin) ----
 TLegend* legend = new TLegend(0.60,0.65,0.80,0.85);
 legend->AddEntry(DEtaDATA);
 legend->AddEntry(DEtaMC);
 legend->SetFillColor(kWhite);
 ///---- legend (end) ----
 
 
 TCanvas* cDPhi = new TCanvas("cDphi","cDphi",800,800);
 DPhiDATA->Draw("PE");
 DPhiMC->Draw("same");
 
 legend->Draw();
 cDPhi->SetGrid();   
 cDPhi->Print("cDPhi.pdf");
 
 TCanvas* cDEta = new TCanvas("cDeta","cDeta",800,800);
 DEtaDATA->Draw("PE");
 DEtaMC->Draw("same");
 
 legend->Draw();
 cDEta->SetGrid();   
 cDEta->Print("cDEta.pdf");
 

 ///---- EB+ ----
 TH1F* DPhiDATAEBplus = new TH1F("DPhiDATAEBplus","#Delta#phi Data",500,-0.04,0.04);
 TH1F* DPhiMCEBplus   = new TH1F("DPhiMCEBplus",  "#Delta#phi MC"  ,500,-0.04,0.04);
 
 TH1F* DEtaDATAEBplus = new TH1F("DEtaDATAEBplus","#Delta#eta Data",500,-0.02,0.02);
 TH1F* DEtaMCEBplus   = new TH1F("DEtaMCEBplus"  ,"#Delta#eta MC"  ,500,-0.02,0.02);
 
 createHisto(DPhiDATAEBplus, DPhiMCEBplus, trDATA, trMC, "deltaPhiSuperClusterAtVtx", "#Delta#phi","eta>0 && eta<1.5");
 createHisto(DEtaDATAEBplus, DEtaMCEBplus, trDATA, trMC, "deltaEtaSuperClusterAtVtx", "#Delta#eta","eta>0 && eta<1.5"); 
 
 TCanvas* cDPhiEBplus = new TCanvas("cDphiEBplus2011","cDphiEBplus2011",800,800);
 DPhiDATAEBplus->Draw("PE");
 DPhiMCEBplus->Draw("same");
 
 legend->Draw();
 cDPhiEBplus->SetGrid();   
 cDPhiEBplus->Print("cDPhiEBplus.pdf");
 
 TCanvas* cDEtaEBplus = new TCanvas("cDeta2011EBplus","cDeta2011EBplus",800,800);
 DEtaDATAEBplus->Draw("PE");
 DEtaMCEBplus->Draw("same");
 
 legend->Draw();
 cDEtaEBplus->SetGrid();   
 cDEtaEBplus->Print("cDEtaEBplus.pdf");
 
 
 ///---- EB- ----
 TH1F* DPhiDATAEBminus = new TH1F("DPhiDATAEBminus","#Delta#phi Data",500,-0.04,0.04);
 TH1F* DPhiMCEBminus   = new TH1F("DPhiMCEBminus",  "#Delta#phi MC"  ,500,-0.04,0.04);
 
 TH1F* DEtaDATAEBminus = new TH1F("DEtaDATAEBminus","#Delta#eta Data",500,-0.02,0.02);
 TH1F* DEtaMCEBminus   = new TH1F("DEtaMCEBminus"  ,"#Delta#eta MC"  ,500,-0.02,0.02);
 
 createHisto(DPhiDATAEBminus, DPhiMCEBminus, trDATA, trMC, "deltaPhiSuperClusterAtVtx", "#Delta#phi","eta<0 && eta>-1.5");
 createHisto(DEtaDATAEBminus, DEtaMCEBminus, trDATA, trMC, "deltaEtaSuperClusterAtVtx", "#Delta#eta","eta<0 && eta>-1.5"); 
 
 TCanvas* cDPhiEBminus = new TCanvas("cDphiEBminus2011","cDphiEBminus2011",800,800);
 DPhiDATAEBminus->Draw("PE");
 DPhiMCEBminus->Draw("same");
 
 legend->Draw();
 cDPhiEBminus->SetGrid();   
 cDPhiEBminus->Print("cDPhiEBminus.pdf");
 
 TCanvas* cDEtaEBminus = new TCanvas("cDeta2011EBminus","cDeta2011EBminus",800,800);
 DEtaDATAEBminus->Draw("PE");
 DEtaMCEBminus->Draw("same");
 
 legend->Draw();
 cDEtaEBminus->SetGrid();   
 cDEtaEBminus->Print("cDEtaEBminus.pdf");
 
 

 
 
 
 
 ///---- EE+ ----
 TH1F* DPhiDATAEEplus = new TH1F("DPhiDATAEEplus","#Delta#phi Data",500,-0.04,0.04);
 TH1F* DPhiMCEEplus   = new TH1F("DPhiMCEEplus",  "#Delta#phi MC"  ,500,-0.04,0.04);
 
 TH1F* DEtaDATAEEplus = new TH1F("DEtaDATAEEplus","#Delta#eta Data",500,-0.02,0.02);
 TH1F* DEtaMCEEplus   = new TH1F("DEtaMCEEplus"  ,"#Delta#eta MC"  ,500,-0.02,0.02);
 
 createHisto(DPhiDATAEEplus, DPhiMCEEplus, trDATA, trMC, "deltaPhiSuperClusterAtVtx", "#Delta#phi","eta>1.5");
 createHisto(DEtaDATAEEplus, DEtaMCEEplus, trDATA, trMC, "deltaEtaSuperClusterAtVtx", "#Delta#eta","eta>1.5"); 
 
 TCanvas* cDPhiEEplus = new TCanvas("cDphiEEplus2011","cDphiEEplus2011",800,800);
 DPhiDATAEEplus->Draw("PE");
 DPhiMCEEplus->Draw("same");
 
 legend->Draw();
 cDPhiEEplus->SetGrid();   
 cDPhiEEplus->Print("cDPhiEEplus.pdf");
 
 TCanvas* cDEtaEEplus = new TCanvas("cDeta2011EEplus","cDeta2011EEplus",800,800);
 DEtaDATAEEplus->Draw("PE");
 DEtaMCEEplus->Draw("same");
 
 legend->Draw();
 cDEtaEEplus->SetGrid();   
 cDEtaEEplus->Print("cDEtaEEplus.pdf");
 
 
 ///---- EE- ----
 TH1F* DPhiDATAEEminus = new TH1F("DPhiDATAEEminus","#Delta#phi Data",500,-0.04,0.04);
 TH1F* DPhiMCEEminus   = new TH1F("DPhiMCEEminus",  "#Delta#phi MC"  ,500,-0.04,0.04);
 
 TH1F* DEtaDATAEEminus = new TH1F("DEtaDATAEEminus","#Delta#eta Data",500,-0.02,0.02);
 TH1F* DEtaMCEEminus   = new TH1F("DEtaMCEEminus"  ,"#Delta#eta MC"  ,500,-0.02,0.02);
 
 createHisto(DPhiDATAEEminus, DPhiMCEEminus, trDATA, trMC, "deltaPhiSuperClusterAtVtx", "#Delta#phi","eta<-1.5");
 createHisto(DEtaDATAEEminus, DEtaMCEEminus, trDATA, trMC, "deltaEtaSuperClusterAtVtx", "#Delta#eta","eta<-1.5"); 
 
 TCanvas* cDPhiEEminus = new TCanvas("cDphiEEminus2011","cDphiEEminus2011",800,800);
 DPhiDATAEEminus->Draw("PE");
 DPhiMCEEminus->Draw("same");
 
 legend->Draw();
 cDPhiEEminus->SetGrid();   
 cDPhiEEminus->Print("cDPhiEEminus.pdf");
 
 TCanvas* cDEtaEEminus = new TCanvas("cDeta2011EEminus","cDeta2011EEminus",800,800);
 DEtaDATAEEminus->Draw("PE");
 DEtaMCEEminus->Draw("same");
 
 legend->Draw();
 cDEtaEEminus->SetGrid();   
 cDEtaEEminus->Print("cDEtaEEminus.pdf");
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 ///---- EB+ ----
 TH1F* DPhiDATAEBpluseplus = new TH1F("DPhiDATAEBpluseplus","#Delta#phi Data",500,-0.04,0.04);
 TH1F* DPhiMCEBpluseplus   = new TH1F("DPhiMCEBpluseplus",  "#Delta#phi MC"  ,500,-0.04,0.04);
 
 TH1F* DEtaDATAEBpluseplus = new TH1F("DEtaDATAEBpluseplus","#Delta#eta Data",500,-0.02,0.02);
 TH1F* DEtaMCEBpluseplus   = new TH1F("DEtaMCEBpluseplus"  ,"#Delta#eta MC"  ,500,-0.02,0.02);
 
 createHisto(DPhiDATAEBpluseplus, DPhiMCEBpluseplus, trDATA, trMC, "deltaPhiSuperClusterAtVtx", "#Delta#phi","eta>0 && eta<1.5 && eleCharge>0");
 createHisto(DEtaDATAEBpluseplus, DEtaMCEBpluseplus, trDATA, trMC, "deltaEtaSuperClusterAtVtx", "#Delta#eta","eta>0 && eta<1.5 && eleCharge>0"); 
 
 TCanvas* cDPhiEBpluseplus = new TCanvas("cDphiEBpluseplus2011","cDphiEBpluseplus2011",800,800);
 DPhiDATAEBpluseplus->Draw("PE");
 DPhiMCEBpluseplus->Draw("same");
 
 legend->Draw();
 cDPhiEBpluseplus->SetGrid();   
 cDPhiEBpluseplus->Print("cDPhiEBpluseplus.pdf");
 
 TCanvas* cDEtaEBpluseplus = new TCanvas("cDeta2011EBpluseplus","cDeta2011EBpluseplus",800,800);
 DEtaDATAEBpluseplus->Draw("PE");
 DEtaMCEBpluseplus->Draw("same");
 
 legend->Draw();
 cDEtaEBpluseplus->SetGrid();   
 cDEtaEBpluseplus->Print("cDEtaEBpluseplus.pdf");
 
 
 ///---- EB- ----
 TH1F* DPhiDATAEBminuseplus = new TH1F("DPhiDATAEBminuseplus","#Delta#phi Data",500,-0.04,0.04);
 TH1F* DPhiMCEBminuseplus   = new TH1F("DPhiMCEBminuseplus",  "#Delta#phi MC"  ,500,-0.04,0.04);
 
 TH1F* DEtaDATAEBminuseplus = new TH1F("DEtaDATAEBminuseplus","#Delta#eta Data",500,-0.02,0.02);
 TH1F* DEtaMCEBminuseplus   = new TH1F("DEtaMCEBminuseplus"  ,"#Delta#eta MC"  ,500,-0.02,0.02);
 
 createHisto(DPhiDATAEBminuseplus, DPhiMCEBminuseplus, trDATA, trMC, "deltaPhiSuperClusterAtVtx", "#Delta#phi","eta<0 && eta>-1.5 && eleCharge>0");
 createHisto(DEtaDATAEBminuseplus, DEtaMCEBminuseplus, trDATA, trMC, "deltaEtaSuperClusterAtVtx", "#Delta#eta","eta<0 && eta>-1.5 && eleCharge>0"); 
 
 TCanvas* cDPhiEBminuseplus = new TCanvas("cDphiEBminuseplus2011","cDphiEBminuseplus2011",800,800);
 DPhiDATAEBminuseplus->Draw("PE");
 DPhiMCEBminuseplus->Draw("same");
 
 legend->Draw();
 cDPhiEBminuseplus->SetGrid();   
 cDPhiEBminuseplus->Print("cDPhiEBminuseplus.pdf");
 
 TCanvas* cDEtaEBminuseplus = new TCanvas("cDeta2011EBminuseplus","cDeta2011EBminuseplus",800,800);
 DEtaDATAEBminuseplus->Draw("PE");
 DEtaMCEBminuseplus->Draw("same");
 
 legend->Draw();
 cDEtaEBminuseplus->SetGrid();   
 cDEtaEBminuseplus->Print("cDEtaEBminuseplus.pdf");
 
 
 
 
 
 
 
 ///---- EE+ ----
 TH1F* DPhiDATAEEpluseplus = new TH1F("DPhiDATAEEpluseplus","#Delta#phi Data",500,-0.04,0.04);
 TH1F* DPhiMCEEpluseplus   = new TH1F("DPhiMCEEpluseplus",  "#Delta#phi MC"  ,500,-0.04,0.04);
 
 TH1F* DEtaDATAEEpluseplus = new TH1F("DEtaDATAEEpluseplus","#Delta#eta Data",500,-0.02,0.02);
 TH1F* DEtaMCEEpluseplus   = new TH1F("DEtaMCEEpluseplus"  ,"#Delta#eta MC"  ,500,-0.02,0.02);
 
 createHisto(DPhiDATAEEpluseplus, DPhiMCEEpluseplus, trDATA, trMC, "deltaPhiSuperClusterAtVtx", "#Delta#phi","eta>1.5 && eleCharge>0");
 createHisto(DEtaDATAEEpluseplus, DEtaMCEEpluseplus, trDATA, trMC, "deltaEtaSuperClusterAtVtx", "#Delta#eta","eta>1.5 && eleCharge>0"); 
 
 TCanvas* cDPhiEEpluseplus = new TCanvas("cDphiEEpluseplus2011","cDphiEEpluseplus2011",800,800);
 DPhiDATAEEpluseplus->Draw("PE");
 DPhiMCEEpluseplus->Draw("same");
 
 legend->Draw();
 cDPhiEEpluseplus->SetGrid();   
 cDPhiEEpluseplus->Print("cDPhiEEpluseplus.pdf");
 
 TCanvas* cDEtaEEpluseplus = new TCanvas("cDeta2011EEpluseplus","cDeta2011EEpluseplus",800,800);
 DEtaDATAEEpluseplus->Draw("PE");
 DEtaMCEEpluseplus->Draw("same");
 
 legend->Draw();
 cDEtaEEpluseplus->SetGrid();   
 cDEtaEEpluseplus->Print("cDEtaEEpluseplus.pdf");
 
 
 ///---- EE- ----
 TH1F* DPhiDATAEEminuseplus = new TH1F("DPhiDATAEEminuseplus","#Delta#phi Data",500,-0.04,0.04);
 TH1F* DPhiMCEEminuseplus   = new TH1F("DPhiMCEEminuseplus",  "#Delta#phi MC"  ,500,-0.04,0.04);
 
 TH1F* DEtaDATAEEminuseplus = new TH1F("DEtaDATAEEminuseplus","#Delta#eta Data",500,-0.02,0.02);
 TH1F* DEtaMCEEminuseplus   = new TH1F("DEtaMCEEminuseplus"  ,"#Delta#eta MC"  ,500,-0.02,0.02);
 
 createHisto(DPhiDATAEEminuseplus, DPhiMCEEminuseplus, trDATA, trMC, "deltaPhiSuperClusterAtVtx", "#Delta#phi","eta<-1.5 && eleCharge>0");
 createHisto(DEtaDATAEEminuseplus, DEtaMCEEminuseplus, trDATA, trMC, "deltaEtaSuperClusterAtVtx", "#Delta#eta","eta<-1.5 && eleCharge>0"); 
 
 TCanvas* cDPhiEEminuseplus = new TCanvas("cDphiEEminuseplus2011","cDphiEEminuseplus2011",800,800);
 DPhiDATAEEminuseplus->Draw("PE");
 DPhiMCEEminuseplus->Draw("same");
 
 legend->Draw();
 cDPhiEEminuseplus->SetGrid();   
 cDPhiEEminuseplus->Print("cDPhiEEminuseplus.pdf");
 
 TCanvas* cDEtaEEminuseplus = new TCanvas("cDeta2011EEminuseplus","cDeta2011EEminuseplus",800,800);
 DEtaDATAEEminuseplus->Draw("PE");
 DEtaMCEEminuseplus->Draw("same");
 
 legend->Draw();
 cDEtaEEminuseplus->SetGrid();   
 cDEtaEEminuseplus->Print("cDEtaEEminuseplus.pdf");
 
 
 
 
 
 
 
 
 
 
 
 
 ///---- EB+ ----
 TH1F* DPhiDATAEBpluseminus = new TH1F("DPhiDATAEBpluseminus","#Delta#phi Data",500,-0.04,0.04);
 TH1F* DPhiMCEBpluseminus   = new TH1F("DPhiMCEBpluseminus",  "#Delta#phi MC"  ,500,-0.04,0.04);
 
 TH1F* DEtaDATAEBpluseminus = new TH1F("DEtaDATAEBpluseminus","#Delta#eta Data",500,-0.02,0.02);
 TH1F* DEtaMCEBpluseminus   = new TH1F("DEtaMCEBpluseminus"  ,"#Delta#eta MC"  ,500,-0.02,0.02);
 
 createHisto(DPhiDATAEBpluseminus, DPhiMCEBpluseminus, trDATA, trMC, "deltaPhiSuperClusterAtVtx", "#Delta#phi","eta>0 && eta<1.5 && eleCharge<0");
 createHisto(DEtaDATAEBpluseminus, DEtaMCEBpluseminus, trDATA, trMC, "deltaEtaSuperClusterAtVtx", "#Delta#eta","eta>0 && eta<1.5 && eleCharge<0"); 
 
 TCanvas* cDPhiEBpluseminus = new TCanvas("cDphiEBpluseminus2011","cDphiEBpluseminus2011",800,800);
 DPhiDATAEBpluseminus->Draw("PE");
 DPhiMCEBpluseminus->Draw("same");
 
 legend->Draw();
 cDPhiEBpluseminus->SetGrid();   
 cDPhiEBpluseminus->Print("cDPhiEBpluseminus.pdf");
 
 TCanvas* cDEtaEBpluseminus = new TCanvas("cDeta2011EBpluseminus","cDeta2011EBpluseminus",800,800);
 DEtaDATAEBpluseminus->Draw("PE");
 DEtaMCEBpluseminus->Draw("same");
 
 legend->Draw();
 cDEtaEBpluseminus->SetGrid();   
 cDEtaEBpluseminus->Print("cDEtaEBpluseminus.pdf");
 
 
 ///---- EB- ----
 TH1F* DPhiDATAEBminuseminus = new TH1F("DPhiDATAEBminuseminus","#Delta#phi Data",500,-0.04,0.04);
 TH1F* DPhiMCEBminuseminus   = new TH1F("DPhiMCEBminuseminus",  "#Delta#phi MC"  ,500,-0.04,0.04);
 
 TH1F* DEtaDATAEBminuseminus = new TH1F("DEtaDATAEBminuseminus","#Delta#eta Data",500,-0.02,0.02);
 TH1F* DEtaMCEBminuseminus   = new TH1F("DEtaMCEBminuseminus"  ,"#Delta#eta MC"  ,500,-0.02,0.02);
 
 createHisto(DPhiDATAEBminuseminus, DPhiMCEBminuseminus, trDATA, trMC, "deltaPhiSuperClusterAtVtx", "#Delta#phi","eta<0 && eta>-1.5 && eleCharge<0");
 createHisto(DEtaDATAEBminuseminus, DEtaMCEBminuseminus, trDATA, trMC, "deltaEtaSuperClusterAtVtx", "#Delta#eta","eta<0 && eta>-1.5 && eleCharge<0"); 
 
 TCanvas* cDPhiEBminuseminus = new TCanvas("cDphiEBminuseminus2011","cDphiEBminuseminus2011",800,800);
 DPhiDATAEBminuseminus->Draw("PE");
 DPhiMCEBminuseminus->Draw("same");
 
 legend->Draw();
 cDPhiEBminuseminus->SetGrid();   
 cDPhiEBminuseminus->Print("cDPhiEBminuseminus.pdf");
 
 TCanvas* cDEtaEBminuseminus = new TCanvas("cDeta2011EBminuseminus","cDeta2011EBminuseminus",800,800);
 DEtaDATAEBminuseminus->Draw("PE");
 DEtaMCEBminuseminus->Draw("same");
 
 legend->Draw();
 cDEtaEBminuseminus->SetGrid();   
 cDEtaEBminuseminus->Print("cDEtaEBminuseminus.pdf");
 
 
 
 
 
 
 
 ///---- EE+ ----
 TH1F* DPhiDATAEEpluseminus = new TH1F("DPhiDATAEEpluseminus","#Delta#phi Data",500,-0.04,0.04);
 TH1F* DPhiMCEEpluseminus   = new TH1F("DPhiMCEEpluseminus",  "#Delta#phi MC"  ,500,-0.04,0.04);
 
 TH1F* DEtaDATAEEpluseminus = new TH1F("DEtaDATAEEpluseminus","#Delta#eta Data",500,-0.02,0.02);
 TH1F* DEtaMCEEpluseminus   = new TH1F("DEtaMCEEpluseminus"  ,"#Delta#eta MC"  ,500,-0.02,0.02);
 
 createHisto(DPhiDATAEEpluseminus, DPhiMCEEpluseminus, trDATA, trMC, "deltaPhiSuperClusterAtVtx", "#Delta#phi","eta>1.5 && eleCharge<0");
 createHisto(DEtaDATAEEpluseminus, DEtaMCEEpluseminus, trDATA, trMC, "deltaEtaSuperClusterAtVtx", "#Delta#eta","eta>1.5 && eleCharge<0"); 
 
 TCanvas* cDPhiEEpluseminus = new TCanvas("cDphiEEpluseminus2011","cDphiEEpluseminus2011",800,800);
 DPhiDATAEEpluseminus->Draw("PE");
 DPhiMCEEpluseminus->Draw("same");
 
 legend->Draw();
 cDPhiEEpluseminus->SetGrid();   
 cDPhiEEpluseminus->Print("cDPhiEEpluseminus.pdf");
 
 TCanvas* cDEtaEEpluseminus = new TCanvas("cDeta2011EEpluseminus","cDeta2011EEpluseminus",800,800);
 DEtaDATAEEpluseminus->Draw("PE");
 DEtaMCEEpluseminus->Draw("same");
 
 legend->Draw();
 cDEtaEEpluseminus->SetGrid();   
 cDEtaEEpluseminus->Print("cDEtaEEpluseminus.pdf");
 
 
 ///---- EE- ----
 TH1F* DPhiDATAEEminuseminus = new TH1F("DPhiDATAEEminuseminus","#Delta#phi Data",500,-0.04,0.04);
 TH1F* DPhiMCEEminuseminus   = new TH1F("DPhiMCEEminuseminus",  "#Delta#phi MC"  ,500,-0.04,0.04);
 
 TH1F* DEtaDATAEEminuseminus = new TH1F("DEtaDATAEEminuseminus","#Delta#eta Data",500,-0.02,0.02);
 TH1F* DEtaMCEEminuseminus   = new TH1F("DEtaMCEEminuseminus"  ,"#Delta#eta MC"  ,500,-0.02,0.02);
 
 createHisto(DPhiDATAEEminuseminus, DPhiMCEEminuseminus, trDATA, trMC, "deltaPhiSuperClusterAtVtx", "#Delta#phi","eta<-1.5 && eleCharge<0");
 createHisto(DEtaDATAEEminuseminus, DEtaMCEEminuseminus, trDATA, trMC, "deltaEtaSuperClusterAtVtx", "#Delta#eta","eta<-1.5 && eleCharge<0"); 
 
 TCanvas* cDPhiEEminuseminus = new TCanvas("cDphiEEminuseminus2011","cDphiEEminuseminus2011",800,800);
 DPhiDATAEEminuseminus->Draw("PE");
 DPhiMCEEminuseminus->Draw("same");
 
 legend->Draw();
 cDPhiEEminuseminus->SetGrid();   
 cDPhiEEminuseminus->Print("cDPhiEEminuseminus.pdf");
 
 TCanvas* cDEtaEEminuseminus = new TCanvas("cDeta2011EEminuseminus","cDeta2011EEminuseminus",800,800);
 DEtaDATAEEminuseminus->Draw("PE");
 DEtaMCEEminuseminus->Draw("same");
 
 legend->Draw();
 cDEtaEEminuseminus->SetGrid();   
 cDEtaEEminuseminus->Print("cDEtaEEminuseminus.pdf");
 
 
 
 
 
 
 
 ///---- eta distribution ----
 TH1F* EtaDATA = new TH1F("EtaDATA","#eta Data",100,-5.0,5.0);
 TH1F* EtaMC   = new TH1F("EtaMC",  "#eta MC"  ,100,-5.0,5.0);
 
 createHisto(DPhiDATA, DPhiMC, trDATA, trMC, "deltaPhiSuperClusterAtVtx", "#Delta#phi","1");
 createHisto(EtaDATA, EtaMC, trDATA, trMC, "etaSC", "#eta_{SC}","1"); 
 
 TCanvas* cEta = new TCanvas("cEta","cEta",800,800);
 EtaDATA -> Draw("PE");
 EtaMC   -> Draw("same");
 
 legend->Draw();
 cEta->SetGrid();   
 cEta->Print("cEta.pdf");
 
 
 
 
 
 std::cout << " Differences " << std::endl;
 
//  std::cout << " DPhi V7 = " << DPhiV7->GetMean() << " +/- " << DPhiV7->GetRMS() / sqrt(DPhiV7->GetEntries()) << std::endl;
//  std::cout << " DPhi V8 = " << DPhiV8->GetMean() << " +/- " << DPhiV8->GetRMS() / sqrt(DPhiV8->GetEntries()) << std::endl;
//  
//  std::cout << " DEta V7 = " << DEtaV7->GetMean() << " +/- " << DEtaV7->GetRMS() / sqrt(DEtaV7->GetEntries()) << std::endl;
//  std::cout << " DEta V8 = " << DEtaV8->GetMean() << " +/- " << DEtaV8->GetRMS() / sqrt(DEtaV8->GetEntries()) << std::endl;
 
 
 gApplication->Terminate(0);
 
}