int nBins_nVtx = 35;
float nVtxMin = 0.5;
float nVtxMax = 35.5;

int nBins_nAvgPU = 30;
float nAvgPUMin = 5.;
float nAvgPUMax = 35;

int nBins_iEta = 35;
float iEtaMin = 1.;
float iEtaMax = 86.;

int nBins_iRing = 20;
float iRingMin = 0.;
float iRingMax = 40.;






void drawNoisePlotsMC(TString InputFileName, TString suffix)
{
  TFile* f = TFile::Open(InputFileName.Data(),"READ");
  f -> cd();
  
  
  TH1F* h_occupancy_vsNvtx   = (TH1F*)( f->Get("h_occupancy_vsNvtx_EB") );
  TH1F* h_occupancy_vsNavgPU = (TH1F*)( f->Get("h_occupancy_vsNavgPU_EB") );
  TH1F* h_occupancy_vsIeta   = (TH1F*)( f->Get("h_occupancy_vsIeta_EB") );
  TH1F* h_occupancy_vsIring  = (TH1F*)( f->Get("h_occupancy_vsIring_EE") );
  
  
    
  TGraphErrors* g_RMSNoise_vsNvtx_EB = new TGraphErrors();
  TGraphErrors* g_RMSNoise_vsNvtx_EE = new TGraphErrors();

  TGraphErrors* g_RMSNoise_vsNavgPU_EB = new TGraphErrors();
  TGraphErrors* g_RMSNoise_vsNavgPU_EE = new TGraphErrors();
  
  int point = 0;
  for(int bin = 1; bin <= nBins_nVtx; ++bin)
  {  
    TH1F* histo;
    char histoName[50];
    
    float binCenter  = h_occupancy_vsNvtx -> GetBinCenter(bin);
    float binLowEdge = h_occupancy_vsNvtx -> GetBinLowEdge(bin);
    float binHigEdge = h_occupancy_vsNvtx -> GetBinLowEdge(bin) + h_occupancy_vsNvtx_EB->GetBinWidth(bin);
    
    if( binCenter < 5. ) continue;
    if( binCenter > 30. ) continue;
        
    sprintf(histoName,"hEB_recHitE_nVtx%02.1f-%02.1f",binLowEdge,binHigEdge);
    histo = (TH1F*)( f->Get(histoName) );
    g_RMSNoise_vsNvtx_EB -> SetPoint(point,binCenter,histo->GetRMS());
    g_RMSNoise_vsNvtx_EB -> SetPointError(point,0.,histo->GetRMSError());
    
    sprintf(histoName,"hEE_recHitE_nVtx%02.1f-%02.1f",binLowEdge,binHigEdge);
    histo = (TH1F*)( f->Get(histoName) );
    g_RMSNoise_vsNvtx_EE -> SetPoint(point,binCenter,histo->GetRMS());
    g_RMSNoise_vsNvtx_EE -> SetPointError(point,0.,histo->GetRMSError());    
    
    ++point;
  }
  
  g_RMSNoise_vsNvtx_EB -> SetPoint(g_RMSNoise_vsNvtx_EB->GetN(),-1.,-1.);
  g_RMSNoise_vsNvtx_EE -> SetPoint(g_RMSNoise_vsNvtx_EE->GetN(),-1.,-1.);
  
  
  
  point = 0;
  for(int bin = 1; bin <= nBins_nAvgPU; ++bin)
  {  
    TH1F* histo;
    char histoName[50];
    
    float binCenter  = h_occupancy_vsNavgPU -> GetBinCenter(bin);
    float binLowEdge = h_occupancy_vsNavgPU -> GetBinLowEdge(bin);
    float binHigEdge = h_occupancy_vsNavgPU -> GetBinLowEdge(bin) + h_occupancy_vsNavgPU_EB->GetBinWidth(bin);
    
    if( binCenter < 8. ) continue;
    if( binCenter > 30. ) continue;
        
    sprintf(histoName,"hEB_recHitE_nAvgPU%02.1f-%02.1f",binLowEdge,binHigEdge);
    histo = (TH1F*)( f->Get(histoName) );
    g_RMSNoise_vsNavgPU_EB -> SetPoint(point,binCenter,histo->GetRMS());
    g_RMSNoise_vsNavgPU_EB -> SetPointError(bin-1,0.,histo->GetRMSError());
    
    sprintf(histoName,"hEE_recHitE_nAvgPU%02.1f-%02.1f",binLowEdge,binHigEdge);
    histo = (TH1F*)( f->Get(histoName) );
    g_RMSNoise_vsNavgPU_EE -> SetPoint(point,binCenter,histo->GetRMS());
    g_RMSNoise_vsNavgPU_EE -> SetPointError(bin-1,0.,histo->GetRMSError());    
    
    ++point;
  }
  
  
  
  
  
  
  TCanvas* c1_RMSNoise_EB = new TCanvas("c1_RMSNoise_EB","EB RMS noise");
  c1_RMSNoise_EB -> cd();
  c1_RMSNoise_EB -> SetGridx();
  c1_RMSNoise_EB -> SetGridy();
  
  g_RMSNoise_vsNvtx_EB -> SetMarkerColor(kRed+2);
  g_RMSNoise_vsNavgPU_EB -> SetMarkerColor(kGreen+2);
  
  g_RMSNoise_vsNvtx_EB -> GetXaxis() -> SetTitle("N_{vtx} or Num^{Int}_{True}");
  g_RMSNoise_vsNvtx_EB -> GetYaxis() -> SetTitle("noise RMS (GeV)");
  g_RMSNoise_vsNvtx_EB -> GetXaxis() -> SetRangeUser(0.,35.);
  g_RMSNoise_vsNvtx_EB -> GetYaxis() -> SetRangeUser(0.040,0.080);
  
  g_RMSNoise_vsNvtx_EB -> Draw("AP");
  g_RMSNoise_vsNavgPU_EB -> Draw("P,same");
  
  TF1* f_pol1_vsNvtx_EB   = new TF1("f_pol1_vsNvtx_EB","[0]+[1]*x",0.,35.);
  f_pol1_vsNvtx_EB -> SetLineColor(kRed+2);
  TF1* f_pol1_vsNavgPU_EB = new TF1("f_pol1_vsNavgPU_EB","[0]+[1]*x",0.,35.);
  f_pol1_vsNavgPU_EB -> SetLineColor(kGreen+2);  
  g_RMSNoise_vsNvtx_EB -> Fit("f_pol1_vsNvtx_EB","QS+","",5.,25.);
  g_RMSNoise_vsNavgPU_EB -> Fit("f_pol1_vsNavgPU_EB","QS+","",10.,25.);
  f_pol1_vsNvtx_EB -> Draw("same");
  f_pol1_vsNavgPU_EB -> Draw("same");
  
  TLegend* legend = new TLegend(0.14, 0.70, 0.60, 0.94);
  legend -> SetFillColor(kWhite);
  legend -> SetFillStyle(0);
  legend -> SetLineWidth(0);
  legend -> SetLineColor(kWhite);
  legend -> SetTextFont(42);  
  legend -> SetTextSize(0.04);
  legend -> AddEntry(g_RMSNoise_vsNvtx_EB,"noise vs N_{vtx}","P");
  legend -> AddEntry(g_RMSNoise_vsNavgPU_EB,"noise vs Num^{Int}_{True}","P");
  legend -> Draw("same");
  
  TString NamePlot = Form("c1_RMSNoise_EB_%s.png",suffix.Data());
  c1_RMSNoise_EB -> SaveAs(NamePlot.Data(),"png");
  
  
  
  TCanvas* c1_RMSNoise_EE = new TCanvas("c1_RMSNoise_EE","EE RMS noise");
  c1_RMSNoise_EE -> cd();
  c1_RMSNoise_EE -> SetGridx();
  c1_RMSNoise_EE -> SetGridy();
  
  g_RMSNoise_vsNvtx_EE -> SetMarkerColor(kRed+2);
  g_RMSNoise_vsNavgPU_EE -> SetMarkerColor(kGreen+2);
    
  g_RMSNoise_vsNvtx_EE -> GetXaxis() -> SetTitle("N_{vtx} or Num^{Int}_{True}");
  g_RMSNoise_vsNvtx_EE -> GetYaxis() -> SetTitle("noise RMS (GeV)");
  g_RMSNoise_vsNvtx_EE -> GetXaxis() -> SetRangeUser(0.,35.);
  g_RMSNoise_vsNvtx_EE -> GetYaxis() -> SetRangeUser(0.20,0.35);
      
  g_RMSNoise_vsNvtx_EE -> Draw("AP");
  g_RMSNoise_vsNavgPU_EE -> Draw("P,same");
  
  TF1* f_pol1_vsNvtx_EE   = new TF1("f_pol1_vsNvtx_EE","[0]+[1]*x",0.,35.);
  f_pol1_vsNvtx_EE -> SetLineColor(kRed+2);
  TF1* f_pol1_vsNavgPU_EE = new TF1("f_pol1_vsNavgPU_EE","[0]+[1]*x",0.,35.);
  f_pol1_vsNavgPU_EE -> SetLineColor(kGreen+2);  
  g_RMSNoise_vsNvtx_EE -> Fit("f_pol1_vsNvtx_EE","QS+","",5.,25.);
  g_RMSNoise_vsNavgPU_EE -> Fit("f_pol1_vsNavgPU_EE","QS+","",10.,25.);
  f_pol1_vsNvtx_EE -> Draw("same");
  f_pol1_vsNavgPU_EE -> Draw("same");
  
  legend -> Draw("same");
 
  NamePlot = Form("c1_RMSNoise_EE_%s.png",suffix.Data());
  c1_RMSNoise_EE -> SaveAs(NamePlot.Data(),"png");
  
  
  
  
  
  
  TGraphErrors* g_noiseAtZero_vsIeta_EB = new TGraphErrors();
  TGraphErrors* g_noiseSlope_vsIeta_EB = new TGraphErrors();
  TGraphErrors* g_addNoise_vsIeta_EB = new TGraphErrors();
  
  for(int iEtaBin = 1; iEtaBin <= nBins_iEta; ++iEtaBin)
  {
    float iEtaBinCenter  = h_occupancy_vsIeta -> GetBinCenter(iEtaBin);
    float iEtaBinLowEdge = h_occupancy_vsIeta -> GetBinLowEdge(iEtaBin);
    float iEtaBinHigEdge = h_occupancy_vsIeta -> GetBinLowEdge(iEtaBin) + h_occupancy_vsIeta_EB->GetBinWidth(bin);
    
    TGraphErrors* g_RMSNoise_vsNavgPU_vsIeta_EB = new TGraphErrors();
    
    int point = 0;
    for(int bin = 1; bin <= nBins_nAvgPU; ++bin)
    {  
      TH1F* histo;
      char histoName[50];
      
      float binCenter  = h_occupancy_vsNavgPU -> GetBinCenter(bin);
      float binLowEdge = h_occupancy_vsNavgPU -> GetBinLowEdge(bin);
      float binHigEdge = h_occupancy_vsNavgPU -> GetBinLowEdge(bin) + h_occupancy_vsNavgPU_EB->GetBinWidth(bin);
      
      if( binCenter < 8. ) continue;
      if( binCenter > 30. ) continue;    
      sprintf(histoName,"hEB_recHitE_nAvgPU%02.1f-%02.1f_iEta%02.1f-%02.1f",binLowEdge,binHigEdge,iEtaBinLowEdge,iEtaBinHigEdge);
      histo = (TH1F*)( f->Get(histoName) );
      
      g_RMSNoise_vsNavgPU_vsIeta_EB -> SetPoint(point,binCenter,histo->GetRMS());
      g_RMSNoise_vsNavgPU_vsIeta_EB -> SetPointError(point,0.,histo->GetRMSError());
      
      ++point;
    }
    
    char funcName[50];
    sprintf(funcName,"f_pol1_iEta%02.1f-%02.1f_EB",iEtaBinLowEdge,iEtaBinHigEdge);
    TF1* f_pol1_vsNavgPU_vsIeta_EB = new TF1(funcName,"[0]+[1]*x",0.,35.);
    g_RMSNoise_vsNavgPU_vsIeta_EB -> Fit(funcName,"QS+","",10.,25.);
    
    g_noiseAtZero_vsIeta_EB -> SetPoint(iEtaBin-1,iEtaBinCenter,f_pol1_vsNavgPU_vsIeta_EB->GetParameter(0.));
    g_noiseAtZero_vsIeta_EB -> SetPointError(iEtaBin-1,0.,f_pol1_vsNavgPU_vsIeta_EB->GetParError(0.));
    g_noiseSlope_vsIeta_EB -> SetPoint(iEtaBin-1,iEtaBinCenter,f_pol1_vsNavgPU_vsIeta_EB->GetParameter(1.));
    g_noiseSlope_vsIeta_EB -> SetPointError(iEtaBin-1,0.,f_pol1_vsNavgPU_vsIeta_EB->GetParError(1.));
    
    g_addNoise_vsIeta_EB -> SetPoint(iEtaBin-1,iEtaBinCenter,sqrt(pow(f_pol1_vsNavgPU_vsIeta_EB->GetParameter(0.)+20.*f_pol1_vsNavgPU_vsIeta_EB->GetParameter(1.),2)-pow(f_pol1_vsNavgPU_vsIeta_EB->GetParameter(0.),2)));
  }
  
  
  
  TCanvas* c1_noiseAtZero_vsIeta_EB = new TCanvas("c1_noiseAtZero_vsIeta_EB","EB noise at 0 PU vs iEta");
  c1_noiseAtZero_vsIeta_EB -> cd();
  c1_noiseAtZero_vsIeta_EB -> SetGridx();
  c1_noiseAtZero_vsIeta_EB -> SetGridy();
  
  g_noiseAtZero_vsIeta_EB -> GetXaxis() -> SetTitle("i#eta");
  g_noiseAtZero_vsIeta_EB -> GetYaxis() -> SetTitle("RMS noise at N_{PU}=0 (GeV)");
  g_noiseAtZero_vsIeta_EB -> Draw("AP");
  
  NamePlot = Form("c1_noiseAtZero_vsIeta_EB_%s.png",suffix.Data());
  c1_noiseAtZero_vsIeta_EB -> SaveAs(NamePlot.Data(),"png");

  
  
  TCanvas* c1_noiseSlope_vsIeta_EB = new TCanvas("c1_noiseSlope_vsIeta_EB","EB noise slope vs iEta");
  c1_noiseSlope_vsIeta_EB -> cd();
  c1_noiseSlope_vsIeta_EB -> SetGridx();
  c1_noiseSlope_vsIeta_EB -> SetGridy();
  
  g_noiseSlope_vsIeta_EB -> GetXaxis() -> SetTitle("i#eta");
  g_noiseSlope_vsIeta_EB -> GetYaxis() -> SetTitle("RMS noise slope wrt N_{PU} (GeV/N_{PU})");
  g_noiseSlope_vsIeta_EB -> Draw("AP");
  
  NamePlot = Form("c1_noiseSlope_vsIeta_EB_%s.png",suffix.Data());
  c1_noiseSlope_vsIeta_EB -> SaveAs(NamePlot.Data(),"png");
  
  TCanvas* c1_addNoise_vsIeta_EB = new TCanvas("c1_addNoise_vsIeta_EB","EB additional noise vs iEta");
  c1_addNoise_vsIeta_EB -> cd();
  c1_addNoise_vsIeta_EB -> SetGridx();
  c1_addNoise_vsIeta_EB -> SetGridy();
  
  g_addNoise_vsIeta_EB -> GetXaxis() -> SetTitle("i#eta");
  g_addNoise_vsIeta_EB -> GetYaxis() -> SetTitle("additional quad. noise at N_{PU}=20 (GeV)");
  g_addNoise_vsIeta_EB -> Draw("AP");


  NamePlot = Form("c1_addNoise_vsIeta_EB_%s.png",suffix.Data());
  c1_addNoise_vsIeta_EB -> SaveAs(NamePlot.Data(),"png");
  
 
  
  TGraphErrors* g_noiseAtZero_vsIring_EE = new TGraphErrors();
  TGraphErrors* g_noiseSlope_vsIring_EE = new TGraphErrors();
  TGraphErrors* g_addNoise_vsIring_EE = new TGraphErrors();
    
  for(int iRingBin = 1; iRingBin <= nBins_iRing; ++iRingBin)
  {
    float iRingBinCenter  = h_occupancy_vsIring -> GetBinCenter(iRingBin);
    float iRingBinLowEdge = h_occupancy_vsIring -> GetBinLowEdge(iRingBin);
    float iRingBinHigEdge = h_occupancy_vsIring -> GetBinLowEdge(iRingBin) + h_occupancy_vsIring_EE->GetBinWidth(bin);
    
    if( iRingBinCenter > 33 ) continue;
    
    TGraphErrors* g_RMSNoise_vsNavgPU_vsIring_EE = new TGraphErrors();
    
    int point = 0;
    for(int bin = 1; bin <= nBins_nAvgPU; ++bin)
    {  
      TH1F* histo;
      char histoName[50];
      
      float binCenter  = h_occupancy_vsNavgPU -> GetBinCenter(bin);
      float binLowEdge = h_occupancy_vsNavgPU -> GetBinLowEdge(bin);
      float binHigEdge = h_occupancy_vsNavgPU -> GetBinLowEdge(bin) + h_occupancy_vsNavgPU_EE->GetBinWidth(bin);
      
      if( binCenter < 8. ) continue;
      if( binCenter > 30. ) continue;    
      sprintf(histoName,"hEE_recHitE_nAvgPU%02.1f-%02.1f_iRing%02.1f-%02.1f",binLowEdge,binHigEdge,iRingBinLowEdge,iRingBinHigEdge);
      histo = (TH1F*)( f->Get(histoName) );
      
      g_RMSNoise_vsNavgPU_vsIring_EE -> SetPoint(point,binCenter,histo->GetRMS());
      g_RMSNoise_vsNavgPU_vsIring_EE -> SetPointError(point,0.,histo->GetRMSError());
      
      ++point;
    }
    
    char funcName[50];
    sprintf(funcName,"f_pol1_iRing%02.1f-%02.1f_EE",iRingBinLowEdge,iRingBinHigEdge);
    TF1* f_pol1_vsNavgPU_vsIring_EE = new TF1(funcName,"[0]+[1]*x",0.,35.);
    g_RMSNoise_vsNavgPU_vsIring_EE -> Fit(funcName,"QS+","",10.,25.);
    
    g_noiseAtZero_vsIring_EE -> SetPoint(iRingBin-1,iRingBinCenter,f_pol1_vsNavgPU_vsIring_EE->GetParameter(0.));
    g_noiseAtZero_vsIring_EE -> SetPointError(iRingBin-1,0.,f_pol1_vsNavgPU_vsIring_EE->GetParError(0.));
    g_noiseSlope_vsIring_EE -> SetPoint(iRingBin-1,iRingBinCenter,f_pol1_vsNavgPU_vsIring_EE->GetParameter(1.));
    g_noiseSlope_vsIring_EE -> SetPointError(iRingBin-1,0.,f_pol1_vsNavgPU_vsIring_EE->GetParError(1.));
    
    g_addNoise_vsIring_EE -> SetPoint(iRingBin-1,iRingBinCenter,sqrt(pow(f_pol1_vsNavgPU_vsIring_EE->GetParameter(0.)+20.*f_pol1_vsNavgPU_vsIring_EE->GetParameter(1.),2)-pow(f_pol1_vsNavgPU_vsIring_EE->GetParameter(0.),2)));    
  }
  
  
  
  TCanvas* c1_noiseAtZero_vsIring_EE = new TCanvas("c1_noiseAtZero_vsIring_EE","EE noise at 0 PU vs iRing");
  c1_noiseAtZero_vsIring_EE -> cd();
  c1_noiseAtZero_vsIring_EE -> SetGridx();
  c1_noiseAtZero_vsIring_EE -> SetGridy();
  
  g_noiseAtZero_vsIring_EE -> GetXaxis() -> SetTitle("ring");
  g_noiseAtZero_vsIring_EE -> GetYaxis() -> SetTitle("RMS noise at N_{PU}=0 (GeV)");
  g_noiseAtZero_vsIring_EE -> Draw("AP");
  
  NamePlot = Form("c1_noiseAtZero_vsIring_EE_%s.png",suffix.Data());
  c1_noiseAtZero_vsIring_EE -> SaveAs(NamePlot.Data(),"png"); 
  
  
  TCanvas* c1_noiseSlope_vsIring_EE = new TCanvas("c1_noiseSlope_vsIring_EE","EE noise slope vs iRing");
  c1_noiseSlope_vsIring_EE -> cd();
  c1_noiseSlope_vsIring_EE -> SetGridx();
  c1_noiseSlope_vsIring_EE -> SetGridy();
  
  g_noiseSlope_vsIring_EE -> GetXaxis() -> SetTitle("ring");
  g_noiseSlope_vsIring_EE -> GetYaxis() -> SetTitle("RMS noise slope wrt N_{PU} (GeV/N_{PU})");
  g_noiseSlope_vsIring_EE -> Draw("AP");  
  
  NamePlot = Form("c1_noiseSlope_vsIring_EE _%s.png",suffix.Data());
  c1_noiseSlope_vsIring_EE  -> SaveAs(NamePlot.Data(),"png");
  
  
  
  TCanvas* c1_addNoise_vsIring_EE = new TCanvas("c1_addNoise_vsIring_EE","EE additional noise vs iEta");
  c1_addNoise_vsIring_EE -> cd();
  c1_addNoise_vsIring_EE -> SetGridx();
  c1_addNoise_vsIring_EE -> SetGridy();
  
  g_addNoise_vsIring_EE -> GetXaxis() -> SetTitle("ring");
  g_addNoise_vsIring_EE -> GetYaxis() -> SetTitle("additional quad. noise at N_{PU}=20 (GeV)");
  g_addNoise_vsIring_EE -> Draw("AP");

  NamePlot = Form("c1_addNoise_vsIring_EE_%s.png",suffix.Data());
  c1_addNoise_vsIring_EE-> SaveAs(NamePlot.Data(),"png");
}
