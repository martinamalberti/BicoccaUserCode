typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> > PtEtaPhiELorentzVector;

double crystalBall(double* x, double* par);



makeZPeak(Char_t* infile1 = 0, Char_t* infile2 = 0)
{
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetOptTitle(0); 
  gStyle->SetOptStat(1110);
  gStyle->SetOptFit(1);
  
  
  
  // Open file
  if( (!infile1) || (!infile2) )
  {
    std::cout << "CHK-STB Error: Input file not specified !" << std::endl;
    return;
  }  
  
  TFile *f = TFile::Open(infile1);  // data
  TFile *m = TFile::Open(infile2);  // data or mc  
  
  
  
  // Get trees
  std::cout << std::endl;
  
  TTree* ntu_DA = (TTree*)(f->Get("ntu"));
  std::cout << "     DATA: " << ntu_DA->GetEntries() << " entries in " << infile1 << std::endl; 
  
  TTree* ntu_MC = (TTree*)(m->Get("ntu"));
  std::cout << "REFERENCE: " << ntu_MC->GetEntries() << " entries in " << infile2 << std::endl;  
  
  
  
  // Set branch addresses  
  int runId,isZ;
  int PV_n;
  int isEB,isEB2;
  float avgLaserCorrection,avgLaserCorrection2; 
  float seedLaserAlpha,seedLaserAlpha2;
  float mZ;
  
  float scE,scE2;
  float scET,scET2;
  float scEta,scEta2;
  float eta,eta2;
  float phi,phi2;
  
  ntu_DA->SetBranchAddress("runId",            &runId); 
  ntu_DA->SetBranchAddress("isZ",              &isZ);
  ntu_DA->SetBranchAddress("PV_n",             &PV_n);
  ntu_DA->SetBranchAddress("ele1_isEB",        &isEB);
  ntu_DA->SetBranchAddress("ele2_isEB",        &isEB2);
  ntu_DA->SetBranchAddress("ele1_scE",         &scE);
  ntu_DA->SetBranchAddress("ele2_scE",         &scE2);
  ntu_DA->SetBranchAddress("ele1_scEta",       &scEta);
  ntu_DA->SetBranchAddress("ele2_scEta",       &scEta2);
  ntu_DA->SetBranchAddress("ele1_eta",         &eta);
  ntu_DA->SetBranchAddress("ele2_eta",         &eta2);
  ntu_DA->SetBranchAddress("ele1_phi",         &phi);
  ntu_DA->SetBranchAddress("ele2_phi",         &phi2);
  ntu_DA->SetBranchAddress("ele1_seedLaserAlpha", &seedLaserAlpha);
  ntu_DA->SetBranchAddress("ele2_seedLaserAlpha", &seedLaserAlpha2);
  ntu_DA->SetBranchAddress("ele1_scLaserCorr", &avgLaserCorrection);
  ntu_DA->SetBranchAddress("ele2_scLaserCorr", &avgLaserCorrection2);

  ntu_MC->SetBranchAddress("isZ",   &isZ);
  ntu_MC->SetBranchAddress("scE1",  &scE);
  ntu_MC->SetBranchAddress("scE2",  &scE2);
  ntu_MC->SetBranchAddress("eta1",  &eta);
  ntu_MC->SetBranchAddress("eta2",  &eta2);
  ntu_MC->SetBranchAddress("phi1",  &phi);
  ntu_MC->SetBranchAddress("phi2",  &phi2);
  
  
  
  // Define histograms
  TH1F* h_mZmc_EBEB = new TH1F("h_mZmc_EBEB", "",100,65.,115.);
  TH1F* h_mZmc_EEEE = new TH1F("h_mZmc_EEEE", "",100,65.,115.);
  TH1F* h_mZmc_EBEE = new TH1F("h_mZmc_EBEE", "",100,65.,115.);
  
  TH1F* h_mZ_EBEB = new TH1F("h_mZ_EBEB", "",100,65.,115.);
  TH1F* h_mZ_EEEE = new TH1F("h_mZ_EEEE", "",100,65.,115.);
  TH1F* h_mZ_EBEE = new TH1F("h_mZ_EBEE", "",100,65.,115.);

  TH1F* h_mZc_EBEB = new TH1F("h_mZc_EBEB", "",100,65.,115.);
  TH1F* h_mZc_EEEE = new TH1F("h_mZc_EEEE", "",100,65.,115.);  
  TH1F* h_mZc_EBEE = new TH1F("h_mZc_EBEE", "",100,65.,115.);
  
  
  // Loop over entries
  int nEntries = ntu_DA -> GetEntriesFast();
  for(int ientry = 0; ientry < nEntries; ++ientry)
  {
    if( ientry%10000 == 0 ) std::cout << "reading saved entry " << ientry << std::endl;
    ntu_DA -> GetEntry(ientry); 
    
    if ( runId < 163232 ) continue;    
    if( isZ != 1 ) continue;
    //if( PV_n < 5 ) continue;
    //if( PV_n > 10 ) continue;
    //if( PV_n < 10 ) continue;
    if( seedLaserAlpha < 1.5 )  continue;
    if( seedLaserAlpha2 < 1.5 ) continue;
    if( (fabs(scEta) > 2.5) || (fabs(scEta2) > 2.5) ) continue;
    if( (fabs(scEta) > 1.4442) && (fabs(scEta) < 1.5660) ) continue;
    if( (fabs(scEta2) > 1.4442) && (fabs(scEta2) < 1.5660) ) continue;
    
    PtEtaPhiELorentzVector sc1(scE *sin(2*atan(exp(-1.*eta))), eta, phi, scE);
    PtEtaPhiELorentzVector sc2(scE2*sin(2*atan(exp(-1.*eta2))),eta2,phi2,scE2);
    mZ = (sc1+sc2).mass();
    
    float scale1 = 1.;
    float scale2 = 1.;
    
    //scale1 = 1.04 * pow(avgLaserCorrection,((1.52-0.7843)/1.52)-1.);
    //scale2 = 1.04 * pow(avgLaserCorrection2,((1.52-0.7843)/1.52)-1.);
    
    scale1 = 1.04 * 1. / (0.4867 + 0.5217*avgLaserCorrection);
    scale2 = 1.04 * 1. / (0.4867 + 0.5217*avgLaserCorrection2);
    
    //scale1 = 1.079 * 1. / avgLaserCorrection;
    //scale2 = 1.079 * 1. / avgLaserCorrection2;
        
    if( (isEB == 1) && (isEB2 == 1) )
    {
      h_mZ_EBEB -> Fill(mZ);
      h_mZc_EBEB -> Fill(mZ);
    }
    
    else if( (isEB == 0) && (isEB2 == 0) )
    {
      h_mZ_EEEE -> Fill(mZ);
      h_mZc_EEEE -> Fill( mZ * sqrt(scale1 * scale2) );
    }
    
    else
    {
      if( isEB == 1)
      {
        h_mZ_EBEE -> Fill(mZ);
        h_mZc_EBEE -> Fill( mZ * sqrt(scale2) );
      }
      else
      {
        h_mZ_EBEE -> Fill(mZ);
        h_mZc_EBEE -> Fill( mZ * sqrt(scale1) );
      }

    }
  }
  
  
  
  nEntries = ntu_MC -> GetEntriesFast();
  for(int ientry = 0; ientry < nEntries; ++ientry)
  {
    if( ientry%10000 == 0 ) std::cout << "reading saved entry " << ientry << std::endl;
    ntu_MC -> GetEntry(ientry); 
    
    if( isZ != 1 ) continue;
    
    
        
    PtEtaPhiELorentzVector sc1(scE* sin(2*atan(exp(-1.*eta))), eta, phi, scE);
    PtEtaPhiELorentzVector sc2(scE2*sin(2*atan(exp(-1.*eta2))),eta2,phi2,scE2);
    mZ = (sc1+sc2).mass();
        
    if( (abs(eta) < 1.479) && (abs(eta2) < 1.479) )
    {
      h_mZmc_EBEB -> Fill(mZ);
    }
    
    else if( (abs(eta) > 1.479) && (abs(eta2) > 1.479) )
    {
      h_mZmc_EEEE -> Fill(mZ);
    }
    
    else
    {
      h_mZmc_EBEE -> Fill(mZ);
    }
  }
  
  
  
  TCanvas* c_mZ = new TCanvas("c_mZ","mZ",100,100,900,500);
  c_mZ -> Divide(2,1);
  int rebin = 1;
  
  
  //c_mZ -> cd(1);
  //
  //h_mZ_EBEB -> GetXaxis() -> SetTitle("m_{SC1+SC2} - (EB,EB)");
  //h_mZ_EBEB -> SetLineColor(kRed+2);
  //h_mZ_EBEB -> SetLineWidth(2);
  //h_mZ_EBEB -> SetMarkerColor(kRed+2);
  //h_mZ_EBEB -> SetMarkerSize(0.7);
  //h_mZ_EBEB -> Rebin(rebin);
  //h_mZ_EBEB -> Draw("");
  //gPad->Update();
  //
  //h_mZc_EBEB -> GetXaxis() -> SetTitle("m_{SC1+SC2} - (EB,EB)");
  //h_mZc_EBEB -> SetLineColor(kGreen+2);
  //h_mZc_EBEB -> SetLineWidth(2);
  //h_mZc_EBEB -> SetMarkerColor(kGreen+2);
  //h_mZc_EBEB -> SetMarkerSize(0.7);
  //h_mZc_EBEB -> Rebin(rebin);
  //h_mZc_EBEB -> Draw("sames");
  //gPad->Update();
  //
  //h_mZmc_EBEB -> Scale(h_mZ_EBEB->Integral()/h_mZmc_EBEB->Integral());  
  //h_mZmc_EBEB -> SetLineColor(kBlack);
  //h_mZmc_EBEB -> SetFillColor(kBlack);
  //h_mZmc_EBEB -> SetFillStyle(3003);
  //h_mZmc_EBEB -> Rebin(rebin);
  //h_mZmc_EBEB -> Draw("same");
  //gPad->Update();
  //  
  //TPaveStats* p_mZ_EBEB = (TPaveStats*)(h_mZ_EBEB->GetListOfFunctions()->FindObject("stats"));  
  //p_mZ_EBEB->SetY1NDC(0.79);
  //p_mZ_EBEB->SetY2NDC(0.99);
  //p_mZ_EBEB->SetTextColor(kRed+2);
  //p_mZ_EBEB->Draw("same");
  //  
  //TPaveStats* p_mZc_EBEB = (TPaveStats*)(h_mZc_EBEB->GetListOfFunctions()->FindObject("stats"));  
  //p_mZc_EBEB->SetY1NDC(0.59);
  //p_mZc_EBEB->SetY2NDC(0.79);
  //p_mZc_EBEB->SetTextColor(kGreen+2);
  //p_mZc_EBEB->Draw("same");
  
  
  
  c_mZ -> cd(1);
    
  h_mZc_EEEE -> GetXaxis() -> SetTitle("m_{SC1+SC2} - (EE,EE)");
  h_mZc_EEEE -> SetLineColor(kGreen+2);
  h_mZc_EEEE -> SetLineWidth(2);
  h_mZc_EEEE -> SetMarkerColor(kGreen+2);
  h_mZc_EEEE -> SetMarkerSize(0.7);
  h_mZc_EEEE -> Rebin(rebin*2);
  h_mZc_EEEE -> DrawCopy("");

  TF1* funzc_EEEE = new TF1("funzc_EEEE", crystalBall, 70., h_mZc_EEEE->GetMean()+1.75*h_mZc_EEEE->GetRMS(), 5);
  funzc_EEEE->SetNpx(10000);
  funzc_EEEE->SetLineColor(kGreen+2);
  funzc_EEEE->SetParameter(0, h_mZc_EEEE->GetEntries()/10.);
  funzc_EEEE->SetParameter(1, 90.);
  funzc_EEEE->SetParameter(2, 2.5);
  funzc_EEEE->SetParameter(3, 1.);
  funzc_EEEE->SetParameter(4, 1.);  
  h_mZc_EEEE->Fit("funzc_EEEE","0QRM+");
  h_mZc_EEEE->Draw("sames");
  gPad->Update();

  h_mZ_EEEE -> GetXaxis() -> SetTitle("m_{SC1+SC2} - (EE,EE)");
  h_mZ_EEEE -> SetLineColor(kRed+2);
  h_mZ_EEEE -> SetLineWidth(2);
  h_mZ_EEEE -> SetMarkerColor(kRed+2);
  h_mZ_EEEE -> SetMarkerSize(0.7);
  h_mZ_EEEE -> Rebin(rebin*2);
  h_mZ_EEEE -> Draw("sames");
  
  TF1* funz_EEEE = new TF1("funz_EEEE", crystalBall, 70., h_mZ_EEEE->GetMean()+1.75*h_mZ_EEEE->GetRMS(), 5);
  funz_EEEE->SetNpx(10000);
  funz_EEEE->SetLineColor(kRed+2);
  funz_EEEE->SetParameter(0, h_mZ_EEEE->GetEntries()/10.);
  funz_EEEE->SetParameter(1, 90.);
  funz_EEEE->SetParameter(2, 2.5);
  funz_EEEE->SetParameter(3, 1.);
  funz_EEEE->SetParameter(4, 1.);  
  h_mZ_EEEE->Fit("funz_EEEE","0QRM+");
  h_mZ_EEEE->Draw("sames");
  gPad->Update();
  
  h_mZmc_EEEE -> Scale(h_mZ_EEEE->Integral()/h_mZmc_EEEE->Integral());  
  h_mZmc_EEEE -> SetLineColor(kBlack);
  h_mZmc_EEEE -> SetFillColor(kBlack);
  h_mZmc_EEEE -> SetFillStyle(3003);
  h_mZmc_EEEE -> Rebin(rebin*2);
  //h_mZmc_EEEE -> Draw("same");
  gPad->Update();
    
  TPaveStats* p_mZ_EEEE = (TPaveStats*)(h_mZ_EEEE->GetListOfFunctions()->FindObject("stats"));  
  p_mZ_EEEE->SetY1NDC(0.69);
  p_mZ_EEEE->SetY2NDC(0.99);
  p_mZ_EEEE->SetTextColor(kRed+2);
  p_mZ_EEEE->Draw("same");
    
  TPaveStats* p_mZc_EEEE = (TPaveStats*)(h_mZc_EEEE->GetListOfFunctions()->FindObject("stats"));  
  p_mZc_EEEE->SetY1NDC(0.39);
  p_mZc_EEEE->SetY2NDC(0.69);
  p_mZc_EEEE->SetTextColor(kGreen+2);
  p_mZc_EEEE->Draw("same"); 
  
  funzc_EEEE->Draw("same");
  funz_EEEE->Draw("same");
  
  
  c_mZ -> cd(2);
  
  h_mZc_EBEE -> GetXaxis() -> SetTitle("m_{SC1+SC2} - (EB,EE)");
  h_mZc_EBEE -> SetLineColor(kGreen+2);
  h_mZc_EBEE -> SetLineWidth(2);
  h_mZc_EBEE -> SetMarkerColor(kGreen+2);
  h_mZc_EBEE -> SetMarkerSize(0.7);
  h_mZc_EBEE -> Rebin(rebin*2);
  h_mZc_EBEE -> DrawCopy("");

  TF1* funzc_EBEE = new TF1("funzc_EBEE", crystalBall, 70., h_mZc_EBEE->GetMean()+1.75*h_mZc_EBEE->GetRMS(), 5);
  funzc_EBEE->SetNpx(10000);
  funzc_EBEE->SetLineColor(kGreen+2);
  funzc_EBEE->SetParameter(0, h_mZc_EBEE->GetEntries()/10.);
  funzc_EBEE->SetParameter(1, 90.);
  funzc_EBEE->SetParameter(2, 2.5);
  funzc_EBEE->SetParameter(3, 1.);
  funzc_EBEE->SetParameter(4, 1.);  
  h_mZc_EBEE->Fit("funzc_EBEE","0QRM+");
  h_mZc_EBEE->Draw("sames");
  gPad->Update();

  h_mZ_EBEE -> GetXaxis() -> SetTitle("m_{SC1+SC2} - (EB,EE)");
  h_mZ_EBEE -> SetLineColor(kRed+2);
  h_mZ_EBEE -> SetLineWidth(2);
  h_mZ_EBEE -> SetMarkerColor(kRed+2);
  h_mZ_EBEE -> SetMarkerSize(0.7);
  h_mZ_EBEE -> Rebin(rebin*2);
  h_mZ_EBEE -> Draw("sames");
  
  TF1* funz_EBEE = new TF1("funz_EBEE", crystalBall, 70., h_mZ_EBEE->GetMean()+1.75*h_mZ_EBEE->GetRMS(), 5);
  funz_EBEE->SetNpx(10000);
  funz_EBEE->SetLineColor(kRed+2);
  funz_EBEE->SetParameter(0, h_mZ_EBEE->GetEntries()/10.);
  funz_EBEE->SetParameter(1, 90.);
  funz_EBEE->SetParameter(2, 2.5);
  funz_EBEE->SetParameter(3, 1.);
  funz_EBEE->SetParameter(4, 1.);  
  h_mZ_EBEE->Fit("funz_EBEE","0QRM+");
  h_mZ_EBEE->Draw("sames");
  gPad->Update();
  
  h_mZmc_EBEE -> Scale(h_mZ_EBEE->Integral()/h_mZmc_EBEE->Integral());  
  h_mZmc_EBEE -> SetLineColor(kBlack);
  h_mZmc_EBEE -> SetFillColor(kBlack);
  h_mZmc_EBEE -> SetFillStyle(3003);
  h_mZmc_EBEE -> Rebin(rebin*2);
  //h_mZmc_EBEE -> Draw("same");
  gPad->Update();
    
  TPaveStats* p_mZ_EBEE = (TPaveStats*)(h_mZ_EBEE->GetListOfFunctions()->FindObject("stats"));  
  p_mZ_EBEE->SetY1NDC(0.69);
  p_mZ_EBEE->SetY2NDC(0.99);
  p_mZ_EBEE->SetTextColor(kRed+2);
  p_mZ_EBEE->Draw("same");
    
  TPaveStats* p_mZc_EBEE = (TPaveStats*)(h_mZc_EBEE->GetListOfFunctions()->FindObject("stats"));  
  p_mZc_EBEE->SetY1NDC(0.39);
  p_mZc_EBEE->SetY2NDC(0.69);
  p_mZc_EBEE->SetTextColor(kGreen+2);
  p_mZc_EBEE->Draw("same"); 
  
  funzc_EBEE->Draw("same");
  funz_EBEE->Draw("same");
  
}






