#include "test/TDRStyle.cc"

void DrawTestMCMCComparison(){
 TDRStyle();
 
 TFile *_fileV7 = TFile::Open("data/EcalAlignment-V7-25Nov2011.root");
 TFile *_fileV8 = TFile::Open("data/EcalAlignment-V8-25Nov2011.root");
 
 TH1F* DPhiV7 = new TH1F("DPhiV7","#Delta#phi V7",100,-0.04,0.04);
 TH1F* DPhiV8 = new TH1F("DPhiV8","#Delta#phi V8",100,-0.04,0.04);

 TH1F* DEtaV7 = new TH1F("DEtaV7","#Delta#eta V7",100,-0.02,0.02);
 TH1F* DEtaV8 = new TH1F("DEtaV8","#Delta#eta V8",100,-0.02,0.02);
 
 TTree* trV7 = (TTree*) _fileV7->Get("ntupleEcalAlignment/myTree");
 TTree* trV8 = (TTree*) _fileV8->Get("ntupleEcalAlignment/myTree");
 
 trV7->Draw("deltaPhiSuperClusterAtVtx >> DPhiV7");
 trV8->Draw("deltaPhiSuperClusterAtVtx >> DPhiV8");
 
 trV7->Draw("deltaEtaSuperClusterAtVtx >> DEtaV7");
 trV8->Draw("deltaEtaSuperClusterAtVtx >> DEtaV8");
 
//  h1.Scale(1./h1.GetEntries())
//  h0.Scale(1./h0.GetEntries())
 
 DPhiV7->GetXaxis()->SetTitle("#Delta#phi");
 DPhiV8->GetXaxis()->SetTitle("#Delta#phi");
 DEtaV7->GetXaxis()->SetTitle("#Delta#eta");
 DEtaV8->GetXaxis()->SetTitle("#Delta#eta");
 
 DPhiV7->SetLineColor(kBlue);
 DPhiV7->SetLineWidth(2);
 DPhiV7->SetFillColor(kBlue);
 DPhiV7->SetFillStyle(3001);
 
 DPhiV8->SetLineColor(kRed);
 DPhiV8->SetLineWidth(2);
 DPhiV8->SetFillColor(kRed);
 DPhiV8->SetFillStyle(3002);
 
 DEtaV7->SetLineColor(kBlue);
 DEtaV7->SetLineWidth(2);
 DEtaV7->SetFillColor(kBlue);
 DEtaV7->SetFillStyle(3001);
 
 DEtaV8->SetLineColor(kRed);
 DEtaV8->SetLineWidth(2);
 DEtaV8->SetFillColor(kRed);
 DEtaV8->SetFillStyle(3002);
 
 
 TCanvas* cDPhi = new TCanvas();
 DPhiV7->Draw();
 DPhiV8->Draw("same");
 
 TLegend* legDPhi = cDPhi->BuildLegend(0.8,0.55,0.98,0.75);
 legDPhi->SetFillColor(kWhite);
 cDPhi->SetGrid();   
 
 TCanvas* cDEta = new TCanvas();
 DEtaV7->Draw();
 DEtaV8->Draw("same");
 
 TLegend* legDEta = cDEta->BuildLegend(0.8,0.55,0.98,0.75);
 legDEta->SetFillColor(kWhite);
 cDEta->SetGrid();   
 

 
 std::cout << " Differences " << std::endl;
 
 std::cout << " DPhi V7 = " << DPhiV7->GetMean() << " +/- " << DPhiV7->GetRMS() / sqrt(DPhiV7->GetEntries()) << std::endl;
 std::cout << " DPhi V8 = " << DPhiV8->GetMean() << " +/- " << DPhiV8->GetRMS() / sqrt(DPhiV8->GetEntries()) << std::endl;
 
 std::cout << " DEta V7 = " << DEtaV7->GetMean() << " +/- " << DEtaV7->GetRMS() / sqrt(DEtaV7->GetEntries()) << std::endl;
 std::cout << " DEta V8 = " << DEtaV8->GetMean() << " +/- " << DEtaV8->GetRMS() / sqrt(DEtaV8->GetEntries()) << std::endl;
 
 
}