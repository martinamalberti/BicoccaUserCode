void DrawTestPt(){
 
 int num = 2;
 double X[10] = {40, 60};
 double errX[10] = {10, 10};
 
 //==== EB+ ====
 double DPhiEPlusMC[10] = {0.00164, 0.000698};
 double errDPhiEPlusMC[10] = {1.31e-05, 2.56e-05};
 
 double DPhiEMinusMC[10] = {-0.000958, 2.73e-05};
 double errDPhiEMinusMC[10] = {1.45e-05, 2.75e-05};
 
 double DPhiEPlusDATA[10] = {0.00155, 0.000705};
 double errDPhiEPlusDATA[10] = {2.22e-05 , 4.32e-05};
 
 double DPhiEMinusDATA[10] = {-0.000839, 2.75e-05 };
 double errDPhiEMinusDATA[10] = {2.4e-05, 4.82e-05};
  
 //==== EB- ====
//  double DPhiEPlusMC[10] = {0.000822, -7.83e-05};
//  double errDPhiEPlusMC[10] = {1.27e-05, 2.31e-05};
//  
//  double DPhiEMinusMC[10] = {-0.00172, -0.000783};
//  double errDPhiEMinusMC[10] = {1.49e-05, 3.05e-05};
//  
//  double DPhiEPlusDATA[10] = {0.000784, -7.03e-05};
//  double errDPhiEPlusDATA[10] = {2.21e-05 , 4.77e-05};
//  
//  double DPhiEMinusDATA[10] = {-0.00164, -0.000678 };
//  double errDPhiEMinusDATA[10] = { 2.55e-05 ,  4.88e-05 };
 
 
 
 TGraphErrors* grDATA_Dphi_ePlus = new TGraphErrors(num,X,DPhiEPlusDATA,errX,errDPhiEPlusDATA);
 grDATA_Dphi_ePlus->SetTitle("#Delta#phi e+ DATA");
 grDATA_Dphi_ePlus->SetMarkerColor(kRed);
 grDATA_Dphi_ePlus->SetLineColor(kRed);
 grDATA_Dphi_ePlus->SetFillColor(kWhite);
 grDATA_Dphi_ePlus->SetMarkerStyle(20);
 grDATA_Dphi_ePlus->SetMarkerSize(1);
 grDATA_Dphi_ePlus->SetLineWidth(2);
 grDATA_Dphi_ePlus->SetLineStyle(2);
 grDATA_Dphi_ePlus->GetXaxis()->SetTitle("iDet");
 grDATA_Dphi_ePlus->GetYaxis()->SetTitle("#Delta#phi");
 
 TGraphErrors* grDATA_Dphi_eMinus = new TGraphErrors(num,X,DPhiEMinusDATA,errX,errDPhiEMinusDATA);
 grDATA_Dphi_eMinus->SetTitle("#Delta#phi e- DATA");
 grDATA_Dphi_eMinus->SetMarkerColor(kBlue);
 grDATA_Dphi_eMinus->SetLineColor(kBlue);
 grDATA_Dphi_eMinus->SetFillColor(kWhite);
 grDATA_Dphi_eMinus->SetMarkerStyle(20);
 grDATA_Dphi_eMinus->SetMarkerSize(1);
 grDATA_Dphi_eMinus->SetLineWidth(2);
 grDATA_Dphi_eMinus->SetLineStyle(2);
 grDATA_Dphi_eMinus->GetXaxis()->SetTitle("iDet");
 grDATA_Dphi_eMinus->GetYaxis()->SetTitle("#Delta#phi");
 
 TGraphErrors* grMC_Dphi_ePlus = new TGraphErrors(num,X,DPhiEPlusMC,errX,errDPhiEPlusMC);
 grMC_Dphi_ePlus->SetTitle("#Delta#phi e+ MC");
 grMC_Dphi_ePlus->SetMarkerColor(kRed);
 grMC_Dphi_ePlus->SetLineColor(kRed);
 grMC_Dphi_ePlus->SetFillColor(kWhite);
 grMC_Dphi_ePlus->SetMarkerStyle(24);
 grMC_Dphi_ePlus->SetMarkerSize(1);
 grMC_Dphi_ePlus->SetLineWidth(1);
 grMC_Dphi_ePlus->GetXaxis()->SetTitle("iDet");
 grMC_Dphi_ePlus->GetYaxis()->SetTitle("#Delta#phi");
 
 TGraphErrors* grMC_Dphi_eMinus = new TGraphErrors(num,X,DPhiEMinusMC,errX,errDPhiEMinusMC);
 grMC_Dphi_eMinus->SetTitle("#Delta#phi e- MC");
 grMC_Dphi_eMinus->SetMarkerColor(kBlue);
 grMC_Dphi_eMinus->SetLineColor(kBlue);
 grMC_Dphi_eMinus->SetFillColor(kWhite);
 grMC_Dphi_eMinus->SetMarkerStyle(24);
 grMC_Dphi_eMinus->SetMarkerSize(1);
 grMC_Dphi_eMinus->SetLineWidth(1);
 grMC_Dphi_eMinus->GetXaxis()->SetTitle("iDet");
 grMC_Dphi_eMinus->GetYaxis()->SetTitle("#Delta#phi");
 
 
 TCanvas* cJointMG = new TCanvas("cJointMG","cJointMG",1000,500);
 cJointMG->SetLeftMargin(0.10);
 cJointMG->SetRightMargin(0.25);
 cJointMG->SetTopMargin(0.05);
 cJointMG->SetBottomMargin(0.20);
 
 cJointMG->cd();
 TMultiGraph* mgr_Dphi = new TMultiGraph();
 mgr_Dphi->Add(grMC_Dphi_eMinus);
 mgr_Dphi->Add(grMC_Dphi_ePlus);
 mgr_Dphi->Add(grDATA_Dphi_eMinus);
 mgr_Dphi->Add(grDATA_Dphi_ePlus);
 mgr_Dphi->Draw("AP");
 mgr_Dphi->GetXaxis()->SetTitle("p_{T}");
 mgr_Dphi->GetYaxis()->SetTitle("#Delta#phi");
 cJointMG->SetGrid();
 TLegend* legQ = cJointMG->BuildLegend(0.8,0.55,0.98,0.75);
 legQ->SetFillColor(kWhite);
 cJointMG->SetGrid();   
 
}