void drawModules(TString nameInFileRoot, TString nameOutputDir, TString nameDATA){
 
 
 TFile* fileIn = new TFile(nameInFileRoot);
 
 TLine lineVert (35.5,-1,35.5,1);
 lineVert.SetLineColor(kGreen);
 lineVert.SetLineWidth(4);
 lineVert.SetLineStyle(3);
 
 gStyle->SetTitleX(0.2f);
 gStyle->SetTitleW(0.5f);
 
 TString nameHisto;
 TCanvas* cc;
 TH1F* h;
 TH1F* hMC;
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
 
 std::vector<double> DATA_Dphi_ePlus;
 std::vector<double> DATA_Dphi_eMinus;
 std::vector<double> DATA_Deta;
 std::vector<double> DATA_err_Dphi_ePlus;
 std::vector<double> DATA_err_Dphi_eMinus;
 std::vector<double> DATA_err_Deta;

 std::vector<double> MC_Dphi_ePlus;
 std::vector<double> MC_Dphi_eMinus;
 std::vector<double> MC_Deta;
 std::vector<double> MC_err_Dphi_ePlus;
 std::vector<double> MC_err_Dphi_eMinus;
 std::vector<double> MC_err_Deta;
 
 TString nameCanvas;
 ///==== EB ====
 for (int iSM = 0; iSM <36; iSM++) {
  if (iSM == 19 || iSM == 20 || iSM == 21 || iSM == 22 || iSM == 23 || iSM == 24 || iSM == 25 || iSM == 26) {
   DATA_Dphi_ePlus.push_back(0);
   DATA_err_Dphi_ePlus.push_back(0);
   MC_Dphi_ePlus.push_back(0);
   MC_err_Dphi_ePlus.push_back(0);

   DATA_Dphi_eMinus.push_back(0);
   DATA_err_Dphi_eMinus.push_back(0);
   MC_Dphi_eMinus.push_back(0);
   MC_err_Dphi_eMinus.push_back(0);
   
   DATA_Deta.push_back(0);
   DATA_err_Deta.push_back(0);
   MC_Deta.push_back(0);
   MC_err_Deta.push_back(0);
   
   continue;
  }
  ///~~~~ Dphi e+ ~~~~
  nameCanvas = Form("All/Var/%d_7_Canvas",iSM+40);
  std::cout << " Dphi e+ nameCanvas = " << nameCanvas.Data() << std::endl;
  cc = (TCanvas*) fileIn->Get(nameCanvas);
//   cc->Draw();
  cc ->cd(0);
  nameHisto = Form ("%s_%d_7_Tot_temp",nameDATA.Data(),iSM+40);
  h = (TH1F*) cc->GetPrimitive(nameHisto);
  if (iSM<18) {
   nameHisto = Form ("W_125_7_Tot_temp");  
   nameCanvas = Form("All/Var/125_7_Canvas");
   cc = (TCanvas*) fileIn->Get(nameCanvas);
  }
  else {
   nameHisto = Form ("W_126_7_Tot_temp");  
   nameCanvas = Form("All/Var/126_7_Canvas");
   cc = (TCanvas*) fileIn->Get(nameCanvas);
  }
  std::cout << " Dphi e+ nameCanvas = " << nameCanvas.Data() << std::endl;
  hMC = (TH1F*) cc->GetPrimitive(nameHisto);
  cJoint->cd();
  h->Draw();
  norm = h->Integral(); hMC->DrawNormalized("HISTsame",norm);
  cJoint->SetGrid();
  TLegend* leg = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
  leg->SetFillColor(kWhite);
  cJoint->SetGrid();   
  toDoROOT = Form("%s/plot/Dphi_EB_%d_ePlus.png",nameOutputDir.Data(),iSM);
  cJoint->SaveAs(toDoROOT);
  cJoint->SaveAs(toDoROOT);
  DATA_Dphi_ePlus.push_back(h->GetMean());
  DATA_err_Dphi_ePlus.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
  MC_Dphi_ePlus.push_back(hMC->GetMean());
  MC_err_Dphi_ePlus.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
  
  ///~~~~ Dphi e- ~~~~
  nameCanvas = Form("All/Var/%d_7_Canvas",iSM+80);
  std::cout << " Dphi e- nameCanvas = " << nameCanvas.Data() << std::endl;
  cc = (TCanvas*) fileIn->Get(nameCanvas);
//   cc->Draw();
  cc ->cd(0);
  nameHisto = Form ("%s_%d_7_Tot_temp",nameDATA.Data(),iSM+80);
  std::cout << " Dphi e- nameHisto = " << nameHisto.Data() << std::endl;
  h = (TH1F*) cc->GetPrimitive(nameHisto);
  if (iSM<18) {
   nameHisto = Form ("W_129_7_Tot_temp");  
   nameCanvas = Form("All/Var/129_7_Canvas");
   cc = (TCanvas*) fileIn->Get(nameCanvas);
  }
  else {
   nameHisto = Form ("W_130_7_Tot_temp");  
   nameCanvas = Form("All/Var/130_7_Canvas");
   cc = (TCanvas*) fileIn->Get(nameCanvas);
  }
  std::cout << " Dphi e- nameHisto = " << nameHisto.Data() << std::endl;
  hMC = (TH1F*) cc->GetPrimitive(nameHisto);
  cJoint->cd();
  h->Draw();
  norm = h->Integral(); hMC->DrawNormalized("HISTsame",norm);
  cJoint->SetGrid();
  TLegend* leg2 = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
  leg2->SetFillColor(kWhite);
  cJoint->SetGrid();   
  toDoROOT = Form("%s/plot/Dphi_EB_%d_eMinus.png",nameOutputDir.Data(),iSM);
  std::cout << " Dphi e- toDoROOT = " << toDoROOT.Data() << std::endl;
  cJoint->SaveAs(toDoROOT);
  DATA_Dphi_eMinus.push_back(h->GetMean());
  DATA_err_Dphi_eMinus.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
  MC_Dphi_eMinus.push_back(hMC->GetMean());
  MC_err_Dphi_eMinus.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
  
  ///~~~~ Deta ~~~~
  nameCanvas = Form("All/Var/%d_6_Canvas",iSM);
  std::cout << " Deta   nameCanvas = " << nameCanvas.Data() << std::endl;
  cc = (TCanvas*) fileIn->Get(nameCanvas);
//   cc->Draw();
  cc ->cd(0);
  nameHisto = Form ("%s_%d_6_Tot_temp",nameDATA.Data(),iSM);
  h = (TH1F*) cc->GetPrimitive(nameHisto);
  if (iSM<18) {
   nameHisto = Form ("W_121_6_Tot_temp");  
   nameCanvas = Form("All/Var/121_6_Canvas");
   cc = (TCanvas*) fileIn->Get(nameCanvas);
  }
  else {
   nameHisto = Form ("W_122_6_Tot_temp");  
   nameCanvas = Form("All/Var/122_6_Canvas");
   cc = (TCanvas*) fileIn->Get(nameCanvas);
  }
  std::cout << " Dphi e+ nameCanvas = " << nameCanvas.Data() << std::endl;
  hMC = (TH1F*) cc->GetPrimitive(nameHisto);
  cJoint->cd();
  h->Draw();
  norm = h->Integral(); hMC->DrawNormalized("HISTsame",norm);
  cJoint->SetGrid();
  TLegend* leg3 = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
  leg3->SetFillColor(kWhite);
  cJoint->SetGrid();   
  toDoROOT = Form("%s/plot/Deta_EB_%d.png",nameOutputDir.Data(),iSM);
  cJoint->SaveAs(toDoROOT);
  DATA_Deta.push_back(h->GetMean());
  DATA_err_Deta.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
  MC_Deta.push_back(hMC->GetMean());
  MC_err_Deta.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
  
 }
 
 
 
 
 
 ///==== EE ====
 for (int iSM = 0; iSM <4; iSM++) {
  ///~~~~ Dphi e+ ~~~~
  nameCanvas = Form("All/Var/%d_7_Canvas",iSM+40+36);
  cc = (TCanvas*) fileIn->Get(nameCanvas);
  cc ->cd(0);
  nameHisto = Form ("%s_%d_7_Tot_temp",nameDATA.Data(),iSM+40+36);
  h = (TH1F*) cc->GetPrimitive(nameHisto);
  if (iSM<2) {
   nameHisto = Form ("W_124_7_Tot_temp");  
   nameCanvas = Form("All/Var/124_7_Canvas");
   cc = (TCanvas*) fileIn->Get(nameCanvas);
  }
  else {
   nameHisto = Form ("W_127_7_Tot_temp");  
   nameCanvas = Form("All/Var/127_7_Canvas");
   cc = (TCanvas*) fileIn->Get(nameCanvas);
  }
  hMC = (TH1F*) cc->GetPrimitive(nameHisto);
  cJoint->cd();
  h->Draw();
  norm = h->Integral(); hMC->DrawNormalized("HISTsame",norm);
  cJoint->SetGrid();
  TLegend* leg = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
  leg->SetFillColor(kWhite);
  cJoint->SetGrid();   
  toDoROOT = Form("%s/plot/Dphi_EE_%d_ePlus.png",nameOutputDir.Data(),iSM);
  cJoint->SaveAs(toDoROOT);
  cJoint->SaveAs(toDoROOT);
  DATA_Dphi_ePlus.push_back(h->GetMean());
  DATA_err_Dphi_ePlus.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
  MC_Dphi_ePlus.push_back(hMC->GetMean());
  MC_err_Dphi_ePlus.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
  
  ///~~~~ Dphi e- ~~~~
  nameCanvas = Form("All/Var/%d_7_Canvas",iSM+80+36);
  cc = (TCanvas*) fileIn->Get(nameCanvas);
  cc ->cd(0);
  nameHisto = Form ("%s_%d_7_Tot_temp",nameDATA.Data(),iSM+80+36);
  h = (TH1F*) cc->GetPrimitive(nameHisto);
  if (iSM<2) {
   nameHisto = Form ("W_128_7_Tot_temp");  
   nameCanvas = Form("All/Var/128_7_Canvas");
   cc = (TCanvas*) fileIn->Get(nameCanvas);
  }
  else {
   nameHisto = Form ("W_131_7_Tot_temp");  
   nameCanvas = Form("All/Var/131_7_Canvas");
   cc = (TCanvas*) fileIn->Get(nameCanvas);
  }
  hMC = (TH1F*) cc->GetPrimitive(nameHisto);
  cJoint->cd();
  h->Draw();
  norm = h->Integral(); hMC->DrawNormalized("HISTsame",norm);
  cJoint->SetGrid();
  TLegend* leg2 = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
  leg2->SetFillColor(kWhite);
  cJoint->SetGrid();   
  toDoROOT = Form("%s/plot/Dphi_EE_%d_eMinus.png",nameOutputDir.Data(),iSM);
  cJoint->SaveAs(toDoROOT);
  DATA_Dphi_eMinus.push_back(h->GetMean());
  DATA_err_Dphi_eMinus.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
  MC_Dphi_eMinus.push_back(hMC->GetMean());
  MC_err_Dphi_eMinus.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
  
  ///~~~~ Deta ~~~~
  ///~~~~ Deta ~~~~
  nameCanvas = Form("All/Var/%d_6_Canvas",iSM+36);
  cc = (TCanvas*) fileIn->Get(nameCanvas);
  cc ->cd(0);
  nameHisto = Form ("%s_%d_6_Tot_temp",nameDATA.Data(),iSM+36);
  h = (TH1F*) cc->GetPrimitive(nameHisto);
  if (iSM<2) {
   nameHisto = Form ("W_120_6_Tot_temp");  
   nameCanvas = Form("All/Var/120_6_Canvas");
   cc = (TCanvas*) fileIn->Get(nameCanvas);
  }
  else {
   nameHisto = Form ("W_123_6_Tot_temp");  
   nameCanvas = Form("All/Var/123_6_Canvas");
   cc = (TCanvas*) fileIn->Get(nameCanvas);
  }
  hMC = (TH1F*) cc->GetPrimitive(nameHisto);
  cJoint->cd();
  h->Draw();
  norm = h->Integral(); hMC->DrawNormalized("HISTsame",norm);
  cJoint->SetGrid();
  TLegend* leg3 = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
  leg3->SetFillColor(kWhite);
  cJoint->SetGrid();   
  toDoROOT = Form("%s/plot/Deta_EE_%d.png",nameOutputDir.Data(),iSM);
  cJoint->SaveAs(toDoROOT);
  DATA_Deta.push_back(h->GetMean());
  DATA_err_Deta.push_back(h->GetRMS() /sqrt( h->GetEffectiveEntries() ));
  MC_Deta.push_back(hMC->GetMean());
  MC_err_Deta.push_back(hMC->GetRMS() /sqrt( hMC->GetEffectiveEntries() ));
  
 }
 
 
 
 ///=======================
 ///==== Summary plots ====
 
 
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
 grDATA_Deta->SetTitle("#Delta#eta e- DATA");
 grDATA_Deta->SetMarkerColor(kRed);
 grDATA_Deta->SetLineColor(kRed);
 grDATA_Deta->SetFillColor(kRed);
 grDATA_Deta->SetMarkerStyle(20);
 grDATA_Deta->SetMarkerSize(1);
 grDATA_Deta->SetLineWidth(1);
 grDATA_Deta->GetXaxis()->SetTitle("iDet");
 grDATA_Deta->GetYaxis()->SetTitle("#Delta#eta");
 
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
 grMC_Deta->SetTitle("#Delta#eta e- MC");
 grMC_Deta->SetMarkerColor(kBlue);
 grMC_Deta->SetLineColor(kBlue);
 grMC_Deta->SetFillColor(kBlue);
 grMC_Deta->SetMarkerStyle(21);
 grMC_Deta->SetMarkerSize(1);
 grMC_Deta->SetLineWidth(1);
 grMC_Deta->GetXaxis()->SetTitle("iDet");
 grMC_Deta->GetYaxis()->SetTitle("#Delta#eta"); 
 
 std::cout << " DATA_Dphi_ePlus.size() = " << DATA_Dphi_ePlus.size() << std::endl;
 for (int iSM = 0; iSM <40; iSM++) { 
  grDATA_Dphi_ePlus->SetPoint(iSM,iSM,DATA_Dphi_ePlus.at(iSM));
  grDATA_Dphi_ePlus->SetPointError(iSM,0,DATA_err_Dphi_ePlus.at(iSM));
  grDATA_Dphi_eMinus->SetPoint(iSM,iSM,DATA_Dphi_eMinus.at(iSM));
  grDATA_Dphi_eMinus->SetPointError(iSM,0,DATA_err_Dphi_eMinus.at(iSM));
  grDATA_Deta->SetPoint(iSM,iSM,DATA_Deta.at(iSM));
  grDATA_Deta->SetPointError(iSM,0,DATA_err_Deta.at(iSM));
  
  grMC_Dphi_ePlus->SetPoint(iSM,iSM,MC_Dphi_ePlus.at(iSM));
  grMC_Dphi_ePlus->SetPointError(iSM,0,MC_err_Dphi_ePlus.at(iSM));
  grMC_Dphi_eMinus->SetPoint(iSM,iSM,MC_Dphi_eMinus.at(iSM));
  grMC_Dphi_eMinus->SetPointError(iSM,0,MC_err_Dphi_eMinus.at(iSM));
  grMC_Deta->SetPoint(iSM,iSM,MC_Deta.at(iSM));
  grMC_Deta->SetPointError(iSM,0,MC_err_Deta.at(iSM));  
 } 
 
 
 
 
 cJoint->Clear();
 cJoint->cd();
 TMultiGraph* mgr_Dphi_ePlus = new TMultiGraph();
 mgr_Dphi_ePlus->Add(grDATA_Dphi_ePlus);
 mgr_Dphi_ePlus->Add(grMC_Dphi_ePlus);
 mgr_Dphi_ePlus->Draw("AP");
 mgr_Dphi_ePlus->GetXaxis()->SetTitle("iDet");
 mgr_Dphi_ePlus->GetYaxis()->SetTitle("#Delta#phi");
 cJoint->SetGrid();
 TLegend* legA = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
 legA->SetFillColor(kWhite);
 lineVert->Draw();
 cJoint->SetGrid();   
 toDoROOT = Form("%s/plot/DphiEPlus.png",nameOutputDir.Data());
 cJoint->SaveAs(toDoROOT);
 
 
 cJoint->Clear();
 cJoint->cd();
 TMultiGraph* mgr_Dphi_eMinus = new TMultiGraph();
 mgr_Dphi_eMinus->Add(grDATA_Dphi_eMinus);
 mgr_Dphi_eMinus->Add(grMC_Dphi_eMinus);
 mgr_Dphi_eMinus->Draw("AP");
 mgr_Dphi_eMinus->GetXaxis()->SetTitle("iDet");
 mgr_Dphi_eMinus->GetYaxis()->SetTitle("#Delta#phi");
 cJoint->SetGrid();
 TLegend* legB = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
 legB->SetFillColor(kWhite);
 lineVert->Draw();
 cJoint->SetGrid();   
 toDoROOT = Form("%s/plot/DphiEMinus.png",nameOutputDir.Data());
 cJoint->SaveAs(toDoROOT);
 
 cJoint->Clear();
 cJoint->cd();
 TMultiGraph* mgr_Deta = new TMultiGraph();
 mgr_Deta->Add(grDATA_Deta);
 mgr_Deta->Add(grMC_Deta);
 mgr_Deta->Draw("AP");
 mgr_Deta->GetXaxis()->SetTitle("iDet");
 mgr_Deta->GetYaxis()->SetTitle("#Delta#eta");
 cJoint->SetGrid();
 TLegend* legC = cJoint->BuildLegend(0.8,0.55,0.98,0.75);
 legC->SetFillColor(kWhite);
 lineVert->Draw();
 cJoint->SetGrid();   
 toDoROOT = Form("%s/plot/Deta.png",nameOutputDir.Data());
 cJoint->SaveAs(toDoROOT);
 
 
 
 
 ///============================
 ///==== to create big HTML ====
 
 
 toDoShell = Form("cp scripts/html/templateModulesBegin.html %s/ModulesBegin.html",nameOutputDir.Data());
 system(toDoShell.Data());

 toDoShell = Form("cp scripts/html/templateModulesEnd.html %s/ModulesEnd.html",nameOutputDir.Data());
 system(toDoShell.Data());
 
 TString nameFileHTML = Form("%s/ModulesMiddle.html",nameOutputDir.Data());
 std::ofstream myfile;
 myfile.precision (3) ;
 myfile.unsetf(std::ios::scientific); 
 myfile.open (nameFileHTML.Data());
 for (int iSM = 0; iSM <36; iSM++) { 
  myfile << "<tr>" << std::endl;
  myfile << "   <td COLSPAN=\"1\" ROWSPAN=\"2\" > <center><a href=\"Module_EB_" << iSM << ".html\" target=\"_blank\"> Module EB " << iSM << " </a></center> </td>" << std::endl;
  myfile << "   <td COLSPAN=\"1\" ROWSPAN=\"1\"> <font color=\"red\">  " << DATA_Dphi_ePlus.at(iSM)  << " &pm; " << DATA_err_Dphi_ePlus.at(iSM)   << " </font></center> </td>" << std::endl;
  myfile << "   <td COLSPAN=\"1\" ROWSPAN=\"1\"> <font color=\"red\">  " << DATA_Dphi_eMinus.at(iSM) << " &pm; " << DATA_err_Dphi_eMinus.at(iSM)  << " </font></center> </td>" << std::endl;
  myfile << "   <td COLSPAN=\"1\" ROWSPAN=\"1\"> <font color=\"red\">  " << DATA_Deta.at(iSM)        << " &pm; " << DATA_err_Deta.at(iSM)         << " </font></center> </td>" << std::endl;
  myfile << "</tr>" << std::endl;
  myfile << "<tr>" << std::endl;
  myfile << "   <td COLSPAN=\"1\" ROWSPAN=\"1\"> <font color=\"blue\">  " << MC_Dphi_ePlus.at(iSM)  << " &pm; " << MC_err_Dphi_ePlus.at(iSM)   << " </font></center> </td>" << std::endl;
  myfile << "   <td COLSPAN=\"1\" ROWSPAN=\"1\"> <font color=\"blue\">  " << MC_Dphi_eMinus.at(iSM) << " &pm; " << MC_err_Dphi_eMinus.at(iSM)  << " </font></center> </td>" << std::endl;
  myfile << "   <td COLSPAN=\"1\" ROWSPAN=\"1\"> <font color=\"blue\">  " << MC_Deta.at(iSM)        << " &pm; " << MC_err_Deta.at(iSM)         << " </font></center> </td>" << std::endl;
  myfile << "</tr>" << std::endl;
 }
 myfile << "<hr>" << std::endl;
 for (int iSM = 0; iSM <4; iSM++) { 
  myfile << "<tr>" << std::endl;
  myfile << "   <td COLSPAN=\"1\" ROWSPAN=\"2\" > <center><a href=\"Module_EE_" << iSM << ".html\" target=\"_blank\"> Module EE " << iSM << " </a></center> </td>" << std::endl;
  myfile << "   <td COLSPAN=\"1\" ROWSPAN=\"1\"> <font color=\"red\">  " << DATA_Dphi_ePlus.at(iSM+36)  << " &pm; " << DATA_err_Dphi_ePlus.at(iSM+36)   << " </font></center> </td>" << std::endl;
  myfile << "   <td COLSPAN=\"1\" ROWSPAN=\"1\"> <font color=\"red\">  " << DATA_Dphi_eMinus.at(iSM+36) << " &pm; " << DATA_err_Dphi_eMinus.at(iSM+36)  << " </font></center> </td>" << std::endl;
  myfile << "   <td COLSPAN=\"1\" ROWSPAN=\"1\"> <font color=\"red\">  " << DATA_Deta.at(iSM+36)        << " &pm; " << DATA_err_Deta.at(iSM+36)         << " </font></center> </td>" << std::endl;
  myfile << "</tr>" << std::endl;
  myfile << "<tr>" << std::endl;
  myfile << "   <td COLSPAN=\"1\" ROWSPAN=\"1\"> <font color=\"blue\">  " << MC_Dphi_ePlus.at(iSM+36)  << " &pm; " << MC_err_Dphi_ePlus.at(iSM+36)   << " </font></center> </td>" << std::endl;
  myfile << "   <td COLSPAN=\"1\" ROWSPAN=\"1\"> <font color=\"blue\">  " << MC_Dphi_eMinus.at(iSM+36) << " &pm; " << MC_err_Dphi_eMinus.at(iSM+36)  << " </font></center> </td>" << std::endl;
  myfile << "   <td COLSPAN=\"1\" ROWSPAN=\"1\"> <font color=\"blue\">  " << MC_Deta.at(iSM+36)        << " &pm; " << MC_err_Deta.at(iSM+36)         << " </font></center> </td>" << std::endl;
  myfile << "</tr>" << std::endl;
 } 
 myfile.close(); 
 
 TString totalnameFileHTML = Form("%s/Modules.html",nameOutputDir.Data());
 std::string toCreateHLTML;
 toCreateHLTML.clear();
 toCreateHLTML.insert(toCreateHLTML.size(),"cat scripts/html/templateModulesBegin.html ");
 toCreateHLTML.insert(toCreateHLTML.size(),nameFileHTML.Data());
 toCreateHLTML.insert(toCreateHLTML.size()," scripts/html/templateModulesEnd.html > ");
 toCreateHLTML.insert(toCreateHLTML.size(),totalnameFileHTML.Data());
 toCreateHLTML.insert(toCreateHLTML.size()," ; \n ");
 
 std::cout << std::endl << std::endl << std::endl;
 std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << std::endl;
 std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << std::endl;
 std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << std::endl;
 std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << std::endl;
 std::cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << std::endl;
 std::cout << std::endl << std::endl << std::endl;
 
 std::cout << toCreateHLTML << std::endl;
 system(toCreateHLTML.c_str());
 
 std::cout << std::endl << std::endl << std::endl;
  
 
 
 
 
 ///===================================
 ///==== to create many small HTML ====
 

 for (int iSM = 0; iSM <36; iSM++) { 
  nameFileHTML = Form("%s/Module_EB_%d_temp.html",nameOutputDir.Data(),iSM);
  std::ofstream myfileCicle;
  myfileCicle.precision (3) ;
  myfileCicle.unsetf(std::ios::scientific); 
  myfileCicle.open (nameFileHTML.Data());

  myfileCicle << "<tr> " << std::endl;
  myfileCicle << "<td COLSPAN=\"1\">" << std::endl;
  myfileCicle << "<b>EB module " << iSM << "</b>" << std::endl;
  myfileCicle << "</td>" << std::endl;
  
  myfileCicle << " <td COLSPAN=\"1\">" << std::endl;
  myfileCicle << "<center><font color=\"red\">  " << DATA_Dphi_ePlus.at(iSM)  << " &pm; " << DATA_err_Dphi_ePlus.at(iSM)  << " </font></center>" << std::endl;
  myfileCicle << "<center><font color=\"blue\"> " << MC_Dphi_ePlus.at(iSM)    << " &pm; " << MC_err_Dphi_ePlus.at(iSM)    << " </font></center>" << std::endl;
  myfileCicle << "</td>" << std::endl;

  myfileCicle << " <td COLSPAN=\"1\">" << std::endl;
  myfileCicle << "<center><font color=\"red\">  " << DATA_Dphi_eMinus.at(iSM)  << " &pm; " << DATA_err_Dphi_eMinus.at(iSM)  << " </font></center>" << std::endl;
  myfileCicle << "<center><font color=\"blue\"> " << MC_Dphi_eMinus.at(iSM)    << " &pm; " << MC_err_Dphi_eMinus.at(iSM)    << " </font></center>" << std::endl;
  myfileCicle << "</td>" << std::endl;

  myfileCicle << " <td COLSPAN=\"2\">" << std::endl;
  myfileCicle << "<center><font color=\"red\">  " << DATA_Deta.at(iSM)  << " &pm; " << DATA_err_Deta.at(iSM)  << " </font></center>" << std::endl;
  myfileCicle << "<center><font color=\"blue\"> " << MC_Deta.at(iSM)    << " &pm; " << MC_err_Deta.at(iSM)    << " </font></center>" << std::endl;
  myfileCicle << "</td>" << std::endl;

  myfileCicle << "</tr> " << std::endl;


  myfileCicle << "<tr> " << std::endl;
  myfileCicle << "<td COLSPAN=\"1\">" << std::endl;
  myfileCicle << "<b>EB module " << iSM << "</b>" << std::endl;
  myfileCicle << "</td>" << std::endl;
  
  myfileCicle << " <td COLSPAN=\"1\">" << std::endl;
  myfileCicle << " <center><a href=\"plot/Dphi_EB_" << iSM << "_ePlus.png\" target=\"_blank\"><img src=\"plot/Dphi_EB_" << iSM << "_ePlus.png\" height=\"100\" width=\"100\"></a></center> " << std::endl;
  myfileCicle << "</td>" << std::endl;
  
  myfileCicle << " <td COLSPAN=\"1\">" << std::endl;
  myfileCicle << " <center><a href=\"plot/Dphi_EB_" << iSM << "_eMinus.png\" target=\"_blank\"><img src=\"plot/Dphi_EB_" << iSM << "_eMinus.png\" height=\"100\" width=\"100\"></a></center> " << std::endl;
  myfileCicle << "</td>" << std::endl;
  
  myfileCicle << " <td COLSPAN=\"2\">" << std::endl;
  myfileCicle << " <center><a href=\"plot/Deta_EB_" << iSM << ".png\" target=\"_blank\"><img src=\"plot/Deta_EB_" << iSM << ".png\" height=\"100\" width=\"100\"></a></center> " << std::endl;
  myfileCicle << "</td>" << std::endl;
  
  myfileCicle << "</tr> " << std::endl;
  
  myfileCicle << "</table> " << std::endl;  
  
  myfileCicle << "</center> " << std::endl;  
  myfileCicle << "</body> " << std::endl;  
  myfileCicle << "</html> " << std::endl;  
  
  myfileCicle.close(); 
  
  TString totalnameFileHTML = Form("%s/Module_EB_%d.html",nameOutputDir.Data(),iSM);
  
  std::string toCreateHLTML;
  toCreateHLTML.clear();
  toCreateHLTML.insert(toCreateHLTML.size(),"cat scripts/html/templateOneModuleBegin.html ");
  toCreateHLTML.insert(toCreateHLTML.size(),nameFileHTML.Data());
  toCreateHLTML.insert(toCreateHLTML.size()," > ");
  toCreateHLTML.insert(toCreateHLTML.size(),totalnameFileHTML.Data());
  toCreateHLTML.insert(toCreateHLTML.size()," ; \n ");
//   std::cout << toCreateHLTML << std::endl;
  system(toCreateHLTML.c_str());
  
 }
 
 
 for (int iSM = 0; iSM <4; iSM++) { 
  nameFileHTML = Form("%s/Module_EE_%d_temp.html",nameOutputDir.Data(),iSM);
  std::ofstream myfileCicle;
  myfileCicle.precision (3) ;
  myfileCicle.unsetf(std::ios::scientific); 
  myfileCicle.open (nameFileHTML.Data());
  
  myfileCicle << "<tr> " << std::endl;
  myfileCicle << "<td COLSPAN=\"1\">" << std::endl;
  myfileCicle << "<b>EE module " << iSM << "</b>" << std::endl;
  myfileCicle << "</td>" << std::endl;
  
  myfileCicle << " <td COLSPAN=\"1\">" << std::endl;
  myfileCicle << "<center><font color=\"red\">  " << DATA_Dphi_ePlus.at(iSM+36)  << " &pm; " << DATA_err_Dphi_ePlus.at(iSM+36)  << " </font></center>" << std::endl;
  myfileCicle << "<center><font color=\"blue\"> " << MC_Dphi_ePlus.at(iSM+36)    << " &pm; " << MC_err_Dphi_ePlus.at(iSM+36)    << " </font></center>" << std::endl;
  myfileCicle << "</td>" << std::endl;
  
  myfileCicle << " <td COLSPAN=\"1\">" << std::endl;
  myfileCicle << "<center><font color=\"red\">  " << DATA_Dphi_eMinus.at(iSM+36)  << " &pm; " << DATA_err_Dphi_eMinus.at(iSM+36)  << " </font></center>" << std::endl;
  myfileCicle << "<center><font color=\"blue\"> " << MC_Dphi_eMinus.at(iSM+36)    << " &pm; " << MC_err_Dphi_eMinus.at(iSM+36)    << " </font></center>" << std::endl;
  myfileCicle << "</td>" << std::endl;
  
  myfileCicle << " <td COLSPAN=\"2\">" << std::endl;
  myfileCicle << "<center><font color=\"red\">  " << DATA_Deta.at(iSM+36)  << " &pm; " << DATA_err_Deta.at(iSM+36)  << " </font></center>" << std::endl;
  myfileCicle << "<center><font color=\"blue\"> " << MC_Deta.at(iSM+36)    << " &pm; " << MC_err_Deta.at(iSM+36)    << " </font></center>" << std::endl;
  myfileCicle << "</td>" << std::endl;
  
  myfileCicle << "</tr> " << std::endl;
  
  
  myfileCicle << "<tr> " << std::endl;
  myfileCicle << "<td COLSPAN=\"1\">" << std::endl;
  myfileCicle << "<b>EE module " << iSM << "</b>" << std::endl;
  myfileCicle << "</td>" << std::endl;
  
  myfileCicle << " <td COLSPAN=\"1\">" << std::endl;
  myfileCicle << " <center><a href=\"plot/Dphi_EE_" << iSM << "_ePlus.png\" target=\"_blank\"><img src=\"plot/Dphi_EE_" << iSM << "_ePlus.png\" height=\"100\" width=\"100\"></a></center> " << std::endl;
  myfileCicle << "</td>" << std::endl;
  
  myfileCicle << " <td COLSPAN=\"1\">" << std::endl;
  myfileCicle << " <center><a href=\"plot/Dphi_EE_" << iSM << "_eMinus.png\" target=\"_blank\"><img src=\"plot/Dphi_EE_" << iSM << "_eMinus.png\" height=\"100\" width=\"100\"></a></center> " << std::endl;
  myfileCicle << "</td>" << std::endl;
  
  myfileCicle << " <td COLSPAN=\"2\">" << std::endl;
  myfileCicle << " <center><a href=\"plot/Deta_EE_" << iSM << ".png\" target=\"_blank\"><img src=\"plot/Deta_EE_" << iSM << ".png\" height=\"100\" width=\"100\"></a></center> " << std::endl;
  myfileCicle << "</td>" << std::endl;
  
  myfileCicle << "</tr> " << std::endl;
  
  myfileCicle << "</table> " << std::endl;  
  
  myfileCicle << "</center> " << std::endl;  
  myfileCicle << "</body> " << std::endl;  
  myfileCicle << "</html> " << std::endl;  
  
  myfileCicle.close(); 
  
  TString totalnameFileHTML = Form("%s/Module_EE_%d.html",nameOutputDir.Data(),iSM);
  std::string toCreateHLTML;
  toCreateHLTML.clear();
  toCreateHLTML.insert(toCreateHLTML.size(),"cat scripts/html/templateOneModuleBegin.html ");
  toCreateHLTML.insert(toCreateHLTML.size(),nameFileHTML.Data());
  toCreateHLTML.insert(toCreateHLTML.size()," > ");
  toCreateHLTML.insert(toCreateHLTML.size(),totalnameFileHTML.Data());
  toCreateHLTML.insert(toCreateHLTML.size()," ; \n ");
//   std::cout << toCreateHLTML << std::endl;
  system(toCreateHLTML.c_str());
  
 }
 
 ///==== exit ====
 gApplication->Terminate(0);
 
 
}