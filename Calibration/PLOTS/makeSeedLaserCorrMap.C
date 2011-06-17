makeSeedLaserCorrMap(Char_t* infile1 =0)
{
  // Open file
  if (!infile1)
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
  int isEB;
  int timeStampHigh;
  float seedLaserCorr, seedLaserAlpha;
  int seedIeta,seedIphi;
  int seedIx,seedIy,seedZside;
  float scLaserCorr;
  float scEta;

  ntu_DA->SetBranchAddress("timeStampHigh",       &timeStampHigh);
  ntu_DA->SetBranchAddress("ele1_isEB",           &isEB);
  ntu_DA->SetBranchAddress("ele1_seedLaserCorr",  &seedLaserCorr);
  ntu_DA->SetBranchAddress("ele1_seedLaserAlpha", &seedLaserAlpha);
  ntu_DA->SetBranchAddress("ele1_seedIeta",       &seedIeta);
  ntu_DA->SetBranchAddress("ele1_seedIphi",       &seedIphi);
  ntu_DA->SetBranchAddress("ele1_seedIx",         &seedIx);
  ntu_DA->SetBranchAddress("ele1_seedIy",         &seedIy);
  ntu_DA->SetBranchAddress("ele1_seedZside",      &seedZside);
  ntu_DA->SetBranchAddress("ele1_scLaserCorr",    &scLaserCorr);
  ntu_DA->SetBranchAddress("ele1_scEta",          &scEta);
  
  
  
  // Define histograms
  TH1F* h_seedLaserCorrDistr_EB  = new TH1F("h_seedLaserCorrDistr_EB", "", 600,0.8,1.2);
  TH1F* h_seedLaserCorrDistr_EEp = new TH1F("h_seedLaserCorrDistr_EEp","", 600,0.8,1.2);
  TH1F* h_seedLaserCorrDistr_EEm = new TH1F("h_seedLaserCorrDistr_EEm","", 600,0.8,1.2);
  
  TProfile* h_seedLaserCorrAvg_vsEta     = new TProfile("h_seedLaserCorrAvg_vsEta",     "",  100, -2.5,  2.5,0.8,1.2);
  TProfile* h_seedLaserCorrAvg_vsEta_EB  = new TProfile("h_seedLaserCorrAvg_vsEta_EB",  "",   60, -1.5,  1.5,0.8,1.2);
  TProfile* h_seedLaserCorrAvg_vsEta_EEp = new TProfile("h_seedLaserCorrAvg_vsEta_EEp", "",   20,  1.5,  2.5,0.8,1.2);
  TProfile* h_seedLaserCorrAvg_vsEta_EEm = new TProfile("h_seedLaserCorrAvg_vsEta_EEm", "",   20, -2.5, -1.5,0.8,1.2);
  
  TH2F* p_seedLaserCorrAvg_vsEta     = new TH2F("p_seedLaserCorrAvg_vsEta",     "",  100, -2.5,  2.5,1000,0.5,1.5);
  TH2F* p_seedLaserCorrAvg_vsEta_EB  = new TH2F("p_seedLaserCorrAvg_vsEta_EB",  "",   60, -1.5,  1.5,1000,0.5,1.5);
  TH2F* p_seedLaserCorrAvg_vsEta_EEp = new TH2F("p_seedLaserCorrAvg_vsEta_EEp", "",   20,  1.5,  2.5,1000,0.5,1.5);
  TH2F* p_seedLaserCorrAvg_vsEta_EEm = new TH2F("p_seedLaserCorrAvg_vsEta_EEm", "",   20, -2.5, -1.5,1000,0.5,1.5);
      
  TProfile2D* h_seedLaserCorrMap_EB  = new TProfile2D("h_seedLaserCorrMap_EB","",  171, -85., 86., 361,   0.,361., 0.8,1.2);
  TProfile2D* h_seedLaserCorrMap_EEp = new TProfile2D("h_seedLaserCorrMap_EEp","", 101,   0.,101., 100,   0.,101., 0.8,1.2);
  TProfile2D* h_seedLaserCorrMap_EEm = new TProfile2D("h_seedLaserCorrMap_EEm","", 101,   0.,101., 100,   0.,101., 0.8,1.2);
  
  
  
  // Loop over entries
  int nEntries = ntu_DA -> GetEntriesFast();
  for(int ientry = 0; ientry < nEntries; ++ientry)
  {
    if( ientry%10000 == 0 ) std::cout << "reading saved entry " << ientry << std::endl;
    ntu_DA -> GetEntry(ientry); 
    
    if( seedLaserAlpha < 1.5 ) continue;
    //if( scLaserCorr >  1.04 ) continue;
    //if( seedLaserCorr == 1. ) continue;
    if( timeStampHigh > 1303862400 ) continue;
        
//    if( seedZside < 0 )
//      if( (seedIx > 20 ) && (seedIx < 50) && (seedIy > 85) && (seedIy < 92) ) continue;
//    if( seedZside == -1 )
//      if( (seedIx > 35 ) && (seedIx < 55) && (seedIy > 80) && (seedIy < 87) ) continue;
//    if( seedZside > 0 )
//      if( (seedIx > 65 ) && (seedIx < 77) && (seedIy > 33) && (seedIy < 57) ) continue;
//    if( seedZside > 0 )
//      if( (seedIx > 75 ) && (seedIx < 93) && (seedIy > 18) && (seedIy < 37) ) continue;
    
    
    
    h_seedLaserCorrAvg_vsEta    -> Fill(scEta,seedLaserCorr);
    
    if( isEB == 1 )
    {
      h_seedLaserCorrDistr_EB -> Fill(seedLaserCorr);
      h_seedLaserCorrMap_EB -> Fill(seedIeta,seedIphi,seedLaserCorr);
      h_seedLaserCorrAvg_vsEta_EB -> Fill(scEta,seedLaserCorr);
      p_seedLaserCorrAvg_vsEta_EB -> Fill(scEta,seedLaserCorr);
    }
    
    else
    {
      if( seedZside > 0)
      {
        h_seedLaserCorrDistr_EEp -> Fill(seedLaserCorr);
        h_seedLaserCorrMap_EEp -> Fill(seedIx,seedIy,seedLaserCorr);
        h_seedLaserCorrAvg_vsEta_EEp -> Fill(scEta,seedLaserCorr);
        p_seedLaserCorrAvg_vsEta_EEp -> Fill(scEta,seedLaserCorr);
      }
      else
      {
        h_seedLaserCorrDistr_EEm -> Fill(seedLaserCorr);
        h_seedLaserCorrMap_EEm -> Fill(seedIx,seedIy,seedLaserCorr);
        h_seedLaserCorrAvg_vsEta_EEm -> Fill(scEta,seedLaserCorr);
        p_seedLaserCorrAvg_vsEta_EEm -> Fill(scEta,seedLaserCorr);
      }
    }
    
  }
  
  
  
  TCanvas* c_seedLaserCorr_EB = new TCanvas("c_seedLaserCorr_EB","seedLaserCorr_EB",100,100,1350,500);
  c_seedLaserCorr_EB -> Divide(3,1);
  
  c_seedLaserCorr_EB -> cd(1);
  h_seedLaserCorrMap_EB -> GetXaxis() -> SetTitle("i#eta");
  h_seedLaserCorrMap_EB -> GetYaxis() -> SetTitle("i#phi");
  h_seedLaserCorrMap_EB -> GetZaxis() -> SetTitle("<seedLaserCorr>");
  h_seedLaserCorrMap_EB -> GetZaxis() -> SetRangeUser(0.95,1.1);
  h_seedLaserCorrMap_EB -> Draw("COLZ");
  
  c_seedLaserCorr_EB -> cd(2);
  gPad -> SetLogy();
  h_seedLaserCorrDistr_EB -> GetXaxis() -> SetTitle("seedLaserCorr");
  h_seedLaserCorrDistr_EB -> Draw();
  
  c_seedLaserCorr_EB -> cd(3);  
  p_seedLaserCorrAvg_vsEta_EB -> GetXaxis() -> SetTitle("#eta");
  p_seedLaserCorrAvg_vsEta_EB -> GetYaxis() -> SetTitle("<seedLaserCorr>");
  //p_seedLaserCorrAvg_vsEta_EB -> GetYaxis() -> SetRangeUser(1.0,1.02);
  p_seedLaserCorrAvg_vsEta_EB -> GetYaxis() -> SetRangeUser(0.5,1.5);
  p_seedLaserCorrAvg_vsEta_EB -> Draw("COLZ");
  
  
  
  TCanvas* c_seedLaserCorr_EEp = new TCanvas("c_seedLaserCorr_EEp","seedLaserCorr_EEp",100,100,1350,500);
  c_seedLaserCorr_EEp -> Divide(3,1);
  
  c_seedLaserCorr_EEp -> cd(1);
  h_seedLaserCorrMap_EEp -> GetXaxis() -> SetTitle("ix");
  h_seedLaserCorrMap_EEp -> GetYaxis() -> SetTitle("iy");
  h_seedLaserCorrMap_EEp -> GetZaxis() -> SetTitle("<seedLaserCorr>");
  h_seedLaserCorrMap_EEp -> GetZaxis() -> SetRangeUser(0.95,1.15);
  h_seedLaserCorrMap_EEp -> Draw("COLZ");
  
  c_seedLaserCorr_EEp -> cd(2);
  gPad -> SetLogy();
  h_seedLaserCorrDistr_EEp -> GetXaxis() -> SetTitle("seedLaserCorr");
  h_seedLaserCorrDistr_EEp -> Draw();
  
  c_seedLaserCorr_EEp -> cd(3);  
  p_seedLaserCorrAvg_vsEta_EEp -> GetXaxis() -> SetTitle("#eta");
  p_seedLaserCorrAvg_vsEta_EEp -> GetYaxis() -> SetTitle("<seedLaserCorr>");
  //p_seedLaserCorrAvg_vsEta_EEp -> GetYaxis() -> SetRangeUser(1.0,1.05);
  p_seedLaserCorrAvg_vsEta_EEp -> GetYaxis() -> SetRangeUser(0.5,1.5);
  p_seedLaserCorrAvg_vsEta_EEp -> Draw("COLZ");
  
  
  
  TCanvas* c_seedLaserCorr_EEm = new TCanvas("c_seedLaserCorr_EEm","seedLaserCorr_EEm",100,100,1350,500);
  c_seedLaserCorr_EEm -> Divide(3,1);
  
  c_seedLaserCorr_EEm -> cd(1);
  h_seedLaserCorrMap_EEm -> GetXaxis() -> SetTitle("ix");
  h_seedLaserCorrMap_EEm -> GetYaxis() -> SetTitle("iy");
  h_seedLaserCorrMap_EEm -> GetZaxis() -> SetTitle("<seedLaserCorr>");
  h_seedLaserCorrMap_EEm -> GetZaxis() -> SetRangeUser(0.95,1.15);
  h_seedLaserCorrMap_EEm -> Draw("COLZ");
  
  c_seedLaserCorr_EEm -> cd(2);
  gPad -> SetLogy();
  h_seedLaserCorrDistr_EEm -> GetXaxis() -> SetTitle("seedLaserCorr");
  h_seedLaserCorrDistr_EEm -> Draw();
  
  c_seedLaserCorr_EEm -> cd(3);  
  p_seedLaserCorrAvg_vsEta_EEm -> GetXaxis() -> SetTitle("#eta");
  p_seedLaserCorrAvg_vsEta_EEm -> GetYaxis() -> SetTitle("<seedLaserCorr>");
  //p_seedLaserCorrAvg_vsEta_EEm -> GetYaxis() -> SetRangeUser(1.0,1.05);
  p_seedLaserCorrAvg_vsEta_EEm -> GetYaxis() -> SetRangeUser(0.5,1.5);
  p_seedLaserCorrAvg_vsEta_EEm -> Draw("COLZ");  
  
  
  
  TCanvas* c_seedLaserCorrAvgMaps = new TCanvas("c_seedLaserCorrAvgMaps","seedLaserCorrAvgMaps",100,100,1350,500);  
  c_seedLaserCorrAvgMaps -> Divide(3);
  
  c_seedLaserCorrAvgMaps -> cd(1);
  h_seedLaserCorrMap_EB -> Draw("COLZ");
  c_seedLaserCorrAvgMaps -> cd(2);
  h_seedLaserCorrMap_EEp -> Draw("COLZ");  
  c_seedLaserCorrAvgMaps -> cd(3);
  h_seedLaserCorrMap_EEm -> Draw("COLZ");
  
  
  
  TCanvas* c_seedLaserCorrAvg_vsEta = new TCanvas("c_seedLaserCorrAvg_vsEta","seedLaserCorrAvg_vsEta");
  
  h_seedLaserCorrAvg_vsEta -> GetXaxis() -> SetTitle("#eta");
  h_seedLaserCorrAvg_vsEta -> GetYaxis() -> SetTitle("<seedLaserCorr>");
  h_seedLaserCorrAvg_vsEta -> Draw();
  
  
  
  TFile* o = new TFile("makeSeedLaserCorrMapHistos.root","RECREATE");
  o -> cd();
  
  h_seedLaserCorrAvg_vsEta -> Write();
  
  o -> Close();
}