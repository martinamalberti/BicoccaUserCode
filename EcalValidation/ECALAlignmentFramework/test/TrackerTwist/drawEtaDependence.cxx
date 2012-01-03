void drawEtaDependence(TString nameInFileRoot, TString nameOutputDir, TString nameDATA){
 
 int Mean = 1;
 
 TFile* fileIn = new TFile(nameInFileRoot);
 
 TLine lineVert (35.5,-1,35.5,1);
 lineVert.SetLineColor(kGreen);
 lineVert.SetLineWidth(4);
 lineVert.SetLineStyle(3);
 
 gStyle->SetTitleX(0.2f);
 gStyle->SetTitleW(0.5f);
 
 TString nameHisto;
 TH1F* h;
 TH1F* hMC;
 THStack* hs;
 TCanvas* cJoint = new TCanvas("cJoint","cJoint",1000,1000);
 cJoint->SetLeftMargin(0.10);
 cJoint->SetRightMargin(0.25);
 cJoint->SetTopMargin(0.05);
 cJoint->SetBottomMargin(0.20);
 
 
 
 TString toDoShell;
 toDoShell = Form("rm -r %s ;",nameOutputDir.Data());
 system(toDoShell.Data());
 toDoShell = Form("mkdir %s ;",nameOutputDir.Data());
 system(toDoShell.Data());
 toDoShell = Form("mkdir %s/plot ;",nameOutputDir.Data());
 system(toDoShell.Data());
 
 TString toDoROOT;
 
 double norm;
 
 std::vector<double> DATA_Dphi;
 std::vector<double> DATA_Dphi_ePlus;
 std::vector<double> DATA_Dphi_eMinus;
 std::vector<double> DATA_Deta;
 std::vector<double> DATA_err_Dphi;
 std::vector<double> DATA_err_Dphi_ePlus;
 std::vector<double> DATA_err_Dphi_eMinus;
 std::vector<double> DATA_err_Deta;
 
 std::vector<double> MC_Dphi;
 std::vector<double> MC_Dphi_ePlus;
 std::vector<double> MC_Dphi_eMinus;
 std::vector<double> MC_Deta;
 std::vector<double> MC_err_Dphi;
 std::vector<double> MC_err_Dphi_ePlus;
 std::vector<double> MC_err_Dphi_eMinus;
 std::vector<double> MC_err_Deta;
 
 TString nameHistoInRootFile;
 
 int numEta = 30;
//  int numEta = 15;
 
 ///==== EB ====
 for (int iEta = 0; iEta <numEta; iEta++) {
  ///~~~~ Dphi ~~~~
  nameHistoInRootFile = Form("Data/%s_%d_7_Tot_temp",nameDATA.Data(),iEta);
  std::cout << " Dphi  nameHistoInRootFile = " << nameHistoInRootFile.Data() << std::endl;
  h = (TH1F*) fileIn->Get(nameHistoInRootFile);
  nameHistoInRootFile = Form("Data/W_%d_7_Tot_temp",iEta);
  std::cout << " Dphi nameHistoInRootFile = " << nameHistoInRootFile.Data() << std::endl;
  hMC = (TH1F*) fileIn->Get(nameHistoInRootFile);
  cJoint->cd();
  h->Draw();
  norm = h->Integral(); hMC->DrawNormalized("HISTsame",norm);
  cJoint->SetGrid();
  TLegend* leg0 = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
  leg0->SetFillColor(kWhite);
  cJoint->SetGrid();   
  toDoROOT = Form("%s/plot/Dphi_Eta_%d.png",nameOutputDir.Data(),iEta);
  cJoint->SaveAs(toDoROOT);
  cJoint->SaveAs(toDoROOT);
  DATA_Dphi.push_back(h->GetMean());
  DATA_err_Dphi.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
  MC_Dphi.push_back(hMC->GetMean());
  MC_err_Dphi.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
  
  ///~~~~ Dphi e+ ~~~~
  nameHistoInRootFile = Form("Data/%s_%d_7_Tot_temp",nameDATA.Data(),iEta+numEta);
  std::cout << " Dphi e+ nameHistoInRootFile = " << nameHistoInRootFile.Data() << std::endl;
  h = (TH1F*) fileIn->Get(nameHistoInRootFile);
  nameHistoInRootFile = Form("Data/W_%d_7_Tot_temp",iEta+numEta);
  std::cout << " Dphi e+ nameHistoInRootFile = " << nameHistoInRootFile.Data() << std::endl;
  hMC = (TH1F*) fileIn->Get(nameHistoInRootFile);
  cJoint->cd();
  h->Draw();
  norm = h->Integral(); hMC->DrawNormalized("HISTsame",norm);
  cJoint->SetGrid();
  TLegend* leg = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
  leg->SetFillColor(kWhite);
  cJoint->SetGrid();   
  toDoROOT = Form("%s/plot/Dphi_Eta_%d_ePlus.png",nameOutputDir.Data(),iEta);
  cJoint->SaveAs(toDoROOT);
  cJoint->SaveAs(toDoROOT);
  DATA_Dphi_ePlus.push_back(h->GetMean());
  DATA_err_Dphi_ePlus.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
  MC_Dphi_ePlus.push_back(hMC->GetMean());
  MC_err_Dphi_ePlus.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
  
  ///~~~~ Dphi e- ~~~~
  nameHistoInRootFile = Form("Data/%s_%d_7_Tot_temp",nameDATA.Data(),iEta+numEta*2);
  std::cout << " Dphi e+ nameHistoInRootFile = " << nameHistoInRootFile.Data() << std::endl;
  h = (TH1F*) fileIn->Get(nameHistoInRootFile);
  nameHistoInRootFile = Form("Data/W_%d_7_Tot_temp",iEta+numEta*2);
  hMC = (TH1F*) fileIn->Get(nameHistoInRootFile);
  cJoint->cd();
  h->Draw();
  norm = h->Integral(); hMC->DrawNormalized("HISTsame",norm);
  cJoint->SetGrid();
  TLegend* leg2 = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
  leg2->SetFillColor(kWhite);
  cJoint->SetGrid();   
  toDoROOT = Form("%s/plot/Dphi_Eta_%d_eMinus.png",nameOutputDir.Data(),iEta);
  std::cout << " Dphi e- toDoROOT = " << toDoROOT.Data() << std::endl;
  cJoint->SaveAs(toDoROOT);
  DATA_Dphi_eMinus.push_back(h->GetMean());
  DATA_err_Dphi_eMinus.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
  MC_Dphi_eMinus.push_back(hMC->GetMean());
  MC_err_Dphi_eMinus.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
  
  ///~~~~ Deta ~~~~
  nameHistoInRootFile = Form("Data/%s_%d_6_Tot_temp",nameDATA.Data(),iEta);
  std::cout << " Dphi e+ nameHistoInRootFile = " << nameHistoInRootFile.Data() << std::endl;
  h = (TH1F*) fileIn->Get(nameHistoInRootFile);
  nameHistoInRootFile = Form("Data/W_%d_6_Tot_temp",iEta);
  hMC = (TH1F*) fileIn->Get(nameHistoInRootFile);
  cJoint->cd();
  h->Draw();
  norm = h->Integral(); hMC->DrawNormalized("HISTsame",norm);
  cJoint->SetGrid();
  TLegend* leg3 = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
  leg3->SetFillColor(kWhite);
  cJoint->SetGrid();   
  toDoROOT = Form("%s/plot/Deta_Eta_%d.png",nameOutputDir.Data(),iEta);
  cJoint->SaveAs(toDoROOT);
  DATA_Deta.push_back(h->GetMean());
  DATA_err_Deta.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
  MC_Deta.push_back(hMC->GetMean());
  MC_err_Deta.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
  
 }
 
 
 ///==================================
 ///==== Summary plots: EB vs eta ====
 
 
 TGraphErrors* grDATA_Dphi = new TGraphErrors();
 grDATA_Dphi->SetTitle("#Delta#phi DATA");
 grDATA_Dphi->SetMarkerColor(kRed);
 grDATA_Dphi->SetLineColor(kRed);
 grDATA_Dphi->SetFillColor(kRed);
 grDATA_Dphi->SetMarkerStyle(20);
 grDATA_Dphi->SetMarkerSize(1);
 grDATA_Dphi->SetLineWidth(1);
 grDATA_Dphi->GetXaxis()->SetTitle("iDet");
 grDATA_Dphi->GetYaxis()->SetTitle("#Delta#phi");
 
 TGraphErrors* grDATA_Dphi_ePlus = new TGraphErrors();
 grDATA_Dphi_ePlus->SetTitle("#Delta#phi e+ DATA");
 grDATA_Dphi_ePlus->SetMarkerColor(kRed);
 grDATA_Dphi_ePlus->SetLineColor(kRed);
 grDATA_Dphi_ePlus->SetFillColor(kRed);
 grDATA_Dphi_ePlus->SetMarkerStyle(20);
 grDATA_Dphi_ePlus->SetMarkerSize(1);
 grDATA_Dphi_ePlus->SetLineWidth(1);
 grDATA_Dphi_ePlus->GetXaxis()->SetTitle("iDet");
 grDATA_Dphi_ePlus->GetYaxis()->SetTitle("#Delta#phi");
 
 TGraphErrors* grDATA_Dphi_eMinus = new TGraphErrors();
 grDATA_Dphi_eMinus->SetTitle("#Delta#phi e- DATA");
 grDATA_Dphi_eMinus->SetMarkerColor(kRed);
 grDATA_Dphi_eMinus->SetLineColor(kRed);
 grDATA_Dphi_eMinus->SetFillColor(kRed);
 grDATA_Dphi_eMinus->SetMarkerStyle(20);
 grDATA_Dphi_eMinus->SetMarkerSize(1);
 grDATA_Dphi_eMinus->SetLineWidth(1);
 grDATA_Dphi_eMinus->GetXaxis()->SetTitle("iDet");
 grDATA_Dphi_eMinus->GetYaxis()->SetTitle("#Delta#phi");
 
 TGraphErrors* grDATA_Deta = new TGraphErrors();
 grDATA_Deta->SetTitle("#Delta#eta DATA");
 grDATA_Deta->SetMarkerColor(kRed);
 grDATA_Deta->SetLineColor(kRed);
 grDATA_Deta->SetFillColor(kRed);
 grDATA_Deta->SetMarkerStyle(20);
 grDATA_Deta->SetMarkerSize(1);
 grDATA_Deta->SetLineWidth(1);
 grDATA_Deta->GetXaxis()->SetTitle("iDet");
 grDATA_Deta->GetYaxis()->SetTitle("#Delta#eta");
 
 TGraphErrors* grMC_Dphi = new TGraphErrors();
 grMC_Dphi->SetTitle("#Delta#phi MC");
 grMC_Dphi->SetMarkerColor(kBlue);
 grMC_Dphi->SetLineColor(kBlue);
 grMC_Dphi->SetFillColor(kBlue);
 grMC_Dphi->SetMarkerStyle(21);
 grMC_Dphi->SetMarkerSize(1);
 grMC_Dphi->SetLineWidth(1);
 grMC_Dphi->GetXaxis()->SetTitle("iDet");
 grMC_Dphi->GetYaxis()->SetTitle("#Delta#phi");
 
 TGraphErrors* grMC_Dphi_ePlus = new TGraphErrors();
 grMC_Dphi_ePlus->SetTitle("#Delta#phi e+ MC");
 grMC_Dphi_ePlus->SetMarkerColor(kBlue);
 grMC_Dphi_ePlus->SetLineColor(kBlue);
 grMC_Dphi_ePlus->SetFillColor(kBlue);
 grMC_Dphi_ePlus->SetMarkerStyle(21);
 grMC_Dphi_ePlus->SetMarkerSize(1);
 grMC_Dphi_ePlus->SetLineWidth(1);
 grMC_Dphi_ePlus->GetXaxis()->SetTitle("iDet");
 grMC_Dphi_ePlus->GetYaxis()->SetTitle("#Delta#phi");
 
 TGraphErrors* grMC_Dphi_eMinus = new TGraphErrors();
 grMC_Dphi_eMinus->SetTitle("#Delta#phi e- MC");
 grMC_Dphi_eMinus->SetMarkerColor(kBlue);
 grMC_Dphi_eMinus->SetLineColor(kBlue);
 grMC_Dphi_eMinus->SetFillColor(kBlue);
 grMC_Dphi_eMinus->SetMarkerStyle(21);
 grMC_Dphi_eMinus->SetMarkerSize(1);
 grMC_Dphi_eMinus->SetLineWidth(1);
 grMC_Dphi_eMinus->GetXaxis()->SetTitle("iDet");
 grMC_Dphi_eMinus->GetYaxis()->SetTitle("#Delta#phi");
 
 TGraphErrors* grMC_Deta = new TGraphErrors();
 grMC_Deta->SetTitle("#Delta#eta MC");
 grMC_Deta->SetMarkerColor(kBlue);
 grMC_Deta->SetLineColor(kBlue);
 grMC_Deta->SetFillColor(kBlue);
 grMC_Deta->SetMarkerStyle(21);
 grMC_Deta->SetMarkerSize(1);
 grMC_Deta->SetLineWidth(1);
 grMC_Deta->GetXaxis()->SetTitle("iDet");
 grMC_Deta->GetYaxis()->SetTitle("#Delta#eta"); 
 
 std::cout << " DATA_Dphi_ePlus.size() = " << DATA_Dphi_ePlus.size() << std::endl;
 for (int iEta = 0; iEta <numEta; iEta++) { 
  grDATA_Dphi->SetPoint(iEta,iEta*0.1+0.05-1.5,DATA_Dphi.at(iEta));
  grDATA_Dphi->SetPointError(iEta,0.05,DATA_err_Dphi.at(iEta));
  grDATA_Dphi_ePlus->SetPoint(iEta,iEta*0.1+0.05-1.5,DATA_Dphi_ePlus.at(iEta));
  grDATA_Dphi_ePlus->SetPointError(iEta,0.05,DATA_err_Dphi_ePlus.at(iEta));
  grDATA_Dphi_eMinus->SetPoint(iEta,iEta*0.1+0.05-1.5,DATA_Dphi_eMinus.at(iEta));
  grDATA_Dphi_eMinus->SetPointError(iEta,0.05,DATA_err_Dphi_eMinus.at(iEta));
  grDATA_Deta->SetPoint(iEta,iEta*0.1+0.05-1.5,DATA_Deta.at(iEta));
  grDATA_Deta->SetPointError(iEta,0.05,DATA_err_Deta.at(iEta));
  
  grMC_Dphi->SetPoint(iEta,iEta*0.1+0.05-1.5,MC_Dphi.at(iEta));
  grMC_Dphi->SetPointError(iEta,0.05,MC_err_Dphi.at(iEta));
  grMC_Dphi_ePlus->SetPoint(iEta,iEta*0.1+0.05-1.5,MC_Dphi_ePlus.at(iEta));
  grMC_Dphi_ePlus->SetPointError(iEta,0.05,MC_err_Dphi_ePlus.at(iEta));
  grMC_Dphi_eMinus->SetPoint(iEta,iEta*0.1+0.05-1.5,MC_Dphi_eMinus.at(iEta));
  grMC_Dphi_eMinus->SetPointError(iEta,0.05,MC_err_Dphi_eMinus.at(iEta));
  grMC_Deta->SetPoint(iEta,iEta*0.1+0.05-1.5,MC_Deta.at(iEta));
  grMC_Deta->SetPointError(iEta,0.05,MC_err_Deta.at(iEta));  
 } 
 
 
 
 TCanvas* cJointMG = new TCanvas("cJointMG","cJointMG",1000,500);
 cJointMG->SetLeftMargin(0.10);
 cJointMG->SetRightMargin(0.25);
 cJointMG->SetTopMargin(0.05);
 cJointMG->SetBottomMargin(0.20);
 
 cJointMG->cd();
 TMultiGraph* mgr_Dphi = new TMultiGraph();
 mgr_Dphi->Add(grDATA_Dphi);
 mgr_Dphi->Add(grMC_Dphi);
 mgr_Dphi->Draw("AP");
 mgr_Dphi->GetXaxis()->SetTitle("#eta");
 mgr_Dphi->GetYaxis()->SetTitle("#Delta#phi");
 cJointMG->SetGrid();
 TLegend* legQ = cJointMG->BuildLegend(0.8,0.55,0.98,0.75);
 legQ->SetFillColor(kWhite);
 lineVert.Draw();
 cJointMG->SetGrid();   
 toDoROOT = Form("%s/plot/Dphi.png",nameOutputDir.Data());
 cJointMG->SaveAs(toDoROOT);
 cJointMG->SaveAs(toDoROOT);
 
 cJointMG->Clear();
 cJointMG->cd();
 TMultiGraph* mgr_Dphi_ePlus = new TMultiGraph();
 mgr_Dphi_ePlus->Add(grDATA_Dphi_ePlus);
 mgr_Dphi_ePlus->Add(grMC_Dphi_ePlus);
 mgr_Dphi_ePlus->Draw("AP");
 mgr_Dphi_ePlus->GetXaxis()->SetTitle("#eta");
 mgr_Dphi_ePlus->GetYaxis()->SetTitle("#Delta#phi");
 cJointMG->SetGrid();
 TLegend* legA = cJointMG->BuildLegend(0.8,0.55,0.98,0.75);
 legA->SetFillColor(kWhite);
 lineVert.Draw();
 cJointMG->SetGrid();   
 toDoROOT = Form("%s/plot/DphiEPlus.png",nameOutputDir.Data());
 cJointMG->SaveAs(toDoROOT);
 cJointMG->SaveAs(toDoROOT);
 
 
 cJointMG->Clear();
 cJointMG->cd();
 TMultiGraph* mgr_Dphi_eMinus = new TMultiGraph();
 mgr_Dphi_eMinus->Add(grDATA_Dphi_eMinus);
 mgr_Dphi_eMinus->Add(grMC_Dphi_eMinus);
 mgr_Dphi_eMinus->Draw("AP");
 mgr_Dphi_eMinus->GetXaxis()->SetTitle("#eta");
 mgr_Dphi_eMinus->GetYaxis()->SetTitle("#Delta#phi");
 cJointMG->SetGrid();
 TLegend* legB = cJointMG->BuildLegend(0.8,0.55,0.98,0.75);
 legB->SetFillColor(kWhite);
 lineVert.Draw();
 cJointMG->SetGrid();   
 toDoROOT = Form("%s/plot/DphiEMinus.png",nameOutputDir.Data());
 cJointMG->SaveAs(toDoROOT);
 
 cJointMG->Clear();
 cJointMG->cd();
 TMultiGraph* mgr_Deta = new TMultiGraph();
 mgr_Deta->Add(grDATA_Deta);
 mgr_Deta->Add(grMC_Deta);
 mgr_Deta->Draw("AP");
 mgr_Deta->GetXaxis()->SetTitle("#eta");
 mgr_Deta->GetYaxis()->SetTitle("#Delta#eta");
 cJointMG->SetGrid();
 TLegend* legC = cJointMG->BuildLegend(0.8,0.55,0.98,0.75);
 legC->SetFillColor(kWhite);
 lineVert.Draw();
 cJointMG->SetGrid();   
 toDoROOT = Form("%s/plot/Deta.png",nameOutputDir.Data());
 cJointMG->SaveAs(toDoROOT);
 
 ///==== exit ====
 gApplication->Terminate(0);
 
 
}