makeZPeakVsNVtx(Char_t* infile1 = 0)
{
  int nBins_nVtx = 4;
  float nVtxMin = 0.5;
  float nVtxMax = 12.5;
  float nVtx_width = (nVtxMax-nVtxMin)/nBins_nVtx;
  
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetOptTitle(0); 
  gStyle->SetOptStat(1110);
  gStyle->SetOptFit(1);
  
  
  
  // Open file
  if( (!infile1) )
  {
    std::cout << "CHK-STB Error: Input file not specified !" << std::endl;
    return;
  }  
  
  TFile *f = TFile::Open(infile1);  // data
  
  
  
  // Get trees
  std::cout << std::endl;
  
  TTree* ntu_DA = (TTree*)(f->Get("ntu"));
  std::cout << "     DATA: " << ntu_DA->GetEntries() << " entries in " << infile1 << std::endl; 
  
  
  
  // Set branch addresses  
  int runId,isZ;
  int PV_n;
  int isEB,isEB2;
  float avgLaserCorrection,avgLaserCorrection2; 
  float seedLaserAlpha,seedLaserAlpha2;
  
  float scE,scE2;
  float scET,scET2;
  float scEta,scEta2;
  float eta,eta2;
  float phi,phi2;
  float m, scM;
  
  ntu_DA->SetBranchAddress("runId",               &runId); 
  ntu_DA->SetBranchAddress("isZ",                 &isZ);
  ntu_DA->SetBranchAddress("PV_n",                &PV_n);
  ntu_DA->SetBranchAddress("ele1_isEB",           &isEB);
  ntu_DA->SetBranchAddress("ele2_isEB",           &isEB2);
  ntu_DA->SetBranchAddress("ele1_scE",            &scE);
  ntu_DA->SetBranchAddress("ele2_scE",            &scE2);
  ntu_DA->SetBranchAddress("ele1_scEta",          &scEta);
  ntu_DA->SetBranchAddress("ele2_scEta",          &scEta2);
  ntu_DA->SetBranchAddress("ele1_eta",            &eta);
  ntu_DA->SetBranchAddress("ele2_eta",            &eta2);
  ntu_DA->SetBranchAddress("ele1_phi",            &phi);
  ntu_DA->SetBranchAddress("ele2_phi",            &phi2);
  ntu_DA->SetBranchAddress("ele1_seedLaserAlpha", &seedLaserAlpha);
  ntu_DA->SetBranchAddress("ele2_seedLaserAlpha", &seedLaserAlpha2);
  ntu_DA->SetBranchAddress("ele1_scLaserCorr",    &avgLaserCorrection);
  ntu_DA->SetBranchAddress("ele2_scLaserCorr",    &avgLaserCorrection2);
  ntu_DA->SetBranchAddress("ele1ele2_m",          &m);
  ntu_DA->SetBranchAddress("ele1ele2_scM",        &scM);
  
  
  // Define histograms
  TH1F* h_scM_EBEB = new TH1F("h_scM_EBEB", "",200,65.,115.);
  h_scM_EBEB -> Sumw2();

  TH1F* h_scM_EBEE = new TH1F("h_scM_EBEE", "",200,65.,115.);
  h_scM_EBEE -> Sumw2();
  
  TH1F* h_scM_EEEE = new TH1F("h_scM_EEEE", "",200,65.,115.);
  h_scM_EEEE -> Sumw2();
      
  TH1F** h_scM_vsNVtx_EBEB = new TH1F[nBins_nVtx];
  TH1F** h_scM_vsNVtx_EBEE = new TH1F[nBins_nVtx];
  TH1F** h_scM_vsNVtx_EEEE = new TH1F[nBins_nVtx];
    
  for(int bin = 0; bin < nBins_nVtx; ++bin)
  {
    char histoName[50];
    
    sprintf(histoName,"h_scM_EBEB_%d",bin);
    h_scM_vsNVtx_EBEB[bin] = new TH1F(histoName, "",200,65.,115.);
    h_scM_vsNVtx_EBEB[bin] -> Sumw2();
    
    sprintf(histoName,"h_scM_EBEE_%d",bin);
    h_scM_vsNVtx_EBEE[bin] = new TH1F(histoName, "",200,65.,115.);
    h_scM_vsNVtx_EBEE[bin] -> Sumw2();
    
    sprintf(histoName,"h_scM_EEEE_%d",bin);
    h_scM_vsNVtx_EEEE[bin] = new TH1F(histoName, "",200,65.,115.);
    h_scM_vsNVtx_EEEE[bin] -> Sumw2();    
  }
  
  
  
  // Loop over entries
  int nEntries = ntu_DA -> GetEntriesFast();
  for(int ientry = 0; ientry < nEntries; ++ientry)
  {
    if( ientry%10000 == 0 ) std::cout << "reading saved entry " << ientry << std::endl;
    ntu_DA -> GetEntry(ientry); 
    
    if ( runId < 163232 ) continue;    
    if( isZ != 1 ) continue;
    //if( PV_n < 5 ) continue;
    //if( PV_n > 10 ) continue;
    //if( PV_n < 10 ) continue;
    //if( seedLaserAlpha < 1.5 )  continue;
    //if( seedLaserAlpha2 < 1.5 ) continue;
    if( (fabs(scEta) > 2.5) || (fabs(scEta2) > 2.5) ) continue;
    if( (fabs(scEta) > 1.4442) && (fabs(scEta) < 1.5660) ) continue;
    if( (fabs(scEta2) > 1.4442) && (fabs(scEta2) < 1.5660) ) continue;
    
    PtEtaPhiELorentzVector sc1(scE *sin(2*atan(exp(-1.*eta))), eta, phi, scE);
    PtEtaPhiELorentzVector sc2(scE2*sin(2*atan(exp(-1.*eta2))),eta2,phi2,scE2);
    
    if( (isEB == 1) && (isEB2 == 1) )
    {
      h_scM_EBEB -> Fill((sc1+sc2).mass());
      
      int bin = -1;
      for(bin = 0; bin < nBins_nVtx; ++bin)
        if( (PV_n > nVtxMin+bin*nVtx_width) && (PV_n < nVtxMin+(bin+1)*nVtx_width) ) break;
      if( bin == nBins_nVtx) --bin;
      
      h_scM_vsNVtx_EBEB[bin] -> Fill((sc1+sc2).mass());
    }
    
    if( (isEB == 1) && (isEB2 == 1) )
    {
      h_scM_EBEB -> Fill((sc1+sc2).mass());
      
      int bin = -1;
      for(bin = 0; bin < nBins_nVtx; ++bin)
        if( (PV_n > nVtxMin+bin*nVtx_width) && (PV_n < nVtxMin+(bin+1)*nVtx_width) ) break;
      if( bin == nBins_nVtx) --bin;
      
      h_scM_vsNVtx_EBEB[bin] -> Fill((sc1+sc2).mass());
    }    
  }
  
  
  
  TCanvas* c_m = new TCanvas("c_m","m");
  int rebin =  1;
  
  h_scM_EBEB -> GetXaxis() -> SetTitle("m_{SC1+SC2} (GeV/c^{2}) - [EB,EB]");
  //h_scM_EBEB -> SetLineColor(kRed+2);
  //h_scM_EBEB -> SetLineWidth(2);
  h_scM_EBEB -> SetMarkerColor(kRed+2);
  h_scM_EBEB -> SetMarkerSize(0.7);
  h_scM_EBEB -> Rebin(rebin);
  h_scM_EBEB -> Draw("");
  
  TF1* scFunz_EBEB = new TF1("scFunz_EBEB", crystalBall, 70., h_scM_EBEB->GetMean()+1.75*h_scM_EBEB->GetRMS(), 5);
  scFunz_EBEB->SetNpx(10000);
  scFunz_EBEB->SetParameter(0, h_scM_EBEB->GetEntries()/10.);
  scFunz_EBEB->SetParameter(1, 90.);
  scFunz_EBEB->SetParameter(2, 2.5);
  scFunz_EBEB->SetParameter(3, 1.);
  scFunz_EBEB->SetParameter(4, 3.);  
  h_scM_EBEB->Fit("scFunz_EBEB","NQRM+");
  h_scM_EBEB->Draw("sames");
  TF1* scFunz2_EBEB = new TF1("scFunz2_EBEB", crystalBall, 70., scFunz_EBEB->GetParameter(1)+2.*scFunz_EBEB->GetParameter(2), 5);
  scFunz2_EBEB->SetNpx(10000);
  scFunz2_EBEB->SetLineColor(kRed);
  scFunz2_EBEB->SetParameter(0, scFunz_EBEB->GetParameter(0));
  scFunz2_EBEB->SetParameter(1, scFunz_EBEB->GetParameter(1));
  scFunz2_EBEB->SetParameter(2, scFunz_EBEB->GetParameter(2));
  scFunz2_EBEB->SetParameter(3, scFunz_EBEB->GetParameter(3));
  scFunz2_EBEB->SetParameter(4, scFunz_EBEB->GetParameter(4));
  scFunz2_EBEB->SetParName(0, "N");
  scFunz2_EBEB->SetParName(1, "#mu");
  scFunz2_EBEB->SetParName(2, "#sigma");
  scFunz2_EBEB->SetParName(3, "#alpha");
  scFunz2_EBEB->SetParName(4, "n");
  h_scM_EBEB->Fit("scFunz2_EBEB","0QRM+");
  h_scM_EBEB->Draw("sames");
  gPad->Update();
  
    
  TPaveStats* p_scM_EBEB = (TPaveStats*)(h_scM_EBEB->GetListOfFunctions()->FindObject("stats"));  
  p_scM_EBEB->SetX1NDC(0.79);
  p_scM_EBEB->SetX2NDC(0.99);
  p_scM_EBEB->SetY1NDC(0.59);
  p_scM_EBEB->SetY2NDC(0.99);
  p_scM_EBEB->SetTextColor(kRed+2);
  p_scM_EBEB->Draw("same"); 
  
  scFunz2_EBEB->Draw("same");
  
  

  TGraphErrors* g_peak_vsNVtx = new TGraphErrors();  
  TGraphErrors* g_sigma_vsNVtx = new TGraphErrors();
  TCanvas* c_m_vsNVtx[100];
  rebin = 2;
  
  for(int bin = 0; bin < nBins_nVtx; ++bin)
  {
    char canvasName[50];
    if( bin%6 == 0 )
    {
      sprintf(canvasName,"m_vsNVtx-%0d", bin/6); 
      c_m_vsNVtx[bin/6] = new TCanvas(canvasName,canvasName);
      c_m_vsNVtx[bin/6] -> Divide(3,2);
    }
    
    c_m_vsNVtx[bin/6] -> cd (bin%6+1);  
    
    h_scM_vsNVtx_EBEB[bin] -> GetXaxis() -> SetTitle("m_{SC1+SC2} (GeV/c^{2})- [EB,EB]");
    h_scM_vsNVtx_EBEB[bin] -> SetMarkerColor(kRed+2);
    h_scM_vsNVtx_EBEB[bin] -> SetMarkerSize(0.7);
    h_scM_vsNVtx_EBEB[bin] -> Rebin(rebin);
    h_scM_vsNVtx_EBEB[bin] -> Draw("");
    
    char funzName[50];
    sprintf(funzName,"scFunz_EBEB_%d",bin);
    TF1* scFunz_vsNVtx_EBEB = new TF1(funzName, crystalBall, 70., h_scM_vsNVtx_EBEB[bin]->GetMean()+1.75*h_scM_vsNVtx_EBEB[bin]->GetRMS(), 5);
    scFunz_vsNVtx_EBEB->SetNpx(10000);
    scFunz_vsNVtx_EBEB->SetParameter(0, h_scM_EBEB->GetEntries()/10.);
    scFunz_vsNVtx_EBEB->SetParameter(1, 90.);
    scFunz_vsNVtx_EBEB->SetParameter(2, 2.5);
    scFunz_vsNVtx_EBEB->SetParameter(3, 1.);
    scFunz_vsNVtx_EBEB->SetParameter(4, 3.);  
    h_scM_vsNVtx_EBEB[bin]->Fit(funzName,"NQRM+");
    h_scM_vsNVtx_EBEB[bin]->Draw("sames");
    
    char funzName2[50];
    sprintf(funzName2,"scFunz2_EBEB_%d",bin);
    TF1* scFunz2_vsNVtx_EBEB = new TF1(funzName2, crystalBall, 70., scFunz_vsNVtx_EBEB->GetParameter(1)+2.*scFunz_vsNVtx_EBEB->GetParameter(2), 5);
    scFunz2_vsNVtx_EBEB->SetNpx(10000);
    scFunz2_vsNVtx_EBEB->SetLineColor(kRed);
    scFunz2_vsNVtx_EBEB->SetParameter(0, scFunz_vsNVtx_EBEB->GetParameter(0));
    scFunz2_vsNVtx_EBEB->SetParameter(1, scFunz_vsNVtx_EBEB->GetParameter(1));
    scFunz2_vsNVtx_EBEB->SetParameter(2, scFunz_vsNVtx_EBEB->GetParameter(2));
    scFunz2_vsNVtx_EBEB->SetParameter(3, scFunz_vsNVtx_EBEB->GetParameter(3));
    scFunz2_vsNVtx_EBEB->SetParameter(4, scFunz_vsNVtx_EBEB->GetParameter(4));
    scFunz2_vsNVtx_EBEB->SetParName(0, "N");
    scFunz2_vsNVtx_EBEB->SetParName(1, "#mu");
    scFunz2_vsNVtx_EBEB->SetParName(2, "#sigma");
    scFunz2_vsNVtx_EBEB->SetParName(3, "#alpha");
    scFunz2_vsNVtx_EBEB->SetParName(4, "n");
    h_scM_vsNVtx_EBEB[bin]->Fit(funzName2,"0QRM+");
    h_scM_vsNVtx_EBEB[bin]->Draw("sames");
    gPad->Update();
    
    
    TPaveStats* p_scM_vsNVtx_EBEB = (TPaveStats*)(h_scM_vsNVtx_EBEB[bin]->GetListOfFunctions()->FindObject("stats"));  
    p_scM_vsNVtx_EBEB->SetX1NDC(0.79);
    p_scM_vsNVtx_EBEB->SetX2NDC(0.99);
    p_scM_vsNVtx_EBEB->SetY1NDC(0.59);
    p_scM_vsNVtx_EBEB->SetY2NDC(0.99);
    p_scM_vsNVtx_EBEB->SetTextColor(kRed+2);
    p_scM_vsNVtx_EBEB->Draw("same"); 
    
    scFunz2_vsNVtx_EBEB->Draw("same");
    
    float sigma = scFunz2_vsNVtx_EBEB->GetParameter(2);
    float esigma = scFunz2_vsNVtx_EBEB->GetParError(2);
    float peak = scFunz2_vsNVtx_EBEB->GetParameter(1);
    float epeak = scFunz2_vsNVtx_EBEB->GetParError(1);

    g_peak_vsNVtx -> SetPoint(bin,nVtxMin+0.5*nVtx_width+bin*nVtx_width,peak);
    g_peak_vsNVtx -> SetPointError(bin,0.5*nVtx_width,epeak);
        
    g_sigma_vsNVtx -> SetPoint(bin,nVtxMin+0.5*nVtx_width+bin*nVtx_width,sigma/peak);
    g_sigma_vsNVtx -> SetPointError(bin,0.5*nVtx_width,esigma/peak);
  }
  
  
  
  TCanvas* c_peak_sigma_vsNVtx = new TCanvas("c_peak_sigma_vsNVtx","c_peak_sigma_vsNVtx");
  c_peak_sigma_vsNVtx -> Divide(2);
  
  c_peak_sigma_vsNVtx -> cd(1);
  
  g_peak_vsNVtx -> SetMarkerColor(kRed+2);
  g_peak_vsNVtx -> GetXaxis() -> SetTitle("N_{vtx}");
  g_peak_vsNVtx -> GetYaxis() -> SetTitle("#mu   (GeV/c^{2})");
  g_peak_vsNVtx -> Draw("AP");
  
  
    
  c_peak_sigma_vsNVtx -> cd(2);
  
  g_sigma_vsNVtx -> SetMarkerColor(kRed+2);
  g_sigma_vsNVtx -> GetXaxis() -> SetTitle("N_{vtx}");
  g_sigma_vsNVtx -> GetYaxis() -> SetTitle("#sigma/#mu");
  g_sigma_vsNVtx -> Draw("AP");
}






double crystalBall(double* x, double* par)
{
  //[0] = N
  //[1] = mean
  //[2] = sigma
  //[3] = alpha
  //[4] = n
  
  
  double xx = x[0];
  double mean = par[1];
  double sigma = par[2];
  double alpha = par[3];
  double n = par[4];
  
  if( (xx-mean)/sigma > -alpha )
  {
    return par[0] * exp(-1. * (xx-mean)*(xx-mean) / (2*sigma*sigma) );
  }
  
  else
  {
    double A = pow(n/fabs(alpha), n) * exp(-0.5 * alpha*alpha);
    double B = n/fabs(alpha) - fabs(alpha);
    
    return par[0] * A * pow(B - (xx-mean)/sigma, -1.*n);
  }
}